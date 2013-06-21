#include "FunctionController.hpp"

#include "RandomSupervisor.hpp"
#include "BackpropagationSupervisor.hpp"
#include "LogSigmoidActivationFunction.hpp"

#include <QDebug>

constexpr int STEPS = 1000;

FunctionController::FunctionController(QwtPlot *plot, QObject *parent):
    QObject(parent),
    m_scriptEngine(new QScriptEngine(this)),
    m_plot(plot),
    m_originalFunction(new QwtPlotCurve(tr("Original function"))),
    m_neuralFunction(new QwtPlotCurve(tr("Neural function"))),
    m_valid(false),
    m_neuralNetworkYScale(1),
    m_initSupervisor(new RandomSupervisor(-5, 5, this)),
    m_supervisor(new BackpropagationSupervisor(this))
{
    NeuralLayer l1, l2;
    const int l1Size = (0.6 * STEPS - 1 - 1) / 2;
    for (int i = 0; i < l1Size; ++i)
    {
        Neuron n1i;
        n1i.setWeights(WeightVector(2));
        l1.pushBack(n1i);
    }
    {
        Neuron n2;
        n2.setWeights(WeightVector(l1Size + 1));
        l2.pushBack(n2);
    }
    m_neuralNetwork.pushBack(l1);
    m_neuralNetwork.pushBack(l2);
    m_neuralNetwork.setActivationFunction(ActivationFunctionPointer(new LogSigmoidActivationFunction(1)));
}

FunctionController::~FunctionController()
{
    delete m_originalFunction;
    delete m_neuralFunction;
}

void FunctionController::setMinX(const qreal minX)
{
    m_minX = minX;
}

void FunctionController::setMaxX(const qreal maxX)
{
    m_maxX = maxX;
}

void FunctionController::setShowOriginalFunction(const bool show)
{
    if (show)
        m_originalFunction->attach(m_plot);
    else
        m_originalFunction->detach();
    m_plot->replot();
}

void FunctionController::setShowNeuralFunction(const bool show)
{
    if (show)
        m_neuralFunction->attach(m_plot);
    else
        m_neuralFunction->detach();
    m_plot->replot();
}

void FunctionController::setScript(const QString &script)
{
    m_script = script;
}

void FunctionController::train()
{
    m_valid = false;
    if (m_scriptEngine->canEvaluate(m_script))
    {
        m_scriptEngine->evaluate(m_script);
        if (m_scriptEngine->hasUncaughtException())
        {
            emit scriptError(tr("Uncaught exception: ") + m_scriptEngine->uncaughtException().toString());
            m_scriptEngine->clearExceptions();
        }
        else
        {
            m_function = m_scriptEngine->evaluate("calc");
            if (m_function.isFunction())
            {
                update();
            }
            else
            {
                emit scriptError(tr("Object \"calc\" is not a function."));
                m_scriptEngine->clearExceptions();
            }
        }
    }
    else
    {
        emit scriptError(tr("Unable to evaluate the script."));
    }
}

void FunctionController::detach()
{
    m_originalFunction->detach();
    m_neuralFunction->detach();
}

void FunctionController::update()
{
    const qreal beginX = qMin(m_minX, m_maxX);
    const qreal endX = qMax(m_minX, m_maxX);
    const qreal diffX = endX - beginX;
    const qreal dx = diffX / STEPS;
    m_neuralNetworkYScale = 0;

    QVector<QPointF> originalData(STEPS + 1);
    for (int i = 0; i <= STEPS; ++i)
    {
        const qreal x = beginX + i * dx;
        originalData[i].setX(x);
        QScriptValue arguments = m_scriptEngine->newArray(1);
        arguments.setProperty(0, x);
        QScriptValue scriptY = m_function.call(QScriptValue(), arguments);
        if (m_scriptEngine->hasUncaughtException())
        {
            emit scriptError(tr("Unable to calculate value: ") + m_scriptEngine->uncaughtException().toString());
            m_scriptEngine->clearExceptions();
            return;
        }
        const qreal y = scriptY.toNumber();

        originalData[i].setY(y);
        m_neuralNetworkYScale = qMax(m_neuralNetworkYScale, qAbs(y));
    }

    m_originalFunction->setSamples(originalData);

    TrainingVector trainingSet(originalData.size());
    for (int i = 0; i < originalData.size(); ++i)
    {
        trainingSet[i].input.fill(originalData[i].x(), 1);
        trainingSet[i].output.fill(originalData[i].y() / m_neuralNetworkYScale, 1);
    }

    m_initSupervisor->train(m_neuralNetwork);
    m_supervisor->setTrainingSet(trainingSet);
    m_supervisor->trainFor(m_neuralNetwork, 1000 * 1000);

    constexpr int NSTEPS = STEPS * 3;
    const qreal ndx = diffX / NSTEPS;
    QVector<QPointF> neuralData(NSTEPS + 1);
    for (int i = 0; i <= NSTEPS; ++i)
    {
        const qreal x = beginX + i * ndx;
        neuralData[i].setX(x);
        const DataVector input = {x};
        const DataVector output = m_neuralNetwork.transform(input);
        Q_ASSERT(output.size() == 1);
        neuralData[i].setY(output.first() * m_neuralNetworkYScale);
    }

    m_neuralFunction->setSamples(neuralData);

    m_valid = true;
    m_plot->replot();
}
