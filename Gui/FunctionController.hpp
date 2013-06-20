#pragma once

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

    ~FunctionController();

signals:
    void scriptError(const QString &error);

public slots:
    void setMinX(const qreal minX);
    void setMaxX(const qreal maxX);

    void setShowOriginalFunction(const bool show);
    void setShowNeuralFunction(const bool show);

    void setScript(const QString &script);

    void train();

    void detach();

private:
    void update();

private:
    QScriptEngine *m_scriptEngine;
    QwtPlot *m_plot;
    QwtPlotCurve *m_originalFunction, *m_neuralFunction;
    qreal m_minX, m_maxX;
    QString m_script;
    QScriptValue m_function;
    bool m_valid;
};
