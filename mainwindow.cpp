#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "namedialog.h"
#include <stdlib.h>
#include <QInputDialog>

/* MainWindow handles all game logic and displaying the game pieces */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    deck_(new Deck),
    curr_card_(nullptr),
    prev_card_(nullptr),
    hand_rect_(HAND_X, HAND_Y, HAND_WIDTH, HAND_HEIGHT),
    hide_(true),
    card_clicked_(false),
    coup_fourre_(false),
    empty_deck_(false),
    draw_(true),
    button_effect_(new QGraphicsDropShadowEffect()),
    hide_effect_(new QGraphicsDropShadowEffect()),
    curr_player_(0)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    QGraphicsView* view = ui->gameView;

    // Turn off Scroll Bars
    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    // Initialize button effects
    button_effect_->setBlurRadius(2);
    button_effect_->setOffset(3,3);
    button_effect_->setColor(QColor("green"));
    button_effect_->setEnabled(false);
    hide_effect_->setBlurRadius(2);
    hide_effect_->setOffset(3,3);
    hide_effect_->setColor(QColor("green"));
    ui->readyButton->setGraphicsEffect(hide_effect_);
    ui->drawButton->setGraphicsEffect(button_effect_);

    // Set scenes and sizes
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(), view->frameSize().height());

    GameStartPopUps(parent);
    InitializeGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeGame() {
    // Intialize game objects, cards, deck, and deal cards to players
    InitializeCards();
    deck_->Shuffle();
    deck_->Deal(players_);
    BuildInitialGame();

    // Initializing game labels / buttons
    SetHandLabel();
    SetLabels(ui->player_1, ui->dist_button_1, ui->safety_button_1, ui->miles_traveled_1, 0);
    SetLabels(ui->player_2, ui->dist_button_2, ui->safety_button_2, ui->miles_traveled_2, 1);
    SetLabels(ui->player_3, ui->dist_button_3, ui->safety_button_3, ui->miles_traveled_3, 2);
    SetLabels(ui->player_4, ui->dist_button_4, ui->safety_button_4, ui->miles_traveled_4, 3);
    SetLabels(ui->player_5, ui->dist_button_5, ui->safety_button_5, ui->miles_traveled_5, 4);
}

/* InitializeCards is a method to instantiate all the cards necessary for the game */
void MainWindow::InitializeCards() {
    std::vector<int> hazard_cards = {ACCIDENT_CARDS, OUT_OF_GAS_CARDS, FLAT_TIRE_CARDS, STOP_CARDS, SPEED_LIMIT_CARDS};
    std::vector<int> remedy_cards = {REPAIRS_CARDS, GASOLINE_CARDS, SPARE_TIRE_CARDS, GO_CARDS, END_OF_LIMIT_CARDS};
    std::vector<int> distance_cards = {DIST_1_CARDS, DIST_2_CARDS, DIST_3_CARDS, DIST_4_CARDS, DIST_5_CARDS};

    // Adding hazard cards
    for (unsigned int i = 0; i < hazard_cards.size(); i++) {
        for (int j = 0; j < hazard_cards[i]; j++) {
            Card *accidents = new HazardCard(static_cast<hazards>(i));
            connect(accidents, &Card::cardClicked, this, &MainWindow::HazardClickedSlot);
            deck_->AddCard(accidents);
        }
    }

    // Adding remedy cards
    for (unsigned int i = 0; i < remedy_cards.size(); i++) {
        for (int j = 0; j < remedy_cards[i]; j++) {
            Card *repairs = new RemedyCard(static_cast<remedies>(i));
            connect(repairs, &Card::cardClicked, this, &MainWindow::RemedyClickedSlot);
            deck_->AddCard(repairs);
        }
    }

    // Adding safety cards
    for (int i = 0; i <= static_cast<int>(safeties::EmergencyVehicle); i++) {
        Card *specialty_card = new SafetyCard(static_cast<safeties>(i));
        connect(specialty_card, &Card::cardClicked, this, &MainWindow::SafetyClickedSlot);
        deck_->AddCard(specialty_card);
    }

    // Adding distance cards
    for (unsigned int i = 0; i < distance_cards.size(); i++) {
        for (int j = 0; j < distance_cards[i]; j++) {
            Card *distances_card = new DistanceCard(static_cast<distances>(i));
            connect(distances_card, &Card::cardClicked, this, &MainWindow::DistanceClickedSlot);
            deck_->AddCard(distances_card);
        }
    }

    // Create Empty Action Cards
    for (int i = 0; i < num_players_; i++) {
        Card* empty_card = new Card;
        QPixmap* empty = CardImgFactory::get_image(true, false);
        empty_card->set_img(empty);

        connect(empty_card, &Card::cardClicked, this, &MainWindow::EmptyClickedSlot);
        deck_->AddEmptyCard(empty_card);
    }

    // Create Back Cards
    for (int i = 0; i < MAX_HAND_SIZE; i++) {
        // Creates backs of cards array for hiding players hands
        card_backs[i] = new Card;
        QPixmap* back_img = CardImgFactory::get_image(false, true);
        card_backs[i]->set_img(back_img);
    }
}

/* on_readyButton_clicked() handles when the "hide/reveal" button is pressed. It either displays the card
 * backs to hide a players hand, or displays the players hand and hides the card backs.
*/
void MainWindow::on_readyButton_clicked()
{
    for (int i = 0; i < MAX_HAND_SIZE; i++) {
        // Reveal players hand
        if (hide_) {
            card_backs[i]->hide();

            QRect rect(HAND_X, HAND_Y + i * H_ZONE_OFFSET, HAND_WIDTH, HAND_HEIGHT);
            players_[curr_player_]->get_hand()[static_cast<unsigned long>(i)]->set_rect(rect);
            scene->addItem(players_[curr_player_]->get_hand()[static_cast<unsigned long>(i)]);
        }
        // Hide players hand
        else {
            scene->removeItem(players_[curr_player_]->get_hand()[static_cast<unsigned long>(i)]);
            card_backs[i]->show();
        }
    }

    // Modify button effects and text
    if (hide_) {
        if (card_clicked_ || draw_) {
            button_effect_->setEnabled(true);
            ui->drawButton->update();
        }
        ui->readyButton->setText("Hide Hand");
    }
    else {
        button_effect_->setEnabled(false);
        ui->drawButton->update();
        ui->readyButton->setText("Show Hand");
    }

    hide_ = !hide_;
    scene->update();
}

