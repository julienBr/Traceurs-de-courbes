#ifndef RPN_utility_H
#define RPN_utility_H

#include <stack>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <utility>
#include <algorithm>
#include <QStringList>
#include <QTextStream>
#include <QStack>
#include <cmath>


enum associativity { NONE, LEFT, RIGHT };

class RPN_utility
{
public:
    RPN_utility(const QString& raw, const QString& rpn = QString(""));


    float calc(float x);

    void setFormula(const QString& new_form);
    inline QString getRawForm() const {return raw_formula; }




    /* Parser */
    QString parse(QString);
    static bool isnumber(const QString&);
    static bool isoperator(const QString&);
    static bool iscommonfunction(const QString &);

private :
    QString raw_formula;
    QStringList rpn_formula;



    void first_parser(QString &);
    void second_parser(QString&);
    void third_parser(QString&);
    QString main_parser(QString& formula);

    static std::map< QChar, float(*)(const float& a, const float& b) > OPERATION;
    static std::map <QChar, std::pair<int, associativity>> TAB_OP;
    static std::map< QString, float(*)(const float& a) > COMMON_FUNCTION;


};

#endif // RPN_utility_H
