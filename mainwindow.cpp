#include "mainwindow.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    area(nullptr),
    formula(nullptr),
    bnew_func(nullptr),
    main_label(nullptr),
    main_layout(nullptr),
    top_layout(nullptr),
    bottom_layout(nullptr),
    func_layout(nullptr),
    func_list(nullptr),
    func_box(nullptr)
{
    main_label = new QWidget(this);
    main_layout = new QVBoxLayout;
    top_layout = new QHBoxLayout;

    //Top layout
        //Right Side
        area = new RenderArea(this);
        //Left side
        func_layout = new QVBoxLayout;
        func_list = new QScrollArea(this);
        func_list->setMinimumSize(QSize(250, height()));
        func_list->setMaximumWidth(250);
        func_list->setWidgetResizable(true);
        func_box = new QGroupBox(QString("Fonctions :"), this);
        func_box->setLayout(func_layout);
        secondary_layout = new QVBoxLayout;
        secondary_layout->setAlignment(Qt::AlignTop);
        func_layout->addLayout(secondary_layout);

        func_list->setWidget(func_box);

    top_layout->addWidget(func_list);
    top_layout->addWidget(area);

    //Bottom layout
    bottom_layout = new QHBoxLayout;

    QLabel* test= new QLabel("Formule :", this);
    formula = new QLineEdit;
    bnew_func = new QPushButton("Nouvelle fonction");
    bottom_layout->addWidget(test);
    bottom_layout->addWidget(formula);
    bottom_layout->addWidget(bnew_func);

    //Main Layout
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);

    //Main label
    main_label->setLayout(main_layout);
    setCentralWidget(main_label);

    //Menu Bar
    file_menu = new QMenu("Fichier",this);
    open_file = new QAction("Ouvrir", this);
    open_file->setShortcut(QKeySequence::Open);
    save_file = new QAction("Sauvegarder", this);
        //Actions
        file_menu->addAction(open_file);
        file_menu->addAction(save_file);
        menuBar()->addMenu(file_menu);



    //Connections
    QObject::connect(bnew_func,SIGNAL(clicked(bool)), this, SLOT(new_func()));
    QObject::connect(open_file, SIGNAL(triggered(bool)), this, SLOT(load_file()));
    QObject::connect(save_file, SIGNAL(triggered(bool)), this, SLOT(fsave_file()));
}

MainWindow::~MainWindow()
{
    delete area;
    delete main_label;
    delete main_layout;
    delete func_layout;
    delete file_menu;
    delete quit;
    delete open_file;
}


void MainWindow::new_func()
{
    create_function(formula->text());
}

void MainWindow::load_file()
{
    QString path = QFileDialog::getOpenFileName(this,"Ouvrir un fichier fonction");

    if (path.isEmpty())
        return;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return; /* need improvement */

    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        create_function(line);
    }
}

void MainWindow::fsave_file()
{
    QString path = QFileDialog::getOpenFileName(this,"Choisir un fichier fonction");

    if (path.isEmpty())
        return;

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return; /* need improvement */

    QTextStream stream(&file);

    for (auto i : storage )
        stream << i->getFormula() << '\n';

    stream.flush();
}

void MainWindow::updateFunction()
{
    for (auto i : storage)
        i->update();
}

void MainWindow::create_function(const QString &input)
{
    try {
        AbstractFunction* f = AbstractFunction::loadFunction(input,this);
        storage.append(f);
        area->add_function(f);

        secondary_layout->addWidget(f);
    } catch (...) {
        std::cerr << "Une erreur est survenue" << std::endl;
        /* need improvement */
    }

    updateFunctions();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if (  event->key() == Qt::Key_Up )
        area->move_up();
    else if ( event->key() == Qt::Key_Right)
        area->move_right();
    else if (event->key() == Qt::Key_Down)
        area->move_down();
    else if (event->key() == Qt::Key_Left)
        area->move_left();
}

void MainWindow::updateSelected(AbstractFunction *func)
{
    for (auto i : storage)
        i->new_select(func);

    updateFunction();
}

void MainWindow::delete_func(AbstractFunction *func)
{
    for (unsigned i = 0; i < (unsigned) storage.length(); i++ )
        if (storage[i] == func ) {
            storage.remove(i);
            delete func;
        }
}

void MainWindow::updateFunctions()
{
    area->update();
}
