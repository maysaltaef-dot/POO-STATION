#include "Entite.h"

Entite::Entite(int posX, int posY) : x(posX), y(posY) {}

int Entite::getX() const {
    return x;
}

int Entite::getY() const {
    return y;
}
