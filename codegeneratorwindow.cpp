#include "codegeneratorwindow.h"
#include "ui_codegeneratorwindow.h"
#include "mainwindow.h"
#include "propertypanel.h"
#include "codegenerator.h"
#include <QGraphicsRectItem>
#include <QObject>
#include <QMimeData>
#include <QDrag>
#include <QGraphicsProxyWidget>
#include <QMenu>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QClipboard>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>
#include <QTimer>

CodeGeneratorWindow::CodeGeneratorWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CodeGeneratorWindow)
    , params()
    , isInCustomMode(false)
{
    ui->setupUi(this);
    // Initialize network visualizer
    m_builderScene = new QGraphicsScene(this);
    m_builderView = new QGraphicsView(m_builderScene, this);
    m_codeDisplay = new QTextEdit(this);
    m_codeDisplay->setReadOnly(true);
    m_propertyPanel = new PropertyPanel(this);
    m_networkVisualizer = new NetworkVisualizer(this);

    // Set main window layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Layer selection area
    QListWidget* layersList = new QListWidget(this);
    layersList->addItem("Input Layer");
    layersList->addItem("Hidden Layer");
    layersList->addItem("Output Layer");
    layersList->addItem("Dense Layer");
    layersList->addItem("Convolutional Layer");
    layersList->addItem("MaxPooling Layer");
    layersList->addItem("AveragePooling Layer");
    layersList->addItem("LSTM Layer");
    layersList->addItem("RNN Layer");
    layersList->addItem("GRU Layer");
    layersList->addItem("Dropout Layer");

    // 创建一个容器，用于放置画布和属性面板
    QWidget* canvasAndPanelContainer = new QWidget(this);
    QHBoxLayout* innerLayout = new QHBoxLayout(canvasAndPanelContainer);
    innerLayout->addWidget(m_builderView);
    innerLayout->addWidget(m_propertyPanel);
    innerLayout->setContentsMargins(0, 0, 0, 0);

    // 创建一个滚动区域，并将容器放入其中
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvasAndPanelContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 左侧层选择列表和滚动区域的布局
    QHBoxLayout* layersLayout = new QHBoxLayout();
    layersLayout->addWidget(layersList);
    layersLayout->addWidget(scrollArea);

    // Code display area
    mainLayout->addLayout(layersLayout);
    mainLayout->addWidget(m_codeDisplay);

    // 创建一个水平布局用于放置现有的代码显示框和复制按钮
    QHBoxLayout* codeLayout = new QHBoxLayout();
    codeLayout->addWidget(m_codeDisplay); // 使用现有的代码显示框
    // 创建复制按钮
    m_copyCodeButton = new QPushButton("copy", this);
    connect(m_copyCodeButton, &QPushButton::clicked, this, &CodeGeneratorWindow::on_copyCodeButton_clicked);
    codeLayout->addWidget(m_copyCodeButton, 0, Qt::AlignTop | Qt::AlignRight);
    mainLayout->addLayout(codeLayout);

    setLayout(mainLayout);

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);

    // Connect signals and slots
    connect(layersList, &QListWidget::itemClicked, this, &CodeGeneratorWindow::on_layersList_itemClicked);
    connect(m_propertyPanel, &PropertyPanel::parametersUpdated, this, &CodeGeneratorWindow::on_propertiesPanel_parametersUpdated);

    // Generate code button
    QPushButton* generateCodeButton = new QPushButton("Generate PyTorch Code", this);
    connect(generateCodeButton, &QPushButton::clicked, this, &CodeGeneratorWindow::on_generateCodeButton_clicked);
    buttonLayout->addWidget(generateCodeButton);
    buttonLayout->setSpacing(10);

    // Delete button
    QPushButton* deleteLayerButton = new QPushButton("Delete Selected Layer", this);
    connect(deleteLayerButton, &QPushButton::clicked, this, &CodeGeneratorWindow::deleteSelectedLayer);
    buttonLayout->addWidget(deleteLayerButton);

    // Return to main menu
    QPushButton* returnButton = new QPushButton("return", this);
    connect(returnButton, &QPushButton::clicked, this, &CodeGeneratorWindow::on_return_mainwindow_clicked);
    buttonLayout->addWidget(returnButton);

    buttonLayout->setContentsMargins(500, 0, 500, 0);
    mainLayout->addLayout(buttonLayout);

    // Enable mouse tracking
    m_builderView->setMouseTracking(true);
    setMouseTracking(true);

    //改变选择的层
    connect(m_builderScene, &QGraphicsScene::selectionChanged,
            this, &CodeGeneratorWindow::onSceneSelectionChanged);

}

