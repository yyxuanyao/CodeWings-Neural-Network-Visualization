#include "codegenerator.h"
#include <QList>
#include <QJsonDocument>
#include <algorithm>

// 比较函数，用于根据层在场景中的位置排序
struct LayerSorter {
    bool operator()(const NeuralLayer* a, const NeuralLayer* b) const {
        // 获取层在场景中的位置
        QPointF posA = a->graphicsItem->scenePos();
        QPointF posB = b->graphicsItem->scenePos();

        // 按纵坐标从上到下排序（y值从小到大）
        return posA.y() < posB.y();
    }
};

// QString CodeGenerator::generatePyTorchCode(const QList<NeuralLayer*>& layers) {

//     QString code = "# PyTorch 神经网络自动生成代码\n";
//     code += "import torch\n";
//     code += "import torch.nn as nn\n\n";
//     code += "class Net(nn.Module):\n";
//     code += "    def __init__(self):\n";
//     code += "        super(Net, self).__init__()\n";

//     // 生成层定义
//     for (int i = 0; i < layers.size() ; ++i) {
//         const NeuralLayer* layer = layers[layers.size()-1-i];
//         if (layer->layerType == "Dense" || layer->layerType == "Input" ||
//             layer->layerType == "Output" || layer->layerType == "Hidden") {
//             code += QString("        self.fc%1 = nn.Linear(%2, %3)\n")
//             .arg(i + 1)
//                 .arg(layer->inputSize)
//                 .arg(layer->neurons);
//         } else if (layer->layerType == "Convolutional") {
//             code += QString("        self.conv%1 = nn.Conv2d(%2, %3, kernel_size=%4)\n")
//                         .arg(i + 1)
//                         .arg(layer->inputSize)  // 输入通道数
//                         .arg(layer->filters)    // 输出通道数
//                         .arg(layer->kernelSize);
//         } else if (layer->layerType == "MaxPooling") {
//             code += QString("        self.pool%1 = nn.MaxPool2d(kernel_size=%2)\n")
//                         .arg(i + 1)
//                         .arg(layer->poolingSize);
//         } else if (layer->layerType == "AveragePooling") {
//             code += QString("        self.pool%1 = nn.AvgPool2d(kernel_size=%2)\n")
//                         .arg(i + 1)
//                         .arg(layer->poolingSize);
//         } else if (layer->layerType == "LSTM") {
//             code += QString("        self.lstm%1 = nn.LSTM(%2, %3)\n")
//                         .arg(i + 1)
//                         .arg(layer->inputSize)
//                         .arg(layer->units);
//         } else if (layer->layerType == "RNN") {
//             code += QString("        self.rnn%1 = nn.RNN(%2, %3)\n")
//                         .arg(i + 1)
//                         .arg(layer->inputSize)
//                         .arg(layer->units);
//         }
//         else if (layer->layerType == "GRU") {
//             code += QString("        self.gru%1 = nn.GRU(%2, %3)\n")
//                         .arg(i + 1)
//                         .arg(layer->inputSize)
//                         .arg(layer->units);
//         }
//         else if (layer->layerType == "Dropout") {
//             code += QString("        self.dropout%1 = nn.Dropout(p=%2)\n")
//                         .arg(i + 1)
//                         .arg(layer->dropoutRate);
//         }
//     }
//     // 生成前向传播函数
//     code += "\n    def forward(self, x):\n";
//     for (int i = 0; i < layers.size() ; ++i) {
//         const NeuralLayer* layer = layers[layers.size()-1-i];
//         if (layer->layerType == "Dense" ||layer->layerType == "Input" ||layer->layerType == "Hidden") {
//             code += QString("        x = self.fc%1(x)\n").arg(i + 1);
//             if (!layer->activationFunction.isEmpty()) {
//                 if (layer->activationFunction == "relu" || layer->activationFunction == "ReLU") {
//                     code += "        x = nn.functional.relu(x)\n";
//                 } else if (layer->activationFunction == "sigmoid") {
//                     code += "        x = torch.sigmoid(x)\n";
//                 } else if (layer->activationFunction == "softmax") {
//                     code += "        x = nn.functional.softmax(x, dim=1)\n";
//                 } else if (layer->activationFunction == "tanh" || layer->activationFunction == "Tanh") {
//                     code += "        x = torch.tanh(x)\n";
//                 }
//             }
//         } else if (layer->layerType == "Convolutional") {
//             code += QString("        x = self.conv%1(x)\n").arg(i + 1);
//             if (!layer->activationFunction.isEmpty()) {
//                 if (layer->activationFunction == "relu") {
//                     code += "        x = nn.functional.relu(x)\n";
//                 } else if (layer->activationFunction == "sigmoid") {
//                     code += "        x = torch.sigmoid(x)\n";
//                 } else if (layer->activationFunction == "softmax") {
//                     code += "        x = nn.functional.softmax(x, dim=1)\n";
//                 } else if (layer->activationFunction == "tanh") {
//                     code += "        x = torch.tanh(x)\n";
//                 }
//             }
//         } else if (layer->layerType == "MaxPooling" || layer->layerType == "AveragePooling") {
//             code += QString("        x = self.pool%1(x)\n").arg(i + 1);
//         } else if (layer->layerType == "LSTM") {
//             code += QString("        x, _ = self.lstm%1(x)\n").arg(i + 1);
//             if (!layer->activationFunction.isEmpty()) {
//                 if (layer->activationFunction == "relu") {
//                     code += "        x = nn.functional.relu(x)\n";
//                 } else if (layer->activationFunction == "sigmoid") {
//                     code += "        x = torch.sigmoid(x)\n";
//                 } else if (layer->activationFunction == "softmax") {
//                     code += "        x = nn.functional.softmax(x, dim=1)\n";
//                 } else if (layer->activationFunction == "tanh") {
//                     code += "        x = torch.tanh(x)\n";
//                 }
//             }
//         } else if (layer->layerType == "RNN") {
//             code += QString("        x, _ = self.rnn%1(x)\n").arg(i + 1);
//             if (!layer->activationFunction.isEmpty()) {
//                 if (layer->activationFunction == "relu") {
//                     code += "        x = nn.functional.relu(x)\n";
//                 } else if (layer->activationFunction == "sigmoid") {
//                     code += "        x = torch.sigmoid(x)\n";
//                 } else if (layer->activationFunction == "softmax") {
//                     code += "        x = nn.functional.softmax(x, dim=1)\n";
//                 } else if (layer->activationFunction == "tanh") {
//                     code += "        x = torch.tanh(x)\n";
//                 }
//             }
//         } else if (layer->layerType == "Dropout") {
//             code += QString("        x = self.dropout%1(x)\n").arg(i + 1);
//         }
//     }
//     code += "        return x\n\n";
//     code += "model = Net()\n";

