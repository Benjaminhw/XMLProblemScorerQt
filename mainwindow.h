#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include "qabstractbutton.h"
#include "qdatetime.h"
#include "problemstructure.h"
#include <QMainWindow>
#include <QLineEdit>

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
    void on_pushButton_clicked();  // 善意的提醒

    void on_pushButton_2_clicked();

private:
    AllData Problems;
    void ProblemPrinter(pAllData pData);
    QDateTime targetTime;
    QTimer* timer;
    QTimer* timer1;
    int RemainingTime;
    void TestScorer(pAllData pData);
    QVector<QVector<QButtonGroup*>> ButtonGroups;
    //QVector<QVector<int>> PickedNumber;
    QVector<QVector<int>> PickedNumber;
    QVector<QLineEdit*> Blanks;
};
#endif // MAINWINDOW_H
