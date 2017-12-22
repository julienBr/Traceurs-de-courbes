#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <iostream>

#include <abstractfunction.h>
#include <rpn_utility.h>
#include <defines.h>


class Function : public AbstractFunction
{
    Q_OBJECT

public :

    explicit Function(const QString& _id, const QString & formula, QWidget* _parent = nullptr );
    ~Function();

    virtual float getOnePoint(float x);
    virtual inline QString getFormula() const { return QString("Function %1 %2").arg(id, rpn_u->getRawForm()); }

    std::pair<unsigned, QPointF*> getPoints(float min, float max);

    static AbstractFunction* loadFunction(const QString& input, QWidget* parent = nullptr);


public slots :

     void change();


protected :

    QLineEdit* equation;
    RPN_utility* rpn_u;

};

#endif // FUNCTION_H
