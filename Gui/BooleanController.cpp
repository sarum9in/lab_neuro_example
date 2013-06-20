#include "BooleanController.hpp"

#include "BinaryStepActivationFunction.hpp"

BooleanController::BooleanController(QObject *parent):
    QObject(parent)
{
    NeuralLayer l1, l2;
    Neuron n11, n12, n13, n2;
    {
        n11.setWeights(WeightVector(3));
        n11.setWeight(1, -1);
        n11.setWeight(2, 1);
        n12.setWeights(WeightVector(3));
        n12.setWeight(1, 1);
        n12.setWeight(2, -1);
        n13.setWeights(WeightVector(3));
        n13.setWeight(1, 1);
        n13.setWeight(2, 1);
        n2.setWeights(WeightVector(4));
        n2.setWeight(1, 2);
        n2.setWeight(2, 2);
        n2.setWeight(3, -1);
    }
    l1.pushBack(n11);
    l1.pushBack(n12);
    l1.pushBack(n13);
    l2.pushBack(n2);
    m_neuralNetwork.pushBack(l1);
    m_neuralNetwork.pushBack(l2);
    m_neuralNetwork.setActivationFunction(ActivationFunctionPointer(new BinaryStepActivationFunction));
}

bool BooleanController::result() const
{
    return m_result;
}

void BooleanController::setX(const bool x)
{
    m_x = x;
    updateResult();
}

void BooleanController::setY(const bool y)
{
    m_y = y;
    updateResult();
}

void BooleanController::updateResult()
{
    DataVector input(2);
    input[0] = m_x;
    input[1] = m_y;
    const DataVector output = m_neuralNetwork.transform(input);
    Q_ASSERT(output.size() == 1);
    const bool newResult = qRound(output[0]);
    if (m_result != newResult)
    {
        m_result = newResult;
        emit resultChanged(m_result);
    }
}
