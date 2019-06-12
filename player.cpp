#include "player.h"
#include "cardimgfactory.h"
#include <map>

Player::Player(QString name) :
    hand_(MAX_HAND_SIZE, nullptr),
    action_card_(nullptr),
    limit_card_(nullptr),
    empty_card_(new Card),
    spec_empty_card_(new Card),
    safety_rect_(SAFETY_X, SAFETY_Y, SAFETY_WIDTH, SAFETY_HEIGHT),
    limit_rect_(LIMIT_X, ACTION_Y, HAND_WIDTH, HAND_HEIGHT),
    safety_count_(0),
    dist_z_value_(0),
    dist_score_(0),
    coupe_fourre_score_(0),
    total_score_(0),
    max_(MAX_HAND_SIZE),
    go_(false),
    speed_limit_(false),
    safe_trip_(true),
    ai_(false),
    name_(name) {

    // Create Empty Cards
    MakeEmptyCard(empty_card_);
    MakeEmptyCard(spec_empty_card_);

    // Add Empty Card
    hand_[MAX_HAND_SIZE - 1] = empty_card_;

    haz_to_rem_.insert(std::pair<hazards, remedies>(hazards::Stop, remedies::Go));
    haz_to_rem_.insert(std::pair<hazards, remedies>(hazards::Accident, remedies::Repairs));
    haz_to_rem_.insert(std::pair<hazards, remedies>(hazards::FlatTire, remedies::SpareTire));
    haz_to_rem_.insert(std::pair<hazards, remedies>(hazards::OutOfGas, remedies::Gasoline));
    haz_to_rem_.insert(std::pair<hazards, remedies>(hazards::SpeedLimit, remedies::EndOfLimit));

    rem_to_haz_.insert(std::pair<remedies, hazards>(remedies::Go, hazards::Stop));
    rem_to_haz_.insert(std::pair<remedies, hazards>(remedies::Repairs, hazards::Accident));
    rem_to_haz_.insert(std::pair<remedies, hazards>(remedies::SpareTire, hazards::FlatTire));
    rem_to_haz_.insert(std::pair<remedies, hazards>(remedies::Gasoline, hazards::OutOfGas));
    rem_to_haz_.insert(std::pair<remedies, hazards>(remedies::EndOfLimit, hazards::SpeedLimit));
}

void Player::Reset() {
    for (auto & card: hand_) {
        delete card;
    }
    hand_.clear();
    for (auto &card: safety_cards_) {
        delete card;
    }
    safety_cards_.clear();

    action_card_ = nullptr;
    empty_card_ = new Card;

    if (spec_empty_card_) {
        delete spec_empty_card_;
    }
    spec_empty_card_ = new Card;
    safety_rect_ = QRect(SAFETY_X, SAFETY_Y, SAFETY_WIDTH, SAFETY_HEIGHT);
    safety_count_ = 0;
    dist_z_value_ = 0;
    coupe_fourre_score_ = 0;
    total_score_ = 0;
    go_ = false;
    speed_limit_ = false;
    safe_trip_ = true;

    // Create Empty Cards
    MakeEmptyCard(empty_card_);
    MakeEmptyCard(spec_empty_card_);

    // Add Empty Card
    hand_[MAX_HAND_SIZE - 1] = empty_card_;

}

/* AddCard is a simple method to add a card to the hand vector */
void Player::AddCard(Card* new_card) {
    hand_.push_back(new_card);
}

/* This overloaded AddCard allows for the user to specify an index to add the card to */
void Player::AddCard(Card* new_card, int index) {
    hand_[static_cast<unsigned long>(index)] = new_card;
}

/* InHand checks to see if the Card passed in is in the players hand */
bool Player::InHand(Card* search_card) {
    for (Card* &card : hand_) {
        if (card == search_card) {
            return true;
        }
    }
    return false;
}

/* InDistCards looks for a card in the distance_cards_ vector */
bool Player::InDistCards(Card* search_card) {
    for (Card* &card : distance_cards_) {
        if (card == search_card) {
            return true;
        }
    }
    return false;
}

