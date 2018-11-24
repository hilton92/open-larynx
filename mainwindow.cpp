#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"
#include <stdlib.h>

#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
    mOSGWidget = new OSGWidget{this};
    this->setCentralWidget(mOSGWidget);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_makeThyroidTransparent_clicked()
{
    mOSGWidget->make_cartilage_transparent(mOSGWidget->get_thyroid_pointer());
}

void MainWindow::on_makeCricoidTransparent_clicked()
{
    mOSGWidget->make_cartilage_transparent(mOSGWidget->get_cricoid_pointer());
}

void MainWindow::on_makeArytenoidTransparent_clicked()
{
    mOSGWidget->make_cartilage_transparent(mOSGWidget->get_arytenoid_pointer());
}
