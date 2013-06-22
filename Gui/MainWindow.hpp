#pragma once

#include "TrainingDialog.hpp"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

public slots:
    void showAbout();
    void showAboutQt();

private:
    Ui::MainWindow *ui;
    TrainingDialog *m_functionTrainingDialog;
};