/* FindEmptyCard returns the memory address of an empty card in the deck. This is useful for knowing the location to place a
 * newly drawn card */
Card** Player::FindEmptyCard() {
    for (auto &card : hand_) {
        if (card == empty_card_ || card == spec_empty_card_) {
            return &card;
        }
    }
    return nullptr;
}

/* FindClickedCard simply looks for a card whose clicked data member is marked as true. It returns an error value if not. */
int Player::FindClickedCard() {
    for (unsigned long i = 0; i < hand_.size(); i++) {
        if (hand_[i]->get_clicked()) {
            return static_cast<int>(i);
        }
    }
    return MAX_HAND_SIZE;
}

void Player::IncrDistScore(int c) {
    dist_score_ += c;
    dist_label_->setText("Miles Traveled: " + QString::number(dist_score_));
}

/* UpdateSafetyRect handles the different positions of the safety cards based on how many have been played */
void Player::UpdateSafetyRect() {
    incr_safety_count();
    switch (safety_count_) {
        case 1:
        case 3:
            safety_rect_.moveLeft(safety_rect_.x() + H_OFFSET);
            break;
        case 2:
            safety_rect_.moveLeft(safety_rect_.x() - H_OFFSET);
            safety_rect_.moveTop(safety_rect_.y() + V_OFFSET);
            break;
        default:
            break;
    }
}

/* MakeEmptyCard is a simple function to make a new empty card. This is only used upon Player instantiation */
void Player::MakeEmptyCard(Card* empty_card) {
    QPixmap* empty = CardImgFactory::get_image(true, false);
    empty_card->set_img(empty);
}

bool Player::LookForSafetyBlock(hazards haz) {
    for (auto &card : safety_cards_) {
        switch (haz) {
            case hazards::Stop:
            case hazards::SpeedLimit: {
                if (card->get_safety() == safeties::EmergencyVehicle) {
                    return true;
                }
                break;
            }
            case hazards::Accident: {
                if (card->get_safety() == safeties::DrivingAce) {
                    return true;
                }
                break;
            }
            case hazards::OutOfGas: {
                if (card->get_safety() == safeties::FuelTank) {
                    return true;
                }
                break;
            }
            case hazards::FlatTire: {
                if (card->get_safety() == safeties::PunctureProof) {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

bool Player::FindSafety(safeties safety) {
    for (auto &card : safety_cards_) {
        if (card->get_safety() == safety) {
            return true;
        }
    }
    return false;
}

unsigned long Player::FindMaxDistance() {
    int max = 0;
    unsigned long index = 0;
    for (unsigned long i = 0; i < hand_.size(); i++) {
        if (hand_[i]->get_type() == cardType::Distance && static_cast<DistanceCard*>(hand_[i])->get_value() > max) {
            int curr_val = static_cast<DistanceCard*>(hand_[i])->get_value();
            if (speed_limit_) {
                if (curr_val > DIST_2) {
                    continue;
                }
            }
            if (dist_score_ + curr_val <= GOAL) {
                max = static_cast<DistanceCard*>(hand_[i])->get_value();
                index = i;
            }
        }
    }
    if (max == 0) {
        return ERROR;
    }
    return index;
}

unsigned long Player::FindRemedyForHazard(hazards haz) {
    for (unsigned long i = 0; i < hand_.size(); i++) {
        if (hand_[i]->get_type() == cardType::Remedy && ( haz_to_rem_[haz] == static_cast<RemedyCard*>(hand_[i])->get_rem()) ) {
            return i;
        }
    }
    return ERROR;
}

unsigned long Player::FindHazardForRemedy(remedies rem) {
    for (unsigned long i = 0; i < hand_.size(); i++) {
        if (hand_[i]->get_type() == cardType::Hazard && ( rem_to_haz_[rem] == static_cast<HazardCard*>(hand_[i])->get_haz()) ) {
            return i;
        }
    }
    return ERROR;
}

