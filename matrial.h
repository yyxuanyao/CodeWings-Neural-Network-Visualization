#ifndef MATRIAL_H
#define MATRIAL_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QResizeEvent>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class Matrial : public QMainWindow
{
    Q_OBJECT

public:
    Matrial(QWidget *parent = nullptr);
    ~Matrial();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void previousImage();
    void nextImage();
    void on_backToMain_clicked();

private:
    void setupUI();
    void loadImages();
    void updateImage();
    void resizeEvent(QResizeEvent *event) override;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QLabel *imageLabel;
    QLabel *pageLabel;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *backToMainButton;

    QStringList imagePaths;
    QList<QPixmap> images;
    int currentIndex;

};

#endif // MATRIAL_H
