#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QtGui>
#include <QApplication>

class MyApplication : public QApplication
{
public:
    MyApplication(int &argc, char**argv): QApplication(argc,argv){}
    virtual ~MyApplication() {}

    //Переопределение метода QApplication для эффективной обработки исключений
    virtual bool notify(QObject *receiver, QEvent *event) override
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception &e)
        {
            qCritical() << "Exception trown:" << e.what();
        }

        return false;
    }
};

#endif // MYAPPLICATION_H
