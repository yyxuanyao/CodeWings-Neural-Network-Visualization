#include "backend.h"
#include "json_utils.h"
#include "codegenerator.h"
#include "programfragmentprocessor.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

QJsonObject NeuralLayer::toJsonObject() const {//将NeuralLayer对象转化为QJsonObject对象
    QJsonObject obj;
    obj["layerType"] = layerType;
    obj["neurons"] = neurons;
    obj["dropoutRate"] =  dropoutRate;
    obj["poolingSize"] =  poolingSize;
    obj["activationFunction"] = activationFunction;
    return obj;
}

NeuralLayer::NeuralLayer() {
    layerType = "";
    neurons = 0;
    dropoutRate=0.5f;
    poolingSize=4;
    activationFunction = "";
    filters = 32;
    kernelSize = 5;
    units = 128;
}

NeuralLayer NeuralLayer::fromJsonObject(const QJsonObject& obj) {//将QJsonObject对象转化为NeuralLayer对象
    NeuralLayer layer;
    if (!obj.contains("layerType") ||!obj.contains("neurons") ||!obj.contains("activationFunction")) {
        qDebug() << "Error form fromJsonObject: Missing required fields in JSON object when converting to NeuralLayer.";
        return layer;
    }
    layer.layerType = obj["layerType"].toString();
    layer.neurons = obj["neurons"].toInt();
    layer.activationFunction = obj["activationFunction"].toString();
    return layer;
}

