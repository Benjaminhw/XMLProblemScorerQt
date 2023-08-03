#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "problemxmlparser.h"
#include "problemstructure.h"
#include <QTimer>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(200,200);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ProblemXMLParserC MainParserFactory;
    //要有数据结构 框架 和抽题的比率
    MainParserFactory.ProblemXMLParser("D:/learning/ProblemScorer/ProblemScorer/ProblemTest.xml",&Problems);
    ProblemPrinter(&Problems);

    targetTime = QDateTime::currentDateTime().addSecs(2400);

    timer = new QTimer;
    timer1 = new QTimer;
    timer1->setSingleShot(false);
    timer1->setInterval(1000);//一秒改变一次计时
    timer->setInterval(2400000);//测试时间为40分钟
    QObject::connect(timer, &QTimer::timeout, this,[&](){
        QMessageBox::information(nullptr, "提示", "考试时间已用完，考试结束！");
        on_pushButton_2_clicked();
        timer->stop();
    });
    QObject::connect(timer1, &QTimer::timeout, this,[&](){
        QDateTime currentTime = QDateTime::currentDateTime();
        qint64 secondsRemaining = currentTime.secsTo(targetTime);
        if (secondsRemaining <= 0) {
            // 倒计时结束
            ui->label_2->setText(u8"倒计时结束");
            timer1->stop();
        }else {
            // 更新倒计时显示
            QString timeString = QString::number(secondsRemaining) + " 秒";
            ui->label_2->setText(timeString);
        }
    });
    timer1->start();
    timer->start();
}


//因为调用ui很麻烦，所以还是放到这个位置来处理
void MainWindow::ProblemPrinter(pAllData pData)
{
    //要根据不同的学科选取不同的知识分布
    //main subject主学科
    for(int x=0;x<3;++x)
    {
        if(pData->SubjectMap.find(x)!=pData->SubjectMap.end())
        {
            auto y=pData->SubjectMap.find(x).value();
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{

}

