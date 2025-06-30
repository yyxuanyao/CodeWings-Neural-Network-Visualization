#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "colorthememanager.h"
#include "backend.h"
#include "propertypanel.h"
#include "networkvisualizer.h"
#include "matrial.h"
#include <QIcon>
#include <QPushButton>
#include <QJsonDocument>
#include <QLabel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsRectItem>
#include <QJsonArray>
#include <QFile>
#include <QProgressBar>
#include <QDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QToolTip>
#include <QApplication>
#include <QPixmap>
#include <QPalette>

PropertyPanel* propertyPanel;

void MainWindow::setupIconButton(QPushButton* button, const QString& iconPath, int size) {
    button->setFixedSize(3*size, size);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(size, size));
    // button->setStyleSheet("background-color:transparent");
}

void MainWindow::setBackground(const QString& background){
    QPixmap pixmap(background);
    QSize windowSize=this->size();
    QPixmap scalePixmap=pixmap.scaled(windowSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette palette=this->palette();
    palette.setBrush(QPalette::Window,QBrush(scalePixmap));
    this->setPalette(palette);
    qDebug() << "背景图片是否加载成功：" << !pixmap.isNull();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    original=1;
    applyTheme("blue");
    original=0;

    imageGenerate=0;

    setWindowTitle("CodeWings:Neural-Network-Visualization");

    setBackground(":/Icon/background.jpg");

    QToolTip::setFont(QFont("Microsoft YaHei", 10));

    QString tooltipStyle = R"(
    QToolTip {
        background-color: #d0eaff;     /* 浅蓝 */
        color: black;                  /* 黑字 */
        border: 1px solid #007acc;
        padding: 6px;
        border-radius: 4px;
    } )";

    qApp->setStyleSheet(qApp->styleSheet() + tooltipStyle);
    ui->userGuide->setToolTip("查看使用说明");
    ui->pageColor->setToolTip("切面界面颜色");
    ui->neuralTheme->setToolTip("切换图像主题");
    ui->selectMode->setToolTip("切换显示模式");
    ui->generateCode->setToolTip("生成 PyTorch 代码");
    ui->generateImage->setToolTip("生成网络结构图像");
    ui->checkHistory->setToolTip("查看已保存的历史");
    ui->startNew->setToolTip("开始新的神经网络");
    ui->lastStep->setToolTip("返回上一步");
    ui->nextStep->setToolTip("展示网络图片");
    ui->saveCurrent->setToolTip("保存当前神经网络结构");

    QMenu* themeMenu = new QMenu("切换主题", this);
    themeMenu->addAction("white", this, [=]() { applyTheme("white"); });
    themeMenu->addAction("pink", this, [=]() { applyTheme("pink"); });
    themeMenu->addAction("blue", this, [=]() { applyTheme("blue"); });
    themeMenu->addAction("purple", this, [=]() { applyTheme("purple"); });
    themeMenu->addAction("yellow", this, [=]() { applyTheme("yellow"); });
    themeMenu->addAction("green", this, [=]() { applyTheme("green"); });
    themeMenu->addAction("grey", this, [=]() { applyTheme("grey"); });

    ui->pageColor->setMenu(themeMenu);  // 设置菜单挂载到按钮

    QMenu* colorMenu = new QMenu(this);

    QAction* classic = new QAction("Classic", this);
    QAction* vibrant = new QAction("Vibrant", this);
    QAction* dark = new QAction("Dark", this);
    QAction* ocean = new QAction("Ocean", this);
    colorMenu->addAction(classic);
    colorMenu->addAction(vibrant);
    colorMenu->addAction(dark);
    colorMenu->addAction(ocean);
    ui->neuralTheme->setMenu(colorMenu);

    connect(classic, &QAction::triggered, this, [=]() {
        ColorThemeManager::setCurrentTheme("Classic");
        showFloatingMessage("设置Classic 按generate——image更新");
    });
    connect(vibrant, &QAction::triggered, this, [=]() {
        ColorThemeManager::setCurrentTheme("Vibrant");
        showFloatingMessage("设置Vibrant 按generate——image更新");
    });
    connect(dark, &QAction::triggered, this, [=]() {
        ColorThemeManager::setCurrentTheme("Dark");
        showFloatingMessage("设置Dark 按generate——image更新 ");
    });
    connect(ocean, &QAction::triggered, this, [=]() {
        ColorThemeManager::setCurrentTheme("Ocean");
        showFloatingMessage("设置Ocean 按generate——image更新");
    });


    codegeneratorwindow = new CodeGeneratorWindow(this);
    connect(ui->generateCode, &QPushButton::clicked, this, &MainWindow::on_generateCode_clicked);
    QMenu* modeMenu = new QMenu(this);

    matrialwindow = new Matrial(this);
    matrialwindow->hide();
    connect(ui->userGuide, &QPushButton::clicked, this, &MainWindow::on_userGuide_clicked);

    QAction* blockGenerateAction = new QAction("BlockGenerate 模式", this);
    QAction* neuronitemGenerateAction = new QAction("NeuronitemGenerate 模式", this);

    modeMenu->addAction(blockGenerateAction);
    modeMenu->addAction(neuronitemGenerateAction);
    ui->selectMode->setMenu(modeMenu);

    connect(blockGenerateAction, &QAction::triggered, this, [=]() {
        currentMode = "BlockGenerate";
        showFloatingMessage("BlockGenerator");
    });
    connect(neuronitemGenerateAction, &QAction::triggered, this, [=]() {
        currentMode = "NeuronitemGenerate";
        showFloatingMessage("NeuronitemGenerate");
    });

    scene = new QGraphicsScene(this);

    currentNetworkSaved=0;

    position=-1;
}

