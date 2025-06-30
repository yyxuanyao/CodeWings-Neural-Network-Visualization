#include "connectionitem.h"
#include "colorthememanager.h"
#include <QPen>

ConnectionItem::ConnectionItem(const QPointF& from, const QPointF& to, double weight)
    : m_weight(weight) {
    setLine(QLineF(from, to));
    updateColor();
    setZValue(0);
}

void ConnectionItem::updateColor() {
    const ColorTheme& theme = ColorThemeManager::currentTheme();
    QPen pen;
    pen.setColor(m_weight > 0.5 ? theme.connectionHighWeight : theme.connectionLowWeight);
    pen.setWidthF(0.1 + m_weight * 1.9);
    setPen(pen);
}

void ConnectionItem::updateLine(const QPointF& from, const QPointF& to) {
    setLine(QLineF(from, to));
}

