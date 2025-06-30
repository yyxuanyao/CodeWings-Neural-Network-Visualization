#ifndef PROGRAMFRAGMENTPROCESSOR_H
#define PROGRAMFRAGMENTPROCESSOR_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>


// 程序片段处理器 - 专注于 PyTorch 解析
class ProgramFragmentProcessor {
public:

    ProgramFragmentProcessor();

    // 从 PyTorch 代码中提取网络结构
    static QJsonObject processFragment(const QJsonObject& fragmentObj);

    // 简单的代码验证
    static QJsonObject validateCode(const QString& code) ;

    // 从代码中提取激活函数
    static QString extractActivationFunction(const QString& code, int startPos);

    // 从 PyTorch 代码中提取网络结构
    static QJsonArray extractPyTorchStructure(const QString& code) ;
};




#endif // PROGRAMFRAGMENTPROCESSOR_H
