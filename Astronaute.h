#ifndef ASTRONAUTE_H
#define ASTRONAUTE_H
#include "Entite.h"

class Astronaute : public Entite {
private:
    int oxygene, age;
public:
    Astronaute(int posX, int posY);
    void agir(Station& station) override;
    char getSymbole() const override;
    bool estMort() const;
};

#endif
