#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QQueue>
#include "rankdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);


private slots:
    void on_btnBegin_clicked();

//    void on_btnPause_toggled(bool checked);

    void on_btnReInit_clicked();

    void on_btnRank_clicked();

    void setHardLevel();

    void timeout();


    void on_btnPause_clicked();

private:
    Ui::Widget *ui;
    void generateFood();
    void initGame();
    void gameOver();
    enum Role{
        NOTHING,SNAKEBODY,SNAKEHEAD,WALL,FOOD
    };
    enum Direction{
        UP,DOWN,LEFT,RIGHT
    };
    Role **board;
    QQueue<QPoint>snake;
    int cellNumX, cellNumY;
    int cellLengthX, cellLengthY;
    Direction direct, newDirect;
    double hardLevel;
    QTimer *timer;
    int foodNum = 0;
    QString rankFile;
    RankDialog *rankDlg;
    bool active = true;
};
#endif // WIDGET_H
