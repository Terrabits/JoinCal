#include "GetCalDialog.h"
#include "ui_GetCalDialog.h"

GetCalDialog::GetCalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetCalDialog)
{
    ui->setupUi(this);
}

GetCalDialog::~GetCalDialog()
{
    delete ui;
}