/* on_drawButton_clicked handles the logic for the draw / discard button */
void MainWindow::on_drawButton_clicked()
{
    if (hide_) {
        return;
    }

    // Handles special case where player will be drawing two cards
    if (!coup_fourre_) {
        button_effect_->setEnabled(false);
    }

    Card** temp = players_[curr_player_]->FindEmptyCard();
    ui->drawButton->update();

    // If Empty Card exists, replace it with new draw card
    if (temp && draw_) {
        scene->removeItem(*temp);
        QRect temp_rect = (*temp)->get_rect();
        *temp = deck_->DrawCard();
        (*temp)->set_rect(temp_rect);
        scene->addItem(*temp);

        // Handles case of Coup Fouree where player needs to draw 2 cards
        if (coup_fourre_) {
            coup_fourre_ = false;
        }
        else {
            draw_ = false;
            ui->drawButton->setText("Discard");
        }
        unsigned long deck_size = deck_->get_draw_deck().size();
        if (!deck_size) {
            empty_deck_ = true;
        }
        ui->drawLabel->setText("Draw Pile: " + QString::number(deck_size));
        scene->update();
    }
    else if (!draw_) {
        if (curr_card_ && card_clicked_) {
            scene->removeItem(curr_card_);
            // Setting previous to current to reset indicators
            prev_card_ = curr_card_;
            ReplaceWithEmpty(players_[curr_player_]->get_empty_card());
            ResetIndicators();
            deck_->DiscardCard(curr_card_);
            ChangeHands();
        }
    }
}

/* HandCardClicked is a general helper method called whenever any card in the players hand is clicked.
 * This method handles the border color changes of the cards based on clicks, and button effects that will need
 * to be implemented upon these clicks. Ex: the discard button gets a green shadow when a card is clicked. Lastly,
 * this method also accounts for when cards are re-clicked or "unclicked".
*/
void MainWindow::HandCardClicked(Card* hand_card) {
    if (draw_) {
        return;
    }

    // Handles clicking a seperate or the same card after clicking one previously
    if (card_clicked_) {
        curr_card_->set_border_color(QColor("black"));
        if (curr_card_ == hand_card) {
            button_effect_->setEnabled(false);
            ui->drawButton->update();
            prev_card_ = curr_card_;
            card_clicked_ = false;
            hand_card->set_clicked(false);
            ResetIndicators();
            return;
        }
    }
    // Handle moments where control of a hand card is being given to the user
    else {
        card_clicked_ = true;

        // Discard button is highlighted
        button_effect_->setEnabled(true);
        ui->drawButton->update();
    }

    // Card clicked is made the new card to play
    hand_card->set_clicked(true);
    hand_card->set_border_color(QColor("red"));
    prev_card_ = curr_card_;
    curr_card_ = hand_card;

    curr_rect_ = hand_card->get_rect();
    ResetIndicators();
}

/* EmptyClickedSlot handles the early game scenarios when one of the empty cards that each player
 * starts with in their action zone is clicked. We handle two main scenarios here: when a player has clicked
 * a Go card in their hand, or the player has clicked an Emergency Vehicle card (which gives them an automatic Go card).
*/
void MainWindow::EmptyClickedSlot(Card* empty_card) {
    // If current players card is the empty card and a card is clicked (in hand)
    if (players_[curr_player_]->get_action_card() == empty_card && card_clicked_) {
        if (curr_card_->get_type() == cardType::Remedy) {
            // If Go Card is clicked in hand, play it in place of the empty card
            if (static_cast<RemedyCard*>(curr_card_)->get_rem() == remedies::Go) {
                players_[curr_player_]->set_go(true);
                players_[curr_player_]->set_go_card(curr_card_);
                PlayActionCard(empty_card, curr_player_);
                ChangeHands();
            }
        }
        else if (curr_card_->get_type() == cardType::Safety) {
            if (static_cast<SafetyCard*>(curr_card_)->get_safety() == safeties::EmergencyVehicle) {
                // Special occasion where a card is instantiated on the fly to accomodate for Emergency Vehicle being played
                Card* special_go_card = new RemedyCard(remedies::Go);
                connect(special_go_card, &Card::cardClicked, this, &MainWindow::RemedyClickedSlot);

                players_[curr_player_]->set_go(true);
                players_[curr_player_]->set_go_card(special_go_card);
                PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);

                // Replace empty card with Emergency Vehicle
                scene->removeItem(empty_card);
                deck_->DiscardCard(empty_card);
                prev_card_ = curr_card_;
                curr_card_ = special_go_card;

                special_go_card->set_rect(players_[curr_player_]->get_action_rect());
                players_[curr_player_]->set_action_card(special_go_card);

                special_go_card->update();
                scene->addItem(special_go_card);
                scene->update();
                ChangeHands();
            }
        }
    }
}

/* RemedyClickedSlot handles any time a remedy card is clicked in the MainWindow. If a player hasn't drawn, the method does
 * nothing. We then handle the logic for a Remedy Card clicked in hand, a remedy card clicked in a players action zone, or
 * a remedy card clicked in another players action zone.
*/
void MainWindow::RemedyClickedSlot(Card* rem_card) {
    // Initially check if player needs to draw
    if (draw_) {
        return;
    }
    // Handles if card in hand is clicked (or re-clicked)
    if (players_[curr_player_]->InHand(rem_card)) {
        Card* action_card = players_[curr_player_]->get_action_card();
        Card* empty_card = deck_->get_empty_action_card(curr_player_);
        Card* limit_card = players_[curr_player_]->get_limit_card();
        HandCardClicked(rem_card);

        if (card_clicked_) {
            // Handle all possible remedy cards played from hand
            switch (static_cast<RemedyCard*>(rem_card)->get_rem()) {
                case remedies::Go: {
                    // If action card is "Empty", any "Remedy" Card, or a "Stop" Card
                    if (action_card == empty_card ||
                            action_card->get_type() == cardType::Remedy ||
                                (action_card->get_type() == cardType::Hazard &&
                                    static_cast<HazardCard*>(action_card)->get_haz() == hazards::Stop)) {
                        action_card->set_border_color(QColor("lime"));
                    }
                    break;
                }
                case remedies::Repairs: {
                    if (action_card->get_type() == cardType::Hazard &&
                            static_cast<HazardCard*>(action_card)->get_haz() == hazards::Accident) {
                        action_card->set_border_color(QColor("lime"));
                    }
                    break;
                }
                case remedies::Gasoline: {
                    if (action_card->get_type() == cardType::Hazard &&
                            static_cast<HazardCard*>(action_card)->get_haz() == hazards::OutOfGas) {
                        action_card->set_border_color(QColor("lime"));
                    }
                    break;
                }
                case remedies::SpareTire: {
                    if (action_card->get_type() == cardType::Hazard &&
                            static_cast<HazardCard*>(action_card)->get_haz() == hazards::FlatTire) {
                        action_card->set_border_color(QColor("lime"));
                    }
                    break;
                }
                case remedies::EndOfLimit: {
                    if (limit_card) {
                        limit_card->set_border_color(QColor("lime"));
                    }
                    break;
                }
            }
        }
    }
    // Handle case when card is clicked, but not in hand
    else if (card_clicked_) {
        // Playing a Hazard Card on the clicked card
        if (curr_card_->get_type() == cardType::Hazard) {
            for (unsigned long i = 0; i < players_.size(); i++) {
                Card* action_card = players_[i]->get_action_card();

                // If opposing players action card is the clicked Remedy Card and they don't have a blocking Safety Card
                if (action_card == rem_card &&
                        !players_[i]->LookForSafetyBlock(static_cast<HazardCard*>(curr_card_)->get_haz())) {
                    action_card->set_border_color(QColor("black"));
                    // Check if opposing player has Coup Fourre
                    for (auto &card : players_[i]->get_hand()) {
                        if (card->get_type() == cardType::Safety) {
                            SafetyCard* safety_card = static_cast<SafetyCard*>(card);
                            if (FindMatch(safety_card->get_safety(), static_cast<HazardCard*>(curr_card_))) {
                                CoupeFourre(rem_card, safety_card, i);
                                return;
                            }
                        }
                    }
                    // Handles case where opponent doesn't have blocking safety card
                    if (static_cast<HazardCard*>(curr_card_)->get_haz() == hazards::SpeedLimit) {
                        Card* curr_limit = players_[i]->get_limit_card();
                        PlayLimitCard(curr_limit, i);
                    }
                    else {
                        players_[i]->set_go(false);
                        PlayActionCard(rem_card, i);
                    }
                    ResetIndicators();
                    ChangeHands();
                    return;
                }
            }
        }
        // Playing a Go Card on a Remedy Card
        else if (curr_card_->get_type() == cardType::Remedy &&
                 players_[curr_player_]->get_action_card() == rem_card) {
            if (static_cast<RemedyCard*>(curr_card_)->get_rem() == remedies::Go) {
                players_[curr_player_]->set_go(true);
                PlayActionCard(rem_card, curr_player_);
                ChangeHands();
            }
        }
        // Play Safety Card for defense / points
        else if (curr_card_->get_type() == cardType::Safety) {
            safeties curr_safety = static_cast<SafetyCard*>(curr_card_)->get_safety();

            if (FindMatch(curr_safety, static_cast<RemedyCard*>(rem_card))){
                SwapForGo();
            }
            PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
            ChangeHands();
        }
    }
}

