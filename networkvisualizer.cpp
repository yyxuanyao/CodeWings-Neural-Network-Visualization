// #include "networkvisualizer.h"
// #include <QRandomGenerator>
// #include <QGraphicsRectItem>
// #include <QMimeData>
// #include <QDrag>
// #include <QApplication>
// #include <QMouseEvent>
// #include <QDragEnterEvent>
// #include <QDropEvent>
// #include <QGraphicsRectItem>
// #include "colorthememanager.h"

// NetworkVisualizer::NetworkVisualizer(QWidget* parent)
//     : QGraphicsView(parent), m_scene(new QGraphicsScene(this)) {
//     setScene(m_scene);
//     setRenderHint(QPainter::Antialiasing);
//     setDragMode(QGraphicsView::RubberBandDrag);
//     setAcceptDrops(true); // 启用拖拽功能
// }

// QGraphicsItemGroup* NetworkVisualizer::createDetailedLayer(
//     const NeuralLayer& layer,

//     int yPos
//     ) {
//     const QString layerName = QString("%1").arg(layer.layerType);
//     const QString activation= QString("%1").arg(layer.activationFunction);
//     if (layerName=="Dropout"){


//     }
//     const int width = 160;
//     const int height = 130;
//     const int x = 100;
//     const ColorTheme& theme = ColorThemeManager::currentTheme();
//     QGraphicsItemGroup* group = new QGraphicsItemGroup();

//     // 背景框
//     QGraphicsRectItem* bg = new QGraphicsRectItem(0, 0, width, height);
//     bg->setBrush(theme.neuronFill);//QColor(240, 240, 240)
//     bg->setPen(QPen(theme.text));//QPen(Qt::blue)
//     group->addToGroup(bg);

//     // 层标签
//     QGraphicsTextItem* title = new QGraphicsTextItem(layerName);
//     title->setPos(40, 5);
//     group->addToGroup(title);

//     // 图形项
//     QGraphicsRectItem* w = nullptr;
//     QGraphicsRectItem* b = nullptr;
//     QGraphicsEllipseItem* plus = nullptr;
//     QGraphicsRectItem* act = nullptr;

//     if (layerName == "Hidden"||layerName == "Dense") {
//         w = new QGraphicsRectItem(0, 0, 30, 30);
//         w->setBrush(theme.weightBoxFill);//Qt::cyan
//         w->setPos(10, 30);
//         group->addToGroup(w);
//         QGraphicsTextItem* wLabel = new QGraphicsTextItem("W", w);
//         wLabel->setPos(8, 5);

//         b = new QGraphicsRectItem(0, 0, 30, 30);
//         b->setBrush(theme.weightBoxFill);//cyan
//         b->setPos(width - 40, 30);
//         group->addToGroup(b);
//         QGraphicsTextItem* bLabel = new QGraphicsTextItem("b", b);
//         bLabel->setPos(8, 5);
//         plus = new QGraphicsEllipseItem(0, 0, 20, 20);
//         plus->setBrush(theme.neuronFill);//Qt::white
//         plus->setPos(width / 2 - 10, 60);
//         group->addToGroup(plus);
//         QGraphicsTextItem* plusLabel = new QGraphicsTextItem("+", plus);
//         plusLabel->setPos(2, 0);
//     }
//     if (layerName == "Dropout") {
//         // 1. 创建 Dropout 层的矩形框（初始宽度设为 100，后面会根据文本调整）
//         QGraphicsRectItem* act = new QGraphicsRectItem(0, 0, 100, 26);
//         act->setBrush(theme.activationBoxFill);
//         act->setPos(30, 90);
//         group->addToGroup(act);

//         // 2. 创建标签文本（强制显示 4 位小数）
//         QString dropoutText = QString("rate: %1").arg(layer.dropoutRate, 0, 'f', 4);
//         QGraphicsTextItem* actLabel = new QGraphicsTextItem(dropoutText, act);
//         actLabel->setPos(10, 5);

