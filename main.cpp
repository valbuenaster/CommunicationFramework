#include <QtCore>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>

#include "plentyfunctions.h"
#include "robot.h"

//Hacer que la definicion del tamano de matrices sea con
//algo como #define XSIZE=5...

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Robot *Corobot=new Robot(1);

// this has to last in order to allow the server catch the incomming connections
    QWidget *ventana=new QWidget;

    QPushButton executeButton(QObject::tr("&Execute"));
    QPushButton StepByStep(QObject::tr("&Not available now"));

    QRadioButton Varsha(QObject::tr("Varsha's code"));
    QRadioButton Jacobi(QObject::tr("Jacobi's code"));
    Varsha.setChecked(true);
    QHBoxLayout *marco=new QHBoxLayout;

    ventana->setWindowTitle(QObject::tr("GUI for the program"));
//    QObject::connect(&executeButton,SIGNAL(clicked()),Corobot,SLOT(SendSpark()));
//    QObject::connect(&executeButton,SIGNAL(clicked()),Corobot,SLOT(hacerEnvio()));
    QObject::connect(&Varsha,SIGNAL(clicked()),Corobot,SLOT(setVarshaOption()));
    QObject::connect(&Jacobi,SIGNAL(clicked()),Corobot,SLOT(setJacobiOption()));

    QObject::connect(&executeButton,SIGNAL(clicked()),Corobot,SLOT(beginSelectedAlgorithm()));
    QObject::connect(&StepByStep,SIGNAL(clicked()),Corobot,SLOT(imprimirMP()));

    marco->addWidget(&Varsha);
    marco->addWidget(&Jacobi);
    marco->addWidget(&executeButton);
    marco->addWidget(&StepByStep);

    ventana->setLayout(marco);

    ventana->show();
    return a.exec();
}
