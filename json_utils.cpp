#include "json_utils.h"
#include <QJsonDocument>
#include <QDebug>

QJsonArray parseNetworkStructure(const QString& jsonStr) {//将JSON字符串解析为QJsonArray对象
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray layersArray;
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        if(!jsonStr.contains("layers")){
            qDebug()<<"Error from parseNetworkStructure:JSON object does not contain 'layers' field";
        }
        layersArray = obj["layers"].toArray();
    }
    return layersArray;
}
QString generateNetworkStructureJson(const QJsonArray& layersArray) {//将QJsonArray对象转化为JSON字符串
    QJsonObject rootObj;
    rootObj["layers"] = layersArray;
    QJsonDocument doc(rootObj);
    return QString::fromUtf8(doc.toJson());
}
