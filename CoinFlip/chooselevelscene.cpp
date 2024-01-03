#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include <mypushbutton.h>
#include <QDebug>
#include <QTimer>
#include <QLabel>


ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{

    //设置固定大小
    setFixedSize(320, 588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar * bar = this->menuBar();
    this->setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });


    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //点击返回
    connect(backBtn, &MyPushButton::clicked, [=](){

        QTimer::singleShot(500, this, [=](){
            emit this->chooseSceneBack();
        });

    });


    //创建选择关卡的按钮
    for(int i = 0; i < 20; i++) {

        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + (i % 4) * 70 , 130 + (i / 4) * 70);

        //监听选择关卡按钮的信号槽
        connect(menuBtn, &MyPushButton::clicked, [=](){

            if(play == nullptr) {  //游戏场景最好不用复用，直接移除掉创建新的场景

                //进入游戏场景
                //将选关场景隐藏
                this->hide();
                //创建游戏场景
                play = new PlayScene(i + 1);
                //显示游戏场景
                play->show();

                //设置游戏场景的初始位置
                play->setGeometry(this->geometry());

                connect(play, &PlayScene::chooseSceneBack, [=](){
                    this->setGeometry(play->geometry());
                    this->show();
                    delete play;
                    play = nullptr;
                });


            }
        });

        //按钮上显示的文字
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i + 1));
        label->move(25 + (i % 4) * 70 , 130 + (i / 4) * 70);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //设置居中
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);  //鼠标事件穿透
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}

