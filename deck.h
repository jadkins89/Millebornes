#ifndef DECK_H
#define DECK_H

#include "cards.h"
#include "player.h"

class Deck {
public:
    // Public Methods
    ~Deck();
    Card* DrawCard();
    void AddCard(Card*);
    void AddEmptyCard(Card*);
    void DiscardCard(Card*);
    void Deal(std::vector<Player*>);
    void Shuffle();

    // Setters & Getters
    std::vector<Card*> get_draw_deck() { return draw_deck_; }
    Card* get_empty_action_card(unsigned long index) { return empty_cards_[index]; }

private:
    // Private Data Members
    std::vector<Card*> draw_deck_;
    std::vector<Card*> discard_deck_;
    std::vector<Card*> empty_cards_;
};

#endif // DECK_H
