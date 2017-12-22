#include "function.h"


Function::Function(const QString& _id, const QString & formula, QWidget* _parent)
    :
    AbstractFunction(_id, _parent),
    equation(nullptr),
    rpn_u(nullptr)
{
  rpn_u = new RPN_utility( formula );

  //Widgets
  equation = new QLineEdit(QString("y=%1").arg(rpn_u->getRawForm()));
  bottom_layout->addWidget(equation);

  //Connections
  QObject::connect(equation, SIGNAL(returnPressed()), this , SLOT(change()));
}

Function::~Function()
{
    delete rpn_u;
    delete equation;
}

float Function::getOnePoint(float x)
{
    return rpn_u->calc(x);
}

std::pair<unsigned, QPointF *> Function::getPoints(float min, float max)
{
    QPointF* tab = new QPointF[FUNCTION_PRECISION];
    float grad =(  max - min  ) / FUNCTION_PRECISION;

    float x = min;

    for (unsigned i = 0; i < FUNCTION_PRECISION; i ++) {
        tab[i] = QPointF(x, rpn_u->calc(x) );
        x+= grad;
    }

    return std::make_pair(FUNCTION_PRECISION,tab);
}

void Function::change()
{
    QString formula = equation->text();
    QStringList list = formula.split(QChar('='));

    rpn_u->setFormula(list[1]);
}

//Static Function
AbstractFunction *Function::loadFunction(const QString & input, QWidget *parent)
{
    QStringList list = input.split(QChar(' '));
    return  new Function(list[1], list[2], parent);

}



