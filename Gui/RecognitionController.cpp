#include "RecognitionController.hpp"

#include "DeltaRuleSupervisor.hpp"
#include "RandomSupervisor.hpp"

#include <QSet>

RecognitionController::RecognitionController(QObject *parent):
    QObject(parent),
    m_model(new QStringListModel(this)),
    m_size(1),
    m_supervisor(new DeltaRuleSupervisor(this)),
    m_initSupervisor(new RandomSupervisor(0, 10, this))
{
    m_supervisor->setLearningSpeed(0.01);
    reset();
}

QAbstractItemModel *RecognitionController::model() const
{
    return m_model;
}

void RecognitionController::setSymbolSet(const QString &set)
{
    QSet<QString> s;
    for (const QChar &c: set)
        s.insert(QString(c.toLower()));
    QStringList lst(s.toList());
    lst.sort(Qt::CaseInsensitive);
    m_model->setStringList(lst);
}

void RecognitionController::fix()
{
}

void RecognitionController::suggestSymbol(const QString &symbol)
{
    m_suggestedSymbol = symbol;
}

void RecognitionController::setSize(const int size)
{
    m_size = size;
    reset();
}

void RecognitionController::setData(const DataVector &data)
{
    m_data = data;
}

void RecognitionController::reset()
{
    m_neuralNetwork.clear();
    NeuralLayer layer;
    for (int i = 0; i < m_size; ++i)
    {
        Neuron neuron;
        neuron.setWeights(WeightVector(m_size));
        layer.pushBack(neuron);
    }
    m_neuralNetwork.pushBack(layer);
    m_initSupervisor->train(m_neuralNetwork);
}