//         // 3. 动态计算文本宽度，并调整矩形框大小
//         QFontMetrics metrics(actLabel->font());
//         int textWidth = metrics.horizontalAdvance(dropoutText);  // 获取文本像素宽度
//         act->setRect(0, 0, textWidth + 20, 26);

//     }

//     if (!activation.trimmed().isEmpty()) {
//         act = new QGraphicsRectItem(0, 0, 100, 26);
//         act->setBrush(theme.activationBoxFill);//QColor(180, 220, 255)
//         act->setPos(30, 90);
//         group->addToGroup(act);
//         QGraphicsTextItem* actLabel = new QGraphicsTextItem(activation, act);
//         actLabel->setPos(10, 5);
//     }

//     if (w) w->setZValue(1);
//     if (b) b->setZValue(1);
//     if (plus) plus->setZValue(1);
//     if (act) act->setZValue(1);


//     // 连线（局部坐标系内连接）
//     if (w && b && plus) {
//         QPointF wCenter = w->pos() + QPointF(w->rect().width() / 2, w->rect().height() / 2);
//         QPointF bCenter = b->pos() + QPointF(b->rect().width() / 2, b->rect().height() / 2);
//         QPointF plusCenter = plus->pos() + QPointF(plus->rect().width() / 2, plus->rect().height() / 2);

//         QPen pen(theme.connectionHighWeight);//Qt::black
//         pen.setWidth(2);
//         QGraphicsLineItem* lineW = new QGraphicsLineItem(QLineF(wCenter, plusCenter));
//         lineW->setPen(pen);
//         lineW->setZValue(0);
//         group->addToGroup(lineW);

//         QGraphicsLineItem* lineB = new QGraphicsLineItem(QLineF(bCenter, plusCenter));
//         lineB->setPen(pen);
//         lineB->setZValue(0);
//         group->addToGroup(lineB);
//     }

//     if (plus && act) {
//         QPointF plusCenter = plus->pos() + QPointF(plus->rect().width() / 2, plus->rect().height() / 2);
//         QPointF actTopCenter = act->pos() + QPointF(act->rect().width() / 2, 0);

//         QPen pen(theme.connectionHighWeight);//Qt::black
//         pen.setWidth(2);
//         QGraphicsLineItem* lineToAct = new QGraphicsLineItem(QLineF(plusCenter, actTopCenter));
//         lineToAct->setPen(pen);
//         lineToAct->setZValue(0);
//         group->addToGroup(lineToAct);
//     }




//     group->setPos(x, yPos);  // 整体移动
//     m_scene->addItem(group);
//     //group->setZValue(0); // 在上层显示

//     return group;
// }

// void NetworkVisualizer::createNetwork(const QList<NeuralLayer>& layers) {
//     m_scene->clear();
//     m_allNeurons.clear();
//     QVector<QVector<NeuronItem*>> allNeurons;

//     const int xSpacing = 200;
//     const int ySpacing = 60;

//     for (int i = 0; i < layers.size(); ++i) {
//         QVector<NeuronItem*> layerNeurons;
//         const NeuralLayer& layer = layers[i];
//         int yOffset = -(layer.neurons - 1) * ySpacing / 2;

//         // 🔹 层前缀标识
//         QString prefix;
//         if (i == 0) prefix = "I";
//         else if (i == layers.size() - 1) prefix = "O";
//         else prefix = "H";

//         // 🔹 添加层标签文本
//         QString label = QString("%1\n(%2)").arg(layer.layerType).arg(layer.activationFunction);
//         QGraphicsTextItem* layerLabel = m_scene->addText(label);
//         layerLabel->setDefaultTextColor(Qt::darkBlue);
//         layerLabel->setPos(i * xSpacing - 30, yOffset - 60);

//         // 🔹 添加神经元
//         for (int j = 0; j < layer.neurons; ++j) {
//             NeuronItem* neuron = new NeuronItem(QString("%1%2").arg(prefix).arg(j + 1));
//             neuron->updateColors();
//             m_scene->addItem(neuron);
//             neuron->setPos(i * xSpacing, yOffset + j * ySpacing);
//             layerNeurons.append(neuron);
//         }