void MainWindow::on_userGuide_clicked()
{
    this->hide();
    matrialwindow->show();
}

void MainWindow::on_generateCode_clicked()
{
    if (!codeWin) {
        codeWin = new CodeGeneratorWindow(this);
    }

    this->hide();              // 隐藏主界面
    codeWin->show();           // 显示弹窗

    imageGenerate = 0;
}

void MainWindow::on_generateImage_clicked()
{
    if (!codeWin) {
        showWarningMessage("尚未创建网络结构！");
        return;
    }

    historySaved.push_back(false);
    QJsonArray structure = codeWin->getNetworkAsJson();
    historyCache.push_back(structure);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QString modeLabel = "Undefined";
    if (currentMode == "BlockGenerate"){
        modeLabel = "Block";
    }
    else if (currentMode == "NeuronitemGenerate"){
        modeLabel = "Neuronitem";
    }
    QString label = QString("%1 | %2").arg(timestamp) .arg(modeLabel);
    historyLabel.push_back(label);
    position = historyCache.size() - 1;

    QList<NeuralLayer> layers;
    for (const QJsonValue& val : structure) {
        if (val.isObject()) {
            layers.append(NeuralLayer::fromJsonObject(val.toObject()));
        }
    }

    if (structure.isEmpty()) {
        showWarningMessage("网络结构为空，无法生成图像！");
        return;
    }

    // 调用你已有的神经网络图像生成逻辑（比如显示在主界面某个区域）
    NetworkVisualizer* visualizer = new NetworkVisualizer();
    QString theme = ColorThemeManager::getCurrentTheme();  // 获取当前主题
    ColorThemeManager::setCurrentTheme(theme);
    if (currentMode=="BlockGenerate"){
        visualizer->createblockNetwork(layers);
        visualizer->show();// 你来实现这个函数，基于 structure 展示图像
        ui->scrollAreavisualizer->setWidget(visualizer);
    }
    else if (currentMode=="NeuronitemGenerate"){
        visualizer->createNetwork(layers);
        visualizer->show();// 你来实现这个函数，基于 structure 展示图像
        ui->scrollAreavisualizer->setWidget(visualizer);
    }
    else{
        showWarningMessage("请选择神经网络图像模式");
    }

    imageGenerate=1;
}

