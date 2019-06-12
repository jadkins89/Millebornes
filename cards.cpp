#include "cards.h"
#include "cardimgfactory.h"
#include "definitions.h"

// Base Class ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
Card::Card() :
    clicked_(false),
    rect_(0,0,0,0),
    img_(new QPixmap),
    card_border_(0,0,0),
    type_(cardType::Default) {}

// Overridden methods from inherited QGraphicsObject /////////////////////////////////////////
QRectF Card::boundingRect() const {
    return QRectF(rect_);
}

QPainterPath Card::shape() const {
    QPainterPath path;

    QRectF gameRec(rect_);
    path.addRect(gameRec);
    return path;
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    StandardPaint(painter, option, HAND_WIDTH, HAND_HEIGHT);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsItem::mousePressEvent(e);
    emit cardClicked(this);
    qDebug() << "default";
}
/////////////////////////////////////////////////////////////////////////////////////////////////

/* set_rect is used to change the position and potentially the size of the Card object.
 * prepareGeometryChange() prepares the scene for this change and calls update if necessary.
 * Param rect: QRect object
*/
void Card::set_rect(QRect rect) {
    prepareGeometryChange();
    rect_.setRect(rect.x(), rect.y(), rect.width(), rect.height());
}

/* set_border_color is a setter for the border color of the Card */
void Card::set_border_color(QColor new_color) {
    card_border_ = new_color;
    this->update();
}

/* StandardPaint is a method that is called when a typical Card is being painted. Using the parameter QPainter,
 * the method adds an image in the form of a QPixmap object and a border around this image. Several render hints are
 * set to ensure the best possible image and border.
*/
void Card::StandardPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, int width, int height) {
    *img_ = img_->scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QRect border_rect(rect_.x() - 1, rect_.y() - 1, rect_.width() + 2, rect_.height() + 2);

    if (option->state) {
        QPen rect_pen = QPen(option->palette.windowText(), 1.3, Qt::SolidLine);
        rect_pen.setColor(card_border_);
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(rect_, *img_);
        painter->setPen(rect_pen);
        painter->setRenderHint(QPainter::HighQualityAntialiasing);
        painter->drawRoundedRect(border_rect, 3, 3);
    }
}

// Remedy Subclass ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

RemedyCard::RemedyCard(remedies rem_card) :
    rem_(rem_card) {
    type_ = cardType::Remedy;
    rect_ = QRect(0,0,0,0);
    img_ = CardImgFactory::get_image(rem_card);
}

void RemedyCard::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsItem::mousePressEvent(e);
    emit cardClicked(this);
    qDebug() << "remedy";
}

// Hazard Subclass///////////////////////////////////////////////////////////////////////////////////////////////////////////////

HazardCard::HazardCard(hazards haz_card) :
    haz_(haz_card) {
    type_ = cardType::Hazard;
    rect_ = QRect(0,0,0,0);
    img_ = CardImgFactory::get_image(haz_card);
}

void HazardCard::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsItem::mousePressEvent(e);
    emit cardClicked(this);
    qDebug() << "hazard";
}

// Safety Subclass///////////////////////////////////////////////////////////////////////////////////////////////////////////////

SafetyCard::SafetyCard(safeties safe_card) :
safety_(safe_card),
coup_fourre_(false),
play_(false) {
    type_ = cardType::Safety;
    rect_ = QRect(0,0,0,0);
    img_ = CardImgFactory::get_image(safe_card);
}

/* SafetyCard has a special paint method that displays the card differently based on some current data members
 * attributes. If coupe_fourre_ is true the image is displayed on its side and small. If play_ it is just displayed
 * small, finally it is displayed traditionally if neither of these are true.
*/
void SafetyCard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);

    if (coup_fourre_) {
        *img_ = img_->scaled(SAFETY_WIDTH, SAFETY_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QRect border_rect(rect_.x() - 1, rect_.y() - 1, rect_.width() + 2, rect_.height() + 2);

        if (option->state) {
            QPen rect_pen = QPen(option->palette.windowText(), 1.3, Qt::SolidLine);
            rect_pen.setColor(card_border_);
            painter->setRenderHint(QPainter::SmoothPixmapTransform);
            painter->translate(rect_.x() + img_->size().width()/2, rect_.y() + img_->size().height()/2);
            painter->rotate(-90);
            painter->translate(-rect_.x() -img_->size().width()/2, -rect_.y() -img_->size().height()/2);

            painter->drawPixmap(rect_, *img_);
            painter->setPen(rect_pen);
            painter->setRenderHint(QPainter::HighQualityAntialiasing);
            painter->drawRoundedRect(border_rect, 3, 3);
        }
    }
    else if (play_) {
        StandardPaint(painter, option, SAFETY_WIDTH, SAFETY_HEIGHT);
    }
    else {
        Card::paint(painter, option, widget);
    }
}

void SafetyCard::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsItem::mousePressEvent(e);
    emit cardClicked(this);
    qDebug() << "safety";
}

// Distance Subclass/////////////////////////////////////////////////////////////////////////////////////////////////////////////

DistanceCard::DistanceCard(distances dist_card) :
    dist_(dist_card) {
    type_ = cardType::Distance;
    rect_ = QRect(0,0,0,0);
    img_ = CardImgFactory::get_image(dist_card);

    int poss_dist[] = { DIST_1, DIST_2, DIST_3, DIST_4, DIST_5 };

    value_ = poss_dist[static_cast<int>(dist_card)];
}

void DistanceCard::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QGraphicsItem::mousePressEvent(e);
    emit cardClicked(this);
    qDebug() << "distance";
}









