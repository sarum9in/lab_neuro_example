#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "BooleanController.hpp"
#include "RecognitionController.hpp"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Boolean
    BooleanController *booleanController = new BooleanController(this);
    // TODO ui->trainBooleanButton
    const auto setX = [booleanController](const int state) { booleanController->setX(state == Qt::Checked); };
    const auto setY = [booleanController](const int state) { booleanController->setY(state == Qt::Checked); };
    const auto setResult = [this](const bool result) { ui->booleanResult->setCheckState(result ? Qt::Checked : Qt::Unchecked); };
    setX(ui->booleanX->checkState());
    setY(ui->booleanY->checkState());
    setResult(booleanController->result());
    connect(ui->booleanX, &QCheckBox::stateChanged, setX);
    connect(ui->booleanY, &QCheckBox::stateChanged, setY);
    connect(booleanController, &BooleanController::resultChanged, setResult);
    // Recognition
    RecognitionController *recognitionController = new RecognitionController(this);
    ui->gridField->setSize(ui->gridSize->value());
    ui->symbolSet->setValidator(new QRegExpValidator(QRegExp("\\w*"), ui->symbolSet));
    recognitionController->setSize(ui->gridSize->value());
    connect(ui->symbolSet, &QLineEdit::textChanged, recognitionController, &RecognitionController::setSymbolSet);
    connect(ui->fixRecognitionButton, &QPushButton::clicked, recognitionController, &RecognitionController::fix);
    ui->recognizedValue->setModel(recognitionController->model());
    connect(ui->recognizedValue, &QComboBox::currentTextChanged, recognitionController, &RecognitionController::suggestSymbol);
    connect(recognitionController, &RecognitionController::recognized, ui->recognizedValue, &QComboBox::setCurrentText);
    connect(ui->gridSize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), recognitionController, &RecognitionController::setSize);
    connect(ui->gridField, &GridField::dataChanged, recognitionController, &RecognitionController::setData);
    connect(recognitionController, &RecognitionController::unrecognized, [this](){ ui->recognizedValue->setCurrentIndex(-1); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About"), tr("Neural networks application example."));
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}