void MainWindow::on_checkHistory_clicked()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("历史记录");
    dialog->setMinimumSize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QListWidget* list = new QListWidget(dialog);

    // 添加历史记录条目
    int cnt = 0;
    for (int i = 0; i < historyCache.size(); ++i) {
        if (historySaved[i]){
            cnt += 1;
            QString label = QString("记录 %1 | ").arg(cnt) + historyLabel[i];
            list->addItem(label);
        }
    }

    layout->addWidget(list);

    // 加载按钮
    QPushButton* loadBtn = new QPushButton("加载选中记录");
    layout->addWidget(loadBtn);
    dialog->setLayout(layout);

    // 连接加载逻辑
    connect(loadBtn, &QPushButton::clicked, this, [=]() {
        int index = list->currentRow();
        if (index < 0 || index >= historyCache.size()) return;

        if (!historySaved[index]) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "未保存更改",
                "当前记录尚未保存，是否仍要加载？",
                QMessageBox::Yes | QMessageBox::No
                );
            if (reply == QMessageBox::No) return;
        }

        const QJsonArray& layers = historyCache[index];
        QList<NeuralLayer> parsedLayers;
        for (const QJsonValue& val : layers) {
            if (val.isObject()) {
                parsedLayers.append(NeuralLayer::fromJsonObject(val.toObject()));
            }
        }

        // 可视化加载
        NetworkVisualizer* visualizer = new NetworkVisualizer(this);
        visualizer->setMinimumSize(600, 400);
        QString theme = ColorThemeManager::getCurrentTheme();
        ColorThemeManager::setCurrentTheme(theme);

        if (currentMode == "BlockGenerate") {
            visualizer->createblockNetwork(parsedLayers);
        } else if (currentMode == "NeuronitemGenerate") {
            visualizer->createNetwork(parsedLayers);
        } else {
            showWarningMessage("请选择神经网络图像模式！");
            delete visualizer;
            return;
        }

        ui->scrollAreavisualizer->setWidget(visualizer);
        showFloatingMessage("✅ 已加载历史记录");

        dialog->accept();  // 关闭弹窗
    });

    dialog->exec();
}

void MainWindow::onHistoryRecordClicked(int index){
    position = index;
    const QJsonArray& structure = historyCache[index];
    QList<NeuralLayer> layers;
    for (const QJsonValue& val : structure) {
        if (val.isObject()) {
            layers.append(NeuralLayer::fromJsonObject(val.toObject()));
        }
    }

    // 创建 NetworkVisualizer 组件并展示
    NetworkVisualizer* visualizer = new NetworkVisualizer(this);
    visualizer->setMinimumSize(600, 400);  // 可调节尺寸

    // 设置主题（如有）
    QString theme = ColorThemeManager::getCurrentTheme();
    ColorThemeManager::setCurrentTheme(theme);

    if (currentMode == "BlockGenerate") {
        visualizer->createblockNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else if (currentMode == "NeuronitemGenerate") {
        visualizer->createNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else {
        delete visualizer;
        showWarningMessage("❗ 当前未选择图像模式，请先设置图像生成模式！");
        return;
    }

    showFloatingMessage(QString("✅ 已加载历史记录：%1").arg(historyLabel[index]));
}

void MainWindow::on_startNew_clicked()
{
    // 1. 弹出确认对话框
    if (!currentNetworkSaved){
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "开始新的神经网络",
            "当前网络结构尚未保存。\n是否继续？继续将清空当前结构。",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }

        if (!codeWin) {
            return;
        }
    }
    else{
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "开始新的神经网络",
            "当前网络结构已保存。\n是否继续？继续将清空当前结构。",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }

        if (!codeWin) {
            return;
        }
    }

    // 2. 确认清空神经网络结构及图像
    if (codeWin) {
        codeWin->clearNetwork();
    }

    QJsonArray structure = codeWin->getNetworkAsJson();
    QList<NeuralLayer> layers;
    for (const QJsonValue& val : structure) {
        if (val.isObject()) {
            layers.append(NeuralLayer::fromJsonObject(val.toObject()));
        }
    }
    NetworkVisualizer* visualizer = new NetworkVisualizer();
    QString theme = ColorThemeManager::getCurrentTheme();
    ColorThemeManager::setCurrentTheme(theme);
    visualizer->createblockNetwork(layers);
    visualizer->show();
    ui->scrollAreavisualizer->setWidget(visualizer);

    currentNetworkSaved = false;  // 标记为未保存
    showFloatingMessage("已清空网络结构，开始新的构建");
}