CodeGeneratorWindow::~CodeGeneratorWindow()
{
    delete ui;
}


void CodeGeneratorWindow::on_return_mainwindow_clicked()
{
    this->close();
    MainWindow::instance()->show();
}

void CodeGeneratorWindow::deleteSelectedLayer() {
    QList<QGraphicsItem*> selectedItems = m_builderScene->selectedItems();
    if (!selectedItems.isEmpty()) {
        QGraphicsRectItem* selectedItem = qgraphicsitem_cast<QGraphicsRectItem*>(selectedItems[0]);
        if (selectedItem) {
            // 删除所有关联的连接线图形项
            foreach (QGraphicsItem* item, m_builderScene->items()) {
                if (item->type() == QGraphicsItem::UserType + 1) { // 假设连接线是自定义类型，可根据实际类型调整
                    m_builderScene->removeItem(item);
                    delete item;
                }
            }

            //将selectedItems对应的层从m_layers中删除
            for (QGraphicsItem* selectedItem : selectedItems) {
                // 找到与选中图元对应的层
                for (int i = 0; i < m_layers.size(); ++i) {
                    if (m_layers[i].layerType == selectedItem->data(0).value<NeuralLayer>().layerType) {
                        // 从 m_layers 中移除对应的层
                        m_layers.removeAt(i);
                        break;
                    }
                }
            }

            // 删除层图形项
            m_builderScene->removeItem(selectedItem);
            delete selectedItem;
        }
    }
    else{
        // 显示短暂提示消息（3秒后自动消失）
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Conform");
        msgBox->setText("Please select a layer");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStandardButtons(QMessageBox::NoButton); // 无按钮
        msgBox->show();
        // 3秒后自动关闭
        QTimer::singleShot(3000, msgBox, &QMessageBox::deleteLater);
    }
}
void CodeGeneratorWindow::on_generateCodeButton_clicked() {
    QList<NeuralLayer> layers;
    QMap<ConnectionPointItem*, NeuralLayer*> pointToLayer; // 连接点到层的映射

    // 解析连接关系（从输出点到输入点）
    QList<NeuralLayer*> orderedLayers;
    QSet<NeuralLayer*> visited;

    for (NeuralLayer& layer : m_layers) {
        orderedLayers.append(&layer);
        visited.insert(&layer);
        qDebug()<<layer.layerType;
    }

    // 生成代码
    QString code = CodeGenerator::generatePyTorchCode(orderedLayers);
    m_codeDisplay->setPlainText(code);
}

