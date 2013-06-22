#include "FunctionController.hpp"

#include "BackpropagationSupervisor.hpp"
#include "StochasticSupervisor.hpp"
#include "LogSigmoidActivationFunction.hpp"

#include <QDebug>

constexpr int STEPS = 400;

FunctionController::FunctionController(QwtPlot *plot, QObject *parent):
    QObject(parent),
    m_scriptEngine(new QScriptEngine(this)),
    m_plot(plot),
    m_originalFunction(new QwtPlotCurve(tr("Original function"))),
    m_neuralFunction(new QwtPlotCurve(tr("Neural function"))),
    m_valid(false),
    m_neuralNetworkYScale(1),
    m_supervisor(new ThreadedSupervisor(new StochasticSupervisor(this), this))
{
    connect(m_supervisor, &ThreadedSupervisor::started, this, &FunctionController::started);
    connect(m_supervisor, &ThreadedSupervisor::finished, this, &FunctionController::finished);
    connect(m_supervisor, &ThreadedSupervisor::targetErrorInfo, this, &FunctionController::targetErrorInfo);
    connect(m_supervisor, &ThreadedSupervisor::iterationInfo, this, &FunctionController::iterationInfo);
    connect(m_supervisor, &ThreadedSupervisor::resultReady, this, &FunctionController::setTrainingResult);
    setOriginalSteps(STEPS);
    setNeuralSteps(3 * STEPS);
    setNeurons(STEPS / 3);
}

FunctionController::~FunctionController()
{
    delete m_originalFunction;
    delete m_neuralFunction;
}

QwtPlotCurve *FunctionController::originalFunction()
{
    return m_originalFunction;
}

QwtPlotCurve *FunctionController::neuralFunction()
{
    return m_neuralFunction;
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

void FunctionController::setOriginalSteps(const int originalSteps)
{
    m_originalSteps = originalSteps;
}

void FunctionController::setNeuralSteps(const int neuralSteps)
{
    m_neuralSteps = neuralSteps;
}

void FunctionController::setNeurons(const int neurons)
{
    m_neurons = neurons;
    NeuralLayer l1, l2;
    const int l1Size = m_neurons;
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

void FunctionController::setTrainingResult(const bool result, const NeuralNetwork &neuralNetwork)
{
    const qreal beginX = qMin(m_minX, m_maxX);
    const qreal endX = qMax(m_minX, m_maxX);
    const qreal diffX = endX - beginX;

    m_neuralNetwork = neuralNetwork;
    const qreal ndx = diffX / m_neuralSteps;
    QVector<QPointF> neuralData(m_neuralSteps + 1);
    for (int i = 0; i <= m_neuralSteps; ++i)
    {
        const qreal x = beginX + i * ndx;
        neuralData[i].setX(x);
        const DataVector input = {x};
        const DataVector output = m_neuralNetwork.transform(input);
        Q_ASSERT(output.size() == 1);
        neuralData[i].setY(neuralToOriginal(output.first()));
    }

    m_neuralFunction->setSamples(neuralData);

    m_plot->replot();
}

void FunctionController::update()
{
    const qreal beginX = qMin(m_minX, m_maxX);
    const qreal endX = qMax(m_minX, m_maxX);
    const qreal diffX = endX - beginX;
    const qreal dx = diffX / m_originalSteps;
    qreal minY, maxY;

    QVector<QPointF> originalData(m_originalSteps + 1);
    for (int i = 0; i <= m_originalSteps; ++i)
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

        if (i)
        {
            minY = qMin(minY, y);
            maxY = qMax(maxY, y);
        }
        else
        {
            minY = maxY = y;
        }
    }

    setOriginalNeuralScale(minY, maxY);

    m_originalFunction->setSamples(originalData);

    TrainingVector trainingSet(originalData.size());
    for (int i = 0; i < originalData.size(); ++i)
    {
        qDebug() << minY << maxY << "|" << m_neuralNetworkYBias << m_neuralNetworkYScale <<
                    "[" <<originalData[i].y() << originalToNeural(originalData[i].y()) << "]";
        //trainingSet[i].input = {originalToNeural(originalData[i].x())};
        trainingSet[i].input = {originalData[i].x()};
        trainingSet[i].output = {originalToNeural(originalData[i].y())};
    }

    m_supervisor->setTrainingSet(trainingSet);
    m_supervisor->trainFor(m_neuralNetwork, 1000 * 1000);

    m_valid = true;
    m_plot->replot();
}

qreal FunctionController::neuralToOriginal(const qreal y) const
{
    return y * m_neuralNetworkYScale + m_neuralNetworkYBias;
}

qreal FunctionController::originalToNeural(const qreal y) const
{
    return (y - m_neuralNetworkYBias) / m_neuralNetworkYScale;
}

void FunctionController::setOriginalNeuralScale(const qreal minY, const qreal maxY)
{
    m_neuralNetworkYBias = minY;
    m_neuralNetworkYScale = 1.1 * (maxY - minY);
}
