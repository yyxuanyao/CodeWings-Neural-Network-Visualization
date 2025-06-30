#include "propertypanel.h"
#include <QFormLayout>
#include <QLabel>

PropertyPanel::PropertyPanel(QWidget *parent) : QWidget(parent)
{
    layout = new QFormLayout(this);
    updateBtn = new QPushButton("update parameters", this);
    layout->addWidget(updateBtn);
    layout->setSpacing(15);

    connect(updateBtn, &QPushButton::clicked, this, &PropertyPanel::onUpdateButtonClicked);
}

void PropertyPanel::setLayerType(const QString& type)
{
    currentLayerType = type;
}

void PropertyPanel::setParameters(const QMap<QString, QString>& params) {
    // 完全清除所有旧参数行（包括标签和输入框）
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        // 删除标签部件
        if (QLabel* label = qobject_cast<QLabel*>(child->widget())) {
            delete label;
        }
        // 删除输入框部件
        else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(child->widget())) {
            delete lineEdit;
        }
        // 删除布局项本身
        delete child;
    }

    // 重新添加更新按钮
    layout->addWidget(updateBtn);

    // 清空字段映射
    fieldMap.clear();

    // 创建新的参数行（标签 + 输入框）
    for (auto it = params.begin(); it != params.end(); ++it) {
        // 创建标签
        QLabel* label = new QLabel(it.key(), this);

        // 创建输入框
        QLineEdit* lineEdit = new QLineEdit(it.value(), this);

        // 添加到布局
        layout->insertRow(layout->rowCount() - 1, label, lineEdit);

        // 存储输入框引用
        fieldMap[it.key()] = lineEdit;
    }
}

void PropertyPanel::onUpdateButtonClicked()
{
    QMap<QString, QString> newParams;
    for (auto it = fieldMap.begin(); it != fieldMap.end(); ++it) {
        newParams[it.key()] = it.value()->text();
    }
    emit parametersUpdated(newParams);
}

void PropertyPanel::clearParameters() {
    // 删除所有行（除了最后的更新按钮）
    while (layout->rowCount() > 1) {
        layout->removeRow(0);  // 删除第一行（包括标签和输入框）
    }
    fieldMap.clear();
}