//         allNeurons.append(layerNeurons);// 改为存储到成员变量
//         m_allNeurons = allNeurons;
//     }

//     // 🔹 添加连接线
//     for (int i = 0; i < allNeurons.size() - 1; ++i) {
//         for (NeuronItem* from : allNeurons[i]) {
//             for (NeuronItem* to : allNeurons[i + 1]) {
//                 double weight = QRandomGenerator::global()->bounded(1.0);
//                 ConnectionItem* conn = new ConnectionItem(from->scenePos(), to->scenePos(), weight);
//                 m_scene->addItem(conn);

//                 from->addOutgoingConnection(conn);
//                 to->addIncomingConnection(conn);
//             }
//         }
//     }

// }

// void NetworkVisualizer::createblockNetwork(const QList<NeuralLayer>& layers) {
//     m_scene->clear();
//     m_layerGroups.clear();

//     const int layerSpacing = 150;
//     QList<QGraphicsItemGroup*> layerGroups;

//     for (int i = 0; i < layers.size(); ++i) {
//         const NeuralLayer& layer = layers[i];

//         QGraphicsItemGroup* group = createDetailedLayer(
//             layer,
//             20 + i * layerSpacing
//             );
//         layerGroups.append(group);
//         m_layerGroups.append(group);
//     }

//     // 连接线
//     for (int i = 0; i < layerGroups.size() - 1; ++i) {
//         auto from = layerGroups[i];
//         auto to = layerGroups[i + 1];

//         QPointF p1 = from->sceneBoundingRect().center();
//         p1.setY(from->sceneBoundingRect().bottom());

//         QPointF p2 = to->sceneBoundingRect().center();
//         p2.setY(to->sceneBoundingRect().top());

//         m_scene->addLine(QLineF(p1, p2), QPen(Qt::black));
//     }

// }
// // networkvisualizer.cpp
// void NetworkVisualizer::applyColorTheme(const QString& themeName) {
//     ColorThemeManager::setCurrentTheme(themeName);
//     const ColorTheme& theme = ColorThemeManager::currentTheme();

//     // 1. 更新层组颜色（合并为单次遍历）
//     for (QGraphicsItemGroup* group : m_layerGroups) {
//         for (QGraphicsItem* item : group->childItems()) {
//             if (auto* rect = dynamic_cast<QGraphicsRectItem*>(item)) {
//                 // 层背景框
//                 if (rect->rect().width() == 160 && rect->rect().height() == 130) {
//                     rect->setBrush(theme.layerBackground);
//                     rect->setPen(QPen(theme.neuronBorder, 1));
//                 }
//                 // 权重/偏置框
//                 else if (rect->rect().width() == 30 && rect->rect().height() == 30) {
//                     rect->setBrush(theme.weightBoxFill);
//                 }
//                 // 激活函数框
//                 else if (rect->rect().width() == 100 && rect->rect().height() == 26) {
//                     rect->setBrush(theme.activationBoxFill);
//                 }
//             }
//             // 文本颜色
//             else if (auto* text = dynamic_cast<QGraphicsTextItem*>(item)) {
//                 text->setDefaultTextColor(theme.text);
//             }
//         }
//     }

//     // 2. 更新神经元颜色
//     for (auto& layer : m_allNeurons) {
//         for (NeuronItem* neuron : layer) {
//             neuron->updateColors();
//         }
//     }

//     // 3. 更新连接线颜色
//     for (QGraphicsItem* item : scene()->items()) {
//         if (auto* conn = dynamic_cast<ConnectionItem*>(item)) {
//             conn->updateColor();
//         }
//     }
// }

// /*

// void NetworkVisualizer::mouseMoveEvent(QMouseEvent* event) {
//     if (m_dragItem) {
//         QPointF pos = event->pos();
//         QPointF delta = pos - m_dragStartPos;
//         m_dragItem->setPos(m_dragItem->pos() + delta);
//         m_dragStartPos = pos;
//     }
//     QGraphicsView::mouseMoveEvent(event);
// }

