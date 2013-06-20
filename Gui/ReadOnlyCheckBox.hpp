#pragma once

#include <QCheckBox>

class ReadOnlyCheckBox: public QCheckBox
{
    Q_OBJECT

public:
    using QCheckBox::QCheckBox;

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
};