void MainWindow::on_lastStep_clicked(){
    qDebug()<<position<<" "<<historyCache.size();
    if (position == 0){
        showWarningMessage("已经是第一步");
        return;
    }
    position -= 1;
    const QJsonArray& structure = historyCache[position];
    QList<NeuralLayer> layers;
    for (const QJsonValue& val : structure) {
        if (val.isObject()) {
            layers.append(NeuralLayer::fromJsonObject(val.toObject()));
        }
    }
    NetworkVisualizer* visualizer = new NetworkVisualizer(this);
    visualizer->setMinimumSize(600, 400);
    QString theme = ColorThemeManager::getCurrentTheme();
    ColorThemeManager::setCurrentTheme(theme);

    if (currentMode == "BlockGenerate") {
        visualizer->createblockNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else if (currentMode == "NeuronitemGenerate") {
        visualizer->createNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else {
        delete visualizer;
        showWarningMessage("❗ 当前未选择图像模式，请先设置图像生成模式！");
        return;
    }
}

void MainWindow::on_nextStep_clicked(){
    if (position == historyCache.size() - 1){
        showWarningMessage("已经是最后一步");
        return;
    }
    position += 1;
    const QJsonArray& structure = historyCache[position];
    QList<NeuralLayer> layers;
    for (const QJsonValue& val : structure) {
        if (val.isObject()) {
            layers.append(NeuralLayer::fromJsonObject(val.toObject()));
        }
    }
    NetworkVisualizer* visualizer = new NetworkVisualizer(this);
    visualizer->setMinimumSize(600, 400);
    QString theme = ColorThemeManager::getCurrentTheme();
    ColorThemeManager::setCurrentTheme(theme);
    if (currentMode == "BlockGenerate") {
        visualizer->createblockNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else if (currentMode == "NeuronitemGenerate") {
        visualizer->createNetwork(layers);
        ui->scrollAreavisualizer->setWidget(visualizer);
    } else {
        delete visualizer;
        showWarningMessage("❗ 当前未选择图像模式，请先设置图像生成模式！");
        return;
    }
}

void MainWindow::on_saveCurrent_clicked(){
    if (!codeWin){
        return;
    }
    if (!imageGenerate){
        historySaved.push_back(false);
        QJsonArray structure = codeWin->getNetworkAsJson();
        historyCache.push_back(structure);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
        QString modeLabel = "Undefined";
        if (currentMode == "BlockGenerate"){
            modeLabel = "Block";
        }
        else if (currentMode == "NeuronitemGenerate"){
            modeLabel = "Neuronitem";
        }
        QString label = QString("%1 | %2").arg(timestamp) .arg(modeLabel);
        historyLabel.push_back(label);
        position = historyCache.size() - 1;
    }
    *(historySaved.rbegin())=true;

    showSaveProgressBarMessage();

    QJsonArray layersArray = codeWin->getNetworkAsJson();
    QJsonObject entry;
    entry["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    entry["mode"] = currentMode;
    entry["network"] = QJsonObject{ { "layers", layersArray } };

    QFile file("history.json");
    QJsonArray history;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isArray()) history = doc.array();
        file.close();
    }

    history.append(entry);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument newDoc(history);
        file.write(newDoc.toJson());
        file.close();
    }

    currentNetworkSaved=1;
}

void MainWindow::handleJsonData(const QString &jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        // 假设obj表示网络结构，从中提取层信息
        QJsonArray layersArray = obj["layers"].toArray();
        for (const QJsonValue &layerValue : layersArray) {
            QJsonObject layerObj = layerValue.toObject();
            NeuralLayer layer = NeuralLayer::fromJsonObject(layerObj);
            // 后续可对layer进行操作
        }
    }
}

