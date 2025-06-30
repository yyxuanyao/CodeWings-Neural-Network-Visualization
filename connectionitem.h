#pragma once
#include <QGraphicsLineItem>

class ConnectionItem : public QGraphicsLineItem {
public:
    ConnectionItem(const QPointF& from, const QPointF& to, double weight);

    void updateLine(const QPointF& from, const QPointF& to);
    QPointF startPos() const { return line().p1(); }
    QPointF endPos() const { return line().p2(); }

    void updateColor();  // 根据当前主题更新颜色
    double weight() const { return m_weight; }

private:
    double m_weight;
};
