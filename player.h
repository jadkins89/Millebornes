#ifndef PLAYER_H
#define PLAYER_H

#include "cards.h"
#include "definitions.h"
#include <vector>
#include <map>
#include <iostream>

class Player {
public:
    // Constructor
    Player(QString);

    // Public Methods
    void AddCard(Card*);
    void AddCard(Card*, int);
    void Discard(int);
    bool InHand(Card*);
    bool InDistCards(Card*);
    Card** FindEmptyCard();
    int FindClickedCard();
    void IncrDistScore(int c);
    void UpdateSafetyRect();
    bool LookForSafetyBlock(hazards);
    bool FindSafety(safeties);
    void Reset();

    // AI Public Methods
    unsigned long FindMaxDistance();
    unsigned long FindRemedyForHazard(hazards haz);
    unsigned long FindHazardForRemedy(remedies rem);

    // Setters & Getters
    std::vector<Card*>& get_hand() { return hand_; }
    std::vector<Card*>& get_distance_cards() { return distance_cards_; }
    std::vector<SafetyCard*> get_safety_cards() { return safety_cards_; }
    void add_distance_card(Card* new_dist) { distance_cards_.push_back(new_dist); }
    void add_safety_card(SafetyCard* new_safety) { safety_cards_.push_back(new_safety); }

    void set_go_card(Card* go) { go_card_ = go; }
    Card* get_go_card() const { return go_card_; }
    void set_action_card(Card* temp) { action_card_ = temp; }
    Card* get_action_card() const { return action_card_; }
    void set_limit_card(Card* limit) { limit_card_ = limit; }
    Card* get_limit_card() const { return limit_card_; }
    void set_empty_card(Card* empty) { empty_card_ = empty; }
    Card* get_empty_card() const { return empty_card_; }
    Card* get_spec_empty_card() const { return spec_empty_card_; }

    void set_action_rect(QRect new_rect) { action_rect_ = new_rect; }
    QRect get_action_rect() const { return action_rect_; }
    void set_dist_rect(QRect new_rect) { dist_rect_ = new_rect; }
    QRect get_dist_rect() const { return dist_rect_; }
    void set_safety_rect(QRect new_rect) { safety_rect_ = new_rect; }
    QRect get_safety_rect() const { return safety_rect_; }
    void set_limit_rect(QRect new_rect) { limit_rect_ = new_rect; }
    QRect get_limit_rect() const { return limit_rect_; }

    void incr_safety_count() { safety_count_++; }
    void incr_dist_z_value() { dist_z_value_++; }
    int get_dist_z_value() const { return dist_z_value_; }
    int get_dist_score() const { return dist_score_; }
    void incr_coupe_fourre_score() { coupe_fourre_score_ += COUPE_FOURRE; }
    int get_coupe_fourre_score() { return coupe_fourre_score_; }
    void incr_total_score(int addition) { total_score_ += addition; }
    int get_total_score() const { return total_score_; }

    void set_go(bool go) { go_ = go; }
    bool get_go() const { return go_; }
    void set_speed_limit(bool speed) { speed_limit_ = speed; }
    bool get_speed_limit() const { return speed_limit_; }
    void set_safe_trip(bool safe) { safe_trip_ = safe; }
    bool get_safe_trip() { return safe_trip_; }
    void set_ai(bool ai) { ai_ = ai; }
    bool get_ai() { return ai_; }

    QString get_name() const { return name_; }
    void set_dist_section(QPushButton* dist_button) { dist_section_ = dist_button; }
    QPushButton* get_dist_section() const { return dist_section_; }
    void set_safety_section(QPushButton* safety_button) { safety_section_ = safety_button; }
    QPushButton* get_safety_section() const { return safety_section_; }
    void set_dist_label(QLabel* dist_label) { dist_label_ = dist_label; }


private:
    // Private Methods
    void MakeEmptyCard(Card*);

    // Private Data Members
    std::vector<Card*> hand_;
    std::vector<Card*> distance_cards_;
    std::vector<SafetyCard*> safety_cards_;

    Card* go_card_;
    Card* action_card_;
    Card* limit_card_;
    Card* empty_card_;
    Card* spec_empty_card_;

    QRect action_rect_;
    QRect dist_rect_;
    QRect safety_rect_;
    QRect limit_rect_;

    int safety_count_;
    int dist_z_value_;
    int dist_score_;
    int coupe_fourre_score_;
    int total_score_;
    int max_;

    bool go_;
    bool speed_limit_;
    bool safe_trip_;
    bool ai_;

    QString name_;
    QPushButton* dist_section_;
    QPushButton* safety_section_;
    QLabel* dist_label_;

    std::map<hazards, remedies> haz_to_rem_;
    std::map<remedies, hazards> rem_to_haz_;
};

#endif // PLAYER_H
