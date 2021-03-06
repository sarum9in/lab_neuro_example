#pragma once

#include <QDialog>

namespace Ui {
class TrainingDialog;
}

class TrainingDialog: public QDialog
{
    Q_OBJECT

public:
    explicit TrainingDialog(QWidget *parent = 0);
    ~TrainingDialog();

signals:
    void aborted();

public slots:
    void start(const int count);
    void finish(const bool result);
    void setTargetErrorInfo(const qreal targetError, const qreal currentError, const qreal bestError);
    void setTterationInfo(const int currentIteration, const int maxIterations);

    int exec() override;
    void reject() override;

private slots:
    void reset();

private:
    Ui::TrainingDialog *ui;
    bool m_running;
};
