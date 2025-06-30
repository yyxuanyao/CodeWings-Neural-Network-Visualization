#pragma once
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

class LayerBlockItem : public QGraphicsItemGroup {
public:
    LayerBlockItem(const QString& title, const QString& activation, int neuronCount, QGraphicsItem* parent = nullptr);


private:
    //void addComponent(const QString& label, const QPointF& pos, const QSizeF& size);
    QGraphicsRectItem* addComponent(const QString& label, const QPointF& pos, const QSizeF& size);


};