// void NetworkVisualizer::mousePressEvent(QMouseEvent* event) {
//     QGraphicsItem* item = itemAt(event->pos());
//     if (item) {
//         m_dragItem = item;
//         m_dragStartPos = event->pos();
//         QMimeData* mimeData = new QMimeData();
//         mimeData->setData("application/x-layer", item->data(0).toByteArray());
//         QDrag* drag = new QDrag(this);
//         drag->setMimeData(mimeData);
//         drag->exec();
//     }
//     QGraphicsView::mousePressEvent(event);
// }*/

// void NetworkVisualizer::dragMoveEvent(QDragMoveEvent* event) {
//     if (event->mimeData()->hasFormat("application/x-layer")) {
//         event->acceptProposedAction();
//     }
// }

// void NetworkVisualizer::dropEvent(QDropEvent* event) {
//     if (event->mimeData()->hasFormat("application/x-layer")) {
//         QByteArray data = event->mimeData()->data("application/x-layer");
//         NeuralLayer layer = qvariant_cast<NeuralLayer>(QVariant::fromValue(data));

//         // 根据层类型设置不同颜色和形状
//         QColor color;
//         if (layer.layerType == "Input") color = Qt::cyan;
//         else if (layer.layerType == "Hidden") color = Qt::yellow;
//         else if (layer.layerType == "Output") color = Qt::magenta;
//         else if (layer.layerType == "Convolutional") color = Qt::green;
//         else if (layer.layerType == "Pooling") color = Qt::blue;
//         else if (layer.layerType == "LSTM") color = Qt::red;
//         else if (layer.layerType == "RNN") color = Qt::darkCyan;
//         else if (layer.layerType == "Dropout") color = Qt::gray;

//         // 创建层的图形项并添加到场景
//         QGraphicsRectItem* layerItem = new QGraphicsRectItem(QRectF(0, 0, 100, 50), nullptr);
//         layerItem->setPos(event->pos());
//         layerItem->setBrush(color);
//         layerItem->setData(0, QVariant::fromValue(layer));
//         m_scene->addItem(layerItem);
//         m_layers.append(layer);
//     }
// }
#include "networkvisualizer.h"
#include <QRandomGenerator>
#include <QGraphicsRectItem>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsRectItem>
#include "colorthememanager.h"
#include "movablelayergroup.h"

NetworkVisualizer::NetworkVisualizer(QWidget* parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)) {
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true); // 启用拖拽功能
}
void NetworkVisualizer::updateConnections() {
    qDebug() << "Updating connections";
    for (const ConnectionLine& conn : m_connections) {
        if (!conn.fromGroup || !conn.toGroup || !conn.line) continue;

        QPointF p1 = conn.fromGroup->mapToScene(
            conn.fromGroup->boundingRect().center().x(),
            conn.fromGroup->boundingRect().bottom()
            );
        QPointF p2 = conn.toGroup->mapToScene(
            conn.toGroup->boundingRect().center().x(),
            conn.toGroup->boundingRect().top()
            );

        conn.line->setLine(QLineF(p1, p2));
    }
}
void NetworkVisualizer::createConnection(MovableLayerGroup* from, MovableLayerGroup* to) {
    QPointF p1 = from->sceneBoundingRect().center();
    p1.setY(from->sceneBoundingRect().bottom());
    QPointF p2 = to->sceneBoundingRect().center();
    p2.setY(to->sceneBoundingRect().top());

    QGraphicsLineItem* line = m_scene->addLine(QLineF(p1, p2), QPen(Qt::black));
    m_connections.append({line, from, to});
}



