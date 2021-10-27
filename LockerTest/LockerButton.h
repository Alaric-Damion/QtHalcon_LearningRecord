

#ifndef LOCKER_BUTTON_H
#define LOCKER_BUTTON_H

#include <QWidget>
#include <QPushButton>

class QLabel;

class LockerButton : public QPushButton
{
    Q_OBJECT
public:
    explicit LockerButton(QWidget* parent = nullptr);

    // 设置按钮图标
    void SetImageLabel(const QPixmap &pixmap);

    // 设置按钮文字
    void SetTextLabel(QString text);

    // 返回图像label句柄
    QLabel* GetImageHandle();

    // 返回文字label句柄
    QLabel* GetTextHandle();

private:
    // 按钮图标
    QLabel* m_imageLabel;
    // 按钮文字
    QLabel* m_textLabel;
};

#endif // LOCKER_BUTTON_H