//     // 定义损失函数和优化器
//     code += "# 定义损失函数和优化器\n";
//     code += "criterion = nn.CrossEntropyLoss()\n";
//     code += "optimizer = optim.Adam(model.parameters(), lr=0.001)\n\n";

//     // 训练循环
//     code += "# 训练循环\n";
//     code += "num_epochs = 10\n";
//     code += "for epoch in range(num_epochs):\n";
//     code += "    running_loss = 0.0\n";
//     code += "    # 这里假设 train_loader 是你的训练数据加载器\n";
//     code += "    # 你需要根据实际情况替换为你的数据加载逻辑\n";
//     code += "    for i, (inputs, labels) in enumerate(train_loader):\n";
//     code += "        # 清零梯度\n";
//     code += "        optimizer.zero_grad()\n";
//     code += "        # 前向传播\n";
//     code += "        outputs = model(inputs)\n";
//     code += "        # 计算损失\n";
//     code += "        loss = criterion(outputs, labels)\n";
//     code += "        # 反向传播和优化\n";
//     code += "        loss.backward()\n";
//     code += "        optimizer.step()\n";
//     code += "        running_loss += loss.item()\n";
//     code += "    print(f'Epoch {epoch + 1}, Loss: {running_loss / len(train_loader)}')\n";
//     return code;
// }