/* HazardClickedSlot handles any time a hazard card is clicked in a players hand, in a players action zone, or in another players action zone.
 * When a hazard card is clicked in someones hand, the method searching for all possible positions that card can be played and marks the
 * corresponding ones with green borders. If a hazard card is clicked in a players action zone, the method checks the card that was clicked in
 * the players hand and chooses the appropriate action to take. When a card in another players action zone has been clicked, if the player also
 * has clicked a hazard card from their hand, then that card will replace the current hazard.
*/
void MainWindow::HazardClickedSlot(Card* haz_card) {
    if (draw_) {
        return;
    }
    // Card clicked is in the current players hand
    if (players_[curr_player_]->InHand(haz_card)) {
        HandCardClicked(haz_card);
        if (card_clicked_) {
            for (unsigned long i = 0; i < players_.size(); i++) {
                // Skip if current players iteration
                if (i == curr_player_) {
                    continue;
                }

                Card* curr_action_card = players_[i]->get_action_card();
                // If other players action card is Remedy or Hazard, highlight as playable
                if (curr_action_card->get_type() == cardType::Remedy || curr_action_card->get_type() == cardType::Hazard) {
                    // Check for safety cards
                    if (!players_[i]->LookForSafetyBlock(static_cast<HazardCard*>(haz_card)->get_haz())) {
                        curr_action_card->set_border_color(QColor("lime"));
                    }
                }
            }
        }
    }
    // Card clicked is not in hand, and hand card is a Remedy
    else if (card_clicked_ && curr_card_->get_type() == cardType::Remedy) {
        HazardCard* curr_haz_card = static_cast<HazardCard*>(haz_card);
        switch (static_cast<RemedyCard*>(curr_card_)->get_rem()) {
            case remedies::Go: {
                if (curr_haz_card->get_haz() == hazards::Stop) {
                    players_[curr_player_]->set_go(true);
                    PlayActionCard(haz_card, curr_player_);
                    ChangeHands();
                }
                break;
            }
            case remedies::Repairs: {
                if (curr_haz_card->get_haz() == hazards::Accident) {
                    PlayActionCard(haz_card, curr_player_);
                    if (players_[curr_player_]->FindSafety(safeties::EmergencyVehicle)) {
                        players_[curr_player_]->set_go(true);
                        SwapForGo();
                    }
                    ChangeHands();
                }
                    break;
            }
            case remedies::Gasoline: {
                if (curr_haz_card->get_haz() == hazards::OutOfGas) {
                    PlayActionCard(haz_card, curr_player_);
                    if (players_[curr_player_]->FindSafety(safeties::EmergencyVehicle)) {
                        players_[curr_player_]->set_go(true);
                        SwapForGo();
                    }
                    ChangeHands();
                }
                    break;
            }
            case remedies::SpareTire: {
                if (curr_haz_card->get_haz() == hazards::FlatTire) {
                    PlayActionCard(haz_card, curr_player_);
                    if (players_[curr_player_]->FindSafety(safeties::EmergencyVehicle)) {
                        players_[curr_player_]->set_go(true);
                        SwapForGo();
                    }
                    ChangeHands();
                }
                    break;
            }
            case remedies::EndOfLimit: {
                if (curr_haz_card->get_haz() == hazards::SpeedLimit) {
                    scene->removeItem(curr_haz_card);
                    scene->removeItem(curr_card_);
                    deck_->DiscardCard(curr_haz_card);
                    deck_->DiscardCard(curr_card_);
                    ReplaceWithEmpty(players_[curr_player_]->get_empty_card());
                    players_[curr_player_]->set_speed_limit(false);
                    ChangeHands();
                }
                    break;
            }
        }
    }
    // Card clicked is not in hand, and hand card is a Safety Card
    else if (card_clicked_ && curr_card_->get_type() == cardType::Safety) {
        HazardCard* curr_haz_card = static_cast<HazardCard*>(haz_card);
        SafetyCard* curr_safety_card = static_cast<SafetyCard*>(curr_card_);
        // Should refactor: safety && remedy || etc
        switch (curr_safety_card->get_safety()) {
            case safeties::FuelTank: {
                if (curr_haz_card->get_haz() == hazards::OutOfGas) {
                    SwapForGo();
                    PlaySafetyCard(curr_safety_card, false);
                    ChangeHands();
                }
                break;
            }
            case safeties::DrivingAce: {
                if (curr_haz_card->get_haz() == hazards::Accident) {
                    SwapForGo();
                    PlaySafetyCard(curr_safety_card, false);
                    ChangeHands();
                }
                break;
            }
            case safeties::PunctureProof: {
                if (curr_haz_card->get_haz() == hazards::FlatTire) {
                    SwapForGo();
                    PlaySafetyCard(curr_safety_card, false);
                    ChangeHands();
                }
                break;
            }
            case safeties::EmergencyVehicle: {
                if (curr_haz_card->get_haz() == hazards::Stop) {
                    SwapForGo();
                    PlaySafetyCard(curr_safety_card, false);
                    ChangeHands();
                }
                else if (curr_haz_card->get_haz() == hazards::SpeedLimit) {
                    scene->removeItem(curr_haz_card);
                    deck_->DiscardCard(curr_haz_card);
                    players_[curr_player_]->set_speed_limit(false);
                    PlaySafetyCard(curr_safety_card, false);
                    ChangeHands();
                }
                break;
            }
        }
    }
    // Card clicked is not in hand, and hand card is a Hazard Card
    else if (card_clicked_ && curr_card_->get_type() == cardType::Hazard) {
        for (unsigned long i = 0; i < players_.size(); i++) {
            if (curr_player_ == i) {
                continue;
            }
            Card* action_card = players_[i]->get_action_card();
            HazardCard* hazard_card = static_cast<HazardCard*>(haz_card);
            if (action_card == haz_card && !players_[i]->LookForSafetyBlock(hazard_card->get_haz())) {
                // Check if opposing player has Coup Fourre
                for (auto &card : players_[i]->get_hand()) {
                    if (card->get_type() == cardType::Safety) {
                        SafetyCard* safety_card = static_cast<SafetyCard*>(card);
                        if (FindMatch(safety_card->get_safety(), static_cast<HazardCard*>(curr_card_))) {
                            CoupeFourre(haz_card, safety_card, i);
                            return;
                        }
                    }
                }
                action_card->set_border_color(QColor("black"));
                if (static_cast<HazardCard*>(curr_card_)->get_haz() == hazards::SpeedLimit) {
                    PlayLimitCard(players_[i]->get_limit_card(), i);
                }
                else {
                    PlayActionCard(haz_card, i);
                }
                ResetIndicators();
                ChangeHands();
                return;
            }
        }
    }
}

