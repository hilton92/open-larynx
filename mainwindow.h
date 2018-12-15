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
    Ui::MainWindowForm *mMainWindowUI;

public slots:
    void on_actionExit_triggered();
    void on_actionSaveOptions_triggered();
    void update_counter(int);

private slots:
    void on_toggleCricoid_clicked();
    void on_toggleArytenoid_clicked();
    void on_toggleThyroid_clicked();
    void on_X_direction_slider_valueChanged(int value);
    void on_Z_direction_slider_valueChanged(int value);
    void on_Animate_clicked();
    void on_Record_clicked();

};

#endif // MAINWINDOW_H
