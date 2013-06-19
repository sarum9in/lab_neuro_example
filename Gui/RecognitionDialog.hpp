#pragma once

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class RecognitionDialog;
}

class RecognitionDialog: public QDialog
{
    Q_OBJECT

public:
    RecognitionDialog(QAbstractItemModel *model, QWidget *parent=nullptr);
    ~RecognitionDialog();

    QString correctSymbol() const;

private:
    Ui::RecognitionDialog *ui;
};
