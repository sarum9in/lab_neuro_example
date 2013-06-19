#include "GridField.hpp"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

GridField::GridField(QWidget *parent):
    QWidget(parent),
    m_size(1),
    m_data(m_size * m_size)
{
    emit dataChanged(m_data);
    update();
}

void GridField::setSize(const int size)
{
    Q_ASSERT(size > 0);
    m_size = size;
    m_data.fill(0, m_size * m_size);
    emit dataChanged(m_data);
    update();
}

bool GridField::value(const int x, const int y) const
{
    const int i = x * m_size + y;
    return qRound(m_data[i]);
}

bool GridField::value(const QPointF &point) const
{
    int x, y;
    parsePoint(point, x, y);
    return value(x, y);
}

void GridField::setValue(const int x, const int y, const bool value)
{
    const int i = x * m_size + y;
    const bool oldValue = this->value(x, y);
    if (value != oldValue)
    {
        m_data[i] = value;
        emit dataChanged(m_data);
        update();
    }
}

void GridField::setValue(const QPointF &point, const bool value)
{
    int x, y;
    parsePoint(point, x, y);
    setValue(x, y, value);
}

void GridField::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::white);
    QSizeF s = size();
    const int dx = s.width() / m_size;
    const int dy = s.height() / m_size;
    for (int x = 0; x < m_size; ++x)
    {
        for (int y = 0; y < m_size; ++y)
        {
            if (value(x, y))
            {
                painter.setBrush(Qt::black);
                painter.drawRect(x * dx, y * dy, dx, dy);
            }
            else
            {
                painter.setBrush(Qt::lightGray);
                painter.drawRect(x * dx + 1, y * dy + 1, dx - 2, dy - 2);
            }
        }
    }
    painter.end();
}

void GridField::mouseMoveEvent(QMouseEvent *event)
{
    mousePressEvent(event);
}

void GridField::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
        setValue(event->localPos(), event->buttons() & Qt::LeftButton);
}

void GridField::parsePoint(const QPointF &point, int &x, int &y) const
{
    const QSizeF s = size();
    x = qFloor(point.x() / (s.width() / m_size));
    y = qFloor(point.y() / (s.height() / m_size));
    x = qMin(qMax(x, 0), m_size - 1);
    y = qMin(qMax(y, 0), m_size - 1);
}
