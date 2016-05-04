#ifndef GETCALDIALOG_H
#define GETCALDIALOG_H

#include <QDialog>

namespace Ui {
class GetCalDialog;
}

class GetCalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetCalDialog(QWidget *parent = 0);
    ~GetCalDialog();

private:
    Ui::GetCalDialog *ui;
};

#endif // GETCALDIALOG_H
