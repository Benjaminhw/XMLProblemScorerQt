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

    //targetTime = QDateTime::currentDateTime().addSecs(2400);
    RemainingTime = 2400;

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
        --RemainingTime;
        //不用带日期的函数原因是
        //QDateTime currentTime = QDateTime::currentDateTime();
        //qint64 secondsRemaining = currentTime.secsTo(targetTime);
        //int secondsRemaining = targetTime;
        if (RemainingTime <= 0) {
            // 倒计时结束
            ui->label_2->setText(u8"倒计时结束");
            timer1->stop();
        }else {
            // 更新倒计时显示
            QString timeString = QString::number(RemainingTime/60) + "分" + QString::number(RemainingTime%60) + " 秒";
            ui->label_2->setText(timeString);
        }
    });
    timer1->start();
    timer->start();
}


//因为调用ui很麻烦，所以还是放到这个位置来处理
void MainWindow::ProblemPrinter(pAllData pData)
{
    QVBoxLayout* TempVLayout = new QVBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(TempVLayout);

    //要根据不同的学科选取不同的知识分布
    //main subject主学科 主学科各占20分，数学，物理，英语三门共60分
    for(int x=0;x<3;++x)
    {
        QVBoxLayout* SubjectLayout = new QVBoxLayout;
        if(pData->SubjectMap.find(x)!=pData->SubjectMap.end())
        {
            auto y=pData->SubjectMap.find(x).value();
            QLabel* SubjectLabel = new QLabel;
            SubjectLabel->setText(y.SubjectName);
            //用frame就可以把线条加进去了
            QFrame* tempLine = new QFrame;
            tempLine->setFrameShape(QFrame::HLine);
            QHBoxLayout* TempHLayout = new QHBoxLayout;
            TempHLayout->addWidget(SubjectLabel);
            TempHLayout->addWidget(tempLine);
            TempHLayout->setStretch(0,0);
            TempHLayout->setStretch(1,1);
            SubjectLayout->addLayout(TempHLayout);
        }
        TempVLayout->addLayout(SubjectLayout);

    }
    //Harry Potter占10分

    //红楼和三国占10分

    //common sense占10分
}

void MainWindow::on_pushButton_2_clicked()
{
    //交卷
    TestScorer();
}

void MainWindow::TestScorer()
{
    //qDebug() << "Nothing happened";
}

