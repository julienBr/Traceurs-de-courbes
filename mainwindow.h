#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QVector>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QErrorMessage>
#include <QListWidget>
#include <QScrollArea>
#include <QGroupBox>

#include <renderarea.h>
#include <abstractfunction.h>
#include <parametricfunction.h>
#include <function.h>



class MainWindow : public QMainWindow

{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots :

    void new_func();
    void load_file();
    void fsave_file();
    void updateSelected(AbstractFunction* func);
    void delete_func(AbstractFunction *func);
    void updateFunctions();
    void updateFunction();




private:

    void create_function(const QString& input);
    void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;

    RenderArea* area;
    QLineEdit* formula;
    QPushButton* bnew_func;
    QWidget* main_label;
    QVBoxLayout* main_layout;
    QHBoxLayout* top_layout;
    QHBoxLayout* bottom_layout;
    QVBoxLayout* func_layout;
    QScrollArea* func_list;
    QGroupBox* func_box;
    QVBoxLayout* secondary_layout;

    QMenu* file_menu;
    QAction* open_file;
    QAction* save_file;
    QAction* quit;
    QVector<AbstractFunction*> storage;

};

#endif // MAINWINDOW_H
