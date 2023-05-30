#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include "problemstructure.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_clicked();

private:
    AllData Problems;
    void ProblemPrinter(pAllData pData);
};
#endif // MAINWINDOW_H
