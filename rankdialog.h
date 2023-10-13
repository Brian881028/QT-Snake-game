#ifndef RANKDIALOG_H
#define RANKDIALOG_H

#include <QDialog>

namespace Ui {
class RankDialog;
}

class RankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RankDialog(QWidget *parent = nullptr, QString rankFile="");
    ~RankDialog();
    void rankAndShow();

private:
    Ui::RankDialog *ui;
    QString rankFile;
};

#endif // RANKDIALOG_H
