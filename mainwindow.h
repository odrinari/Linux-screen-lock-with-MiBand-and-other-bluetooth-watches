#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_DialogButtons_accepted();

    void on_DialogButtons_rejected();

    void on_FactorySettings_clicked();

    void on_help_triggered();

    void on_about_triggered();

private:
    Ui::MainWindow *ui;
    void ReadSettings();
};


#endif // MAINWINDOW_H
