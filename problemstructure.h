﻿#ifndef PROBLEMSTRUCTURE_H
#define PROBLEMSTRUCTURE_H
#pragma once
#include <QString>
#include <QVector>
#include <QMap>

#endif // PROBLEMSTRUCTURE_H

enum class ProblemType
{
    MultiChoice,
    BriefAnswerQuestion,
    BlankFilling,
};

//可以用来统一读入统一处理
enum class ProblemSubject
{
    //main subject
    Math,
    English,
    Physics,
    //assisted subject
    ClassicalWorks,
    HarryPotter,
    //
    Commonsense,
};

class BaseProblemStructure
{
public:
    QString head="";
    QString picturepath="";
    virtual void ShowYourSelf()=0;
};

class MultiChoices:public BaseProblemStructure
{
 public:
    QVector<QString> nameofchoice;      //选项的名称
    QVector<QString> choices;           //选项的内容
    int answer=-1;                      //答案 默认-1
    void ShowYourSelf() override{};     //将自己打印至试卷的函数
};

class BriefAnswerQuestion:public BaseProblemStructure
{
 public:
    QString answer;
    void ShowYourSelf() override{};
};

class BlankFilling:public BaseProblemStructure
{
 public:
    QString answer;
    void ShowYourSelf() override{};
};

class SubjectSets{
public:
    int Subject;
    QString SubjectName;
    QVector<MultiChoices> MultiChoicesSet;
    QVector<BriefAnswerQuestion> BriefAnswerSet;
    QVector<BlankFilling> BlankFillingSet;
};

class AnswerSet{
public:
//    int Subject;
//    QString SubjectName;
//    QVector<int> MultiChoicesAnswers;
//    QVector<QString> BriefAnswerAnswers;
//    QVector<QString> BlankFillingAnswers;
    QVector<int> AnswerNums;
    QVector<QString> AnswerStrings;
};


typedef class AllData{
public:
    QMap<int,SubjectSets> SubjectMap;
    AnswerSet Answers;
    int Score=0;
}*pAllData;
