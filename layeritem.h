#ifndef LAYERITEM_H
#define LAYERITEM_H

#include <QGraphicsItem>
#include <QString>

class LayerItem : public QGraphicsItem
{
public:
    LayerItem(const QString& type, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    QString layerType;
};

#endif

