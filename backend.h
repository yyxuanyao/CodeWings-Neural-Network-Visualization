#ifndef BACKEND_H
#define BACKEND_H

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QGraphicsItem>

class NeuralLayer
{
public:

    QString layerType;//层定义
    int neurons = 1000;//神经元数量
    int inputSize = 128;//输入维度
    QString activationFunction = "relu";//激活函数
    //指向图形项的指针
    QGraphicsItem* graphicsItem = nullptr;

    NeuralLayer();
    QJsonObject toJsonObject()const;//将NeuralLayer对象转化为QJsonObject，以后可拼接为QJsonArray
    static NeuralLayer fromJsonObject(const QJsonObject& obj);// 从QJsonObject构造NeuralLayer的静态函数声明
    //obj 应该包含"layerType" "neurons" "activationFunction"

    // 卷积层特有参数
    int filters = 32;
    int kernelSize = 5;

    // 池化层特有参数
    int poolingSize = 5;

    // LSTM 或 RNN 特有参数
    int units = 64;

    // Dropout 层特有参数
    float dropoutRate = 0.5f;
};

#endif // BACKEND_H
