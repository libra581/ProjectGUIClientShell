#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("://img/icon_client.png"));

    //Инициализация пути до клиентского С-backend приложения
    program_Path = QCoreApplication::applicationDirPath()+"/clientTCP";
             //"/home/user/build-untitled1-Desktop_Qt_5_12_0_GCC_64bit-Debug/untitled1";

    //Инициализация структуры подачи каждой команды
    QList<QString> plan_26_Command;
    plan_26_Command << "input operation number:"
                    << "operation #26 -  please  input id :"
                    << "operation #26 -  please  input parametry :";

    //Реализация возможности множественных наборов структур
    messageForEnter << plan_26_Command;

    //Настройка графических виджетов
    ui->plainText_enter->setEnabled(false);
    ui->plainText_req->setEnabled(false);
    ui->pushButton_Enter->setEnabled(false);
    ui->btn_connecting->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_connecting_clicked()
{
    //Создание потока, в который помещается процесс ClientTCP
    QThread *thread = new QThread();
    client = new myClientProcess(program_Path, arguments);

    connect(thread, &QThread::started,  client,   &myClientProcess::run);
    connect(client,   &myClientProcess::finished,    thread, &QThread::terminate);
    connect(client, &myClientProcess::dataToEnterSignal, this, &MainWindow::dataToEnter);
    connect(client, &myClientProcess::dataToErrorSignal, this, &MainWindow::dataToError);

    client->moveToThread(thread);

    thread->start();
}

//Событие клика на кнопку Команда №26
void MainWindow::on_pushButton_26_clicked()
{
    //Настройка всех элементов для подготовки ввода и выставление
    //первого запроса у пользователя: "input number commad"
    flag_com = 26;
    arguments.clear();
    ui->plainTextEdit->clear();
    arguments << QString::number(flag_com);

    ui->label_3->setStyleSheet(" color: rgba(231, 76, 60,1.0); ");
    ui->btn_connecting->setStyleSheet("color: grey;");

    ui->plainText_enter->setEnabled(true);
    ui->plainText_req->setEnabled(true);
    ui->pushButton_Enter->setEnabled(true);
    ui->btn_connecting->setEnabled(false);
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->plainTextEdit_class_id->setEnabled(true);
    ui->plainTextEdit_inputData->setPlainText("");

    ui->plainText_req->setPlainText(messageForEnter.at(0)[0]);
    ui->plainText_enter->setText(QString::number(flag_com));

    numStep = 0;
    on_pushButton_Enter_clicked();
}

//Слот получения сообщения из потока stdout запущенного процесса
void MainWindow::dataToEnter(QByteArray str)
{    
    QByteArray lbl = "from serv  :";

    //Поверка отправителя сообщения
    if(str.contains(lbl))
    {
        QString temp_str = str;

        qDebug() << temp_str.right(temp_str.length()-temp_str.indexOf(lbl)-lbl.length()-1);

        if(temp_str.right(temp_str.length()-temp_str.indexOf(lbl)) == "0")
        {
            ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText() + "from serv: OK");
        }
        else
        {
            ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText() + str);
        }
    }
    else
    {
       ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText() + str);
    }
}

//Слот получения сообщения из потока stderr запущенного процесса
void MainWindow::dataToError(QByteArray str)
{
        int ret = QMessageBox::warning(this, tr("Ошибка подключения!"),
                                   tr("Пожалуйста, повторите позже!\n"+str),
                                       QMessageBox::Ok);

        Q_UNUSED(ret);

        ui->plainText_enter->setEnabled(false);
        ui->plainText_req->clear();
        ui->plainText_req->setEnabled(false);
        ui->pushButton_Enter->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->plainTextEdit_class_id->setEnabled(false);
}

void MainWindow::on_pushButton_Enter_clicked()
{
    //Проверка на номер подаваемой команды
    if(flag_com == 26)
    {
        //Пошаговая реализация команды 26 (ввод данных)
        //Пошаговая подача запросов на ввод клиенту:
        //"input operation number:"
        //"operation #26 -  please  input id :"
        //"operation #26 -  please  input parametry :";
        switch (numStep)
        {            
            case 0:
                ++numStep;
                ui->plainText_req->setPlainText(messageForEnter.at(0)[numStep]);
                qDebug() << numStep;

                break;

            case 1:
                ++numStep;

                arguments << ui->comboBox->currentText().left(1)+
                             ui->comboBox_2->currentText().left(1)+
                             ui->comboBox_3->currentText().left(1)+ "@"+
                             ui->plainTextEdit_class_id->text()+ " "+
                             ui->plainText_enter->text();

                ui->plainText_req->setPlainText(messageForEnter.at(0)[numStep]);

                ui->plainTextEdit_inputData->setPlainText(
                            ui->plainTextEdit_inputData->toPlainText() +
                           ( ui->plainText_req->toPlainText() + " " +
                             arguments.last() + "\n"));

                ui->comboBox->setEnabled(false);
                ui->comboBox_2->setEnabled(false);
                ui->comboBox_3->setEnabled(false);
                ui->plainTextEdit_class_id->setEnabled(false);

                qDebug() << numStep;
                break;

            case 2:
                arguments << ui->plainText_enter->text();

                ui->btn_connecting->setEnabled(true);
                ui->btn_connecting->setStyleSheet("color: red;");
                ui->pushButton_Enter->setEnabled(false);

                ui->plainTextEdit_inputData->setPlainText(
                            ui->plainTextEdit_inputData->toPlainText() +
                           (ui->plainText_req->toPlainText() + " " +
                            arguments.last() + "\n"));

                numStep = 0;
                break;

            default:
                numStep = 0;
                break;
        }
    }
    else
    {
        qDebug() << flag_com;
    }
}

//Слот изменения текста в поле Классификатор ID
void MainWindow::on_plainTextEdit_class_id_textChanged()
{
    ui->plainText_enter->setMaxLength(MAX_LENGTH - ui->plainTextEdit_class_id->text().length());
}
