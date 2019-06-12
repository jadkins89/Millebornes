#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "deck.h"
#include "cardimgfactory.h"
#include "gameresults.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void HandleAITurn();

public slots:
    void EmptyClickedSlot(Card*);
    void RemedyClickedSlot(Card*);
    void HazardClickedSlot(Card*);
    void SafetyClickedSlot(Card*);
    void DistanceClickedSlot(Card*);

    void PlayAI();
    void CloseApplication();
    void PlayAgainSlot();

private slots:
    void on_readyButton_clicked();
    void on_drawButton_clicked();

    void on_dist_button_1_clicked();
    void on_dist_button_2_clicked();
    void on_dist_button_3_clicked();
    void on_dist_button_4_clicked();
    void on_dist_button_5_clicked();

    void on_safety_button_1_clicked();
    void on_safety_button_2_clicked();
    void on_safety_button_3_clicked();
    void on_safety_button_4_clicked();
    void on_safety_button_5_clicked();

private:
    // Private Methods
    void DisplayCurrentPlayer(Player*);
    void InitializeCards();
    void GameStartPopUps(QWidget *parent);
    void SetLabels(QLabel* name, QPushButton* distance_button, QPushButton* safety_button, QLabel* dist_label, int index);
    void SetHandLabel();
    void HandCardClicked(Card*);
    void BuildInitialGame();
    void PlayActionCard(Card*, unsigned long);
    bool PlayDistCard();
    void PlaySafetyCard(SafetyCard*, bool);
    void PlayLimitCard(Card* curr_limit, unsigned long index);
    void ReplaceWithEmpty(Card*);
    void ChangeHands();
    void SwapForGo();
    void CoupeFourre(Card* rem_card, SafetyCard* safety_card, unsigned long i);
    bool FindMatch(safeties curr_safety, RemedyCard* action_card);
    bool FindMatch(safeties curr_safety, HazardCard* action_card);
    void ResetIndicators();
    bool CheckForGameOver();
    bool CheckForEmptyHand(Player* player);
    void GameOver();
    void AI(Player* player);
    void InitializeGame();
    void ResetAllPlayers();

    //Private Data Members
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    GameResults* results_window_;

    Deck* deck_;
    Card* card_backs[MAX_HAND_SIZE];
    Card* curr_card_;
    Card* prev_card_;
    QRect curr_rect_;
    QRect hand_rect_;
    bool hide_;
    bool card_clicked_;
    bool coup_fourre_;
    bool empty_deck_;
    bool game_over_;
    bool draw_;

    QGraphicsDropShadowEffect* button_effect_;
    QGraphicsDropShadowEffect* hide_effect_;

    std::vector<Player*> players_;
    std::vector<QString> names_;
    int num_players_;
    unsigned long curr_player_;

};

#endif // MAINWINDOW_H
