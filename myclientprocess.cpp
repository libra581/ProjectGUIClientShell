#include "myclientprocess.h"
#include <QDebug>
myClientProcess::myClientProcess(QString &path, QStringList &args, QObject *parent)
    : QObject(parent), path(path), args(args)
{
    prcs_Client = new QProcess(this);

    connect(prcs_Client,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdout()));
    connect(prcs_Client,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(readStderr(int,QProcess::ExitStatus)));
}

//Запуск консольного клиента, как объект процесса
void myClientProcess::run()
{    
    //Запуск модуля (процесса)
    prcs_Client->start(path, args);

    //Ожидание инициализации процесса
    if (!prcs_Client->waitForStarted())
        return;

    /*
      Здесь может быть код нужных манипуляций с потоками процесса:
      записать данные write() в поток stdin и считать из потока stdout
    */

    //Ожидание инициализации процесса
    if (!prcs_Client->waitForFinished())
        return;

    emit finished();
}

//Слот, принимающий сообщения из потока stdout клиента
void myClientProcess::readStdout()
{    
    QByteArray str = prcs_Client->readAll();

    emit dataToEnterSignal(str);
}

//Слот, принимающий ошибки из потока stderr клиента
void myClientProcess::readStderr(int res, QProcess::ExitStatus status)
{    
    Q_UNUSED(status);
    QByteArray str = prcs_Client->readAllStandardError();

    if(str.contains("error") || res == EXIT_FAILURE || res == 255)
    {
        emit dataToErrorSignal(str);
    }

}