MovableLayerGroup* NetworkVisualizer::createDetailedLayer(
    const NeuralLayer& layer,

    int yPos
    ) {
    const QString layerName = QString("%1").arg(layer.layerType);
    const QString activation= QString("%1").arg(layer.activationFunction);
    const int width = 160;
    const int height = 130;
    const int x = 100;
    const ColorTheme& theme = ColorThemeManager::currentTheme();
    //QGraphicsItemGroup* group = new QGraphicsItemGroup();
    MovableLayerGroup* group = new MovableLayerGroup();
    connect(group, &MovableLayerGroup::positionChanged, this, &NetworkVisualizer::updateConnections);


    // 背景框
    QGraphicsRectItem* bg = new QGraphicsRectItem(0, 0, width, height);
    bg->setBrush(theme.neuronFill);//QColor(240, 240, 240)
    bg->setPen(QPen(theme.text));//QPen(Qt::blue)
    group->addToGroup(bg);

    // 层标签
    QGraphicsTextItem* title = new QGraphicsTextItem(layerName);
    title->setPos(40, 5);
    group->addToGroup(title);

    // 图形
    QGraphicsRectItem* w = nullptr;
    QGraphicsRectItem* b = nullptr;
    QGraphicsEllipseItem* plus = nullptr;
    QGraphicsRectItem* act = nullptr;

    if (layerName == "Hidden"||layerName == "Dense") {
        w = new QGraphicsRectItem(0, 0, 30, 30);
        w->setBrush(theme.weightBoxFill);//Qt::cyan
        w->setPos(10, 30);
        group->addToGroup(w);
        QGraphicsTextItem* wLabel = new QGraphicsTextItem("W", w);
        wLabel->setPos(8, 5);

        b = new QGraphicsRectItem(0, 0, 30, 30);
        b->setBrush(theme.weightBoxFill);//cyan
        b->setPos(width - 40, 30);
        group->addToGroup(b);
        QGraphicsTextItem* bLabel = new QGraphicsTextItem("b", b);
        bLabel->setPos(8, 5);
        plus = new QGraphicsEllipseItem(0, 0, 20, 20);
        plus->setBrush(theme.neuronFill);//Qt::white
        plus->setPos(width / 2 - 10, 60);
        group->addToGroup(plus);
        QGraphicsTextItem* plusLabel = new QGraphicsTextItem("+", plus);
        plusLabel->setPos(2, 0);
    }
    if (layerName == "Dropout") {
        //Dropout 层框
        QGraphicsRectItem* act = new QGraphicsRectItem(0, 0, 100, 26);
        act->setBrush(theme.activationBoxFill);
        act->setPos(30, 90);
        group->addToGroup(act);

        // 标签文本
        QString dropoutText = QString("rate: %1").arg(layer.dropoutRate, 0, 'f', 4);//（4位小数）
        QGraphicsTextItem* actLabel = new QGraphicsTextItem(dropoutText, act);
        actLabel->setPos(10, 5);

        // 动态计算文本宽度调整矩形框大小
        QFontMetrics metrics(actLabel->font());
        int textWidth = metrics.horizontalAdvance(dropoutText);  // 获取文本像素宽度
        act->setRect(0, 0, textWidth + 20, 26);

    }

    if (layerName== "LSTM" || layerName == "RNN" || layerName == "GRU"){
        QGraphicsRectItem* act = new QGraphicsRectItem(0, 0, 100, 26);
        act->setBrush(theme.activationBoxFill);
        act->setPos(30, 90);
        group->addToGroup(act);

        // 标签文本
        QString dropoutText = QString("units: %1").arg(layer.units);
        QGraphicsTextItem* actLabel = new QGraphicsTextItem(dropoutText, act);
        actLabel->setPos(10, 5);

        // 动态计算文本宽度调整矩形框大小
        QFontMetrics metrics(actLabel->font());
        int textWidth = metrics.horizontalAdvance(dropoutText);  // 获取文本像素宽度
        act->setRect(0, 0, textWidth + 20, 26);
    }

    if (layerName == "Convolutional"){

        QGraphicsRectItem* filtersRect = new QGraphicsRectItem(0, 0, 100, 26);
        filtersRect->setBrush(theme.activationBoxFill);
        filtersRect->setPos(30, 60);
        group->addToGroup(filtersRect);

        QGraphicsRectItem* kernelRect = new QGraphicsRectItem(0, 0, 100, 26);
        kernelRect->setBrush(theme.activationBoxFill);
        kernelRect->setPos(30, 90);  // 放在filters下方
        group->addToGroup(kernelRect);
        // 标签文本
        QString filtersText = QString("filters: %1").arg(layer.filters);
        QGraphicsTextItem* filtersLabel = new QGraphicsTextItem(filtersText, filtersRect);
        filtersLabel->setPos(10, 5);


        QString kernelText = QString("kernel: %1").arg(layer.kernelSize);
        QGraphicsTextItem* kernelLabel = new QGraphicsTextItem(kernelText, kernelRect);
        kernelLabel->setPos(10, 5);

        // 动态计算文本宽度调整矩形框大小
        QFontMetrics metrics(filtersLabel->font());
        int filtersWidth = metrics.horizontalAdvance(filtersText) + 20;
        filtersRect->setRect(0, 0, filtersWidth, 26);


        int kernelWidth = metrics.horizontalAdvance(kernelText) + 20;
        kernelRect->setRect(0, 0, kernelWidth, 26);
    }

    if (layerName== "MaxPooling" || layerName == "AveragePooling")  {
        //pooling层框
        QGraphicsRectItem* act = new QGraphicsRectItem(0, 0, 100, 26);
        act->setBrush(theme.activationBoxFill);
        act->setPos(30, 90);
        group->addToGroup(act);

        // 标签文本
        QString dropoutText = QString("poolingSize: %1").arg(layer.poolingSize);
        QGraphicsTextItem* actLabel = new QGraphicsTextItem(dropoutText, act);
        actLabel->setPos(10, 5);

        // 动态计算文本宽度调整矩形框大小
        QFontMetrics metrics(actLabel->font());
        int textWidth = metrics.horizontalAdvance(dropoutText);  // 获取文本像素宽度
        act->setRect(0, 0, textWidth + 20, 26);

    }

    if (!activation.trimmed().isEmpty()&&(layerName == "Hidden"||layerName == "Dense")) {
        act = new QGraphicsRectItem(0, 0, 100, 26);
        act->setBrush(theme.activationBoxFill);//QColor(180, 220, 255)
        act->setPos(30, 90);
        group->addToGroup(act);
        QGraphicsTextItem* actLabel = new QGraphicsTextItem(activation, act);
        actLabel->setPos(10, 5);
    }

    if (w) w->setZValue(1);
    if (b) b->setZValue(1);
    if (plus) plus->setZValue(1);
    if (act) act->setZValue(1);


    // 连线（局部坐标系内）
    if (w && b && plus) {
        QPointF wCenter = w->pos() + QPointF(w->rect().width() / 2, w->rect().height() / 2);
        QPointF bCenter = b->pos() + QPointF(b->rect().width() / 2, b->rect().height() / 2);
        QPointF plusCenter = plus->pos() + QPointF(plus->rect().width() / 2, plus->rect().height() / 2);

        QPen pen(theme.connectionHighWeight);//Qt::black
        pen.setWidth(2);
        QGraphicsLineItem* lineW = new QGraphicsLineItem(QLineF(wCenter, plusCenter));
        lineW->setPen(pen);
        lineW->setZValue(0);
        group->addToGroup(lineW);

        QGraphicsLineItem* lineB = new QGraphicsLineItem(QLineF(bCenter, plusCenter));
        lineB->setPen(pen);
        lineB->setZValue(0);
        group->addToGroup(lineB);
    }

    if (plus && act) {
        QPointF plusCenter = plus->pos() + QPointF(plus->rect().width() / 2, plus->rect().height() / 2);
        QPointF actTopCenter = act->pos() + QPointF(act->rect().width() / 2, 0);

        QPen pen(theme.connectionHighWeight);//Qt::black
        pen.setWidth(2);
        QGraphicsLineItem* lineToAct = new QGraphicsLineItem(QLineF(plusCenter, actTopCenter));
        lineToAct->setPen(pen);
        lineToAct->setZValue(0);
        group->addToGroup(lineToAct);
    }




    group->setPos(x, yPos);  // 整体移动
    m_scene->addItem(group);
    //group->setZValue(0); // 在上层显示

    return group;
}


