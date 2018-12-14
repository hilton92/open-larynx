#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"
#include <stdlib.h>
#include <QFileDialog>

#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
   //mOSGWidget = new OSGWidget{this};
   // this->setCentralWidget(mOSGWidget);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionSaveOptions_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Options", "", "txt");
    mMainWindowUI->mOSGWidget->write_to_file(fileName.toStdString());

}

void MainWindow::on_toggleThyroid_clicked()
{
    if (!mMainWindowUI->mOSGWidget->thyroidTransparent)
    {
        mMainWindowUI->mOSGWidget->make_cartilage_transparent(mMainWindowUI->mOSGWidget->get_thyroid_pointer());
        mMainWindowUI->mOSGWidget->thyroidTransparent = true;
    }
    else
    {
        mMainWindowUI->mOSGWidget->make_cartilage_opaque(mMainWindowUI->mOSGWidget->get_thyroid_pointer());
        mMainWindowUI->mOSGWidget->thyroidTransparent = false;

    }
}

void MainWindow::on_toggleArytenoid_clicked()
{
    if (!mMainWindowUI->mOSGWidget->arytenoidTransparent)
    {
        mMainWindowUI->mOSGWidget->make_cartilage_transparent(mMainWindowUI->mOSGWidget->get_arytenoid_pointer());
        mMainWindowUI->mOSGWidget->arytenoidTransparent = true;
    }
    else
    {
        mMainWindowUI->mOSGWidget->make_cartilage_opaque(mMainWindowUI->mOSGWidget->get_arytenoid_pointer());
        mMainWindowUI->mOSGWidget->arytenoidTransparent = false;

    }
}

void MainWindow::on_toggleCricoid_clicked()
{
    if (!mMainWindowUI->mOSGWidget->cricoidTransparent)
    {
        mMainWindowUI->mOSGWidget->make_cartilage_transparent(mMainWindowUI->mOSGWidget->get_cricoid_pointer());
        mMainWindowUI->mOSGWidget->cricoidTransparent = true;
    }
    else
    {
        mMainWindowUI->mOSGWidget->make_cartilage_opaque(mMainWindowUI->mOSGWidget->get_cricoid_pointer());
        mMainWindowUI->mOSGWidget->cricoidTransparent = false;

    }
}

void MainWindow::on_X_direction_slider_valueChanged(int value)
{
    float val = (2.f * value) / 100;
    mMainWindowUI->mOSGWidget->xLocation = -2.f + val;
}

void MainWindow::on_Z_direction_slider_valueChanged(int value)
{
    float val = (2.f * value) / 100;
    mMainWindowUI->mOSGWidget->zLocation = val;
}
