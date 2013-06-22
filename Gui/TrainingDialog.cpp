#include "TrainingDialog.hpp"
#include "ui_TrainingDialog.h"

#include <QPushButton>

constexpr int ERROR_MAX = 100;

TrainingDialog::TrainingDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::TrainingDialog),
    m_running(false)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->currentErrorBar->setMaximum(ERROR_MAX);
    ui->bestErrorBar->setMaximum(ERROR_MAX);
    reset();
}

TrainingDialog::~TrainingDialog()
{
    delete ui;
}

void TrainingDialog::start(const int count)
{
    m_running = true;
    ui->iterationsBar->setMaximum(count);
    ui->stateLabel->setText(tr("Running..."));
}

void TrainingDialog::finish(const bool result)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    if (result)
        ui->stateLabel->setText(tr("Success."));
    else
        ui->stateLabel->setText(tr("Failed."));
    m_running = false;
}

void TrainingDialog::setTargetErrorInfo(const qreal targetError, const qreal currentError, const qreal bestError)
{
    int current = ERROR_MAX * (targetError / currentError);
    int best = ERROR_MAX * (targetError / bestError);
    current = qMin(ERROR_MAX, qMax(0, current));
    best = qMin(ERROR_MAX, qMax(0, best));
    ui->currentErrorBar->setValue(current);
    ui->bestErrorBar->setValue(best);
}

void TrainingDialog::setTterationInfo(const int currentIteration, const int maxIterations)
{
    Q_ASSERT(ui->iterationsBar->maximum() == maxIterations);
    ui->iterationsBar->setValue(currentIteration);
}

int TrainingDialog::exec()
{
    if (!m_running)
        reset();
    return QDialog::exec();
}

void TrainingDialog::reject()
{
    emit aborted();
    QDialog::reject();
}

void TrainingDialog::reset()
{
    ui->iterationsBar->setValue(0);
    ui->currentErrorBar->setValue(0);
    ui->bestErrorBar->setValue(0);
    ui->stateLabel->clear();
}
