#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void winloaddb(bool force=false);

private slots:
    void on_actionAbout_triggered();

    void on_actionQuit_2_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionSetting_triggered();

    void on_actionSetting_2_triggered();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    bool globaldbLoaded;

    void winupdatedb();
};

#endif // MAINWINDOW_H
