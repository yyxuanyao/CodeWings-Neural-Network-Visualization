#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <QJsonObject>
#include <QJsonArray>


QJsonArray parseNetworkStructure(const QString& jsonStr);// JSON解析接口声明，根据JSON数据类型解析为QJsonArray对象
//jsonStr 应该包含"layers"

QString generateNetworkStructureJson(const QJsonArray& layersArray);// JSON生成接口声明，将网络结构相关数据转换为JSON格式

// 其他与JSON数据结构相关的声明
// 比如表示完整网络结构的类等

#endif // JSON_UTILS_H
