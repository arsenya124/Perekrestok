#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "p2p_structs.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    timer_stand_alone.setInterval(3000);

    connect(&timer_stand_alone, &QTimer::timeout,
            this, &MainWindow::stand_alone_flashing);
    connect(&timer_auto, &QTimer::timeout,
            this, &MainWindow::auto_flashing);


    //инициализация класса связи
    //по протоколу Точка-Точка
    connector = new Tp2pConnector(this);

    connect(connector, &Tp2pConnector::handshaked,
            this, &MainWindow::onHandshaked);
    connect(connector, &Tp2pConnector::recieved,
            this, &MainWindow::onReceived);
    connector->handshake();

    ui->statusbar->showMessage("Нет соединения");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//от коннектора пришло сообщение
void MainWindow::onReceived(QByteArray msg)
{
    if (msg.size() < sizeof(int))
    {
        QMessageBox::critical(this, "P2P error", "wrong size of message");
        return;
    }
    int * id = nullptr;
    id = reinterpret_cast<int *> (msg.data());

    if (defPower == *id)
    {
        if (msg.size() < sizeof(MessagePower))
        {
            QMessageBox::critical(this, "P2P error", "wrong size of message");
            return;
        }
        MessagePower * message = reinterpret_cast<MessagePower *> (msg.data());
        setPower(message->isOn);
    }
    else if (defStandAlone == *id)
    {
        if (msg.size() < sizeof(MessageStandAlone))
        {
            QMessageBox::critical(this, "P2P error", "wrong size of message");
            return;
        }
        setStandAloneMode();
    }
    else if (defAuto == *id)
    {
        if (msg.size() < sizeof(MessageAuto))
        {
            QMessageBox::critical(this, "P2P error", "wrong size of message");
            return;
        }
        MessageAuto * message = reinterpret_cast<MessageAuto *> (msg.data());
        setAutoMode(message->time_yellow, message->time_green, message->time_red);
    }
    else if (defManual == *id)
    {
        if (msg.size() < sizeof(MessageManual))
        {
            QMessageBox::critical(this, "P2P error", "wrong size of message");
            return;
        }
        MessageManual * message = reinterpret_cast<MessageManual *> (msg.data());


        QColor color_left;
        QColor color_bottom;
        switch (message->color_left)
        {
        case defRed:
            color_left = Qt::red;
            break;
        case defGreen:
            color_left = Qt::green;
            break;
        case defYellow:
            color_left = Qt::yellow;
            break;
        }
        switch (message->color_bottom)
        {
        case defRed:
            color_bottom = Qt::red;
            break;
        case defGreen:
            color_bottom = Qt::green;
            break;
        case defYellow:
            color_bottom = Qt::yellow;
            break;
        }
        setManualMode(color_left, color_bottom);
    }
    else
    {
        QMessageBox::critical(this, "P2P error", "wrong id of message");
        return;
    }

}

//от коннектора пришёл ответ
//что связь установлена
void MainWindow::onHandshaked()
{
    ui->statusbar->showMessage("Соединение установлено");
}

//мигания светофоров в автономном режиме
void MainWindow::stand_alone_flashing()
{
    ui->graphicsView->setColorT_L(isYellow ? QColor::fromRgb(241, 241, 241) : Qt::yellow,
                                  TrafficLightsView::bottom_POS);
    ui->graphicsView->setColorT_L(isYellow ? QColor::fromRgb(241, 241, 241) : Qt::yellow,
                                  TrafficLightsView::left_POS);
    ui->graphicsView->setColorT_L(isYellow ? QColor::fromRgb(241, 241, 241) : Qt::yellow,
                                  TrafficLightsView::top_POS);
    ui->graphicsView->setColorT_L(isYellow ? QColor::fromRgb(241, 241, 241) : Qt::yellow,
                                  TrafficLightsView::right_POS);
    isYellow = !isYellow;
}


