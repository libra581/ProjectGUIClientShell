# Графическая оболочка Qt клиента
*Проект сдела в рамках заказа на фриланс бирже KWORK*<br>

Программа является GUI-оберткой над клиенским модулем обмена информации с сервером. <br>

**Клиентский модуль** представлял собой консольное приложение, которая запрашивает у пользователя данные 
с клавиатуры и пересылает их серверу (язык исходного кода Си под Linux).<br>

**GUI-модуль** реализует графический интерфейс для запроса у пользователя входных данных для формирования запроса, который пересылается в backend-модуль (Си язык). GUI 
позволяет оставить код взаимодействия с сервером на стороне backend-модуля. <br>

Тестирование программы проводилось на платформах: <br>
- [x] OC Window 7, 10; 
- [x] OC Linux Mint.

## Структура программы:
```
main()
│
├─ MyApplication()
└─ Mainwindow()
   │
   ├─ QThread(parent = nullptr) <- myClientProcess(path, args)
   └─ Settings interface objects
```

## Ключевые места в программе
За логику подачи сообщений в QPlainText для запросов у пользователя данных используется переменная *QList<QList<QString>> messageForEnter*, 
а также алгоритм слота *void MainWindow::on_pushButton_Enter_clicked()* в классе *MainWindow*. <br>

За создание объекта процесса работы с backend-модулем отвечает слот клика на кнопку Connect:

```C++
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
```
Класс типа QApplication переопределяет виртуальный метод *notify* для эффективной обработки исключений:

```C++
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
```


## Скриншоты интерфейса пользователя
![interface](https://i.pinimg.com/originals/dc/8a/58/dc8a589839389287007ed9eb34e97fe7.png "Интерфейс пользователя главного окна")
