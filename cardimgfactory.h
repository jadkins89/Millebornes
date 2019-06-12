#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include "cards.h"
#include "definitions.h"

/* CardImgFactory is a Flyweight Factory Class for creating and referencing QPixmap objects */
class CardImgFactory {
public:
    // overloaded public methods
    static QPixmap* get_image(remedies);
    static QPixmap* get_image(hazards);
    static QPixmap* get_image(safeties);
    static QPixmap* get_image(distances);
    static QPixmap* get_image(bool empty, bool back);
private:
    static QPixmap* remedy_pool[];
    static QPixmap* hazard_pool[];
    static QPixmap* safety_pool[];
    static QPixmap* distance_pool[];
    static QPixmap* other_pool[];
};

#endif // CARDFACTORY_H
