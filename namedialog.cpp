#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent, int num) :
    QDialog(parent),
    ui(new Ui::NameDialog),
    name_(""),
    player_number_(num),
    ai_(false)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    ui->label->setText("Player " + QString::number(num) + "'s name:");
}

NameDialog::~NameDialog() {
    delete ui;
}

void NameDialog::on_buttonBox_accepted()
{
    name_ = ui->lineEdit->text();
}

void NameDialog::on_checkBox_toggled(bool checked)
{
    ai_ = checked;
}