/* SafetyClickedSlot handles any time a safety card is clicked in a players hand, any other time will result in nothing occuring. The method
 * will highlight possible cards that the safety can be played on to erase or if there are no current cards to play on, it will highlight the
 * safety box where a player can always play a safety card if they currently have one.
*/
void MainWindow::SafetyClickedSlot(Card* safety_card) {
    if (!draw_ && players_[curr_player_]->InHand(safety_card)) {
        HandCardClicked(safety_card);
        if (card_clicked_) {
            // Check if player has appropriate remedy card currently in their action zone
            Card* action_card = players_[curr_player_]->get_action_card();
            if (action_card->get_type() == cardType::Remedy) {
                if (FindMatch(static_cast<SafetyCard*>(safety_card)->get_safety(), static_cast<RemedyCard*>(action_card))) {
                    action_card->set_border_color(QColor("lime"));
                }
            }
            // Check if player has appropriate hazard card currently in their action zone
            // Need to refactor
            else if (action_card->get_type() == cardType::Hazard) {
                switch (static_cast<SafetyCard*>(safety_card)->get_safety()) {
                    case safeties::FuelTank: {
                        if (static_cast<HazardCard*>(action_card)->get_haz() == hazards::OutOfGas) {
                            action_card->set_border_color(QColor("lime"));
                        }
                        break;
                    }
                    case safeties::DrivingAce: {
                        if (static_cast<HazardCard*>(action_card)->get_haz() == hazards::Accident) {
                            action_card->set_border_color(QColor("lime"));
                        }
                        break;
                    }
                    case safeties::PunctureProof: {
                        if (static_cast<HazardCard*>(action_card)->get_haz() == hazards::FlatTire) {
                            action_card->set_border_color(QColor("lime"));
                        }
                        break;
                    }
                    // Shows remedy cards as clickable except Go, in case player wants to use Emergency Vehicle to replace Go
                    case safeties::EmergencyVehicle: {
                        if (static_cast<HazardCard*>(action_card)->get_haz() == hazards::Stop) {
                            action_card->set_border_color(QColor("lime"));
                        }
                        else if (players_[curr_player_]->get_speed_limit()) {
                            players_[curr_player_]->get_limit_card()->set_border_color(QColor("lime"));
                        }
                        break;
                    }
                }
            }
            else {
                // Handles Empty case where Emergency Vehicle can be played
                if (static_cast<SafetyCard*>(safety_card)->get_safety() == safeties::EmergencyVehicle) {
                    players_[curr_player_]->get_action_card()->set_border_color(QColor("lime"));
                }
            }
            // Handles Box to place safety cards if you aren't inheritly playing them for another card
            if (action_card->get_border_color() != QColor("lime")) {
                players_[curr_player_]->get_safety_section()->setEnabled(true);
            }
        }
    }
}

/* DistanceClickedSlot handles any time a distance card is clicked on the MainWindow. If a card is clicked in a players hand, the method
 * will highlight the distance zone if no other distance cards have been played or it will highlight the last distance card played, if the move
 * if valid. The method checks to see if a speed limit is currently being enforced on the player, or if the miles card they would play would put
 * them over the goal. If a card is clicked in the distance zone / section, the method checks if the current card is a distance card, and that it
 * won't allow the player to go over the goal or violate a speed limit.
*/
void MainWindow::DistanceClickedSlot(Card* dist_card) {
    if (draw_) {
        return;
    }

    std::vector<Card*> temp_dist = players_[curr_player_]->get_distance_cards();
    bool not_app = false;
    int dist_val = static_cast<DistanceCard*>(dist_card)->get_value();

    // Card clicked is in hand
    if (players_[curr_player_]->InHand(dist_card)) {
        // Checks to see if speed limit card is currently on player
        if ( (players_[curr_player_]->get_speed_limit() && dist_val > DIST_2) ||
                (players_[curr_player_]->get_dist_score() + dist_val > GOAL) )
            {
                not_app = true;
            }
        HandCardClicked(dist_card);
        // Highlight distance section or distance card
        if (players_[curr_player_]->get_go() && !not_app) {
            if (card_clicked_ && temp_dist.size() == 0) {
                players_[curr_player_]->get_dist_section()->setEnabled(true);
            }
            else if (card_clicked_) {
                temp_dist.back()->set_border_color(QColor("lime"));
            }
        }
    }
    // Card clicked is in players distance pile
    else if (players_[curr_player_]->InDistCards(dist_card)) {
        if (players_[curr_player_]->get_go() && !not_app) {
            temp_dist.back()->set_border_color(QColor("black"));
            PlayDistCard();
        }
    }
}

