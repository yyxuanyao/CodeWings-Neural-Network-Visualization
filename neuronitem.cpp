#include "neuronitem.h"
#include "colorthememanager.h"  // 假设已实现全局颜色管理器
#include "connectionitem.h"
#include <QBrush>
#include <QPen>


NeuronItem::NeuronItem(const QString& label, QGraphicsItem* parent)
    : QGraphicsEllipseItem(parent) {
    setRect(-10, -10, 20, 20);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);// 允许拖动和选择
    setAcceptHoverEvents(true); // 支持鼠标悬停事件

    m_label = new QGraphicsTextItem(label, this);
    m_label->setPos(-15, -30);

    updateColors();
    setZValue(1);
}
void NeuronItem::addOutgoingConnection(ConnectionItem* conn) {
    m_outgoingConnections.append(conn);
}

void NeuronItem::addIncomingConnection(ConnectionItem* conn) {
    m_incomingConnections.append(conn);
}

QVariant NeuronItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemPositionHasChanged) {
        QPointF newPos = value.toPointF();
        for (ConnectionItem* conn : m_outgoingConnections) {
            conn->updateLine(newPos, conn->endPos());
        }
        for (ConnectionItem* conn : m_incomingConnections) {
            conn->updateLine(conn->startPos(), newPos);
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void NeuronItem::updateColors() {
    const ColorTheme& theme = ColorThemeManager::currentTheme();

    setBrush(QBrush(theme.neuronFill));
    setPen(QPen(theme.neuronBorder, 1));

    m_label->setDefaultTextColor(theme.text);
}

