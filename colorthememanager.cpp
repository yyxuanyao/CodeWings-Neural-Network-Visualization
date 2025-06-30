#include "colorthememanager.h"
#include <qcolor.h>

QString ColorThemeManager::s_currentTheme = "Vibrant";

const QMap<QString, ColorTheme> ColorThemeManager::s_themes = {
    {"Classic",
     {QColor("#AEDCF0"), Qt::black, Qt::darkCyan, Qt::blue, Qt::black,QColor("#AEDCF0"), Qt::darkCyan,Qt::blue}},
    {"Dark",
     {QColor(45, 45, 45), QColor(100, 100, 100), QColor(0, 255, 255), QColor(100, 149, 237), Qt::blue,QColor(45, 45, 45), QColor(100, 100, 100), QColor(0, 255, 255)}},
    {"Vibrant",
     {QColor(255, 204, 204), QColor(255, 102, 102), QColor(255, 0, 127), QColor(127, 0, 255), Qt::black,QColor(255, 204, 204), QColor(255, 102, 102), QColor(255, 0, 127)}},
    {"Ocean",
     {QColor(204, 229, 255), QColor(102, 178, 255), QColor(0, 128, 255), QColor(0, 64, 128), Qt::black,QColor(204, 229, 255), QColor(102, 178, 255), QColor(0, 128, 255)}}
};

const QMap<QString, ColorTheme>& ColorThemeManager::themes() {
    return s_themes;
}

ColorTheme ColorThemeManager::currentTheme() {
    return s_themes[s_currentTheme];
}

void ColorThemeManager::setCurrentTheme(const QString& themeName) {
    if (s_themes.contains(themeName)) {
        s_currentTheme = themeName;
    }
}
QString ColorThemeManager::getCurrentTheme() {
    return s_currentTheme;
}


