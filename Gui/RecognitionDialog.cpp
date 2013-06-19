#include "RecognitionDialog.hpp"
#include "ui_RecognitionDialog.h"

RecognitionDialog::RecognitionDialog(QAbstractItemModel *model, QWidget *parent):
    QDialog(parent),
    ui(new Ui::RecognitionDialog)
{
    ui->setupUi(this);
    ui->correctSymbol->setModel(model);
}

RecognitionDialog::~RecognitionDialog()
{
    delete ui;
}

QString RecognitionDialog::correctSymbol() const
{
    return ui->correctSymbol->currentText();
}

