#ifndef RESERVE_OXYGENE_H
#define RESERVE_OXYGENE_H
#include "Entite.h"

class ReserveOxygene : public Entite {
private:
    int concentration;
public:
    ReserveOxygene(int posX, int posY, int conc = 5);
    void agir(Station& station) override;
    char getSymbole() const override;
    int getConcentration() const;
    void consommer();
};

#endif
