#ifndef MYCLIENTPROCESS_H
#define MYCLIENTPROCESS_H

#include <QObject>
#include <QProcess>

//Класс-обертка над клиенским C-backend'ом (QProccess)
//для последующей отправки в поток
class myClientProcess : public QObject
{
    Q_OBJECT

    QProcess *prcs_Client = nullptr;
    QString path = "";
    QStringList args;

public:
    explicit myClientProcess(QString &path, QStringList &args, QObject *parent = nullptr);

signals:
    void finished         ();    // Сигнал, по которому будем завершать поток, после завершения метода run
    void dataToEnterSignal(QByteArray);
    void dataToErrorSignal(QByteArray);

public slots:
    void run       (); // Метод с полезной нагрузкой, который может выполняться в цикле
    void readStdout();
    void readStderr(int, QProcess::ExitStatus);
};

#endif // MYCLIENTPROCESS_H
