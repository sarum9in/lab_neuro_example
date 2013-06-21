#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "BooleanController.hpp"
#include "RecognitionController.hpp"
#include "FunctionController.hpp"

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
    // Function
    FunctionController *functionController = new FunctionController(ui->functionPlot, this);
    const auto setShowOriginalFunction = [functionController](const int state) { functionController->setShowOriginalFunction(state == Qt::Checked); };
    const auto setShowNeuralFunction = [functionController](const int state) { functionController->setShowNeuralFunction(state == Qt::Checked); };
    setShowOriginalFunction(ui->showOriginalFunction->checkState());
    setShowNeuralFunction(ui->showNeuralFunction->checkState());
    connect(ui->showOriginalFunction, &QCheckBox::stateChanged, setShowOriginalFunction);
    connect(ui->showNeuralFunction, &QCheckBox::stateChanged, setShowNeuralFunction);
    functionController->setMinX(ui->functionMinX->value());
    functionController->setMaxX(ui->functionMaxX->value());
    connect(ui->functionMinX, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged), functionController, &FunctionController::setMinX);
    connect(ui->functionMaxX, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged), functionController, &FunctionController::setMaxX);
    connect(ui->trainFunctionButton, &QPushButton::clicked, [this, functionController](){ functionController->setScript(ui->script->toPlainText()); functionController->train(); });
    connect(functionController, &FunctionController::scriptError, [this](const QString &error){ QMessageBox::warning(this, tr("Script error"), error); });
    connect(this, &QObject::destroyed, functionController, &FunctionController::detach);
    // set colors
    {
        QPen pen = functionController->neuralFunction()->pen();
        pen.setColor(Qt::red);
        functionController->neuralFunction()->setPen(pen);
    }
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
