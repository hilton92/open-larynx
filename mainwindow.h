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
    void on_actionSaveOptions_triggered();


private slots:
    void on_toggleCricoid_clicked();

private slots:
    void on_toggleArytenoid_clicked();

private slots:
    void on_toggleThyroid_clicked();

    void on_X_direction_slider_valueChanged(int value);

    void on_Z_direction_slider_valueChanged(int value);

    void on_Animate_clicked();

    void on_Record_clicked();

private:
    Ui::MainWindowForm *mMainWindowUI;
   // OSGWidget *mOSGWidget{nullptr};
};

#endif // MAINWINDOW_H
