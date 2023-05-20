#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "problemxmlparser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //要有数据结构 框架 和抽题的比率
    ProblemXMLParser("ProblemTest.xml",&Problems);

}

