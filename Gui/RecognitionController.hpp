#pragma once

#include "DataTransformation.hpp"
#include "Supervisor.hpp"
#include "NeuralNetwork.hpp"

#include <QObject>
#include <QStringListModel>

class RecognitionController: public QObject
{
    Q_OBJECT

public:
    explicit RecognitionController(QObject *parent=nullptr);

    QAbstractItemModel *model() const;

signals:
    void recognized(const QString &symbol);

public slots:
    void suggestSymbol(const QString &symbol);
    void setSymbolSet(const QString &set);
    void fix();
    void setSize(const int size);
    void setData(const DataVector &data);

    void recognize();
    void reset();

private:
    QString m_suggestedSymbol;
    QStringListModel *m_model;
    int m_size;
    DataVector m_data;
    NeuralNetwork m_neuralNetwork;
    Supervisor *m_supervisor, *m_initSupervisor;
    ActivationFunctionPointer m_activationFunction;
};
