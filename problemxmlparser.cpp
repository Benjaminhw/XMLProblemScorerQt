#include "problemxmlparser.h"
#include "tinyxml/tinyxml.h"
#include "QDebug"


void ProblemXMLParserC::ProblemPrinter(pAllData pData)
{
    //要根据不同的学科选取不同的知识分布
    //main subject主学科

}

//应该可以通过搜寻的对象反过来去命名
void ProblemXMLParserC::ProblemXMLParser(QString problemfilepath,pAllData pData)
{
    TiXmlDocument* tinyXMLDoc = new TiXmlDocument;//创建一个XML读取器
    if(!tinyXMLDoc->LoadFile(problemfilepath.toStdString().data()))
    {
        // 读取失败，打印失败原因
        qDebug()<<"Could not load xml file"+ problemfilepath+QString::fromStdString(tinyXMLDoc->ErrorDesc());
        return ;
    }
    qDebug()<<"XML parse SUCCESS!";
    TiXmlElement* ProblemLibrary;
    ProblemLibrary=tinyXMLDoc->RootElement();


    //    QMap<int,QString> NameMap;
    //    NameMap.insert((int)ProblemSubject::Math,QString("math"));
    //    NameMap.insert((int)ProblemSubject::English,QString("english"));
    //    NameMap.insert((int)ProblemSubject::Physics,QString("physics"));

    //2023-05-25 BenjaminH @MZ Tech 真好用
    QVector<QString> Names={"math",
                            "english",
                            "physics",
                            "classicalworks",
                            "harrypotter",
                            "Commonsense",};

    int count=-1;
    for(auto& x:Names)
    {
        ++count;
        TiXmlElement* theSubjectName=ProblemLibrary->FirstChildElement(x.toStdString().c_str());
        //这一段就是数据结构总结的很好才可以
        if(theSubjectName)
        {
            SubjectSets tempSubjectSet;
            tempSubjectSet.Subject=count;
            TiXmlElement* multichoicepart=theSubjectName->FirstChildElement("multichoicepart");
            if(multichoicepart)
            {
                TiXmlElement* multichoice=multichoicepart->FirstChildElement("multichoice");
                if(multichoice)
                {
                    for(;multichoice!=nullptr;multichoice=multichoice->NextSiblingElement("multichoice"))
                    {
                        TiXmlElement* head=multichoice->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        TiXmlElement* picture=multichoice->FirstChildElement("picture");
                        //题目或者图片一定要有，以这个来决定是否立项
                        if((head && head->GetText())||(picture&&picture->GetText()))
                        {
                            MultiChoices tempMulti;
                            //GetText失败会返回0所以可以直接用
                            if(head && head->GetText())
                            {
                                tempMulti.head = head->GetText();//名称录入
                                qDebug()<<tempMulti.head;
                            }
                            if(picture&&picture->GetText())
                            {
                                tempMulti.picturepath=picture->GetText();
                                qDebug()<<tempMulti.picturepath;
                            }
                            TiXmlElement* choices=multichoice->FirstChildElement("choices");
                            if(choices)
                            {
                                for(auto choice=choices->FirstChildElement();choice!=nullptr;choice=choice->NextSiblingElement())
                                {
                                    tempMulti.nameofchoice.emplaceBack(choice->Value());
                                    qDebug()<<choice->Value();
                                    if(choice->GetText())
                                    {
                                        tempMulti.choices.emplaceBack(choice->GetText());
                                        qDebug()<<choice->GetText();
                                    }
                                }
                            }
                            //答案录入
                            TiXmlElement* answer=multichoice->FirstChildElement("answer");
                            if(answer)
                            {
                                tempMulti.answer=atoi(answer->GetText());
                            }
                            tempSubjectSet.MultiChoicesSet.emplaceBack(tempMulti);
                        }
                        else
                        {//边界的处理，如果是空指针，或者题目没有内容，就直接开始读下一个节点
                            //qDebug()<<"Empty Problem!"<<Qt::endl;
                            continue;
                        }
                    }
                }
            }
            TiXmlElement* blankfillingpart=theSubjectName->FirstChildElement("blankfillingpart");
            if(blankfillingpart)
            {
                TiXmlElement* blankfilling=blankfillingpart->FirstChildElement("blankfilling");
                if(blankfilling)
                {
                    for(;blankfilling!=nullptr;blankfilling=blankfilling->NextSiblingElement("blankfilling"))
                    {
                        TiXmlElement* head=blankfilling->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        TiXmlElement* picture=blankfilling->FirstChildElement("picture");
                        if((head && head->GetText())||(picture&&picture->GetText()))
                        {
                            BlankFilling tempBF;
                            if(head && head->GetText())
                            {
                                tempBF.head = head->GetText();//名称录入
                                qDebug()<<tempBF.head;
                            }
                            if(picture && picture->GetText())
                            {
                                tempBF.picturepath=picture->GetText();
                                qDebug()<<tempBF.picturepath;
                            }
                            TiXmlElement* answer=blankfilling->FirstChildElement("answer");
                            if(answer)
                            {
                                tempBF.answer=answer->GetText();
                            }
                            tempSubjectSet.BlankFillingSet.emplaceBack(tempBF);
                        }
                        else
                        {//边界的处理，如果是空指针，就直接continue
                            continue;
                        }
                    }
                }
            }
            TiXmlElement* briefanswerquestionpart=theSubjectName->FirstChildElement("briefanswerquestionpart");
            if(briefanswerquestionpart)
            {
                TiXmlElement* briefanswerquestion=briefanswerquestionpart->FirstChildElement("briefanswerquestion");
                if(briefanswerquestion)
                {
                    for(;briefanswerquestion!=nullptr;briefanswerquestion=briefanswerquestion->
                        NextSiblingElement("briefanswerquestion"))
                    {
                        TiXmlElement* head=briefanswerquestion->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        TiXmlElement* picture=briefanswerquestion->FirstChildElement("picture");
                        if((head && head->GetText())||(picture&&picture->GetText()))
                        {
                            BriefAnswerQuestion tempBAQ;
                            if(head && head->GetText())
                            {
                                tempBAQ.head = head->GetText();//名称录入
                                qDebug()<<tempBAQ.head;
                            }
                            if(picture && picture->GetText())
                            {
                                tempBAQ.picturepath=picture->GetText();
                                qDebug()<<tempBAQ.picturepath;
                            }
                            TiXmlElement* answer=briefanswerquestion->FirstChildElement("answer");
                            if(answer)
                            {
                                tempBAQ.answer=answer->GetText();
                            }
                            tempSubjectSet.BriefAnswerSet.emplaceBack(tempBAQ);
                        }
                        else
                        {//边界的处理，如果是空指针，就直接continue
                            continue;
                        }
                    }
                }
            }
            pData->SubjectMap.insert(count,tempSubjectSet);
        }
    }
}
