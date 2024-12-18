#include "trafficlightsview.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QResizeEvent>

#define road_width  100
#define T_L_width   90
#define T_L_height  50

TrafficLightsView::TrafficLightsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

QRectF TrafficLightsView::visibleSceneRect()
{
    return mapToScene(viewport()->geometry()).boundingRect();
}

QGraphicsRectItem* TrafficLightsView::getRect(T_L_POSITION pos)
{
    QGraphicsRectItem* rect {nullptr};
    switch (pos)
    {
    case left_POS:
        rect = left_T_L;
        break;
    case bottom_POS:
        rect = bottom_T_L;
        break;
    case top_POS:
        rect = top_T_L;
        break;
    case right_POS:
        rect = right_T_L;
        break;
    default:
        break;
    }
    return rect;
}

void TrafficLightsView::setColorT_L(QColor color, T_L_POSITION pos)
{
    auto rect = getRect(pos);
    if (rect)
    {
        rect->setBrush(color);
    }
}

void TrafficLightsView::drawGraphics()
{
    // Initialize the view
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setFrameStyle(0);
    setViewportMargins(0, 0, 0, 0);

    if (scene)
        delete scene;

    scene = new QGraphicsScene();
    setScene(scene);

    scene->setSceneRect(mapToScene(rect()).boundingRect());

    // Middle points
    int middle_w = visibleSceneRect().width() / 2;
    int middle_h = visibleSceneRect().height() / 2;

    // Roads
    scene->addRect(QRectF(-middle_w - 5, -road_width / 2, visibleSceneRect().width() + 10, road_width),
                   QPen(Qt::black, 1),
                   QBrush(QColor::fromRgb(200, 200, 200))); // Horizontal road

    scene->addRect(QRectF(-road_width / 2, -middle_h - 5, road_width, visibleSceneRect().height() + 10),
                   QPen(Qt::black, 1),
                   QBrush(QColor::fromRgb(200, 200, 200))); // Vertical road

    // Intersection area
    scene->addRect(QRectF(-road_width / 2 - 1, -road_width / 2, road_width + 2, road_width),
                   QPen(Qt::transparent, 1),
                   QBrush(QColor::fromRgb(210, 210, 210))); // Lighter gray

    // Add dashed lines on the roads
    QPen dashedPen(Qt::white);
    dashedPen.setStyle(Qt::DashLine);

    // Horizontal dashed line
    for (int x = -middle_w; x < middle_w; x += 40)
    {
        scene->addLine(x, 0, x + 20, 0, dashedPen);
    }

    // Vertical dashed line
    for (int y = -middle_h; y < middle_h; y += 40)
    {
        scene->addLine(0, y, 0, y + 20, dashedPen);
    }

    // Left traffic light (rectangle)
    left_T_L = scene->addRect(QRectF(-road_width / 2 - 15 - T_L_width, -T_L_height / 2, T_L_width, T_L_height),
                              QPen(Qt::black, 1),
                              QColor::fromRgb(220, 220, 220));
    left_T_L->setVisible(isLeftT_L_visible);

    // Bottom traffic light (rectangle)
    bottom_T_L = scene->addRect(QRectF(-T_L_width / 2, road_width / 2 + 10, T_L_width, T_L_height),
                                QPen(Qt::black, 1),
                                QColor::fromRgb(220, 220, 220));
    bottom_T_L->setVisible(isBottomT_L_visible);

    // Top traffic light (rectangle)
    top_T_L = scene->addRect(QRectF(-T_L_width / 2, -road_width / 2 - 15 - T_L_height, T_L_width, T_L_height),
                             QPen(Qt::black, 1),
                             QColor::fromRgb(220, 220, 220));
    top_T_L->setVisible(isTopT_L_visible);

    // Right traffic light (rectangle)
    right_T_L = scene->addRect(QRectF(road_width / 2 + 10, -T_L_height / 2, T_L_width, T_L_height),
                               QPen(Qt::black, 1),
                               QColor::fromRgb(220, 220, 220));
    right_T_L->setVisible(isRightT_L_visible);
}

void TrafficLightsView::resizeEvent(QResizeEvent* event)
{
    drawGraphics();
    QGraphicsView::resizeEvent(event);
}

void TrafficLightsView::setVisibility(bool isVisible, T_L_POSITION pos)
{
    auto rect = getRect(pos);
    if (rect)
    {
        rect->setVisible(isVisible);
    }

    switch (pos)
    {
    case left_POS:
        isLeftT_L_visible = isVisible;
        break;
    case bottom_POS:
        isBottomT_L_visible = isVisible;
        break;
    case top_POS:
        isTopT_L_visible = isVisible;
        break;
    case right_POS:
        isRightT_L_visible = isVisible;
        break;
    default:
        break;
    }
}
