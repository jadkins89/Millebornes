#include "gameresults.h"
#include "ui_gameresults.h"

GameResults::GameResults(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameResults)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}

GameResults::~GameResults()
{
    delete ui;
}

void GameResults::on_exitButton_clicked()
{
    ExitButtonClicked();
}

void GameResults::InputResults(QString name, QString results) {
    ui->congrats_label->setText("Congratulations! " + name + " has won.");
    ui->resultsField->setHtml(results);
}

void GameResults::on_playAgainButton_clicked()
{
    PlayAgainButtonClicked();
    close();
}
