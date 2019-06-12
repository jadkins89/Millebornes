#include "cardimgfactory.h"

// Initialize all vectors to the necessary lengths
QPixmap* CardImgFactory::hazard_pool[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
QPixmap* CardImgFactory::remedy_pool[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
QPixmap* CardImgFactory::safety_pool[] = {nullptr, nullptr, nullptr, nullptr};
QPixmap* CardImgFactory::distance_pool[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
QPixmap* CardImgFactory::other_pool[] = {nullptr, nullptr};

/* get_image takes a remedy and returns a pointer to a Qpixmap object. If the remedy
 * desired has an associated QPixmap already, it is simply returned. Else, a new object is
 * created and saved in the remedy_pool vector.
 * Param cur_rem: remedies enum class type
 * Return: QPixmap*
*/
QPixmap* CardImgFactory::get_image(remedies cur_rem) {
    int i = static_cast<int>(cur_rem);
    if (!remedy_pool[i]) {
        QPixmap* rem_img = new QPixmap;
        switch (cur_rem) {
            case remedies::Go:
                *rem_img = QPixmap(":/resources/images/green_light.jpg");
                break;
            case remedies::Repairs:
                *rem_img = QPixmap(":/resources/images/repairs.jpg");
                break;
            case remedies::Gasoline:
                *rem_img = QPixmap(":/resources/images/gas.jpg");
                break;
            case remedies::SpareTire:
                *rem_img = QPixmap(":/resources/images/spare_tire.jpg");
                break;
            case remedies::EndOfLimit:
                *rem_img = QPixmap(":/resources/images/end_of_limit.jpg");
                break;
        }
        remedy_pool[i] = rem_img;
    }
    return remedy_pool[i];
}

/* get_image takes a hazard and returns a pointer to a Qpixmap object. If the hazard
 * desired has an associated QPixmap already, it is simply returned. Else, a new object is
 * created and saved in the hazard_pool vector.
 * Param cur_haz: hazards enum class type
 * Return: QPixmap*
*/
QPixmap* CardImgFactory::get_image(hazards cur_haz) {
    int i = static_cast<int>(cur_haz);
    if (!hazard_pool[i]) {
        QPixmap* haz_img = new QPixmap;
        switch (cur_haz) {
            case hazards::Stop:
                *haz_img = QPixmap(":/resources/images/red_light.jpg");
                break;
            case hazards::Accident:
                *haz_img = QPixmap(":/resources/images/accident.jpg");
                break;
            case hazards::FlatTire:
                *haz_img = QPixmap(":/resources/images/flat_tire.jpg");
                break;
            case hazards::OutOfGas:
                *haz_img = QPixmap(":/resources/images/out_of_gas.jpg");
                break;
            case hazards::SpeedLimit:
                *haz_img = QPixmap(":/resources/images/speed_limit.jpg");
                break;
        }
        hazard_pool[i] = haz_img;
    }
    return hazard_pool[i];
}

/* get_image takes a safety and returns a pointer to a Qpixmap object. If the safety
 * desired has an associated QPixmap already, it is simply returned. Else, a new object is
 * created and saved in the safety_pool vector.
 * Param cur_safety: safeties enum class type
 * Return: QPixmap*
*/
QPixmap* CardImgFactory::get_image(safeties cur_safety) {
    int i = static_cast<int>(cur_safety);
    if (!safety_pool[i]) {
        QPixmap* saf_img = new QPixmap;
        switch (cur_safety) {
            case safeties::FuelTank:
                *saf_img = QPixmap(":/resources/images/fuel_tank.jpg");
                break;
            case safeties::DrivingAce:
                *saf_img = QPixmap(":/resources/images/driving_ace.jpg");
                break;
            case safeties::PunctureProof:
                *saf_img = QPixmap(":/resources/images/puncture_proof.jpg");
                break;
            case safeties::EmergencyVehicle:
                *saf_img = QPixmap(":/resources/images/emergency_vehicle.jpg");
                break;
        }
        safety_pool[i] = saf_img;
    }
    return safety_pool[i];
}

/* get_image takes a distance and returns a pointer to a Qpixmap object. If the distance
 * desired has an associated QPixmap already, it is simply returned. Else, a new object is
 * created and saved in the distance_pool vector.
 * Param cur_dist: distance enum class type
 * Return: QPixmap*
*/
QPixmap* CardImgFactory::get_image(distances cur_dist) {
    int i = static_cast<int>(cur_dist);
    if (!distance_pool[i]) {
        QPixmap* dist_img = new QPixmap;
        switch (cur_dist) {
            case distances::Dist_1:
                *dist_img = QPixmap(":/resources/images/twenty_five.jpg");
                break;
            case distances::Dist_2:
                *dist_img = QPixmap(":/resources/images/fifty.jpg");
                break;
            case distances::Dist_3:
                *dist_img = QPixmap(":/resources/images/seventy_five.jpg");
                break;
            case distances::Dist_4:
                *dist_img = QPixmap(":/resources/images/one_hundred.jpg");
                break;
            case distances::Dist_5:
                *dist_img = QPixmap(":/resources/images/two_hundred.jpg");
                break;
        }
        distance_pool[i] = dist_img;
    }
    return distance_pool[i];
}

/* get_image takes two bools and returns a pointer to a Qpixmap object. If both are true,
 * nullptr is returned since this was not the desired input. If either are singularly true
 * and has an associated QPixmap already, it is simply returned. Else, a new object is
 * created and saved in the other_pool.
 * Param cur_rem: remedies enum class type
 * Return: QPixmap*
*/
QPixmap* CardImgFactory::get_image(bool empty, bool back) {
    if ( (empty && back) || (!empty && !back) ) {
        return nullptr;
    }

    // Setting appropriate index for desired object
    int i = -1;
    if (empty) {
        i = 0;
    }
    else {
        i = 1;
    }

    if (!other_pool[i]) {
        if (empty) {
            QPixmap* empty_img = new QPixmap(HAND_WIDTH, HAND_HEIGHT);
            empty_img->fill(Qt::white);
            QPainter painter( empty_img );
            painter.setFont( QFont("Arial") );
            painter.drawText( QPoint(8, static_cast<int>((empty_img->height() / 2.0) + 3)), "EMPTY" );

            other_pool[i] = empty_img;
        }
        else {
            QPixmap* back_img = new QPixmap(":/resources/images/card_back.png");
            other_pool[i] = back_img;
        }
    }
    return other_pool[i];
}
