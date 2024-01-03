#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <mycoin.h>
#include <dataconfig.h>
#include <QPropertyAnimation>

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}


PlayScene::PlayScene(int levelNum) {

    this->levelIndex = levelNum;

    //设置窗口固定大小
    this->setFixedSize(320, 588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("翻金币");

    //创建菜单栏
    QMenuBar * bar = this->menuBar();
    this->setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });


    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //返回按钮功能实现
    connect(backBtn, &MyPushButton::clicked, [=](){
        QTimer::singleShot(500, this, [=](){
            this->hide();
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
        }
                           );
    });

    //当前关卡标题
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString("Leavel: %1").arg(this->levelIndex);
    label->setText(str);
    label->setGeometry(QRect(30, this->height() - 50, 120, 50)); //设置大小和位置


    //初始化二维数组
    dataConfig config;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());

    //创建金币的背景图片
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            //绘制背景图片
            QLabel *label = new QLabel;
            label->setGeometry(0, 0, 50, 50);
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->setParent(this);
            label->move(57 + i * 50, 200 + j * 50);

            //创建金币
            QString str;
            if (this->gameArray[i][j] == 1) {
                //显示金币
                str = ":/res/Coin0001.png";
            } else {
                str = ":/res/Coin0008.png";
            }

            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i * 50, 204 + j * 50);
            coin->posX = i; //记录x坐标
            coin->posY = j; //记录y坐标
            coin->flag = this->gameArray[i][j]; //记录正反标志

            coinBtn[i][j] = coin;

            connect(coin, &MyCoin::clicked, [=](){

                //点击进行翻转瞬间，禁止所有翻转
                for(int i = 0 ; i < 4;i++) {
                    for(int j = 0 ; j < 4; j++) {
                        coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0; //数组内部记录的标志同步修改

                //翻转周围硬币
                QTimer::singleShot(300, this, [=](){
                    //右
                    if(coin->posX+1 <=3) {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    //左
                    if(coin->posX-1>=0) {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                    }
                    //上
                    if(coin->posY+1<=3) {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    //下
                    if(coin->posY-1>=0) {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }

                    //还原
                    for(int i = 0 ; i < 4;i++) {
                        for(int j = 0 ; j < 4; j++) {
                            coinBtn[i][j]->isWin = false;
                        }
                    }

                    this->isWin = true;
                    for(int i = 0 ; i < 4; i++) {
                        for(int j = 0; j < 4; j++) {
                            //qDebug() << coinBtn[i][j]->flag ;
                            if(coinBtn[i][j]->flag == false) {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin) {
                        qDebug() << "胜利";

                        //将所有按钮的胜利标志改为true
                        for(int i = 0 ; i < 4; i++) {
                            for(int j = 0; j < 4; j++) {
                                coinBtn[i][j]->isWin = true;
                            }
                        }

                        //将胜利的图片向下移动
                        QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel, "geometry");
                        //设置时间间隔
                        animation1->setDuration(1000);
                        //设置开始时间
                        animation1->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        //设置结束位置
                        animation1->setEndValue(QRect(winLabel->x(), winLabel->y() + 114, winLabel->width(), winLabel->height()));
                        //设置缓和曲线
                        animation1->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation1->start();
                    }
                });
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *) {

    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap( 10, 30, pix.width(), pix.height(), pix);

}
