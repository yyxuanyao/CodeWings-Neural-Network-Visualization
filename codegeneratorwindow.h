#ifndef CODEGENERATORWINDOW_H
#define CODEGENERATORWINDOW_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QListWidget>
#include <QStackedWidget>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "networkvisualizer.h"
#include "propertypanel.h"

namespace Ui {
class CodeGeneratorWindow;
}

class MyGraphicsRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit MyGraphicsRectItem(QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(parent) {
        setFlags(ItemIsMovable | ItemSendsGeometryChanges);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
        if (change == ItemPositionHasChanged && scene()) {
            emit positionChanged();
        }
        return QGraphicsRectItem::itemChange(change, value);
    }

signals:
    void positionChanged();
};

class ConnectionPointItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    explicit ConnectionPointItem(QGraphicsItem* parentItem = nullptr)
        : QGraphicsEllipseItem(0, 0, 10, 10, parentItem) {
        setBrush(Qt::blue);
        setData(0, "connection_point");
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setZValue(2);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        emit mousePressed(event);
        event->accept();
    }

signals:
    void mousePressed(QGraphicsSceneMouseEvent* event);
};

class CodeGeneratorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CodeGeneratorWindow(QWidget *parent = nullptr);
    QJsonArray getNetworkAsJson() const;
    ~CodeGeneratorWindow();
    void onSceneSelectionChanged();

public slots:
    void on_return_mainwindow_clicked();//
    void on_generateCodeButton_clicked();//
    void on_layersList_itemClicked(QListWidgetItem* item);//
    void on_propertiesPanel_parametersUpdated(const QMap<QString, QString>& params);//
    void deleteSelectedLayer();//
    QColor colorForLayerType(const QString& layerType);//
    void dropEvent(QDropEvent* event) override;//
    void updateConnections();//
    void addConnectionPoints(QGraphicsRectItem* layerItem);//
    void startConnectionDrag(ConnectionPointItem* connectionPoint, QGraphicsSceneMouseEvent* event);//
    void updateDragLine(const QPointF& pos);//
    void endConnectionDrag(QGraphicsSceneMouseEvent* event);//
    void createConnection(ConnectionPointItem* fromPoint, ConnectionPointItem* toPoint);//
    void updateDragPath(const QPointF& pos);//
    void updateLayerConnections(QGraphicsRectItem* layerItem);//
    void on_copyCodeButton_clicked();
    void clearNetwork();

protected:
    void mouseMoveEvent(QMouseEvent* event) override;//
    void mouseReleaseEvent(QMouseEvent* event) override;//
    //bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::CodeGeneratorWindow *ui;
    QList<QSharedPointer<NeuralLayer>> m_layerStorage;
    NetworkVisualizer* m_networkVisualizer;
    QGraphicsView* m_builderView;
    QGraphicsScene* m_builderScene;
    QTextEdit* m_codeDisplay;
    PropertyPanel* m_propertyPanel;
    QList<NeuralLayer*> m_layers;
    QMap<QString, QString> params;
    QList<QPair<ConnectionPointItem*,ConnectionPointItem*>> m_connections;
    ConnectionPointItem* m_dragConnectionPoint;
    QGraphicsPathItem* m_dragPath;
    bool isInCustomMode;
    QList<QGraphicsPathItem*> m_connectionItems;
    QGraphicsLineItem* m_dragLine;
    QPushButton* m_copyCodeButton;
};

#endif // CODEGENERATORWINDOW_H
