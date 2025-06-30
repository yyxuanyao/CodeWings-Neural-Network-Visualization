#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QJsonArray>
#include <QGraphicsScene>
#include "codegeneratorwindow.h"
#include "networkvisualizer.h"
#include "matrial.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void handleJsonData(const QString &jsonStr);
    //void generateJson();//使用示例
    /*
    static MainWindow* instance() {
        static MainWindow* instance = new MainWindow();
        return instance;
    }*/
    static MainWindow* instance();
    static void setInstance(MainWindow* window);//
    QString currentMode = "unselected";
    void showFloatingMessage(const QString& text);
    void showSaveProgressBarMessage();
    void showWarningMessage(const QString& text);
    void onHistoryRecordClicked(int index);
    bool currentNetworkSaved;
    void visualizeNetwork(const QJsonArray& layers);
    void clearPreviewArea();
    void applyTheme(const QString& theme);
    bool original;
    QVector<QJsonArray> historyCache;
    QVector<bool> historySaved;
    QVector<QString> historyLabel;
    bool imageGenerate;
    int position;

private:
    Ui::MainWindow *ui;
    static MainWindow* s_instance;
    void setupIconButton(QPushButton* button, const QString& iconPath, int size = 40);
    void setBackground(const QString& background);
    CodeGeneratorWindow* codegeneratorwindow;
    Matrial* matrialwindow;
    QGraphicsScene* scene;
    QJsonArray m_cachedNetworkJson;
    CodeGeneratorWindow* codeWin = nullptr;
    NetworkVisualizer* visualizer = nullptr;

private slots:
    void on_userGuide_clicked();
    void on_generateCode_clicked();
    void on_generateImage_clicked();
    void on_checkHistory_clicked();
    void on_startNew_clicked();
    void on_lastStep_clicked();
    void on_nextStep_clicked();
    void on_saveCurrent_clicked();

};
#endif // MAINWINDOW_H
