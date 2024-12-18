#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "p2pconnector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //таймер запускающийся в автономном режиме
    QTimer timer_stand_alone;
    bool isYellow {false};
    //таймер запускающийся в автоматическом режиме
    QTimer timer_auto;
    //Qt::transparent служит только для инициализации
    //еслит текущий transparent мы переходим на green
    //цвета left и right_botttom (у bottom и top наоборот)
    QColor cur_color_left_auto {Qt::transparent};
    QColor prev_color_left_auto {Qt::transparent};
    //время горения жёлтым в автоматическом режиме (мс)
    quint64 yellow_ms_auto {0};
    //время горения зелёным в автоматическом режиме (мс)
    quint64 green_ms_auto {0};
    //время горения красным в автоматическом режиме (мс)
    quint64 red_ms_auto {0};

    //перейти в автономный режим
    void setStandAloneMode();
    //перейти в автоматический режим
    void setAutoMode( quint64 yellow_ms,    //время горения жёлтым (мс)
                      quint64 green_ms,     //время горения зелёным (мс)
                      quint64 red_ms);      //время горения красным (мс)
    //перейти в ручной режим
    //(или просто задать время если в ручном режиме)
    void setManualMode( QColor color_left_right,    //цвет верхнего левого и нижнего правого
                        QColor color_bottom_top);   //цвет верхнего правого и нижнего левого
    //пульт включен/отключён
    void setPower(bool isOn);

    //класс связи по протоколу "точка-точка"
    Tp2pConnector * connector {nullptr};

private slots:

    //мигания светофоров в автономном режиме
    void stand_alone_flashing();
    //изменения цветов светофоров в автоматическом режиме
    void auto_flashing();

    //от коннектора пришёл ответ
    //что связь установлена
    void onHandshaked();
    //от коннектора пришло сообщение
    void onReceived(QByteArray msg);
};
#endif // MAINWINDOW_H
