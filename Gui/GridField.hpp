#pragma once

#include "DataTransformation.hpp"

#include <QWidget>

class GridField: public QWidget
{
    Q_OBJECT

public:
    explicit GridField(QWidget *parent=nullptr);

    const DataVector &data() const;

    bool value(const int x, const int y) const;
    bool value(const QPointF &point) const;

signals:
    void dataChanged(const DataVector &data);

public slots:
    void setValue(const int x, const int y, const bool value);
    void setValue(const QPointF &point, const bool value);

    void setSize(const int size);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void parsePoint(const QPointF &point, int &x, int &y) const;

private:
    int m_size;
    DataVector m_data;
};
