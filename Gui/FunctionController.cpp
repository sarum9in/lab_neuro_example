#include "FunctionController.hpp"

#include <QDebug>

FunctionController::FunctionController(QwtPlot *plot, QObject *parent):
    QObject(parent),
    m_scriptEngine(new QScriptEngine(this)),
    m_plot(plot),
    m_originalFunction(new QwtPlotCurve(tr("Original function"))),
    m_neuralFunction(new QwtPlotCurve(tr("Neural function"))),
    m_valid(false)
{
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

constexpr int STEPS = 1000;

void FunctionController::update()
{
    const qreal beginX = qMin(m_minX, m_maxX);
    const qreal endX = qMax(m_minX, m_maxX);
    const qreal diffX = endX - beginX;
    const qreal dx = diffX / STEPS;

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
    }

    m_originalFunction->setSamples(originalData);

    m_valid = true;
    m_plot->replot();
}