void NetworkVisualizer::createNetwork(const QList<NeuralLayer>& layers) {
    m_scene->clear();
    m_allNeurons.clear();
    QVector<QVector<NeuronItem*>> allNeurons;

    const int xSpacing = 200;
    const int ySpacing = 60;

    for (int i = 0; i < layers.size(); ++i) {
        QVector<NeuronItem*> layerNeurons;
        const NeuralLayer& layer = layers[i];
        int yOffset = -(layer.neurons - 1) * ySpacing / 2;

        //层前缀标识
        QString prefix;
        if (i == 0) prefix = "I";
        else if (i == layers.size() - 1) prefix = "O";
        else prefix = "H";

        // 层标签文本
        QString label = QString("%1\n(%2)").arg(layer.layerType).arg(layer.activationFunction);
        QGraphicsTextItem* layerLabel = m_scene->addText(label);
        layerLabel->setDefaultTextColor(Qt::darkBlue);
        layerLabel->setPos(i * xSpacing - 30, yOffset - 60);

        // 神经元
        for (int j = 0; j < layer.neurons; ++j) {
            NeuronItem* neuron = new NeuronItem(QString("%1%2").arg(prefix).arg(j + 1));
            neuron->updateColors();
            m_scene->addItem(neuron);
            neuron->setPos(i * xSpacing, yOffset + j * ySpacing);
            layerNeurons.append(neuron);
        }

        allNeurons.append(layerNeurons);// 存储到成员变量
        m_allNeurons = allNeurons;
    }

    // 连接线
    for (int i = 0; i < allNeurons.size() - 1; ++i) {
        for (NeuronItem* from : allNeurons[i]) {
            for (NeuronItem* to : allNeurons[i + 1]) {
                double weight = QRandomGenerator::global()->bounded(1.0);
                ConnectionItem* conn = new ConnectionItem(from->scenePos(), to->scenePos(), weight);
                m_scene->addItem(conn);

                from->addOutgoingConnection(conn);
                to->addIncomingConnection(conn);
            }
        }
    }

}

