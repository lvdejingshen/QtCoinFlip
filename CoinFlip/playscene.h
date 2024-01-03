#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <mycoin.h>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    // explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int levelNum);

    void paintEvent(QPaintEvent *);

    //内部成员属性 记录所选关卡
    int levelIndex;

    int gameArray[4][4];

    MyCoin *coinBtn[4][4]; //金币按钮数组

    bool isWin; //是否胜利

signals:

    void chooseSceneBack();

};

#endif // PLAYSCENE_H
