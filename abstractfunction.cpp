#include "abstractfunction.h"

#include "function.h"
#include "parametricfunction.h"


AbstractFunction::AbstractFunction(const QString & _id, QWidget *parent)
    : QWidget(parent),
      id(_id),
      is_selected(false),
      main_layout(nullptr),
      top_layout(nullptr),
      check(nullptr),
      color_button(nullptr),
      icon_color(nullptr),
      pix_color(nullptr),
      id_func(nullptr),
      bottom_layout(nullptr),
      bdelete(nullptr)
{
    main_layout = new QVBoxLayout(this);
    top_layout = new QHBoxLayout;
    bottom_layout = new QVBoxLayout;

    //Check box
    check = new QCheckBox;
    check->setChecked(true);

    //Id Label
    id_func = new QLineEdit;
    id_func->setText(QString("%1").arg(id));

    //Color_button
    pix_color = new QPixmap(10,10);
    color = randomColor();
    pix_color->fill(color);
    icon_color = new QIcon(*pix_color);
    color_button = new QPushButton(*icon_color,QString(""), this);

    //Delete button
    bdelete = new QPushButton(QIcon(":/images/delete_button.png"),QString(""),this);
    bdelete->setVisible(false); /* need debug */

    //Layout management
    top_layout->addWidget(check,0,Qt::AlignLeft);
    top_layout->addWidget(color_button,0,Qt::AlignLeft);
    top_layout->addWidget(id_func,0,Qt::AlignLeft);
    top_layout->addWidget(bdelete);

    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);

    setLayout(main_layout);

    //Connections
    QObject::connect(this, SIGNAL(selected(AbstractFunction*)), parentWidget(), SLOT(updateSelected(AbstractFunction*)));
    QObject::connect(this, SIGNAL(delete_me(AbstractFunction*)), parentWidget(), SLOT(delete_func(AbstractFunction*)));
    QObject::connect(this, SIGNAL(needUpdate()), parentWidget(), SLOT(updateFunctions()));
    QObject::connect(bdelete,SIGNAL(clicked(bool)), this, SLOT(emit_delete_me()));
    QObject::connect(color_button, SIGNAL(clicked(bool)), this, SLOT(choseColor()));
    QObject::connect(id_func, SIGNAL(returnPressed()), this, SLOT(changeName()));
    QObject::connect(check, SIGNAL(toggled(bool)), parentWidget(), SLOT(updateFunctions()));
}

AbstractFunction::~AbstractFunction()
{
    delete main_layout;
    delete top_layout;
    delete id_func;
    delete id_func;
    delete bottom_layout;
    delete color_button;
    delete icon_color;
    delete pix_color;
    delete bdelete;
    delete check;
}

void AbstractFunction::mouseReleaseEvent(QMouseEvent *)
{
    is_selected = !is_selected;

    if (is_selected)
        emit selected(this);
    else
        emit selected(nullptr);

    update();
    emit needUpdate();
}

void AbstractFunction::choseColor()
{
    color = QColorDialog::getColor(Qt::green, this);
    pix_color->fill(color);
    delete icon_color;
    icon_color = new QIcon(*pix_color);
    color_button->setIcon(*icon_color);

    emit needUpdate();
}

void AbstractFunction::changeName()
{
    id = id_func->text();
}

void AbstractFunction::emit_delete_me()
{
    emit delete_me(this);
}

void AbstractFunction::new_select(AbstractFunction * new_func)
{
    if (new_func == this)
        is_selected = true;
    else
        is_selected = false;

    update();
}


void AbstractFunction::paintEvent(QPaintEvent *)
{
    QPainter pen(this);

    if (is_selected)
        pen.fillRect(0, 0, width(), height(), 0xA0A9AA);
    else
        pen.fillRect(0, 0, width(), height(), Qt::lightGray);
}

//Static function
AbstractFunction *AbstractFunction::loadFunction(const QString & input, QWidget *parent)
{
    QStringList list = input.split(QChar(' '));

    if ( list[0] == QString("Function") )
        return Function::loadFunction(input, parent);
    else if (list[0] ==  QString("Parametric"))
        return ParametricFunction::loadFunction(input, parent);
    else
        throw "Une erreur"; /* need improvement */
}