void CodeGeneratorWindow::on_layersList_itemClicked(QListWidgetItem* item) {
        if (!isInCustomMode){
        QString layerType = item->text();
        layerType.remove(" Layer");

        NeuralLayer layer;
        layer.layerType = layerType;

        QColor color = colorForLayerType(layerType);

        MyGraphicsRectItem* layerItem = new MyGraphicsRectItem();
        layerItem->setRect(0, 0, 100, 50);
        layerItem->setBrush(color);

        // 新增：将图形项指针存储在层对象中
        layer.graphicsItem = layerItem;
        layer.activationFunction = "relu";

        layerItem->setData(0, QVariant::fromValue(layer));
        m_builderScene->addItem(layerItem);

        params["LayerType"] = layerType;
        if(layerType =="Dense"){
            params["neurons"] = "10";
            params["activation"] = "ReLU";
            layer.neurons = 10;
            layer.activationFunction = "ReLU";
        }
        else if (layerType == "Convolutional") {
            params["filters"] = "32";
            params["kernel_size"] = "5";
            layer.filters = 32;
            layer.kernelSize = 5;
            layer.neurons = 1;
        }
        else if (layerType == "MaxPooling" || layerType == "AveragePooling") {
            params["pooling_size"] = "4";
            layer.poolingSize =4;
            layer.neurons = 1;
        }
        else if (layerType == "LSTM" || layerType == "RNN" || layerType == "GRU") {
            params["units"] = "128";
            layer.neurons = 1;
            layer.units = 128;
        }
        else if (layerType == "Dropout") {
            params["dropout_rate"] = "0.5";
            layer.dropoutRate = 0.5f;
            layer.neurons =1;
        }
        else if (layerType == "Input") {
            params["neurons"] = "15";
            layer.neurons = 15;
        }
        else if (layerType == "Output") {
            params["neurons"] = "2";
            layer.neurons = 2;
        }
        else if (layerType == "Hidden") {
            params["neurons"] = "10";
            params["activation"] = "ReLU";
            layer.neurons = 10;
            layer.activationFunction = "ReLU";
        }
        m_layers.append(layer);

        m_propertyPanel->clearParameters();

        m_propertyPanel->setParameters(params);

        layerItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        layerItem->setAcceptDrops(true);

        QGraphicsTextItem* layerNameText = new QGraphicsTextItem(layerType, layerItem);
        layerNameText->setPos(layerItem->rect().center().x() - layerNameText->boundingRect().width() / 2,
                              layerItem->rect().center().y() - layerNameText->boundingRect().height() / 2);

        layerItem->setZValue(1);

    }
    else{
        m_propertyPanel->clearParameters();
        QString layerType = item->text();
        layerType.remove(" Layer");

        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData();
        mimeData->setText(layerType);
        drag->setMimeData(mimeData);

        QPixmap pixmap(100, 50);
        pixmap.fill(colorForLayerType(layerType));
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(50, 25));

        drag->exec(Qt::CopyAction);
    }
}

void CodeGeneratorWindow::on_propertiesPanel_parametersUpdated(const QMap<QString, QString>& params) {
    // 更新选中的层的参数
    QList<QGraphicsItem*> selectedItems = m_builderScene->selectedItems();
    if (!selectedItems.isEmpty()) {
        QGraphicsItem* selectedItem = selectedItems.first();
        QVariant data = selectedItem->data(0);
        if (!data.isValid()) return;

        NeuralLayer layer = data.value<NeuralLayer>();
        QString layerType = layer.layerType;  // 从层实例获取类型

        // 根据层类型更新参数
        if (layerType == "Dense" || layerType == "Hidden" || layerType == "Input" || layerType == "Output") {
            layer.neurons = params["neurons"].toInt();
            layer.activationFunction = params["activation"];
        } else if (layerType == "Convolutional") {
            layer.filters = params["filters"].toInt();
            layer.kernelSize = params["kernel_size"].toInt();
        }
        else if (layerType == "MaxPooling" || layerType == "AveragePooling") {
            layer.poolingSize = params["pooling_size"].toInt();
        }
        else if (layerType == "LSTM" || layerType == "RNN") {
            layer.units = params["units"].toInt();
        }
        else if (layerType == "Dropout") {
            layer.dropoutRate = params["dropout_rate"].toFloat();
        }


        // 更新图形项中的数据
        selectedItem->setData(0, QVariant::fromValue(layer));

        // 更新层列表中的数据
        for (int i = 0; i < m_layers.size(); ++i) {
            if (m_layers[i].layerType == layerType && m_layers[i].graphicsItem == selectedItem) {
                m_layers[i] = layer;
                break;
            }
        }
    }
    else{
        // 显示短暂提示消息（3秒后自动消失）
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Conform");
        msgBox->setText("Please select a layer");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStandardButtons(QMessageBox::NoButton); // 无按钮
        msgBox->show();
        // 3秒后自动关闭
        QTimer::singleShot(3000, msgBox, &QMessageBox::deleteLater);
    }
}

