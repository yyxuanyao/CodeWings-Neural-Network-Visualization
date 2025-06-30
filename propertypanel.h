#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>

class PropertyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyPanel(QWidget *parent = nullptr);

    void setLayerType(const QString& type);
    void setParameters(const QMap<QString, QString>& params); // 设置参数字段和当前值
    void clearParameters();

signals:
    void parametersUpdated(const QMap<QString, QString>& newParams); // 参数修改后发出信号

private slots:
    void onUpdateButtonClicked();

private:
    QFormLayout* layout;
    QPushButton* updateBtn;
    QMap<QString, QLineEdit*> fieldMap;
    QString currentLayerType;
};


#endif // PROPERTYPANEL_H
