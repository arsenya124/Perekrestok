#ifndef P2P_STRUCTS_H
#define P2P_STRUCTS_H

//id сообщений
#define defPower 1
#define defStandAlone 2
#define defAuto 3
#define defManual 4

//id цветов
#define defRed 1
#define defGreen 2
#define defYellow 3

#pragma pack(push, 1)
//структура сообщения вкл/выкл пульта
struct MessagePower
{
    int id {defPower};
    bool isOn;  //true - включён, false - отключён
};


//структура сообщения автономного режима
struct MessageStandAlone
{
    int id {defStandAlone};
};


//структура сообщения автоматического режима
struct MessageAuto
{
    int id {defAuto};
    int time_red;           //мс
    int time_green;         //мс
    int time_yellow;        //мс
};


//структура сообщения ручного режима
struct MessageManual
{
    int id {defManual};
    int color_left;         //см id цветов
    int color_bottom;      //см id цветов
    int color_right;     //см id цветов
    int color_top;        //см id цветов
};


#pragma pack(pop)
#endif // P2P_STRUCTS_H