QString CodeGenerator::generateCodeFromJson(const QString& jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (!doc.isObject()) {
        qDebug() << "错误: JSON 不是对象";
        return "";
    }

    QJsonObject obj = doc.object();
    if (!obj.contains("input") ||!obj.contains("output")) {
        qDebug() << "错误: JSON 对象缺少 'input' 或 'output' 字段";
        return "";
    }

    int inputSize = obj["input"].toInt();
    int outputSize = obj["output"].toInt();

    QList<NeuralLayer*> layers;

    // 处理输入层到第一个隐藏层
    NeuralLayer* inputLayer = new NeuralLayer();
    inputLayer->layerType = "Dense";
    inputLayer->inputSize = inputSize;
    if (obj.contains("hidden") && obj["hidden"].isArray()) {
        QJsonArray hiddenArray = obj["hidden"].toArray();
        if (!hiddenArray.isEmpty()) {
            inputLayer->neurons = hiddenArray[0].toInt();
        }
    }
    layers.append(inputLayer);

    // 处理隐藏层
    if (obj.contains("hidden") && obj["hidden"].isArray()) {
        QJsonArray hiddenArray = obj["hidden"].toArray();
        for (int i = 0; i < hiddenArray.size() - 1; ++i) {
            NeuralLayer* hiddenLayer = new NeuralLayer();
            hiddenLayer->layerType = "Dense";
            hiddenLayer->inputSize = hiddenArray[i].toInt();
            hiddenLayer->neurons = hiddenArray[i + 1].toInt();
            layers.append(hiddenLayer);
        }
    }

    // 处理最后一个隐藏层到输出层
    NeuralLayer* outputLayer = new NeuralLayer();
    outputLayer->layerType = "Dense";
    if (obj.contains("hidden") && obj["hidden"].isArray()) {
        QJsonArray hiddenArray = obj["hidden"].toArray();
        if (!hiddenArray.isEmpty()) {
            outputLayer->inputSize = hiddenArray.last().toInt();
        } else {
            outputLayer->inputSize = inputSize;
        }
    }
    outputLayer->neurons = outputSize;
    layers.append(outputLayer);

    // 生成代码
    QString code = generatePyTorchCode(layers);

    // 释放内存
    for (NeuralLayer* layer : layers) {
        delete layer;
    }

    return code;
}

