#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::transparent));
    ui->label->setMaximumSize(80,30);
    ui->lcdNumber->setMaximumSize(80,30);
    cellNumX = cellNumY = 20;
    cellLengthX = cellLengthY = 20;
    rankFile = "D:/03818 Powei/Documents/build-Snake-Desktop_Qt_6_7_0_MSVC2019_64bit-Debug/score.txt.txt";
    rankDlg = new RankDialog(this, rankFile);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeout()));
    connect(ui->radioEasy, SIGNAL(clicked()), this,SLOT(setHardLevel()));
    connect(ui->radioMid, SIGNAL(clicked()), this,SLOT(setHardLevel()));
    connect(ui->radioHard, SIGNAL(clicked()), this,SLOT(setHardLevel()));
    resize(100 + 10 + cellNumX * cellLengthX, 20 + cellNumY * cellLengthY);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    board = new Role * [cellNumX];
    for(int i = 0; i < cellNumX; i++)
        board[i] = new Role[cellNumY];
    initGame();
}

Widget::~Widget()
{
    delete ui;
    for(int i = 0;i < cellNumX;i++)
        delete board[i];
    delete board;
}

void Widget::initGame(){
    foodNum = 0;
    ui->lcdNumber->display(foodNum);
    direct = newDirect = RIGHT;
    ui->radioEasy->setChecked(true);
    ui->btnBegin->show();
    ui->btnPause->hide();
    ui->btnReInit->hide();
    for(int i = 1; i < cellNumX-1;i++)
        for(int j = 1;j<cellNumY-1;j++)
            board[i][j] = NOTHING;
    for(int i = 0;i < cellNumX;i++){
        board[i][0] = WALL;
        board[i][cellNumY-1] = WALL;
        board[0][i] = WALL;
        board[cellNumX-1][i] = WALL;
    }
    snake.clear();
    snake.enqueue(QPoint(1,cellNumY/2));
    snake.enqueue(QPoint(2,cellNumY/2));
    snake.enqueue(QPoint(3,cellNumY/2));
    board[1][cellNumY/2] = board[2][cellNumY/2] = SNAKEBODY;
    board[3][cellNumY/2] = SNAKEHEAD;
    generateFood();
    update();
}

void Widget::generateFood(){
    QRandomGenerator randomGenerator(QTime::currentTime().msec());

    do{
        int x = 1 + randomGenerator.bounded(cellNumX);
        int y = 1 + randomGenerator.bounded(cellNumY);
        if(board[x][y] == NOTHING){
            board[x][y] = FOOD;
            break;
        }
    }while (true);
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QBrush brush(Qt::SolidPattern);
    for(int i = 0; i < cellNumX;i++)
        for(int j = 0; j < cellNumY; j++){
            switch(board[i][j]){
            case NOTHING:
                brush.setColor(Qt::black);
                break;
            case WALL:
                brush.setColor(Qt::white);
                break;
            case SNAKEBODY:
                brush.setColor(Qt::blue);
                break;
            case SNAKEHEAD:
                brush.setColor(Qt::red);
                break;
            case FOOD:
                brush.setColor(Qt::yellow);
                break;
            }
            painter.setBrush(brush);
            painter.drawRect(100 + i * cellLengthX,10 + j * cellLengthY, cellLengthX, cellLengthY);
        }
    ui->lcdNumber->display(foodNum);
}

void Widget::setHardLevel(){
    if(ui->radioEasy->isChecked())
        hardLevel = 4;
    else if(ui->radioMid->isChecked())
        hardLevel = 6;
    else
        hardLevel = 10;
    timer->setInterval(1000/hardLevel);
}

void Widget::on_btnBegin_clicked()
{
    setHardLevel();
    timer->start(1000/hardLevel);
    ui->btnBegin->hide();
    ui->btnPause->show();
    ui->btnReInit->show();

}


//void Widget::on_btnPause_toggled(bool checked)
//{

//    if(checked){
//        timer->blockSignals(true);
//        ui->btnPause->setText("繼續");
//    }else{
//        timer->blockSignals(false);
//        ui->btnPause->setText("暫停");
//    }
//}


void Widget::on_btnReInit_clicked()
{
    timer->stop();
    initGame();
}


void Widget::on_btnRank_clicked()
{
    if(rankDlg == nullptr)
        rankDlg = new RankDialog(this,rankFile);
    rankDlg->rankAndShow();
}

void Widget::timeout(){
    QPoint delPoint;
    QPoint snakeHead = snake.last();
    int newCellX = snakeHead.x();
    int newCellY = snakeHead.y();
    switch(direct){
        case UP:
            if(newDirect!=DOWN)
                direct = newDirect;
            break;
        case DOWN:
            if(newDirect!=UP)
                direct = newDirect;
            break;
        case LEFT:
            if(newDirect!=RIGHT)
                direct = newDirect;
            break;
        case RIGHT:
            if(newDirect!=LEFT)
                direct = newDirect;
            break;
    }
    switch(direct){
        case UP:
            newCellY -=1;
            break;
        case DOWN:
            newCellY +=1;
            break;
        case LEFT:
            newCellX -=1;
            break;
        case RIGHT:
            newCellX +=1;
            break;
    }
    switch(board[newCellX][newCellY]){
        case WALL:
        case SNAKEHEAD:
        case SNAKEBODY:
            gameOver();
            break;
        case NOTHING:
            snake.enqueue(QPoint(newCellX,newCellY));
            delPoint = snake.dequeue();
            board[snakeHead.x()][snakeHead.y()] = SNAKEBODY;
            board[newCellX][newCellY] = SNAKEHEAD;
            board[delPoint.x()][delPoint.y()] = NOTHING;
            break;

        case FOOD:
            snake.enqueue(QPoint(newCellX, newCellY));
            board[snakeHead.x()][snakeHead.y()] = SNAKEBODY;
            board[newCellX][newCellY] = SNAKEHEAD;
            foodNum++;
            generateFood();
            break;
    }
    update();
}

void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_S:
            newDirect = DOWN;
            break;
        case Qt::Key_W:
            newDirect = UP;
            break;
        case Qt::Key_A:
            newDirect = LEFT;
            break;
        case Qt::Key_D:
            newDirect = RIGHT;
            break;
        }
}

void Widget::gameOver(){
    timer->stop();
    QString playerName = QInputDialog::getText(this,"Game Over！","Input your name");
    if(!playerName.isEmpty()){
        qDebug()<<rankFile;
        QFile file(rankFile);
        if(!file.exists()){
            file.open(QIODevice::ReadWrite|QIODevice::Text);
            QTextStream stream(&file);
            stream<<" "<< playerName<<" "<<foodNum;
            file.close();
        }else if(!file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
            QMessageBox::information(this,"Oops","Open fail");
        else{
            QTextStream stream(&file);
            stream<<" "<<playerName<<" "<<foodNum;
            file.close();
        }
    }
    initGame();
}


void Widget::on_btnPause_clicked()
{
//    qDebug()<<active;
    if(active){
        timer->blockSignals(true);
        active = false;
        ui->btnPause->setText("Start");
    }else{
        timer->blockSignals(false);
        active = true;
        ui->btnPause->setText("Stop");
    }
}

