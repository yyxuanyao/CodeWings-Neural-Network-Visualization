#include "programfragmentprocessor.h"


ProgramFragmentProcessor::ProgramFragmentProcessor() {}

QJsonObject ProgramFragmentProcessor::processFragment(const QJsonObject& fragmentObj) {
    QString language = fragmentObj["language"].toString();
    QString code = fragmentObj["code"].toString();
    QString action = fragmentObj["action"].toString();

    QJsonObject result;
    result["status"] = "success";
    result["timestamp"] = QDateTime::currentDateTime().toString();

    if (action == "validate") {
        result["validationResult"] = validateCode(code);
    }
    else if (action == "extract-structure") {
        if (language == "python" && code.contains("torch.nn")) {
            result["networkStructure"] = extractPyTorchStructure(code);
        } else {
            result["error"] = "仅支持解析 PyTorch 代码";
        }
    }
    else {
        result["error"] = "不支持的操作: " + action;
    }

    return result;
}


// 从 PyTorch 代码中提取网络结构
QJsonArray ProgramFragmentProcessor::extractPyTorchStructure(const QString& code) {
    QJsonArray layersArray;

    // 提取 nn.Linear 层
    QRegularExpression linearPattern("nn\\.Linear\\s*\\(\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\)");
    QRegularExpressionMatchIterator linearMatches = linearPattern.globalMatch(code);

    while (linearMatches.hasNext()) {
        QRegularExpressionMatch match = linearMatches.next();
        int inputSize = match.captured(1).toInt();
        int outputSize = match.captured(2).toInt();

        QJsonObject layerObj;
        layerObj["layerType"] = "Dense";
        layerObj["inputSize"] = inputSize;
        layerObj["neurons"] = outputSize;

        // 提取激活函数
        QString activation = extractActivationFunction(code, match.capturedStart());
        layerObj["activationFunction"] = activation;

        layersArray.append(layerObj);
    }

    // 提取 Conv2d 层
    QRegularExpression convPattern("nn\\.Conv2d\\s*\\(\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*kernel_size\\s*=\\s*(\\d+)");
    QRegularExpressionMatchIterator convMatches = convPattern.globalMatch(code);

    while (convMatches.hasNext()) {
        QRegularExpressionMatch match = convMatches.next();
        int inChannels = match.captured(1).toInt();
        int outChannels = match.captured(2).toInt();
        int kernelSize = match.captured(3).toInt();

        QJsonObject layerObj;
        layerObj["layerType"] = "Conv2d";
        layerObj["inputSize"] = inChannels;  // 简化处理
        layerObj["neurons"] = outChannels;  // 简化处理
        layerObj["kernelSize"] = kernelSize;

        // 提取激活函数
        QString activation = extractActivationFunction(code, match.capturedStart());
        layerObj["activationFunction"] = activation;

        layersArray.append(layerObj);
    }

    return layersArray;
}

// 从代码中提取激活函数
QString ProgramFragmentProcessor::extractActivationFunction(const QString& code, int startPos) {
    QString snippet = code.mid(startPos, 200);

    if (snippet.contains("nn.ReLU") || snippet.contains("F.relu")) return "relu";
    if (snippet.contains("nn.Sigmoid") || snippet.contains("F.sigmoid")) return "sigmoid";
    if (snippet.contains("nn.Softmax") || snippet.contains("F.softmax")) return "softmax";
    if (snippet.contains("nn.Tanh") || snippet.contains("F.tanh")) return "tanh";

    return "";
}

// 简单的代码验证
QJsonObject ProgramFragmentProcessor::validateCode(const QString& code) {
    QJsonObject validationResult;
    validationResult["valid"] = true;
    QJsonArray errors;

    // 基本语法检查
    if (code.trimmed().isEmpty()) {
        errors.append("代码不能为空");
        validationResult["valid"] = false;
        validationResult["errors"] = errors;
        return validationResult;
    }

    // 检查Python缩进错误 (简单检查：每行缩进必须是4的倍数)
    QStringList lines = code.split('\n');
    QRegularExpression indentRegex("^(\\s*)");

    for (int i = 0; i < lines.size(); i++) {
        QString line = lines[i].trimmed();
        if (line.isEmpty() || line.startsWith("#")) continue; // 跳过空行和注释

        // 获取当前行的缩进空格数
        QRegularExpressionMatch match = indentRegex.match(lines[i]);
        int indent = match.captured(1).length();

        // 检查缩进是否是4的倍数
        if (indent % 4 != 0) {
            errors.append(QString("第 %1 行：缩进必须是4个空格的倍数").arg(i + 1));
            validationResult["valid"] = false;
        }

        // 简单的缩进层次检查
        if (line.endsWith(":")) {
            // 冒号行后应该增加缩进
            if (i + 1 < lines.size()) {
                QRegularExpressionMatch nextMatch = indentRegex.match(lines[i + 1]);
                int nextIndent = nextMatch.captured(1).length();
                if (nextIndent <= indent) {
                    errors.append(QString("第 %1 行：冒号后需要增加缩进").arg(i + 1));
                    validationResult["valid"] = false;
                }
            }
        }
    }

    // 检查常见的PyTorch相关错误
    if (code.contains("nn.") || code.contains("torch.")) {
        // 检查块引用错误
        if (!code.contains("import torch") && !code.contains("import torch.nn")) {
            errors.append("使用PyTorch模块但未导入torch或torch.nn");
            validationResult["valid"] = false;
        }

        // 检查常见的层定义错误
        QRegularExpression layerRegex("self\\.(conv|fc|pool|lstm|gru|rnn)\\d+\\s*=\\s*nn\\.");
        QRegularExpressionMatchIterator it = layerRegex.globalMatch(code);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString layerDef = match.captured(0);

            // 检查是否缺少括号
            if (!layerDef.contains("(") || !layerDef.contains(")")) {
                errors.append("层定义语法错误：缺少括号");
                validationResult["valid"] = false;
            }
        }
    }

    validationResult["errors"] = errors;
    return validationResult;
}