//изменения цветов светофоров в автоматическом режиме
void MainWindow::auto_flashing()
{
    //определяем следующий цвет
    QColor color_left_right;
    QColor color_bottom_top;
    if (QColor(Qt::transparent) == cur_color_left_auto)
    {
        color_left_right = Qt::green;
        color_bottom_top = Qt::red;
        timer_auto.start(green_ms_auto);
    }
    else if (QColor(Qt::green) == cur_color_left_auto)
    {
        color_left_right = Qt::yellow;
        color_bottom_top = Qt::yellow;
        timer_auto.start(yellow_ms_auto);
    }
    else if (QColor(Qt::yellow) == cur_color_left_auto
             && QColor(Qt::green) == prev_color_left_auto)
    {
        color_left_right = Qt::red;
        color_bottom_top = Qt::green;
        timer_auto.start(red_ms_auto);
    }
    else if (QColor(Qt::red) == cur_color_left_auto)
    {
        color_left_right = Qt::yellow;
        color_bottom_top = Qt::yellow;
        timer_auto.start(yellow_ms_auto);
    }
    else if (QColor(Qt::yellow) == cur_color_left_auto
             && QColor(Qt::red) == prev_color_left_auto)
    {
        color_left_right = Qt::green;
        color_bottom_top = Qt::red;
        timer_auto.start(green_ms_auto);
    }

    ui->graphicsView->setColorT_L(color_bottom_top,
                                  TrafficLightsView::bottom_POS);
    ui->graphicsView->setColorT_L(color_left_right,
                                  TrafficLightsView::left_POS);
    ui->graphicsView->setColorT_L(color_bottom_top,
                                  TrafficLightsView::top_POS);
    ui->graphicsView->setColorT_L(color_left_right,
                                  TrafficLightsView::right_POS);

    prev_color_left_auto = cur_color_left_auto;
    cur_color_left_auto = color_left_right;
}

//пульт включен/отключён
void MainWindow::setPower(bool isOn)
{
    if (!isOn)
    {
        timer_stand_alone.stop();
        timer_auto.stop();

        ui->graphicsView->setColorT_L(QColor::fromRgb(241, 241, 241),
                                      TrafficLightsView::bottom_POS);
        ui->graphicsView->setColorT_L(QColor::fromRgb(241, 241, 241),
                                      TrafficLightsView::left_POS);
        ui->graphicsView->setColorT_L(QColor::fromRgb(241, 241, 241),
                                      TrafficLightsView::top_POS);
        ui->graphicsView->setColorT_L(QColor::fromRgb(241, 241, 241),
                                      TrafficLightsView::right_POS);
        statusBar()->showMessage("Пульт отключён");
    }
    else
    {
        statusBar()->showMessage("Пульт включён");
    }
}

//перейти в автономный режим
void MainWindow::setStandAloneMode()
{
    ui->statusbar->showMessage("Режим - Автономный");
    isYellow = false;
    timer_auto.stop();
    timer_stand_alone.start();
    stand_alone_flashing();
}

//перейти в автоматический режим
void MainWindow::setAutoMode( quint64 yellow_ms,    //время горения жёлтым (мс)
                  quint64 green_ms,     //время горения зелёным (мс)
                  quint64 red_ms)       //время горения красным (мс)
{
    ui->statusbar->showMessage("Режим - Управление автоматическое");
    cur_color_left_auto = Qt::transparent;
    prev_color_left_auto = Qt::transparent;
    timer_stand_alone.stop();

    yellow_ms_auto = yellow_ms;
    green_ms_auto = green_ms;
    red_ms_auto = red_ms;
    auto_flashing();
}

//перейти в ручной режим
//(или просто задать время если в ручном режиме)
void MainWindow::setManualMode( QColor color_left_right,    //цвет верхнего левого и нижнего правого
                                QColor color_bottom_top)   //цвет верхнего правого и нижнего левого
{
    ui->statusbar->showMessage("Режим - Управление ручное");
    timer_stand_alone.stop();
    timer_auto.stop();

    ui->graphicsView->setColorT_L(color_bottom_top,
                                  TrafficLightsView::bottom_POS);
    ui->graphicsView->setColorT_L(color_left_right,
                                  TrafficLightsView::left_POS);
    ui->graphicsView->setColorT_L(color_bottom_top,
                                  TrafficLightsView::top_POS);
    ui->graphicsView->setColorT_L(color_left_right,
                                  TrafficLightsView::right_POS);
}


