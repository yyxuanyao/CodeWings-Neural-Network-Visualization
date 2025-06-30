#include "backend.h"
#include "codegenerator.h"
#include "programfragmentprocessor.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

QJsonObject NeuralLayer::toJsonObject() const {//将NeuralLayer对象转化为QJsonObject对象
    QJsonObject obj;
    obj["layerType"] = layerType;
    obj["neurons"] = neurons;
    obj["activationFunction"] = activationFunction;
    obj["dropoutRate"] =  dropoutRate;
    obj["poolingSize"] =  poolingSize;
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



// // 接收网络结构数据接口
// void receiveNetworkStructure(const QString& jsonStr) {
//     QJsonArray layersArray = parseNetworkStructure(jsonStr);
//     if (layersArray.isEmpty()) {
//         qDebug() << "错误:解析的层数组为空";
//         return;
//     }

//     QList<NeuralLayer*> layers;
//     for (const QJsonValue &layerValue : layersArray) {
//         NeuralLayer layer = NeuralLayer::fromJsonObject(layerValue.toObject());
//         NeuralLayer* layerPtr = new NeuralLayer(layer);

//         layers.append(layerPtr);
//         qDebug() << "解析层:" << layerPtr->layerType << layerPtr->neurons;
//     }

//     // 生成 PyTorch 代码并返回
//     QString generatedCode = CodeGenerator::generatePyTorchCode(layers);

//     QJsonObject codeObj;
//     codeObj["generatedCode"] = generatedCode;
//     codeObj["language"] = "python";
//     codeObj["framework"] = "pytorch";

//     QJsonDocument doc(codeObj);
//     returnGeneratedProgram(doc.toJson(QJsonDocument::Compact));
// }

// // 返回生成程序接口
// void returnGeneratedProgram(const QString& programJsonStr) {
//     // 实际项目中发送给前端
//     qDebug() << "返回生成的程序:" << programJsonStr;
//     //emit sendDataToFrontend(programJsonStr); // 假设这是一个信号
// }

// // 接收程序片段接口
// void receiveProgramFragment(const QString& jsonStr) {
//     QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
//     if (!doc.isObject()) {
//         qDebug() << "错误: JSON 不是对象";
//         return;
//     }

//     QJsonObject fragmentObj = doc.object();
//     QJsonObject result = ProgramFragmentProcessor::processFragment(fragmentObj);

//     QJsonDocument resultDoc(result);
//     QString resultJsonStr = resultDoc.toJson(QJsonDocument::Compact);

//     qDebug() << "处理片段结果:" << resultJsonStr;
//     //emit sendDataToFrontend(resultJsonStr); // 假设这是一个信号
// }

// // 返回网络结构接口
// void returnNetworkStructure(const QString& networkStructureJsonStr) {
//     // 实际项目中应通过网络发送给前端
//     qDebug() << "返回网络结构:" << networkStructureJsonStr;
//     //emit sendDataToFrontend(networkStructureJsonStr); // 假设这是一个信号
// }