QString CodeGenerator::generatePyTorchCode(const QList<NeuralLayer*>& layers) {
    QList<NeuralLayer*> sortedLayers = layers;
    std::sort(sortedLayers.begin(), sortedLayers.end(), LayerSorter());

    QString code = "# PyTorch 神经网络自动生成代码\n";
    code += "import torch\n";
    code += "import torch.nn as nn\n";
    code += "import torch.nn.functional as F\n\n";
    code += "class Net(nn.Module):\n";
    code += "    def __init__(self):\n";
    code += "        super(Net, self).__init__()\n";

    int layerIndex = 1;
    bool addedFlatten = false;

    for (int i = 0; i < sortedLayers.size(); ++i) {
        const NeuralLayer* layer = sortedLayers[i];

        if (layer->layerType == "Dense" || layer->layerType == "Input" ||
            layer->layerType == "Output" || layer->layerType == "Hidden") {

            int inputSize = 0;
            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];

                if ((prevLayer->layerType == "Convolutional" ||
                     prevLayer->layerType == "MaxPooling" ||
                     prevLayer->layerType == "AvgPooling") && !addedFlatten) {
                    code += "        self.flatten = nn.Flatten()\n";
                    addedFlatten = true;
                }

                if (prevLayer->layerType == "Convolutional" ||
                    prevLayer->layerType == "MaxPooling" ||
                    prevLayer->layerType == "AvgPooling") {
                    inputSize = prevLayer->filters * 16;
                } else {
                    inputSize = prevLayer->neurons;
                }
            } else {
                inputSize = layer->inputSize;
            }

            code += QString("        self.fc%1 = nn.Linear(%2, %3)\n")
                        .arg(layerIndex)
                        .arg(inputSize)
                        .arg(layer->neurons);
            layerIndex++;

        } else if (layer->layerType == "Convolutional") {
            int inChannels = 3;
            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];
                if (prevLayer->layerType == "Convolutional") {
                    inChannels = prevLayer->filters;
                }
            }

            code += QString("        self.conv%1 = nn.Conv2d(%2, %3, kernel_size=%4, padding=%5)\n")
                        .arg(layerIndex)
                        .arg(inChannels)
                        .arg(layer->filters)
                        .arg(layer->kernelSize)
                        .arg(layer->kernelSize / 2); // 假设padding为kernel_size/2
            layerIndex++;

        } else if (layer->layerType == "MaxPooling") {
            code += QString("        self.pool%1 = nn.MaxPool2d(kernel_size=%2, stride=%3)\n")
            .arg(layerIndex)
                .arg(layer->poolingSize)
                .arg(2);
            layerIndex++;

        } else if (layer->layerType == "AvgPooling") {
            code += QString("        self.pool%1 = nn.AvgPool2d(kernel_size=%2, stride=%3)\n")
            .arg(layerIndex)
                .arg(layer->poolingSize)
                .arg(2);
            layerIndex++;

        } else if (layer->layerType == "LSTM") {
            int inputSize = layer->inputSize;
            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];
                if (prevLayer->layerType == "Dense" ||
                    prevLayer->layerType == "Hidden" ||
                    prevLayer->layerType == "Output") {
                    inputSize = prevLayer->neurons;
                }
            }

            code += QString("        self.lstm%1 = nn.LSTM(%2, %3, batch_first=True)\n")
                        .arg(layerIndex)
                        .arg(inputSize)
                        .arg(layer->units);
            layerIndex++;

        } else if (layer->layerType == "RNN") {
            int inputSize = layer->inputSize;
            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];
                if (prevLayer->layerType == "Dense" ||
                    prevLayer->layerType == "Hidden" ||
                    prevLayer->layerType == "Output") {
                    inputSize = prevLayer->neurons;
                }
            }

            code += QString("        self.rnn%1 = nn.RNN(%2, %3, batch_first=True)\n")
                        .arg(layerIndex)
                        .arg(inputSize)
                        .arg(layer->units);
            layerIndex++;

        } else if (layer->layerType == "GRU") {
            int inputSize = layer->inputSize;
            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];
                if (prevLayer->layerType == "Dense" ||
                    prevLayer->layerType == "Hidden" ||
                    prevLayer->layerType == "Output") {
                    inputSize = prevLayer->neurons;
                }
            }

            code += QString("        self.gru%1 = nn.GRU(%2, %3, batch_first=True)\n")
                        .arg(layerIndex)
                        .arg(inputSize)
                        .arg(layer->units);
            layerIndex++;

        } else if (layer->layerType == "Dropout") {
            code += QString("        self.dropout%1 = nn.Dropout(p=%2)\n")
            .arg(layerIndex)
                .arg(layer->dropoutRate);
            layerIndex++;

        } else if (layer->layerType == "Flatten") {
            code += "        self.flatten = nn.Flatten()\n";
        }
    }

    code += "\n    def forward(self, x):\n";
    bool isFirstLayer = true;
    addedFlatten = false;

    for (int i = 0; i < sortedLayers.size(); ++i) {
        const NeuralLayer* layer = sortedLayers[i];

        if (layer->layerType == "Dense" || layer->layerType == "Input" ||
            layer->layerType == "Output" || layer->layerType == "Hidden") {

            if (i > 0) {
                const NeuralLayer* prevLayer = sortedLayers[i-1];
                if ((prevLayer->layerType == "Convolutional" ||
                     prevLayer->layerType == "MaxPooling" ||
                     prevLayer->layerType == "AvgPooling") && !addedFlatten) {
                    code += "        x = self.flatten(x)\n";
                    addedFlatten = true;
                }
            }

            code += QString("        x = self.fc%1(x)\n").arg(i+1);

            if (!layer->activationFunction.isEmpty()) {
                if (layer->activationFunction == "relu") {
                    code += "        x = F.relu(x)\n";
                } else if (layer->activationFunction == "sigmoid") {
                    code += "        x = torch.sigmoid(x)\n";
                } else if (layer->activationFunction == "tanh") {
                    code += "        x = torch.tanh(x)\n";
                } else if (layer->activationFunction == "softmax") {
                    code += "        x = F.softmax(x, dim=1)\n";
                } else if (layer->activationFunction == "leaky_relu") {
                    code += "        x = F.leaky_relu(x)\n";
                }
            }

        } else if (layer->layerType == "Convolutional") {
            code += QString("        x = self.conv%1(x)\n").arg(i+1);

            if (!layer->activationFunction.isEmpty()) {
                if (layer->activationFunction == "relu") {
                    code += "        x = F.relu(x)\n";
                } else if (layer->activationFunction == "sigmoid") {
                    code += "        x = torch.sigmoid(x)\n";
                } else if (layer->activationFunction == "tanh") {
                    code += "        x = torch.tanh(x)\n";
                }
            }

        } else if (layer->layerType == "MaxPooling") {
            code += QString("        x = self.pool%1(x)\n").arg(i+1);

        } else if (layer->layerType == "AvgPooling") {
            code += QString("        x = self.pool%1(x)\n").arg(i+1);

        } else if (layer->layerType == "LSTM") {
            code += QString("        x, _ = self.lstm%1(x)\n").arg(i+1);

            if (!layer->activationFunction.isEmpty()) {
                if (layer->activationFunction == "relu") {
                    code += "        x = F.relu(x)\n";
                } else if (layer->activationFunction == "tanh") {
                    code += "        x = torch.tanh(x)\n";
                }
            }

        } else if (layer->layerType == "RNN") {
            code += QString("        x, _ = self.rnn%1(x)\n").arg(i+1);

            if (!layer->activationFunction.isEmpty()) {
                if (layer->activationFunction == "relu") {
                    code += "        x = F.relu(x)\n";
                } else if (layer->activationFunction == "tanh") {
                    code += "        x = torch.tanh(x)\n";
                }
            }

        } else if (layer->layerType == "GRU") {
            code += QString("        x, _ = self.gru%1(x)\n").arg(i+1);

            if (!layer->activationFunction.isEmpty()) {
                if (layer->activationFunction == "relu") {
                    code += "        x = F.relu(x)\n";
                } else if (layer->activationFunction == "tanh") {
                    code += "        x = torch.tanh(x)\n";
                }
            }

        } else if (layer->layerType == "Dropout") {
            code += QString("        x = self.dropout%1(x)\n").arg(i+1);

        } else if (layer->layerType == "Flatten") {
            code += "        x = self.flatten(x)\n";
        }

        // 如果是第一层且是卷积层，可能需要调整输入形状
        if (isFirstLayer && layer->layerType == "Convolutional") {
            code += "        # Assuming input shape (batch_size, channels, height, width)\n";
            isFirstLayer = false;
        }
    }

    code += "        return x\n\n";

    code += "# 模型实例化\n";
    code += "model = Net()\n\n";

    code += "# 定义损失函数和优化器\n";
    code += "criterion = nn.CrossEntropyLoss()\n";
    code += "optimizer = torch.optim.Adam(model.parameters(), lr=0.001)\n\n";

    code += "# 训练循环\n";
    code += "def train(model, train_loader, num_epochs=10):\n";
    code += "    model.train()\n";
    code += "    for epoch in range(num_epochs):\n";
    code += "        running_loss = 0.0\n";
    code += "        for i, (inputs, labels) in enumerate(train_loader):\n";
    code += "            optimizer.zero_grad()\n";
    code += "            outputs = model(inputs)\n";
    code += "            loss = criterion(outputs, labels)\n";
    code += "            loss.backward()\n";
    code += "            optimizer.step()\n";
    code += "            running_loss += loss.item()\n";
    code += "            \n";
    code += "            if i % 100 == 99:  # 每100个batch打印一次\n";
    code += "                print(f'Epoch [{epoch+1}/{num_epochs}], Batch [{i+1}], Loss: {running_loss/100:.4f}')\n";
    code += "                running_loss = 0.0\n\n";

    code += "        print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {running_loss/len(train_loader):.4f}')\n\n";

    code += "# 示例用法（需要提供train_loader）\n";
    code += "# train(model, train_loader, num_epochs=10)\n";

    return code;
}