QColor CodeGeneratorWindow::colorForLayerType(const QString& layerType) {
    if (layerType == "Input") return Qt::cyan;
    else if (layerType == "Hidden") return Qt::yellow;
    else if (layerType == "Output") return Qt::magenta;
    else if (layerType == "Convolutional") return Qt::green;
    else if (layerType == "MaxPooling") return Qt::blue;
    else if (layerType == "AveragePooling") return Qt::blue;
    else if (layerType == "LSTM") return Qt::red;
    else if (layerType == "RNN") return Qt::darkCyan;
    else if (layerType == "Dropout") return Qt::gray;
    else if (layerType == "Dense") return Qt::darkYellow;
    else return Qt::white;
}

void CodeGeneratorWindow::dropEvent(QDropEvent* event) {
    if (!isInCustomMode) {
        // 如果不在自定义模式，执行原有的代码生成功能逻辑
        return;
    }

    if (event->mimeData()->hasText()) {
        QString layerType = event->mimeData()->text();

        // 创建一个新的 NeuralLayer 对象
        NeuralLayer layer;
        layer.layerType = layerType;

        // 创建一个新的标签来表示神经网络层，并放置在工作区
        QLabel* layerLabel = new QLabel(this);
        layerLabel->setText(layerType);
        layerLabel->setFixedSize(150, 50);
        layerLabel->setAlignment(Qt::AlignCenter);
        layerLabel->setStyleSheet("background-color: white; border: 1px solid black;");
        //layerLabel->move(event->pos());
        layerLabel->move(event->position().toPoint());

        // 保存层到列表中，以便后续可以访问
        m_layers.append(layer);

        // 添加层到场景中
        QGraphicsRectItem* layerItem = new QGraphicsRectItem(0, 0, 100, 50);
        layerItem->setBrush(colorForLayerType(layerType));
        layerItem->setData(0, QVariant::fromValue(layer));
        m_builderScene->addItem(layerItem);
    }
}

void CodeGeneratorWindow::on_copyCodeButton_clicked() {
    // 获取代码显示框中的文本
    QString code = m_codeDisplay->toPlainText();
    // 获取系统剪贴板
    QClipboard* clipboard = QApplication::clipboard();
    // 将代码复制到剪贴板
    clipboard->setText(code);
}

QJsonArray CodeGeneratorWindow::getNetworkAsJson() const {
    QJsonArray array;
    for (const NeuralLayer& layer : m_layers) {
        array.append(layer.toJsonObject());
    }
    return array;
}

void CodeGeneratorWindow::clearNetwork()
{
    m_layers.clear();              // 清空网络层数据
    m_builderScene->clear();      // 清空画布
    m_codeDisplay->clear();       // 清空代码框
}

void CodeGeneratorWindow::onSceneSelectionChanged() {
    QList<QGraphicsItem*> selectedItems = m_builderScene->selectedItems();
    if (selectedItems.isEmpty()) {
        m_propertyPanel->clearParameters();
        return;
    }

    // 只处理第一个选中的项
    QGraphicsItem* selectedItem = selectedItems.first();
    NeuralLayer layer = selectedItem->data(0).value<NeuralLayer>();

    // 准备当前层的参数
    QMap<QString, QString> currentParams;
    currentParams["LayerType"] = layer.layerType;

    if (layer.layerType == "Dense" || layer.layerType == "Hidden") {
        currentParams["neurons"] = QString::number(layer.neurons);
        currentParams["activation"] = layer.activationFunction;
    } else if (layer.layerType == "Convolutional") {
        currentParams["filters"] = QString::number(layer.filters);
        currentParams["kernel_size"] = QString::number(layer.kernelSize);
    } else if (layer.layerType == "MaxPooling" || layer.layerType == "AvgPooling") {
        currentParams["pooling_size"] = QString::number(layer.poolingSize);
    } else if (layer.layerType == "LSTM" || layer.layerType == "RNN" || layer.layerType == "GRU") {
        currentParams["units"] = QString::number(layer.units);
    } else if (layer.layerType == "Dropout") {
        currentParams["dropout_rate"] = QString::number(layer.dropoutRate);
    } else if (layer.layerType == "Input" || layer.layerType == "Output") {
        currentParams["neurons"] = QString::number(layer.neurons);
    }

    // 更新属性面板（会清除旧参数）
    m_propertyPanel->setParameters(currentParams);
}
