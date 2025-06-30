#include "layerblockitem.h"
#include <QPen>

LayerBlockItem::LayerBlockItem(const QString& title, const QString& activation, int neuronCount, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{
    QRectF outerRect(0, 0, 150, 200);
    QGraphicsRectItem* frame = new QGraphicsRectItem(outerRect);
    frame->setPen(QPen(Qt::black));
    addToGroup(frame);

    // Title
    QGraphicsTextItem* titleText = new QGraphicsTextItem(title);
    titleText->setPos(outerRect.center().x() - 25, -20);
    addToGroup(titleText);

    QGraphicsTextItem* countText = new QGraphicsTextItem(QString::number(neuronCount));
    countText->setPos(outerRect.center().x() - 5, outerRect.bottom() + 5);
    addToGroup(countText);

    // W and b
    QGraphicsRectItem* wBox = addComponent("W", QPointF(20, 30), QSizeF(30, 30));
    QGraphicsRectItem* bBox = addComponent("b", QPointF(100, 30), QSizeF(30, 30));

    // Plus
    QGraphicsEllipseItem* plusCircle = new QGraphicsEllipseItem(55, 80, 20, 20);
    plusCircle->setPen(QPen(Qt::black));
    plusCircle->setBrush(Qt::white); // 若想实心
    plusCircle->setZValue(1);
    addToGroup(plusCircle);
    QGraphicsTextItem* plusText = new QGraphicsTextItem("+", plusCircle);
    plusText->setPos(60, 78);
    QPointF plusCenter = plusCircle->sceneBoundingRect().center();
    //获取 W → + 线起止点（右边 → 中心）
    QPointF wRight = wBox->sceneBoundingRect().center() + QPointF(wBox->rect().width() / 2, 0);
    QGraphicsLineItem* wLine = new QGraphicsLineItem(QLineF(wRight, plusCenter));
    addToGroup(wLine);
    // 获取 b → + 线起止点（左边 → 中心）
    QPointF bLeft = bBox->sceneBoundingRect().center() - QPointF(bBox->rect().width() / 2, 0);
    QGraphicsLineItem* bLine = new QGraphicsLineItem(QLineF(bLeft, plusCenter));
    addToGroup(bLine);

    // 🔗 连线：W → +，b → +


    // 激活函数框
    QGraphicsRectItem* actBox = new QGraphicsRectItem(30, 120, 90, 30);
    actBox->setBrush(QColor("#AEDCF0"));
    addToGroup(actBox);
    QGraphicsTextItem* actText = new QGraphicsTextItem(activation, actBox);
    actText->setPos(45, 122);

    // 🔗 加号 → 激活函数

    QPointF actTop = actBox->sceneBoundingRect().topLeft() + QPointF(actBox->rect().width() / 2, 0);
    QGraphicsLineItem* plusToActLine = new QGraphicsLineItem(QLineF(plusCenter, actTop));
    addToGroup(plusToActLine);

}
/*
void LayerBlockItem::addComponent(const QString& label, const QPointF& pos, const QSizeF& size) {
    QGraphicsRectItem* box = new QGraphicsRectItem(QRectF(pos, size));
    box->setBrush(QColor("#E0F7FA"));
    QGraphicsTextItem* text = new QGraphicsTextItem(label, box);
    text->setPos(pos.x() + 8, pos.y() + 5);
    addToGroup(box);
}
*/
QGraphicsRectItem* LayerBlockItem::addComponent(const QString& label, const QPointF& pos, const QSizeF& size) {
    QGraphicsRectItem* box = new QGraphicsRectItem(QRectF(pos, size));
    box->setBrush(QColor("#E0F7FA"));
    addToGroup(box);

    QGraphicsTextItem* text = new QGraphicsTextItem(label, box);
    text->setPos(pos.x() + 8, pos.y() + 5);

    return box;
}
