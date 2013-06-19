#include "RecognitionController.hpp"

#include "DeltaRuleSupervisor.hpp"
#include "RandomSupervisor.hpp"
#include "BinaryStepActivationFunction.hpp"

#include <QSet>

RecognitionController::RecognitionController(QObject *parent):
    QObject(parent),
    m_model(new QStringListModel(this)),
    m_size(1),
    m_supervisor(new DeltaRuleSupervisor(this)),
    m_initSupervisor(new RandomSupervisor(0, 10, this)),
    m_activationFunction(new BinaryStepActivationFunction)
{
    m_supervisor->setLearningSpeed(0.1);
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
    reset();
}

void RecognitionController::fix()
{
    TrainingVector set = m_supervisor->trainingSet();
    DataVector output(m_model->rowCount());
    const QStringList &lst = m_model->stringList();
    for (int i = 0; i < lst.size(); ++i)
        if (lst[i] == m_suggestedSymbol)
            output[i] = 1;
    const TrainingExample example = {
        .input = m_data,
        .output = output
    };
    set.push_back(example);
    m_supervisor->setTrainingSet(set);
    m_supervisor->train(m_neuralNetwork);
    recognize();
}

void RecognitionController::suggestSymbol(const QString &symbol)
{
    m_suggestedSymbol = symbol;
}

void RecognitionController::setSize(const int size)
{
    m_size = size;
    m_data.fill(0, m_size * m_size);
    reset();
}

void RecognitionController::setData(const DataVector &data)
{
    m_data = data;
    if (m_size * m_size == m_data.size())
        recognize();
}

void RecognitionController::recognize()
{
    if (m_model->rowCount())
    {
        const DataVector out = m_neuralNetwork.transform(m_data);
        for (int i = 0; i < out.size(); ++i)
            if (qRound(out[i]))
            {
                emit recognized(m_model->stringList()[i]);
                break;
            }
    }
}

void RecognitionController::reset()
{
    m_neuralNetwork.clear();
    NeuralLayer layer;
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        Neuron neuron;
        neuron.setActivationFunction(m_activationFunction);
        neuron.setWeights(WeightVector(1 + m_size * m_size));
        layer.pushBack(neuron);
    }
    m_neuralNetwork.pushBack(layer);
    m_initSupervisor->train(m_neuralNetwork);
}