/* GameStartPopUps handles the initial game start up where windows are displayed asking the user how many players will be playing,
 * what their names are, and if they are a computer. I used a subclassed "NameDialog" class of the QDialog base class to create the
 * text box that users enter their name and check if the player is a computer or not.
*/
void MainWindow::GameStartPopUps(QWidget *parent) {
    // Dialog box asking how many players, restricted to 2 - 5
    num_players_ = QInputDialog::getInt(parent,"Mille Bornes","How many players: ", 2,2,5);

    for (int i = 0; i < num_players_; i++) {
        // Initialize Dialog box and set position
        NameDialog player_info_box(this, i + 1);
        QRect screenGeometry = QGuiApplication::screens().first()->availableGeometry();
        int x = (screenGeometry.width() - player_info_box.width()) / 2;
        int y = (screenGeometry.height() - player_info_box.height()) / 2;
        player_info_box.move(x, y);
        bool automated = false;

        // Handles dialog box information gathering
        if (player_info_box.exec() == QDialog::Accepted) {
            names_.push_back(player_info_box.get_name());
            automated = player_info_box.get_ai();
        }
        else {
            names_.push_back("");
        }
        Player* new_player = new Player(names_.back());
        new_player->set_ai(automated);

        // Set Action / Distance Zones location
        QRect zone_dim(ACTION_X, ACTION_Y + (i * V_ZONE_OFFSET), PLAYED_WIDTH, PLAYED_HEIGHT);
        new_player->set_action_rect(zone_dim);
        zone_dim.moveLeft(zone_dim.x() + 100);
        new_player->set_dist_rect(zone_dim);

        // Set Safety Zone locations
        QRect safety_zone_dim(SAFETY_X, SAFETY_Y + (i * V_ZONE_OFFSET), SAFETY_WIDTH, SAFETY_HEIGHT);
        new_player->set_safety_rect(safety_zone_dim);

        // Set Limit Zone locations
        QRect limit_zone_dim(LIMIT_X, ACTION_Y + (i * V_ZONE_OFFSET), PLAYED_WIDTH, PLAYED_HEIGHT);
        new_player->set_limit_rect(limit_zone_dim);

        players_.push_back(new_player);
    }
}

/* SetLabels is used during MainWindows instantiation to set appropriate names and hide appropriate areas based on number of players */
void MainWindow::SetLabels(QLabel* name, QPushButton* distance_button, QPushButton* safety_button, QLabel* dist_label, int index) {
    if (index > num_players_ - 1) {
        name->hide();
        distance_button->hide();
        safety_button->hide();
        dist_label->hide();
    }
    else {
        dist_label->setText("Miles Traveled: 0");
        name->setText(names_.at(static_cast<unsigned long>(index)));
        players_[static_cast<unsigned long>(index)]->set_dist_section(distance_button);
        players_[static_cast<unsigned long>(index)]->set_safety_section(safety_button);
        players_[static_cast<unsigned long>(index)]->set_dist_label(dist_label);
    }
}

/* SetHandLabel is a simple setter method to adjust the label above the current players hand */
void MainWindow::SetHandLabel() {
    ui->handLabel->setText(names_[curr_player_] + "'s Hand");
    ui->handLabel->setAlignment(Qt::AlignCenter);
}

/* BuildInitialGame is another method called during MainWindows instantiation that displays players initial action zones (empty cards),
 * and the card backs that will be replaced by the first players hand when the on_ready button is pressed.
*/
void MainWindow::BuildInitialGame() {

    ui->drawLabel->setText("Draw Pile: " + QString::number(deck_->get_draw_deck().size()));

    // Display Players Action Zone
    for (int i = 0; i < static_cast<int>(players_.size()); i++) {
        Card* temp = players_[static_cast<unsigned long>(i)]->get_action_card();
        QRect temp_rect = players_[static_cast<unsigned long>(i)]->get_action_rect();
        temp->set_rect(temp_rect);
        scene->addItem(temp);
    }

    // Display Card Backs in first player's hand
    for (int i = 0; i < MAX_HAND_SIZE; i++) {
        QRect temp_rect(hand_rect_);
        temp_rect.moveTop(hand_rect_.y() + (i * 90));
        card_backs[i]->set_rect(temp_rect);
        scene->addItem(card_backs[i]);
    }
}

/* ReplaceWithEmpty is a method to handle replacing a played card with an empty card in a players hand. This is acheived by locating the
 * index of the clicked card and simply adding an empty card to that location.
*/
void MainWindow::ReplaceWithEmpty(Card* empty_card) {
    int index = players_[curr_player_]->FindClickedCard();
    players_[curr_player_]->AddCard(empty_card, index);
    empty_card->set_rect(curr_rect_);

    // Set appropriate draw button display based on draw pile
    card_clicked_ = false;
    if (empty_deck_) {
        draw_ = false;
    }
    else {
        draw_ = true;
        ui->drawButton->setText("Draw");
    }
    button_effect_->setEnabled(true);
    ui->drawButton->update();

    // In a coupe fourre scenario, the respective method handles add / removing elements from scene because of alternate variables
    if (!coup_fourre_) {
        scene->addItem(empty_card);
    }
    scene->update();
}

/* PlayDistCard handles the logic of playing a distance card into a distance section.  */
bool MainWindow::PlayDistCard() {
    if (card_clicked_ && curr_card_->get_type() == cardType::Distance) {
        // Violates "safe trip" standard
        if (static_cast<DistanceCard*>(curr_card_)->get_dist() == distances::Dist_5) {
            players_[curr_player_]->set_safe_trip(false);
        }
        // Removes card from hand, modifies its position, and places it in respective action zone
        scene->removeItem(curr_card_);
        QRect temp = players_[curr_player_]->get_dist_rect();
        curr_card_->set_rect(temp);
        curr_card_->setZValue(players_[curr_player_]->get_dist_z_value());
        curr_card_->set_border_color(QColor("black"));

        players_[curr_player_]->add_distance_card(curr_card_);
        players_[curr_player_]->IncrDistScore(static_cast<DistanceCard*>(curr_card_)->get_value());

        scene->addItem(curr_card_);
        scene->update();
        ReplaceWithEmpty(players_[curr_player_]->get_empty_card());

        QRect new_dist_rect(temp.x() + 20, temp.y(), temp.width(), temp.height());
        players_[curr_player_]->set_dist_rect(new_dist_rect);
        players_[curr_player_]->incr_dist_z_value();

        ChangeHands();
        return true;
    }
    return false;
}

/* PlaySafetyCard handles the logic of moving a safety card from a players hand, to their safety zone. If the parameter coup_fourre is true
 * a special empty card is used so that two cards will need to be draw upon the control being handed back to the player whom dealt the card.
*/
void MainWindow::PlaySafetyCard(SafetyCard* curr_safety_card, bool coup_fourre) {
    scene->removeItem(curr_safety_card);
    if (!coup_fourre) {
        ReplaceWithEmpty(players_[curr_player_]->get_empty_card());
        curr_safety_card->set_play(true);
    }
    else {
        coup_fourre_ = true;
        ReplaceWithEmpty(players_[curr_player_]->get_spec_empty_card());
        curr_safety_card->set_coup_fourre(true);
    }
    curr_safety_card->set_border_color(QColor("black"));
    curr_safety_card->set_rect(players_[curr_player_]->get_safety_rect());
    scene->addItem(curr_safety_card);
    players_[curr_player_]->UpdateSafetyRect();
    players_[curr_player_]->add_safety_card(curr_safety_card);
}

