#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include "neuronitem.h"
#include "backend.h"
#include "movablelayergroup.h"
#include "connectionitem.h"
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>

class NetworkVisualizer : public QGraphicsView {
    Q_OBJECT
public:
    NetworkVisualizer(QWidget* parent = nullptr);
    void createNetwork(const QList<NeuralLayer>& layers);
    //void createNetwork(const QJsonArray& layersJson);
    void createblockNetwork(const QList<NeuralLayer>& layers);
    void applyColorTheme(const QString& themeName);
    //QGraphicsItemGroup* createDetailedLayer(const NeuralLayer& layer , int yPos);
    MovableLayerGroup* createDetailedLayer(const NeuralLayer& layer , int yPos);
    void createConnection(MovableLayerGroup* from, MovableLayerGroup* to);
    void refreshLayerItem(NeuralLayer* layer);

protected:
    //void mousePressEvent(QMouseEvent* event) override;
    //void mouseMoveEvent(QMouseEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;


private:
    QGraphicsScene* m_scene;
    QList<NeuralLayer> m_layers;
    QGraphicsItem* m_dragItem = nullptr;
    QPointF m_dragStartPos;
    QVector<QVector<NeuronItem*>> m_allNeurons; // 存储神经元指针以便更新
    //QList<QGraphicsItemGroup*> m_layerGroups;
    QList<MovableLayerGroup*> m_layerGroups;
    struct ConnectionLine {
        QGraphicsLineItem* line;
        QGraphicsItemGroup* fromGroup;
        QGraphicsItemGroup* toGroup;
    };

    QList<ConnectionLine> m_connections;
private slots:
    void updateConnections();
};
