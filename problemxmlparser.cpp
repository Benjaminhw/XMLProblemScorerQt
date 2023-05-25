#include "problemxmlparser.h"
#include "tinyxml/tinyxml.h"
#include "QDebug"


//应该可以通过搜寻的对象反过来去命名
ProblemXMLParser::ProblemXMLParser(QString problemfilepath,pAllData pData)
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
        if(theSubjectName)
        {
            SubjectSets tempSubjectSet;
            TiXmlElement* multichoicepart=theSubjectName->FirstChildElement("multichoicepart");
            if(multichoicepart)
            {
                TiXmlElement* multichoice=multichoicepart->FirstChildElement("multichoice");
                if(multichoice)
                {
                    for(;multichoice!=nullptr;multichoice=multichoice->NextSiblingElement("multichoice"))
                    {
                        MultiChoices tempMulti;
                        TiXmlElement* head=multichoice->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        TiXmlElement* picture=multichoice->FirstChildElement("picture");
                        if(head||picture)
                        {
                            //GetText失败会返回0所以可以直接用
                            //题目或者图片一定要有
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
                                for(auto choice=choices->FirstChildElement();choice!=nullptr;choice->NextSiblingElement())
                                {
                                    tempMulti.nameofchoice.emplaceBack(choice->Value());
                                    //qDebug()<<choice->Value();
                                    tempMulti.choices.emplaceBack(choice->GetText());
                                    qDebug()<<choice->GetText();
                                }
                            }
                        }
                        else
                        {//边界的处理，如果是空指针，或者题目没有内容，就直接开始读下一个节点
                            qDebug()<<"Empty Problem!"<<Qt::endl;
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
                        BlankFilling tempBF;
                        TiXmlElement* head=blankfilling->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        if(head&& head->GetText())
                        {
                            tempBF.head = head->GetText();//名称录入
                            qDebug()<<tempBF.head;
                        }
                        else
                        {//边界的处理，如果是空指针，就直接break2
                            break;
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
                        BriefAnswerQuestion tempBAQ;
                        TiXmlElement* head=briefanswerquestion->FirstChildElement("head");//名称 寻找property的第一个名为name的孩子
                        if(head&& head->GetText())
                        {
                            tempBAQ.head = head->GetText();//名称录入
                            qDebug()<<head->GetText();
                        }
                        else
                        {//边界的处理，如果是空指针，就直接break2
                            break;
                        }
                    }
                }
            }
        }
    }
}


//void ProblemXMLParser::TinyXMLReader(const char* cfgfilepath,QMap<QString,struct node>& ModelConfigDataMap)
//{
//    TiXmlDocument* tinyXMLDoc = new TiXmlDocument;//创建一个XML读取器
//    if(!tinyXMLDoc->LoadFile(cfgfilepath))
//    {
//        // 读取失败，打印失败原因
//        qDebug()<<"Could not load example xml file"+ QString::fromStdString(cfgfilepath)+QString::fromStdString(tinyXMLDoc->ErrorDesc());
//        return ;
//    }
//    //打开成功，继续操作
//    //std::string data1;
//    //tinyXMLDoc->Parse(data1.c_str());
//    qDebug()<<"XML parse SUCCESS!";
//    //qDebug()<<data1.c_str();
//    TiXmlElement* properties;
//    properties=tinyXMLDoc->RootElement();
//    TiXmlElement* property=properties->FirstChildElement("property1");
//    struct node tempNode;
//    QString tempName;
//    if(property)
//    {
//        for(;property!=nullptr;property=property->NextSiblingElement("property1"))//通过第二层节点的兄弟节点来遍历所有的第二层节点（全部都叫property1）
//        {
//            TiXmlElement* name=property->FirstChildElement("name");//名称 寻找property的第一个名为name的孩子
//            if(name)
//            {
//                tempName = name->GetText();//名称录入
//                qDebug()<<name->GetText();
//            }
//            else
//            {//边界的处理，如果是空指针，就直接break2
//                break;
//            }
//            TiXmlElement* value=property->FirstChildElement("value");//数值
//            if(value)
//            {
//                tempNode.value = std::atof(value->GetText());//将字符串转换成双精度浮点数
//                qDebug()<<value->GetText();
//            }
//            else
//            {
//                break;
//            }
//            TiXmlElement* unit=property->FirstChildElement("unit");//单位
//            if(unit)
//            {
//                tempNode.unit = QString::fromStdString(unit->GetText());
//                qDebug()<<unit->GetText();
//            }
//            else
//            {
//                break;
//            }

//            TiXmlElement* parameter=property->FirstChildElement("parameter");
//            if(parameter)
//            {//输入一次函数的两个参数
//                TiXmlElement* a=parameter->FirstChildElement("a");
//                if(a)
//                {
//                    tempNode.parametera=std::atof(a->GetText());
//                    qDebug()<<std::atof(a->GetText());
//                }
//                TiXmlElement* b=parameter->FirstChildElement("b");
//                if(b)
//                {
//                    tempNode.parameterb=std::atof(b->GetText());
//                    qDebug()<<std::atof(b->GetText());
//                }
//            }
//            //只按照顺序读取录入三个属性 多的不进行读入
//            ModelConfigDataMap.insert(tempName,tempNode);
//        }
//    }
//    data.codeConfigDataMap=ModelConfigDataMap;//存进去，最后全部从data里读，这就是其意义
//}
