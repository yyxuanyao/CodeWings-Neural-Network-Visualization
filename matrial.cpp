#include "matrial.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QPixmap>
#include <QSize>
#include <QMessageBox>
#include <QKeyEvent>

Matrial::Matrial(QWidget *parent)
    : QMainWindow(parent), currentIndex(0)
{
    setupUI();
    loadImages();
    updateImage();

    // 设置窗口属性
    setWindowTitle("神经网络结构图片浏览器");
    setMinimumSize(800, 600);
    resize(1200, 900);

    backToMainButton = new QPushButton("返回主界面", this);
    backToMainButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1976D2;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "}"
        );
    connect(backToMainButton, &QPushButton::clicked, this, &Matrial::on_backToMain_clicked);
    buttonLayout->addWidget(backToMainButton);
}

Matrial::~Matrial()
{
}

void Matrial::setupUI()
{
    // 创建中央部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    mainLayout = new QVBoxLayout(centralWidget);

    // 创建图片显示标签
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("QLabel { border: 2px solid #cccccc; background-color: #f5f5f5; }");
    imageLabel->setMinimumSize(600, 400);
    imageLabel->setScaledContents(false);

    // 创建页码标签
    pageLabel = new QLabel("第 1 页 共 1 页", this);
    pageLabel->setAlignment(Qt::AlignCenter);
    pageLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; color: #333333; }");

    // 创建按钮布局
    buttonLayout = new QHBoxLayout();

    // 创建上一页按钮
    prevButton = new QPushButton("← 上一页", this);
    prevButton->setEnabled(false);
    prevButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "}"
        );

    // 创建下一页按钮
    nextButton = new QPushButton("下一页 →", this);
    nextButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1976D2;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "}"
        );

    // 连接信号和槽
    connect(prevButton, &QPushButton::clicked, this, &Matrial::previousImage);
    connect(nextButton, &QPushButton::clicked, this, &Matrial::nextImage);

    // 添加按钮到布局
    buttonLayout->addStretch();
    buttonLayout->addWidget(prevButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addStretch();

    // 添加组件到主布局
    mainLayout->addWidget(imageLabel, 1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(pageLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(10);

    // 设置布局边距
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void Matrial::loadImages()
{
    // 从资源文件中加载图片路径
    // 您需要根据实际的资源文件结构修改这些路径
    imagePaths << ":/images/neural_network_1.png"
               << ":/images/neural_network_2.png"
               << ":/images/neural_network_3.png"
               << ":/images/neural_network_4.png"
               << ":/images/neural_network_5.png";

    // 预加载所有图片
    images.clear();
    for (const QString &path : imagePaths) {
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            images.append(pixmap);
        } else {
            // 如果图片加载失败，创建一个占位图片
            QPixmap placeholder(400, 300);
            placeholder.fill(Qt::lightGray);
            images.append(placeholder);
        }
    }

    // 如果没有图片，添加一个默认图片
    if (images.isEmpty()) {
        QPixmap defaultPixmap(400, 300);
        defaultPixmap.fill(Qt::lightGray);
        images.append(defaultPixmap);
        imagePaths.append("默认图片");
    }
}

void Matrial::updateImage()
{
    if (images.isEmpty() || currentIndex < 0 || currentIndex >= images.size()) {
        return;
    }

    // 获取当前图片
    QPixmap currentPixmap = images[currentIndex];

    // 缩放图片以适应标签大小，保持纵横比
    QSize labelSize = imageLabel->size();
    QPixmap scaledPixmap = currentPixmap.scaled(
        labelSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    imageLabel->setPixmap(scaledPixmap);

    // 更新页码标签
    pageLabel->setText(QString("第 %1 页 共 %2 页").arg(currentIndex + 1).arg(images.size()));

    // 更新按钮状态
    prevButton->setEnabled(currentIndex > 0);
    nextButton->setEnabled(currentIndex < images.size() - 1);
}

void Matrial::previousImage()
{
    if (currentIndex > 0) {
        currentIndex--;
        updateImage();
    }
}

void Matrial::nextImage()
{
    if (currentIndex < images.size() - 1) {
        currentIndex++;
        updateImage();
    }
}

void Matrial::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // 窗口大小改变时重新缩放图片
    updateImage();
}

void Matrial::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Up:
        previousImage();
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_Space:
        nextImage();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void Matrial::on_backToMain_clicked()
{
    this->close();
    MainWindow::instance()->show();
}
