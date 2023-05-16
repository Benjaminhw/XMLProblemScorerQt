#ifndef PROBLEMSTRUCTURE_H
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
    Math,
    English,
    Physics,
    ClassicalWorks,
    HarryPotter,
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
    QVector<QString> choices;
    int answer;
    void ShowYourSelf() override;
};

class BriefAnswerQuestion:public BaseProblemStructure
{
 public:
    QString answer;
    void ShowYourSelf() override;
};

class BlankFilling:public BaseProblemStructure
{
 public:
    QString answer;
    void ShowYourSelf() override;
};

class SubjectSets{
public:
    int Subject;
    QVector<MultiChoices> MultiChoicesSet;
    QVector<BriefAnswerQuestion> BriefAnswerSet;
    QVector<BlankFilling> BlankFillingSet;
};

typedef class AllData{
public:
    QMap<int,SubjectSets> SubjectMap;
}*pAllData;
