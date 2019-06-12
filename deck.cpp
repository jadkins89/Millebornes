#include <algorithm>
#include <random>
#include <chrono>
#include "deck.h"
#include "definitions.h"

Deck::~Deck() {
    for (auto& card : draw_deck_) {
        delete card;
    }
    for (auto& card : discard_deck_) {
        delete card;
    }
}

/* DrawCard returns the last Card in the deck and pops it off. If the deck is empty
 * then nullptr is returned.
 * Return: Card*
*/
Card* Deck::DrawCard() {
    if (!draw_deck_.empty()) {
        Card* temp = draw_deck_.back();
        draw_deck_.pop_back();
        return temp;
    }
    return nullptr;
}

/* AddCard takes a pointer to a card object and adds it to the deck vector. */
void Deck::AddCard(Card* new_card) {
    draw_deck_.push_back(new_card);
}

/* AddEmptyCard takes a pointer to a card object and adds it to the empty_cards vector. */
void Deck::AddEmptyCard(Card * new_card) {
    empty_cards_.push_back(new_card);
}

/* DiscardCard takes a pointer to a card object and adds it to the discard_deck vector. */
void Deck::DiscardCard(Card *discard_card) {
    discard_deck_.push_back(discard_card);
}

/* Deal takes a vector of players and deals MAX_HAND_SIZE cards to each player. The last card dealt
 * is an empty card to allow each player to draw on the beginning of their turn.
 * Params: std::vector<Player*> players
*/
void Deck::Deal(std::vector<Player*> players) {
    for (int i = 0; i < MAX_HAND_SIZE - 1; i++) {
        for (int j = 0; j < static_cast<int>(players.size()); j++) {
            Card* temp = DrawCard();
            players[static_cast<unsigned long>(j)]->AddCard(temp, i);
        }
    }

    for (unsigned long i = 0; i < players.size(); i++) {
        players[i]->set_action_card(empty_cards_[i]);
    }
}

/* Shuffle is a simple function to shuffle the deck vector using a shuffle function */
void Deck::Shuffle() {
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(draw_deck_.begin(), draw_deck_.end(), std::default_random_engine(seed));
}