/* PlayActionCard handles the logic of moving a card from a players hand to a players action zone. */
void MainWindow::PlayActionCard(Card* action_card, unsigned long index) {
    scene->removeItem(action_card);
    scene->removeItem(curr_card_);
    deck_->DiscardCard(action_card);
    prev_card_ = curr_card_;

    curr_card_->set_rect(players_[index]->get_action_rect());
    curr_card_->set_border_color(QColor("black"));
    players_[index]->set_action_card(curr_card_);

    scene->addItem(curr_card_);
    scene->update();

    ReplaceWithEmpty(players_[curr_player_]->get_empty_card());
}

/* PlayLimitCard handles the logic of moving a card from a players hand to a players speed limit area / zone. */
void MainWindow::PlayLimitCard(Card* curr_limit, unsigned long index) {
    if (curr_limit) {
        deck_->DiscardCard(curr_limit);
        scene->removeItem(curr_limit);
    }
    players_[index]->set_speed_limit(true);
    players_[index]->set_limit_card(curr_card_);
    players_[index]->get_action_card()->set_border_color(QColor("black"));

    scene->removeItem(curr_card_);
    prev_card_ = curr_card_;
    curr_card_->set_rect(players_[index]->get_limit_rect());

    scene->addItem(curr_card_);
    scene->update();

    ReplaceWithEmpty(players_[curr_player_]->get_empty_card());
}

/* ChangeHands handles passing control to the next player. It first checks to see if the game is not currently over. Then hides the
 * hand with card backs, increments the current players turn to a player whom has cards in their hand, sets the appropriate labels and
 * calls the AI method if necessary for the specific player.
*/
void MainWindow::ChangeHands() {
    if (CheckForGameOver()) {
        GameOver();
        return;
    }

    hide_ = false;
    curr_card_ = nullptr;
    card_clicked_ = false;

    // Hide hand
    on_readyButton_clicked();

    // Find current player with cards to play
    do {
        if (curr_player_ == static_cast<unsigned long>(num_players_ - 1)) {
            curr_player_ = 0;
        }
        else {
            curr_player_++;
        }
     } while (empty_deck_ && CheckForEmptyHand(players_[curr_player_]));

    // If deck is empty, instantiate a new empty card for the player to be able to replace their played card with
    if (empty_deck_) {
        Card* empty_card = new Card;
        QPixmap* empty_img = CardImgFactory::get_image(true, false);
        empty_card->set_img(empty_img);
        players_[curr_player_]->set_empty_card(empty_card);
    }
    SetHandLabel();

    if (players_[curr_player_]->get_ai()) {
        HandleAITurn();
    }
}

/* SwapForGo is a method that handles the logic of swapping in a go card in the action zone of a player. This is mostly used
 * when playing safety cards that eliminate hazards / remedy cards.
*/
void MainWindow::SwapForGo() {
    Card* curr_action_card = players_[curr_player_]->get_action_card();
    Card* curr_go_card = players_[curr_player_]->get_go_card();

    scene->removeItem(curr_action_card);
    deck_->DiscardCard(curr_action_card);

    players_[curr_player_]->set_action_card(curr_go_card);
    players_[curr_player_]->set_go(true);
    scene->addItem(curr_go_card);
    scene->update();
}

/* CoupeFourre handles the logic necessary when a player plays a hazard on another player whom has a safety card to combat it.
 * This is a special situation where control is then given to that player, they get bonus points, and then draw two cards and play.
 * A window is displayed with the name of the player whom plays the card and an image of the card that was played.
*/
void MainWindow::CoupeFourre(Card* played_card, SafetyCard* safety_card, unsigned long i) {
    PlayActionCard(played_card, i);
    ResetIndicators();

    // Don't display window is players is AI
    if (!players_[i]->get_ai()) {
        QPixmap* icon = safety_card->get_img();
        QString message(players_[i]->get_name() + ": Coupe Fourre!");

        QMessageBox coupe_message(this);
        coupe_message.setText(message);
        coupe_message.setIconPixmap(*icon);
        coupe_message.exec();
    }

    hide_ = false;
    on_readyButton_clicked();
    curr_player_ = i;
    players_[curr_player_]->incr_total_score(COUPE_FOURRE);
    safety_card->set_clicked(true);
    SwapForGo();
    PlaySafetyCard(safety_card, true);
    SetHandLabel();

    // If the player who played the card is AI, have them continue play
    if (players_[curr_player_]->get_ai()) {
        HandleAITurn();
    }
}

/* FindMatch is a helper function to find a matching safety and remedy card. This method could be dramatically downsized with the use
 * of a map like I implemented in the Player class */
bool MainWindow::FindMatch(safeties curr_safety, RemedyCard* action_card) {
    if (curr_safety == safeties::EmergencyVehicle && action_card->get_rem() != remedies::Go) {
        return true;
    }

    switch (action_card->get_rem()) {
        case remedies::EndOfLimit:
        case remedies::Go:
            return false;
        case remedies::Repairs: {
            if (curr_safety == safeties::DrivingAce) {
                return true;
            }
            break;
        }
        case remedies::Gasoline: {
            if (curr_safety == safeties::FuelTank) {
                return true;
            }
            break;
        }
        case remedies::SpareTire: {
            if (curr_safety == safeties::PunctureProof) {
                return true;
            }
            break;
        }
    }
    return false;
}

/* FindMatch is a helper function to find a matching safety and hazard card. This method could be dramatically downsized with the use
 * of a map like I implemented in the Player class */
bool MainWindow::FindMatch(safeties curr_safety, HazardCard* action_card) {
    switch (action_card->get_haz()) {
        case hazards::SpeedLimit:
        case hazards::Stop: {
            if (curr_safety == safeties::EmergencyVehicle) {
                return true;
            }
            break;
        }
        case hazards::Accident: {
            if (curr_safety == safeties::DrivingAce) {
                return true;
            }
            break;
        }
        case hazards::OutOfGas: {
            if (curr_safety == safeties::FuelTank) {
                return true;
            }
            break;
        }
        case hazards::FlatTire: {
            if (curr_safety == safeties::PunctureProof) {
                return true;
            }
            break;
        }
    }
    return false;
}

