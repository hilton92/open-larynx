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

void MainWindow::on_toggleThyroid_clicked()
{
    if (!mOSGWidget->thyroidTransparent)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_thyroid_pointer());
        mOSGWidget->thyroidTransparent = true;
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_thyroid_pointer());
        mOSGWidget->thyroidTransparent = false;

    }
}

void MainWindow::on_toggleArytenoid_clicked()
{
    if (!mOSGWidget->arytenoidTransparent)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_arytenoid_pointer());
        mOSGWidget->arytenoidTransparent = true;
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_arytenoid_pointer());
        mOSGWidget->arytenoidTransparent = false;

    }
}

void MainWindow::on_toggleCricoid_clicked()
{
    if (!mOSGWidget->cricoidTransparent)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_cricoid_pointer());
        mOSGWidget->cricoidTransparent = true;
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_cricoid_pointer());
        mOSGWidget->cricoidTransparent = false;

    }
}
