#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H
#include <QString>
#include "backend.h"

class CodeGenerator
{
public:
    CodeGenerator();
    static QString generatePyTorchCode(const QList<NeuralLayer*>& layers);//生成PyTorch框架下的代码
    QString generateCodeFromJson(const QString& jsonStr);
};

#endif // CODEGENERATOR_H
