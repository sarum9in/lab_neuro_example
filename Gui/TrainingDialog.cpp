#include "TrainingDialog.hpp"
#include "ui_TrainingDialog.h"

#include <QPushButton>

constexpr int ERROR_MAX = 100;

TrainingDialog::TrainingDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::TrainingDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->currentErrorBar->setMaximum(ERROR_MAX);
    ui->bestErrorBar->setMaximum(ERROR_MAX);
}

TrainingDialog::~TrainingDialog()
{
    delete ui;
}

void TrainingDialog::start(const int count)
{
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

void TrainingDialog::reject()
{
    emit aborted();
    QDialog::reject();
}
