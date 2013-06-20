#pragma once

#include "NeuralNetwork.hpp"

#include <QObject>

class BooleanController: public QObject
{
    Q_OBJECT

public:
    explicit BooleanController(QObject *parent=nullptr);

    bool result() const;

signals:
    void resultChanged(const bool result);

public slots:
    void setX(const bool x);
    void setY(const bool y);

private:
    void updateResult();

private:
    NeuralNetwork m_neuralNetwork;
    bool m_x;
    bool m_y;
    bool m_result;
};
