#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "RecognitionController.hpp"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Recognition
    ui->gridField->setSize(ui->gridSize->value());
    ui->symbolSet->setValidator(new QRegExpValidator(QRegExp("\\w*"), ui->symbolSet));
    RecognitionController *recognitionController = new RecognitionController(this);
    connect(ui->symbolSet, &QLineEdit::textChanged, recognitionController, &RecognitionController::setSymbolSet);
    connect(ui->fixRecognitionButton, &QPushButton::clicked, recognitionController, &RecognitionController::fix);
    ui->recognizedValue->setModel(recognitionController->model());
    connect(ui->recognizedValue, &QComboBox::currentTextChanged, recognitionController, &RecognitionController::suggestSymbol);
    connect(recognitionController, &RecognitionController::recognized, ui->recognizedValue, &QComboBox::setCurrentText);
    connect(ui->gridSize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), recognitionController, &RecognitionController::setSize);
    connect(ui->gridField, &GridField::dataChanged, recognitionController, &RecognitionController::setData);
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
