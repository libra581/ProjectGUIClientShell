#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAX_LENGTH_CLASS_ID 5
#define MAX_LENGTH 39
#define ERROR_SERVER -1

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QThread>

#include "myclientprocess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_connecting_clicked();

    void on_pushButton_26_clicked();

    void dataToEnter(QByteArray str);

    void dataToError(QByteArray str);

    void on_pushButton_Enter_clicked();

    void on_plainTextEdit_class_id_textChanged();

private:
    Ui::MainWindow *ui;

    QString program_Path = "";
    QStringList arguments;
    int flag_com = -1;
    myClientProcess *client = nullptr;
    QList<QList<QString>> messageForEnter;
    int numStep = 0;
};

#endif // MAINWINDOW_H
