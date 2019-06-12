#ifndef GAMERESULTS_H
#define GAMERESULTS_H

#include <QDialog>

namespace Ui {
class GameResults;
}

/* GameResults is a subclass of QDialog to display the game results. */
class GameResults : public QDialog
{
    Q_OBJECT

public:
    explicit GameResults(QWidget *parent = nullptr);
    ~GameResults();

    // Public Methods
    void InputResults(QString, QString);

signals:
    void ExitButtonClicked();
    void PlayAgainButtonClicked();

private slots:
    void on_exitButton_clicked();


    void on_playAgainButton_clicked();

private:
    Ui::GameResults *ui;
};

#endif // GAMERESULTS_H
