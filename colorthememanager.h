#ifndef COLORTHEMEMANAGER_H
#define COLORTHEMEMANAGER_H

#include <QColor>
#include <QMap>

struct ColorTheme {
    QColor neuronFill;
    QColor neuronBorder;
    QColor connectionHighWeight;
    QColor connectionLowWeight;
    QColor text;

    // 层组相关
    QColor layerBackground;     // 层背景色
    QColor weightBoxFill;       // 权重框颜色
    QColor activationBoxFill;   // 激活函数框颜色
};

class ColorThemeManager {
public:
    static const QMap<QString, ColorTheme>& themes();
    static ColorTheme currentTheme();
    static QString getCurrentTheme();
    static void setCurrentTheme(const QString& themeName);

private:
    static QString s_currentTheme;
    static const QMap<QString, ColorTheme> s_themes;
};
#endif // COLORTHEMEMANAGER_H

