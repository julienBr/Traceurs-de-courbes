#include "parametricfunction.h"


ParametricFunction::ParametricFunction(const QString& id, const QString& _x_formula, const QString& _y_formula, const QPointF _bornes, QWidget* parent)
    : AbstractFunction(id, parent),
      points(nullptr),
      x_formula(_x_formula),
      y_formula(_y_formula),
      bornes(_bornes),
      x_equation(nullptr),
      y_equation(nullptr)
{
    setPoints();

    y_equation = new QLineEdit(QString("y=%1").arg(y_formula), this);
    x_equation = new QLineEdit(QString("x=%1").arg(x_formula), this);

    bottom_layout->addWidget(x_equation);
    bottom_layout->addWidget(y_equation);

    QObject::connect(x_equation, SIGNAL(returnPressed()), this, SLOT(changeX()));
    QObject::connect(y_equation, SIGNAL(returnPressed()), this, SLOT(changeY()));
}

ParametricFunction::~ParametricFunction()
{
    delete[] points;
    delete x_equation;
    delete y_equation;
}

std::pair<unsigned, QPointF*> ParametricFunction::getPoints(float min, float max)
{
    Q_UNUSED(min)
    Q_UNUSED(max)
    return std::make_pair( PARAMETRIC_PRECISION, points) ;
}

float ParametricFunction::getOnePoint(float x)
{
    Q_UNUSED(x)
    return .0f;
}

void ParametricFunction::setPoints()
{

    points = new QPointF[PARAMETRIC_PRECISION];
    float grad = ( bornes.y() - bornes.x() ) / PARAMETRIC_PRECISION;
    RPN_utility* rpn = new RPN_utility(x_formula);
    float t = grad + bornes.x();

    for (unsigned i = 0; i < PARAMETRIC_PRECISION; i++) {
        points[i].setX(rpn->calc(t));
        t += grad;
    }
    delete rpn;

    rpn = new RPN_utility(y_formula);
    t = grad + bornes.x()
            ;
    for (unsigned i = 0; i < PARAMETRIC_PRECISION; i++) {
        points[i].setY(rpn->calc(t));
        t += grad;
    }

    delete rpn;
}

void ParametricFunction::changeX()
{
    QString formula = x_equation->text();
    QStringList list = formula.split(QChar('='));
    x_formula = list[1];
    setPoints();
    x_equation->setText(QString("y=%1").arg(y_formula));
}

void ParametricFunction::changeY()
{

    QString formula = y_equation->text();
    QStringList list = formula.split(QChar('='));
    y_formula = list[1];
    setPoints();
    y_equation->setText(QString("x=%1").arg(x_formula));
}

//Static function
AbstractFunction *ParametricFunction::loadFunction(const QString & input, QWidget *parent)
{
    QStringList list = input.split(QChar(' '));
    QStringList list_bornes = list[4].split(QRegularExpression("[[\\;\\]]"), QString::SkipEmptyParts);

    return  new ParametricFunction(list[1], list[2], list[3], QPointF(list_bornes[0].toFloat(), list_bornes[1].toFloat()), parent);
}
