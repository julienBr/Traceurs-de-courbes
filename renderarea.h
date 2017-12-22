#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include <QLabel>
#include <QVector>
#include <defines.h>
#include <abstractfunction.h>


class RenderArea : public QWidget
{
    Q_OBJECT

public:

    explicit RenderArea(QWidget *parent = 0);
    ~RenderArea();

    void move_up();
    void move_right();
    void move_down();
    void move_left();


public slots :

    inline void add_function(AbstractFunction* f) { ext_function.append(f);  }


protected:

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * ) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;


private :

    void drawAxes();
    void drawFunction(AbstractFunction *f);
    void calcCoord();

    QPainter* pen;
    QPoint center;

    // ZOOM/DEZOOM
    QPointF pix_unite;
    QPointF unite;
    QPointF base;
    QPoint current_tab;
    QPointF unite_per_pix;

    // DEPLACEMENT
    bool drag;
    QPoint old_cursor;
    QLabel* pos_label;

    //STORAGE
    QVector<AbstractFunction*> ext_function;
    QVector<AbstractFunction*> intern_func;

};




#endif // RENDERAREA_H
