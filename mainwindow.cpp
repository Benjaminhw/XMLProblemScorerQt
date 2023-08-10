#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "problemxmlparser.h"
#include "problemstructure.h"
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QButtonGroup>
#include "qpixmap.h"
#include <QRandomGenerator>
#include <QImage>
#include <QImageReader>

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
    //这个是试卷的总layout
    QVBoxLayout* OverallLayout = new QVBoxLayout;
    //ui->scrollArea->setLayout(OverallLayout);//会压缩
    ui->scrollAreaWidgetContents->setLayout(OverallLayout);
    QSize theSize(150,100);
    ui->scrollAreaWidgetContents->setMinimumSize(theSize);

    //要根据不同的学科选取不同的知识分布
    //main subject主学科 主学科各占20分，数学，物理，英语三门共60分
    for(int x=0;x<6;++x)
    {
        int ProblemsNumberNeeded=0;
        if(x<3)
        {
            //主学科选填都为2
            ProblemsNumberNeeded=2;
        }
        else if(x<5)
        {
            //名著与HarryPotter都为1
            ProblemsNumberNeeded=1;
        }
        else if(x<6)
        {
            //常识为2
            ProblemsNumberNeeded=2;
        }
        QVBoxLayout* SubjectLayout = new QVBoxLayout;
        if(pData->SubjectMap.find(x)!=pData->SubjectMap.end())
        {
            auto y=pData->SubjectMap.find(x).value();
            QLabel* SubjectLabel = new QLabel;
            SubjectLabel->setText(y.SubjectName);
            //用frame就可以把线条加进去了
            QFrame* tempLine = new QFrame;
            tempLine->setFrameShape(QFrame::HLine);
            //增加学科的分界线
            QHBoxLayout* TempHLayout = new QHBoxLayout;
            TempHLayout->addWidget(SubjectLabel);
            TempHLayout->addWidget(tempLine);
            TempHLayout->setStretch(0,0);
            TempHLayout->setStretch(1,1);
            TempHLayout->setContentsMargins(0,5,0,5);
            SubjectLayout->addLayout(TempHLayout);

            //处理选择题区域 主学科预计两题选择
            //同时进行数据处理和UI处理
            //随机挑两题（按size来生成随机数）  //注意，这里主学科至少要两题
            QVector<int> target;
            for(int i=0;i<ProblemsNumberNeeded;++i)
            {
                int tempsize = y.MultiChoicesSet.size();
                int w=QRandomGenerator::global()->bounded(tempsize);
                while(target.contains(w))
                {
                    w=QRandomGenerator::global()->bounded(tempsize);
                }
                target.emplaceBack(w);
            }
            //UI处理
            QVector<QButtonGroup*> tempVectorButtonGroup;
            //for(auto& z:y.MultiChoicesSet)
            for(auto& v:target)
            {
                auto z =y.MultiChoicesSet[v];
                QVBoxLayout* ProblemLayout = new QVBoxLayout;
                QLabel* ProblemHead = new QLabel;
                if(z.head!="")
                {
                    ProblemHead->setText(z.head);
                    ProblemHead->setWordWrap(true);
                }
                else if(z.picturepath!="")
                {
                    QImage image(z.picturepath);
                    QImageReader imageReader(z.picturepath);
                    QSize PicSize=imageReader.size();
                    QImage imageC=image.scaled(PicSize,Qt::KeepAspectRatio);
                    //ProblemHead->setPixmap(QPixmap(z.picturepath));
                    ProblemHead->setPixmap(QPixmap::fromImage(imageC));
                    //ProblemHead->setMinimumHeight(PicSize.height()-100);
                    //ProblemHead->setMinimumWidth(PicSize.width()-100*PicSize.width()/PicSize.height());
                    QSize PicSizeMax=QSize(PicSize.height()+100,PicSize.width()+100*PicSize.width()/PicSize.height());
                    //No Big Use
                    //QSize PicSizeMin=QSize(PicSize.height()-100,PicSize.width()-100*PicSize.width()/PicSize.height());
                    ProblemHead->setMinimumSize(PicSize);
                    ProblemHead->setMaximumSize(PicSizeMax);
                    ProblemHead->setContentsMargins(0,0,0,10);//不设置空白的话莫名其妙会出现一些重叠
                    ProblemHead->setScaledContents(true);
                }
                ProblemLayout->addWidget(ProblemHead);
                //QFrame* tempFrame = new QFrame;
                QButtonGroup* tempGroup = new QButtonGroup;
                for(int k=0;k<z.choices.size();++k)
                {
                    QPushButton* TempPushButton = new QPushButton;
                    TempPushButton->setText(/*z.nameofchoice[k]+". "+*/z.choices[k]);
                    TempPushButton->setCheckable(true);
                    TempPushButton->setMaximumWidth(180);
                    tempGroup->addButton(TempPushButton);
                    tempGroup->setId(TempPushButton,k);
                    ProblemLayout->addWidget(TempPushButton);
                }
                tempVectorButtonGroup.emplaceBack(tempGroup);
                SubjectLayout->addLayout(ProblemLayout);
                //DATA-录入答案。选择题录选项，填空题录字符串（多选项匹配）
                AnswerSet tempAnswerSet;
                //tempAnswerSet.Type=0; //类型为0代表是选择题
                //tempAnswerSet.AnswerNum=z.answer;
                pData->Answers.AnswerNums.emplace_back(z.answer);
            }
            //UI-一个学科的[选择题]处理完，把选项全部推入ButtonGroup的二维数组，由STL控制内存的删除
            ButtonGroups.emplaceBack(tempVectorButtonGroup);
        }
        //UI-学科添加完毕
        OverallLayout->addLayout(SubjectLayout);
    }
    //Harry Potter占10分

    //红楼和三国(classics)占10分 但这样可能2题都是红楼或者两题都是三国

    //common sense占20分

    OverallLayout->addStretch();
}

void MainWindow::on_pushButton_2_clicked()
{
    //交卷
    TestScorer(&Problems);
}

void MainWindow::TestScorer(pAllData pData)
{
    //qDebug() << "Nothing happened.";
    int i =0;
    for(auto& x:this->ButtonGroups)
    {
        for(auto& y:x)
        {
            qDebug() <<"选择的ID是" << y->checkedId();
            qDebug() <<"答案的ID是" << pData->Answers.AnswerNums[i];
            if(y->checkedId()==pData->Answers.AnswerNums[i])
            {
                //一题五分
                pData->Score+=5;
            }
            ++i;
        }
    }
    qDebug()<<pData->Score;
}

