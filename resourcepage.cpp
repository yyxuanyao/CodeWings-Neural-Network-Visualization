#include "resourcepage.h"

ResourcePage::ResourcePage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("深度学习开发资源", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 30px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    layout->addWidget(createLinkButton("PyTorch 官方网站", SLOT(openPyTorch())));
    layout->addWidget(createLinkButton("TensorFlow 官方网站", SLOT(openTensorFlow())));
    layout->addWidget(createLinkButton("Keras 官方网站", SLOT(openKeras())));
    layout->addWidget(createLinkButton("Coursera 深度学习课程", SLOT(openCoursera())));

    QPushButton *returnButton = new QPushButton("返回主界面", this);
    returnButton->setFixedSize(200, 50);
    returnButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border-radius: 10px;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );
    connect(returnButton, &QPushButton::clicked, this, &ResourcePage::returnToMain);
    layout->addWidget(returnButton, 0, Qt::AlignCenter);

    setLayout(layout);
    setStyleSheet("background-color: #f0f8ff;"); // 浅蓝色背景
}

QPushButton *ResourcePage::createLinkButton(const QString &text, const char *slot)
{
    QPushButton *button = new QPushButton(text, this);
    button->setFixedSize(300, 50);
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #e6f7ff;"
        "   border: 2px solid #91d5ff;"
        "   border-radius: 10px;"
        "   color: #1890ff;"
        "   font-size: 16px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #bae7ff;"
        "}"
        );
    connect(button, SIGNAL(clicked()), this, slot);
    return button;
}

void ResourcePage::openPyTorch()
{
    QDesktopServices::openUrl(QUrl("https://pytorch.org/"));
}

void ResourcePage::openTensorFlow()
{
    QDesktopServices::openUrl(QUrl("https://www.tensorflow.org/"));
}

void ResourcePage::openKeras()
{
    QDesktopServices::openUrl(QUrl("https://keras.io/"));
}

void ResourcePage::openCoursera()
{
    QDesktopServices::openUrl(QUrl("https://www.coursera.org/specializations/deep-learning"));
}
