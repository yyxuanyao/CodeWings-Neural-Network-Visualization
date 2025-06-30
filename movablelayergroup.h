#pragma once

#include <QGraphicsItemGroup>
#include <QObject>

class MovableLayerGroup : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    MovableLayerGroup(QGraphicsItem* parent = nullptr)
        : QObject(), QGraphicsItemGroup(parent) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    }




signals:
    void positionChanged(MovableLayerGroup* group);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
        if (change == ItemPositionHasChanged) {
            emit positionChanged(this);
        }
        return QGraphicsItemGroup::itemChange(change, value);
    }
};
