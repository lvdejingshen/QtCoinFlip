#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyPushButton(QWidget *parent = nullptr);

    //构造函数 参数1 正常显示的图片路径 参数2 按下后显示的图片路径
    MyPushButton(QString normalImg, QString pressImg = "");

    void zoom1();
    void zoom2();

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    //成员属性 保存用户传入的默认显示路径 以及 按下后图片的显示路径
    QString normalImgPath;
    QString pressImgPath;


signals:
};

#endif // MYPUSHBUTTON_H