/* ResetIndicators is the method that handles reseting any highlighted cards or areas that have been set once a player makes a move or simply
 * clicks another card or unclicks a card. Each subclass of card is handled for specific situations.
*/
void MainWindow::ResetIndicators() {
    // Edge case for first turn
    if (!prev_card_) {
        return;
    }

    // Edge case for same card clicked multiple times
    if (prev_card_ != curr_card_) {
        prev_card_->set_clicked(false);
    }

    switch (prev_card_->get_type()) {
        case cardType::Distance: {
            // Hand card isn't currently clicked or isn't a distance card
            if (!card_clicked_ || curr_card_->get_type() != cardType::Distance) {
                std::vector<Card*> temp_dist = players_[curr_player_]->get_distance_cards();
                // Reset distance section borders to black if they exist
                if (temp_dist.size()) {
                    temp_dist.back()->set_border_color(QColor("black"));
                }
                // Disable distance section if no distance cards have been played
                else {
                    players_[curr_player_]->get_dist_section()->setEnabled(false);
                }
            }
            break;
        }
        case cardType::Remedy: {
            // Clicked on Remedy Card in Hand
            if (card_clicked_ && curr_card_->get_type() == cardType::Remedy) {
                if (static_cast<RemedyCard*>(prev_card_)->get_rem() != static_cast<RemedyCard*>(curr_card_)->get_rem()) {
                    players_[curr_player_]->get_action_card()->set_border_color(QColor("black"));
                }
            }
            else {
                Card* curr_limit = players_[curr_player_]->get_limit_card();
                if (static_cast<RemedyCard*>(prev_card_)->get_rem() == remedies::EndOfLimit && curr_limit) {
                    curr_limit->set_border_color(QColor("black"));
                }
                players_[curr_player_]->get_action_card()->set_border_color(QColor("black"));
            }
                break;
        }
        case cardType::Safety: {
            if (!card_clicked_ || curr_card_->get_type() != cardType::Safety) {
                players_[curr_player_]->get_action_card()->set_border_color(QColor("black"));
                players_[curr_player_]->get_safety_section()->setEnabled(false);
            }
            break;
        }
        case cardType::Hazard: {
            hazards prev_haz = static_cast<HazardCard*>(prev_card_)->get_haz();
            if (card_clicked_ && curr_card_->get_type() == cardType::Hazard) {
                if (prev_haz != static_cast<HazardCard*>(curr_card_)->get_haz()) {
                    for (auto &player : players_) {
                        if (prev_haz == hazards::SpeedLimit && player->get_limit_card()) {
                            player->get_limit_card()->set_border_color(QColor("black"));
                        }
                        player->get_action_card()->set_border_color(QColor("black"));
                    }
                }
            }
            else {
                for (auto &player : players_) {
                    if (prev_haz == hazards::SpeedLimit && player->get_limit_card()) {
                        player->get_limit_card()->set_border_color(QColor("black"));
                    }
                    player->get_action_card()->set_border_color(QColor("black"));
                }
            }
            break;
        }
        case cardType::Default:
            break;
    }
}