void MainWindow::showFloatingMessage(const QString& text)
{
    QLabel* label = new QLabel(this);
    label->setText(text);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background-color: rgba(50, 50, 50, 180);"
        "color: white;"
        "font-size: 16px;"
        "padding: 10px;"
        "border-radius: 8px;"
        );
    label->adjustSize();

    int x = (width() - label->width()) / 2;
    int y = (height() - label->height()) / 10;
    label->move(x, y);
    label->show();

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(effect);
    effect->setOpacity(1.0);

    QPropertyAnimation* fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation* moveUp = new QPropertyAnimation(label, "pos");
    moveUp->setDuration(1000);
    moveUp->setStartValue(QPoint(x, y));
    moveUp->setEndValue(QPoint(x, y - 50));
    moveUp->setEasingCurve(QEasingCurve::OutQuad);

    QParallelAnimationGroup* group = new QParallelAnimationGroup(label);
    group->addAnimation(fadeOut);
    group->addAnimation(moveUp);

    QTimer::singleShot(100, this, [group]() {
        group->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(group, &QPropertyAnimation::finished, label, &QLabel::deleteLater);
}

void MainWindow::showSaveProgressBarMessage()
{
    QWidget* popup = new QWidget(this);
    popup->setStyleSheet("background-color: rgba(50, 50, 50, 180); border-radius: 10px;");
    popup->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout* layout = new QVBoxLayout(popup);

    QLabel* label = new QLabel("✔ 已保存当前网络配置！");
    label->setStyleSheet("color: white; font-size: 16px;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QProgressBar* progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(10);
    progressBar->setStyleSheet(R"(
        QProgressBar {
            background-color: rgba(255, 255, 255, 50);
            border: 1px solid white;
            border-radius: 5px;
        }
        QProgressBar::chunk {
            background-color: limegreen;
            border-radius: 5px;
        }
    )");
    layout->addWidget(progressBar);

    popup->setLayout(layout);
    popup->adjustSize();

    int x = (width() - popup->width()) / 2;
    int y = (height() - popup->height()) / 10;
    popup->move(x, y);
    popup->show();

    QTimer* timer = new QTimer(popup);
    int interval = 30;
    QObject::connect(timer, &QTimer::timeout, this, [=]() mutable {
        int val = progressBar->value();
        if (val >= 100) {
            timer->stop();
            popup->close();
        } else {
            progressBar->setValue(val + 2);
        }
    });
    timer->start(interval);
}

void MainWindow::showWarningMessage(const QString& text)
{
    QLabel* label = new QLabel(this);
    label->setText("⚠ " + text);  // 加警告图标
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background-color: rgba(180, 30, 30, 200);"  // 深红半透明
        "color: white;"
        "font-size: 16px;"
        "padding: 10px;"
        "border: 2px solid #ffaaaa;"
        "border-radius: 8px;"
        );
    label->adjustSize();

    // 显示在中上位置
    int x = (width() - label->width()) / 2;
    int y = (height() - label->height()) / 10;
    label->move(x, y);
    label->show();

    // 添加淡出动画
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(effect);
    effect->setOpacity(1.0);

    QPropertyAnimation* fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::OutQuad);

    QTimer::singleShot(1500, this, [=]() {
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(fadeOut, &QPropertyAnimation::finished, label, &QLabel::deleteLater);
}

void MainWindow::visualizeNetwork(const QJsonArray& layers)
{
    QGraphicsScene* scene = new QGraphicsScene(this);

    int x = 50;
    int y = 50;
    int spacing = 150;

    for (const QJsonValue& val : layers) {
        QJsonObject obj = val.toObject();
        QString type = obj["layerType"].toString();

        // 1. 图层框（灰框）
        QGraphicsRectItem* item = new QGraphicsRectItem(0, 0, 120, 60);
        item->setPos(x, y);

        QColor color = Qt::lightGray;
        if (type == "Input") color = Qt::cyan;
        else if (type == "Dense") color = Qt::yellow;
        else if (type == "Dropout") color = Qt::gray;
        else if (type == "LSTM") color = Qt::green;
        else if (type == "RNN") color = Qt::blue;

        item->setBrush(color);
        scene->addItem(item);

        // 2. 类型文字
        QGraphicsTextItem* text = new QGraphicsTextItem(type);
        text->setPos(x + 10, y + 20);
        scene->addItem(text);

        // 3. 准备下一层位置
        y += spacing;
    }

    // 展示到画布
    QGraphicsView* view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumHeight(400);

    // 自适应缩放（可选）
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // 清空展示区并添加
    QLayout* layout = ui->previewArea->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->previewArea);
        ui->previewArea->setLayout(layout);
    } else {
        QLayoutItem* item;
        while ((item = layout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
    }

    layout->addWidget(view);
}

void MainWindow::clearPreviewArea()
{
    QLayout* layout = ui->previewArea->layout();
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) {
            item->widget()->deleteLater();  // 删除控件
        }
        delete item;  // 删除布局项
    }
}