void NetworkVisualizer::createblockNetwork(const QList<NeuralLayer>& layers) {
    m_scene->clear();
    m_layerGroups.clear();

    const int layerSpacing = 150;
    //QList<QGraphicsItemGroup*> layerGroups;
    QList<MovableLayerGroup*> layerGroups;


    for (int i = 0; i < layers.size(); ++i) {
        const NeuralLayer& layer = layers[i];
        MovableLayerGroup* group = createDetailedLayer(layer, 20 + i * layerSpacing);
        group->setData(0, QVariant::fromValue(const_cast<NeuralLayer*>(&layer)));  // 需要存储指针关联

        layerGroups.append(group);
        m_layerGroups.append(group);
    }

    // 连接线
    for (int i = 0; i < layerGroups.size() - 1; ++i) {
        auto from = layerGroups[i];
        auto to = layerGroups[i + 1];

        QPointF p1 = from->sceneBoundingRect().center();
        p1.setY(from->sceneBoundingRect().bottom());

        QPointF p2 = to->sceneBoundingRect().center();
        p2.setY(to->sceneBoundingRect().top());

        QGraphicsLineItem* line = m_scene->addLine(QLineF(p1, p2), QPen(Qt::black));
        m_connections.append({line, from, to});

        // 连接信号 - 现在类型匹配了
        connect(from, &MovableLayerGroup::positionChanged,
                this, &NetworkVisualizer::updateConnections);
        connect(to, &MovableLayerGroup::positionChanged,
                this, &NetworkVisualizer::updateConnections);
    }
}





