#ifndef CARDS_H
#define CARDS_H

#include <QColor>
#include <QGraphicsItem>
#include <QPixmap>
#include <QtWidgets>
#include <QDebug>

enum class cardType {
    Default, Hazard, Remedy, Safety, Distance
};

enum class hazards {
    Accident, OutOfGas, FlatTire, Stop, SpeedLimit
};

enum class remedies {
    Repairs, Gasoline, SpareTire, Go, EndOfLimit
};

enum class safeties {
    DrivingAce, FuelTank, PunctureProof, EmergencyVehicle
};

enum class distances {
    Dist_1, Dist_2, Dist_3, Dist_4, Dist_5
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Card: public QGraphicsObject {
    Q_OBJECT
public:
    Card();

    // QGraphicsItem overidden functions
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    // Setters & Getters
    bool get_clicked() const { return clicked_; }
    void set_clicked(bool choice) { clicked_ = choice; }
    void set_rect(QRect rect);
    QRect get_rect() const { return rect_; }
    void set_img(QPixmap* img) { img_ = img; }
    QPixmap* get_img() const { return img_; }
    void set_border_color(QColor new_color);
    QColor get_border_color() const { return card_border_; }
    cardType get_type() const { return type_; }

signals:
    void cardClicked(Card* c);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
    void StandardPaint(QPainter *painter, const QStyleOptionGraphicsItem *item, int width, int height);

    bool clicked_;
    QRect rect_;
    QPixmap* img_;
    QColor card_border_;
    cardType type_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemedyCard: public Card {
    Q_OBJECT
public:
    RemedyCard(remedies);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);

    remedies get_rem() { return rem_; }

private:
    remedies rem_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HazardCard: public Card {
    Q_OBJECT
public:
    HazardCard(hazards);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);

    hazards get_haz() { return haz_; }

private:
    hazards haz_;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SafetyCard: public Card {
    Q_OBJECT
public:
    // Constructor
    SafetyCard(safeties);

    // QGraphicsItem overidden functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    // Card overridden method
    void mousePressEvent(QGraphicsSceneMouseEvent* e);

    // Setters & Getters
    safeties get_safety() { return safety_; }
    void set_coup_fourre(bool res) { coup_fourre_ = res; }
    bool get_coup_fourre() const { return coup_fourre_; }
    void set_play(bool res) { play_ = res; }
    bool get_play() const { return play_; }

private:
    safeties safety_;
    bool coup_fourre_;
    bool play_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DistanceCard: public Card {
    Q_OBJECT
public:
    DistanceCard(distances);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);

    distances get_dist() { return dist_; }
    int get_value() { return value_; }

private:
    distances dist_;
    int value_;
};


#endif // CARDS_H
