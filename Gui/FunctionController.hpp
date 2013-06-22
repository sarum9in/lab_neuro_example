#pragma once

#include "NeuralNetwork.hpp"
#include "ThreadedSupervisor.hpp"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <QObject>
#include <QScriptEngine>

class FunctionController: public QObject
{
    Q_OBJECT

public:
    /// \param plot Does not take ownership.
    explicit FunctionController(QwtPlot *plot, QObject *parent=nullptr);

    QwtPlotCurve *originalFunction();
    QwtPlotCurve *neuralFunction();

    ~FunctionController();

signals:
    void scriptError(const QString &error);

    void started(const int count);
    void finished(const bool result);
    void targetErrorInfo(const qreal targetError, const qreal currentError, const qreal bestError);
    void iterationInfo(const int currentIteration, const int maxIterations);

public slots:
    void setMinX(const qreal minX);
    void setMaxX(const qreal maxX);

    void setShowOriginalFunction(const bool show);
    void setShowNeuralFunction(const bool show);

    void setScript(const QString &script);

    void train();

    void detach();

    void setOriginalSteps(const int originalSteps);
    void setNeuralSteps(const int neuralSteps);
    void setNeurons(const int neurons);

    void setTrainingResult(const bool result, const NeuralNetwork &neuralNetwork);

private:
    void update();

    qreal neuralToOriginal(const qreal y) const;
    qreal originalToNeural(const qreal y) const;
    void setOriginalNeuralScale(const qreal minY, const qreal maxY);

private:
    QScriptEngine *m_scriptEngine;
    QwtPlot *m_plot;
    QwtPlotCurve *m_originalFunction, *m_neuralFunction;
    qreal m_minX, m_maxX;
    QString m_script;
    QScriptValue m_function;
    bool m_valid;
    NeuralNetwork m_neuralNetwork;
    qreal m_neuralNetworkYScale, m_neuralNetworkYBias;
    ThreadedSupervisor *m_supervisor;
    int m_originalSteps, m_neuralSteps, m_neurons;
};
