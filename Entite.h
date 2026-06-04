#ifndef ENTITE_H
#define ENTITE_H

class Station;

class Entite {
protected:
    int x, y;
public:
    Entite(int posX, int posY);
    virtual ~Entite() = default;
    virtual void agir(Station& station) = 0;
    virtual char getSymbole() const = 0;
    
    // Par défaut, une entité n'est pas morte
    virtual bool estMort() const { return false; }
    
    int getX() const;
    int getY() const;
};

#include "Station.h"
#endif