/* CheckForGameOver looks for two conditions. The first one being if the current player has reached the distance goal.
 * The second one being if all players have empty hands. If either are true, the game is over.
*/
bool MainWindow::CheckForGameOver() {
    // Check if current player reached the goal
    if (players_[curr_player_]->get_dist_score() == GOAL) {
        return true;
    }
    // Check if all players have played all their cards
    if (empty_deck_) {
        for (auto &player : players_) {
            if (!CheckForEmptyHand(player)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

/* This is a simple helper function to check if a players hand is empty. This will only occur towards the end of the game. */
bool MainWindow::CheckForEmptyHand(Player* player) {
    for (auto &card : player->get_hand()) {
        if (card->get_type() != cardType::Default) {
            return false;
        }
    }
    return true;
}

/* GameOver handles the score tallying at the end of the game and building a display window to show the winners results */
void MainWindow::GameOver() {
    // Calculates Points
    for (auto &player : players_) {
        std::vector<SafetyCard*> safeties = player->get_safety_cards();

        for (unsigned long i = 0; i < safeties.size(); i++) {
            player->incr_total_score(SAFETY_PLAYED);
        }
        if (safeties.size() == 4) {
            player->incr_total_score(ALL_FOUR);
        }
        if (player->get_safe_trip()) {
            player->incr_total_score(SAFE_TRIP);
        }
        if (player->get_dist_score() == GOAL) {
            player->incr_total_score(TRIP_COMPLETED);
        }
        player->incr_total_score(player->get_dist_score());
    }

    // Finds winner
    Player* winner = players_[0];
    int max = 0;

    for (auto &player : players_) {
        if (player->get_total_score() > max) {
            max = player->get_total_score();
            winner = player;
        }
    }

    // Grabs winners specific points values
    int trip_completed = 0;
    int delayed_action = 0;
    int safe_trip = 0;

    // If winner completed the trip
    if (winner->get_dist_score() == 1000) {
        trip_completed = TRIP_COMPLETED;
        // If winner completed the trip after the deck was emptied
        if (empty_deck_) {
            delayed_action = DELAYED_ACTION;
        }
        // If winner completed trip without playing any 200 dist cards
        if (winner->get_safe_trip()) {
            safe_trip = SAFE_TRIP;
        }
    }

    QString results = "<table><tr><td>" + winner->get_name() + "'s Final Score: </td><td><strong>" + QString::number(winner->get_total_score()) +
          "</td></tr></strong><tr><td>Distance:       </td><td><strong>" + QString::number(winner->get_dist_score()) +
          "</td></tr></strong><tr><td>Safeties:       </td><td><strong>" + QString::number(winner->get_safety_cards().size() * 100) +
          "</td></tr></strong><tr><td>Coupe Fourres:  </td><td><strong>" + QString::number(winner->get_coupe_fourre_score()) +
          "</td></tr></strong><tr><td>Trip Completed: </td><td><strong>" + QString::number(trip_completed) +
          "</td></tr></strong><tr><td>Delayed Action: </td><td><strong>" + QString::number(delayed_action) +
          "</td></tr></strong><tr><td>Safe Trip:      </td><td><strong>" + QString::number(safe_trip) + "<td></table>";

    // Instantiate a GameResults box, if exit is hit on this box the application is closed. Play again doesn't yet work.
    results_window_ = new GameResults(this);
    connect(results_window_, &GameResults::ExitButtonClicked, this, &MainWindow::CloseApplication);
    connect(results_window_, &GameResults::PlayAgainButtonClicked, this, &MainWindow::PlayAgainSlot);
    results_window_->InputResults(winner->get_name(), results);
    results_window_->show();
}

/* AI handles the "basic" AI of a computer opponent. It runs through conditionals to check for certain cards it may have the
 * ability to play. If they are accessible, they are played. The final condition to discard.
*/
void MainWindow::AI(Player* player) {
    Card* action_card = player->get_action_card();
    std::vector<Card*> &hand = player->get_hand();

    bool action_remedy = action_card->get_type() == cardType::Remedy;
    bool action_hazard = action_card->get_type() == cardType::Hazard;
    Card* limit_card = player->get_limit_card();

    // Plays Go Card whenever empty or non-Go remedy card is in action zone
    if (action_card->get_type() == cardType::Default ||
            (action_remedy && static_cast<RemedyCard*>(action_card)->get_rem() != remedies::Go) ) {
        for (unsigned long i = 0; i < hand.size(); i++) {
            if (hand[i]->get_type() == cardType::Remedy && static_cast<RemedyCard*>(hand[i])->get_rem() == remedies::Go) {
                hand[i]->cardClicked(hand[i]);
                action_card->cardClicked(action_card);
                return;
            }
        }
    }

    // Play a End of Limit if available if Speed Limit is on player
    if (limit_card && player->get_speed_limit()) {
        for (unsigned long i = 0; i < hand.size(); i++) {
            if (hand[i]->get_type() == cardType::Remedy && static_cast<RemedyCard*>(hand[i])->get_rem() == remedies::EndOfLimit) {
                RemedyClickedSlot((hand[i]));
                HazardClickedSlot(limit_card);
                return;
            }
        }
    }

    // Plays Distance Card whenever possible if Go Card is in action zone
    if (action_remedy && static_cast<RemedyCard*>(action_card)->get_rem() == remedies::Go) {
        unsigned long index = player->FindMaxDistance();
        if (index != ERROR) {
            hand[index]->cardClicked(hand[index]);
            if (player->get_distance_cards().empty()) {
                player->get_dist_section()->click();
            }
            else {
                Card* dist_back = player->get_distance_cards().back();
                dist_back->cardClicked(dist_back);
            }
            return;
        }
    }

    // Play Remedy Card whenever available if Hazard Card is in action zone
    if (action_hazard) {
        hazards curr_haz = static_cast<HazardCard*>(action_card)->get_haz();
        unsigned long index = player->FindRemedyForHazard(curr_haz);
        if (index != ERROR) {
            RemedyClickedSlot(hand[index]);
            HazardClickedSlot(action_card);
            return;
        }
    }

    // Play a Hazard on first available player if in hand
    Card* curr_action_card = nullptr;
    for (unsigned long i = 0; i < players_.size(); i++) {
        if (curr_player_ == i) {
            continue;
        }
        curr_action_card = players_[i]->get_action_card();
        if (curr_action_card->get_type() == cardType::Remedy) {
            unsigned long index = player->FindHazardForRemedy(static_cast<RemedyCard*>(curr_action_card)->get_rem());
            if (index != ERROR && !players_[i]->LookForSafetyBlock(static_cast<HazardCard*>(hand[index])->get_haz())) {
//                QTimer::singleShot(100, this, SLOT(HazardClickedSlot(hand[index])));
//                QTimer::singleShot(100, this, SLOT(RemedyClickedSlot(curr_action_card)));
//                QTimer::singleShot(100, this, SLOT(return));
                HazardClickedSlot(hand[index]);
                RemedyClickedSlot(curr_action_card);
                return;
            }
        }
    }

    // If you have a safety, play it
    for (unsigned long i = 0; i < hand.size(); i++) {
        if (hand[i]->get_type() == cardType::Safety) {
            SafetyClickedSlot(hand[i]);
            if (action_card->get_border_color() == QColor("lime")) {
                action_card->cardClicked(action_card);
            }
            else {
                player->get_safety_section()->click();
            }
            return;
        }
    }

    // Discard
    bool discarded = false;
    while (!discarded) {
        // All possible indecies initially, choose one randomly and check if not empty
        std::vector<unsigned long> index_map = { 0, 1, 2, 3, 4, 5, 6 };
        unsigned long index = index_map[static_cast<unsigned long>(rand()) % index_map.size()];
        if (hand[index]->get_type() != cardType::Default) {
            hand[index]->cardClicked(hand[index]);
            on_drawButton_clicked();
            discarded = true;
        }
        // Remove index from index_map if empty card at index
        else {
            index_map.erase(index_map.begin() + static_cast<int>(index));
        }
    }
}

void MainWindow::HandleAITurn() {
    if (players_[curr_player_]->get_ai()) {
        QTimer::singleShot(100, this, SLOT(on_readyButton_clicked()));
        QTimer::singleShot(300, this, SLOT(on_drawButton_clicked()));
        if (coup_fourre_) {
            QTimer::singleShot(500, this, SLOT(on_drawButton_clicked()));
        }
        QTimer::singleShot(600, this, SLOT(PlayAI()));
    }
}

void MainWindow::PlayAI() {
    AI(players_[curr_player_]);
}

void MainWindow::ResetAllPlayers() {
    for (auto &player: players_) {
        for (auto &card : player->get_distance_cards()) {
            scene->removeItem(card);
        }
        deck_->DiscardCard(player->get_action_card());
        player->Reset();
    }
}

void MainWindow::PlayAgainSlot() {
    // Need to reset "Show Hand" button text
    // Currently FindEmptyCard is returning null upon restarting
    ResetAllPlayers();
    delete deck_;
    curr_card_ = nullptr;
    prev_card_ = nullptr;
    hide_ = true;
    card_clicked_ = false;
    coup_fourre_ = false;
    draw_ = true;
    curr_player_ = 0;
    scene->update();
    deck_ = new Deck();
    InitializeGame();
}

// Safety Button Slots //////////////////////////////

/* Safety Button Slots handle when a safety card is selected that can't currently be played on a users action cards but
 * will instead be simply played for defensive / points reasons
*/
void MainWindow::on_safety_button_1_clicked()
{
    PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
    ui->safety_button_1->setEnabled(false);
    ChangeHands();
}

void MainWindow::on_safety_button_2_clicked()
{
    PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
    ui->safety_button_2->setEnabled(false);
    ChangeHands();
}

void MainWindow::on_safety_button_3_clicked()
{
    PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
    ui->safety_button_3->setEnabled(false);
    ChangeHands();
}

void MainWindow::on_safety_button_4_clicked()
{
    PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
    ui->safety_button_4->setEnabled(false);
    ChangeHands();
}

void MainWindow::on_safety_button_5_clicked()
{
    PlaySafetyCard(static_cast<SafetyCard*>(curr_card_), false);
    ui->safety_button_5->setEnabled(false);
    ChangeHands();
}

// Initial Distance Button Slots ////////////////////

/* Distance Button Slots handles the initial distance card added to the distance section, after which the button is disabled for the
 * rest of the game.
*/
void MainWindow::on_dist_button_1_clicked()
{
    if (PlayDistCard()) {
        ui->dist_button_1->hide();
    }
}

void MainWindow::on_dist_button_2_clicked()
{
    if (PlayDistCard()) {
        ui->dist_button_2->hide();
    }
}

void MainWindow::on_dist_button_3_clicked()
{
    if (PlayDistCard()) {
        ui->dist_button_3->hide();
    }
}

void MainWindow::on_dist_button_4_clicked()
{
    if (PlayDistCard()) {
        ui->dist_button_4->hide();
    }
}

void MainWindow::on_dist_button_5_clicked()
{
    if (PlayDistCard()) {
        ui->dist_button_5->hide();
    }
}

// Close Application Slot

void MainWindow::CloseApplication() {
    close();
}
