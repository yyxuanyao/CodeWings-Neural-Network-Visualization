#include "layeritem.h"
#include <QPainter>

LayerItem::LayerItem(const QString& type, QGraphicsItem* parent)
    : QGraphicsItem(parent), layerType(type) {}

QRectF LayerItem::boundingRect() const {
    return QRectF(0, 0, 100, 50); // 固定尺寸
}

void LayerItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setBrush(Qt::cyan);
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, layerType);
}
