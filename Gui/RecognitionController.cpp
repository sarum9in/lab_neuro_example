#include "RecognitionController.hpp"

#include <QSet>

RecognitionController::RecognitionController(QObject *parent):
    QObject(parent),
    m_model(new QStringListModel(this)),
    m_size(1)
{
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
    // TODO reset
}

void RecognitionController::setData(const DataVector &data)
{
    m_data = data;
}
