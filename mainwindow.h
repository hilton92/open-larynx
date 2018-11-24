#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

class OutputWindow;
class OSGWidget;

namespace Ui {
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_actionExit_triggered();

private slots:
    void on_makeArytenoidTransparent_clicked();

private slots:
    void on_makeCricoidTransparent_clicked();

private slots:
    void on_makeThyroidTransparent_clicked();

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget *mOSGWidget{nullptr};
};

#endif // MAINWINDOW_H
