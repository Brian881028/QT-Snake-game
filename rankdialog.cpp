#include "rankdialog.h"
#include "ui_rankdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QTextStream>
#include <QFileDialog>

RankDialog::RankDialog(QWidget *parent, QString rankFile) :
    QDialog(parent),
    ui(new Ui::RankDialog)
{
    ui->setupUi(this);
    this->rankFile = rankFile;
}

RankDialog::~RankDialog()
{
    delete ui;
}

void RankDialog::rankAndShow(){
//    rankFile = QFileDialog::getOpenFileName(this,"Open as","","*.txt");
    QFile file(rankFile);
    if(!file.exists()){
        QMessageBox::warning(this,"Oops","Cannot find the file");
        return;
    }
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(this,"Oops", "Open the file error");
        return;
    }

    QString playerName;
    int playerScore;
    QList<QString> nameList;
    QList<int> scoreList;
    QTextStream stream(&file);
    while(!stream.atEnd()){
        stream>>playerName>>playerScore;
        nameList.append(playerName);
        scoreList.append(playerScore);
    }
    file.close();
    for(int i = 0; i < scoreList.size() - 1;i++)
        for(int j = i + 1;j < scoreList.size();j++)
            if(scoreList.at(j) > scoreList.at(i)){
                int tmp = scoreList[i];
                scoreList[i] = scoreList[j];
                scoreList[j] = tmp;
                QString tmpStr = nameList[i];
                nameList[i] = nameList[j];
                nameList[j] = tmpStr;
            }
    ui->rankList->clear();
    for(int i = 0;i < nameList.size()&&i<5;i++)
        ui->rankList->addItem("Number " + QString::number(i+1)  +"\t"+ nameList.at(i) + "\t" + QString::number(scoreList.at(i)) + "score");
    show();
}
