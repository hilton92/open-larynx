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

void MainWindow::on_thyroidTransparency_toggled(bool checked)
{
    if (checked)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_thyroid_pointer());
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_thyroid_pointer());
    }
}

void MainWindow::on_cricoidTransparency_toggled(bool checked)
{
    if (checked)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_cricoid_pointer());
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_cricoid_pointer());
    }
}

void MainWindow::on_arytenoidTransparency_toggled(bool checked)
{
    if (checked)
    {
        mOSGWidget->make_cartilage_transparent(mOSGWidget->get_arytenoid_pointer());
    }
    else
    {
        mOSGWidget->make_cartilage_opaque(mOSGWidget->get_arytenoid_pointer());
    }
}
