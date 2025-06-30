#ifndef NEURONITEM_H
#define NEURONITEM_H
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include "connectionitem.h"

class NeuronItem : public QGraphicsEllipseItem {
public:
    NeuronItem(const QString& label, QGraphicsItem* parent = nullptr);
    void updateColors();  // 根据当前主题更新颜色
    void addOutgoingConnection(ConnectionItem* conn);
    void addIncomingConnection(ConnectionItem* conn);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    QGraphicsTextItem* m_label;
    QList<ConnectionItem*> m_outgoingConnections;
    QList<ConnectionItem*> m_incomingConnections;
};


#endif // NEURONITEM_H