void NetworkVisualizer::applyColorTheme(const QString& themeName) {
    ColorThemeManager::setCurrentTheme(themeName);
    const ColorTheme& theme = ColorThemeManager::currentTheme();

    // 更新层组颜色
    for (QGraphicsItemGroup* group : m_layerGroups) {
        for (QGraphicsItem* item : group->childItems()) {
            if (auto* rect = dynamic_cast<QGraphicsRectItem*>(item)) {
                // 层背景
                if (rect->rect().width() == 160 && rect->rect().height() == 130) {
                    rect->setBrush(theme.layerBackground);
                    rect->setPen(QPen(theme.neuronBorder, 1));
                }
                // 权重/偏置
                else if (rect->rect().width() == 30 && rect->rect().height() == 30) {
                    rect->setBrush(theme.weightBoxFill);
                }
                // 激活函数框
                else if (rect->rect().width() == 100 && rect->rect().height() == 26) {
                    rect->setBrush(theme.activationBoxFill);
                }
            }
            // text颜色
            else if (auto* text = dynamic_cast<QGraphicsTextItem*>(item)) {
                text->setDefaultTextColor(theme.text);
            }
        }
    }

    // 神经元颜色
    for (auto& layer : m_allNeurons) {
        for (NeuronItem* neuron : layer) {
            neuron->updateColors();
        }
    }

    // 更新连接线
    for (QGraphicsItem* item : scene()->items()) {
        if (auto* conn = dynamic_cast<ConnectionItem*>(item)) {
            conn->updateColor();
        }
    }
}

void NetworkVisualizer::refreshLayerItem(NeuralLayer* layer) {
    for (int i = 0; i < m_layerGroups.size(); ++i) {
        auto group = m_layerGroups[i];
        if (group->data(0).value<NeuralLayer*>() == layer) {
            QPointF oldPos = group->pos();

            // 删除旧连接线
            for (auto it = m_connections.begin(); it != m_connections.end(); ) {
                if (it->fromGroup == group || it->toGroup == group) {
                    m_scene->removeItem(it->line);
                    delete it->line;
                    it = m_connections.erase(it);
                } else {
                    ++it;
                }
            }

            //重建图层
            m_scene->removeItem(group);
            delete group;

            auto newGroup = createDetailedLayer(*layer, oldPos.y());
            newGroup->setPos(oldPos);
            newGroup->setData(0, QVariant::fromValue(layer));
            m_layerGroups[i] = newGroup;

            //重建连接线
            if (i > 0) {
                createConnection(m_layerGroups[i-1], newGroup);
            }
            if (i < m_layerGroups.size()-1) {
                createConnection(newGroup, m_layerGroups[i+1]);
            }

            update();
            break;
        }
    }
}


void NetworkVisualizer::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-layer")) {
        event->acceptProposedAction();
    }
}

void NetworkVisualizer::dropEvent(QDropEvent* event) {
    if (event->mimeData()->hasFormat("application/x-layer")) {
        QByteArray data = event->mimeData()->data("application/x-layer");
        NeuralLayer layer = qvariant_cast<NeuralLayer>(QVariant::fromValue(data));

        // 根据层类型设颜色和形状
        QColor color;
        if (layer.layerType == "Input") color = Qt::cyan;
        else if (layer.layerType == "Hidden") color = Qt::yellow;
        else if (layer.layerType == "Output") color = Qt::magenta;
        else if (layer.layerType == "Convolutional") color = Qt::green;
        else if (layer.layerType == "Pooling") color = Qt::blue;
        else if (layer.layerType == "LSTM") color = Qt::red;
        else if (layer.layerType == "RNN") color = Qt::darkCyan;
        else if (layer.layerType == "Dropout") color = Qt::gray;

        // 创建层的图形项并添加到场景
        QGraphicsRectItem* layerItem = new QGraphicsRectItem(QRectF(0, 0, 100, 50), nullptr);
        layerItem->setPos(event->pos());
        layerItem->setBrush(color);
        layerItem->setData(0, QVariant::fromValue(layer));
        m_scene->addItem(layerItem);
        m_layers.append(layer);
    }
}
