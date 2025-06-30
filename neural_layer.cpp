#include "mainwindow.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

NeuralLayer::NeuralLayer(LayerType type, QGraphicsItem* parent)
    : QGraphicsRectItem(0, 0, 120, 60, parent)
    , m_type(type)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    // 设置不同层类型的颜色
    QColor color;
    switch (type) {
    case InputLayer:
        color = QColor(180, 220, 255); // 蓝色调
        break;
    case DenseLayer:
        color = QColor(255, 220, 180); // 橙色调
        break;
    case Conv2DLayer:
        color = QColor(180, 255, 180); // 绿色调
        break;
    case MaxPooling2DLayer:
        color = QColor(220, 180, 255); // 紫色调
        break;
    case FlattenLayer:
        color = QColor(255, 180, 180); // 红色调
        break;
    case OutputLayer:
        color = QColor(180, 180, 255); // 靛色调
        break;
    }

    setBrush(color);
    setPen(QPen(Qt::black, 1));

    // 添加层名称文本
    m_textItem = new QGraphicsTextItem(getTypeName(), this);
    m_textItem->setPos(10, 10);
}

QString NeuralLayer::getTypeName() const
{
    switch (m_type) {
    case InputLayer: return "Input";
    case DenseLayer: return "Dense";
    case Conv2DLayer: return "Conv2D";
    case MaxPooling2DLayer: return "MaxPool2D";
    case FlattenLayer: return "Flatten";
    case OutputLayer: return "Output";
    default: return "Unknown";
    }
}

void NeuralLayer::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void NeuralLayer::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    // 开始拖放操作
    QDrag* drag = new QDrag(event->widget());
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("application/x-neural-layer", QByteArray::number(m_type));

    drag->setMimeData(mimeData);

    // 创建拖动时的图像
    QPixmap pixmap(boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter, nullptr, nullptr);
    painter.end();

    drag->setPixmap(pixmap);
    drag->setHotSpot(m_dragStartPosition.toPoint());

    drag->exec(Qt::CopyAction);
}

void NeuralLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);
}

QJsonObject NeuralLayer::toJsonObject() const
{
    QJsonObject obj;
    obj["type"] = m_type;
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    return obj;
}

NeuralLayer* NeuralLayer::fromJsonObject(const QJsonObject& obj, QGraphicsItem* parent)
{
    LayerType type = static_cast<LayerType>(obj["type"].toInt());
    NeuralLayer* layer = new NeuralLayer(type, parent);
    layer->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    return layer;
}
