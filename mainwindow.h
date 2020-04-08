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

private:
    Ui::MainWindow *ui;
    void ReadSettings();
};

#endif // MAINWINDOW_H
