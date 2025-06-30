#ifndef RESOURCEPAGE_H
#define RESOURCEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

class ResourcePage : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcePage(QWidget *parent = nullptr);

signals:
    void returnToMain();

private slots:
    void openPyTorch();
    void openTensorFlow();
    void openKeras();
    void openCoursera();

private:
    QPushButton *createLinkButton(const QString &text, const char *slot);
};

#endif // RESOURCEPAGE_H