void MainWindow::applyTheme(const QString& theme)
{
    QString bgColor;
    QString btnColor;
    QString btnHover;
    QString tooltipBg = "#d0eaff";
    QString tooltipColor = "black";

    if (theme == "white") {
        bgColor = "#ffffff";
        btnColor = "#f0f0f0";
        btnHover = "#dddddd";
    } else if (theme == "pink") {
        bgColor = "#ffe6f2";
        btnColor = "#ff99cc";
        btnHover = "#ff66b2";
    } else if (theme == "blue") {
        bgColor = "#e6f2ff";
        btnColor = "#99ccff";
        btnHover = "#66b3ff";
    } else if (theme == "purple") {
        bgColor = "#f5e6ff";
        btnColor = "#cc99ff";
        btnHover = "#b366ff";
    } else if (theme == "yellow") {
        bgColor = "#fffbe6";
        btnColor = "#ffeb99";
        btnHover = "#ffe066";
    } else if (theme == "green") {
        bgColor = "#e6ffe6";
        btnColor = "#99ff99";
        btnHover = "#66ff66";
    } else if (theme == "grey") {
        bgColor = "#f0f0f0";
        btnColor = "#d0d0d0";
        btnHover = "#bbbbbb";
    }

    // 设置全局样式

    qApp->setStyleSheet(QString(R"(
        QWidget {
            background-color: %1;
            color: black;
        }
        /*
        QPushButton {
            background-color: %2;
            border: 1px solid #666;
            padding: 5px;
            border-radius: 5px;
        }*/
        QPushButton:hover {
            background-color: %3;
        }
        QToolTip {
            background-color: %4;
            color: %5;
            border: 1px solid gray;
            padding: 6px;
            border-radius: 4px;
        }
    )")
                            .arg(bgColor)
                            .arg(btnColor)
                            .arg(btnHover)
                            .arg(tooltipBg)
                            .arg(tooltipColor)
                        );

    setupIconButton(ui->userGuide, ":/Icon/user-"+theme+".png");
    setupIconButton(ui->pageColor, ":/Icon/theme-"+theme+".png");
    setupIconButton(ui->neuralTheme, ":/Icon/color-"+theme+".png");
    setupIconButton(ui->selectMode, ":/Icon/mode-"+theme+".png");
    setupIconButton(ui->generateCode, ":/Icon/code-"+theme+".png");
    setupIconButton(ui->generateImage, ":/Icon/image-"+theme+".png");
    setupIconButton(ui->checkHistory, ":/Icon/history-"+theme+".png");
    setupIconButton(ui->startNew, ":/Icon/new-"+theme+".png");
    setupIconButton(ui->lastStep, ":/Icon/previous-"+theme+".png");
    setupIconButton(ui->nextStep, ":/Icon/turnback-"+theme+".png");
    setupIconButton(ui->saveCurrent, ":/Icon/save-"+theme+".png");

    if (!original) {
        showFloatingMessage("🎨 已切换主题：" + theme);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
// 静态成员定义
MainWindow* MainWindow::s_instance = nullptr;

MainWindow* MainWindow::instance() {
    return s_instance;
}

void MainWindow::setInstance(MainWindow* window) {
    s_instance = window;
}
