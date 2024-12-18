#ifndef TRAFFICLIGHTSVIEW_H
#define TRAFFICLIGHTSVIEW_H

#include <QGraphicsView>
#include <QTimer>

class TrafficLightsView : public QGraphicsView
{
    Q_OBJECT

public:
    TrafficLightsView(QWidget *parent = nullptr);

    enum T_L_POSITION {left_POS, bottom_POS, top_POS, right_POS};

public slots:
    //установить цвет светофору в позиции
    void setColorT_L (QColor color, T_L_POSITION pos);
    //установить видимость светофору в позиции
    void setVisibility (bool isVisible, T_L_POSITION pos);

protected:
    //обработчик изменения размеров вьюхи
    void resizeEvent ( QResizeEvent * event ) override;

private:
    QGraphicsScene* scene {nullptr};

    QRectF visibleSceneRect();

    //светофоры
    QGraphicsRectItem *left_T_L {nullptr};
    QGraphicsRectItem *bottom_T_L {nullptr};
    QGraphicsRectItem *top_T_L {nullptr};
    QGraphicsRectItem *right_T_L {nullptr};

    //флаги видимости светофоров
    bool isLeftT_L_visible {true};
    bool isBottomT_L_visible {true};
    bool isTopT_L_visible {true};
    bool isRightT_L_visible {true};

    //возвращает эллипс из позиции
    QGraphicsRectItem * getRect(T_L_POSITION pos);

private slots:
    //рисуем дороги и светофоры
    //нельзя рисовать в конструкторе потому что в нём не правильные размеры окна
    void drawGraphics();

};

#endif // TRAFFICLIGHTSVIEW_H
