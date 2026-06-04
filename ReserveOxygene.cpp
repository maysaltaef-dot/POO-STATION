#include "Station.h"
#include "ReserveOxygene.h"

ReserveOxygene::ReserveOxygene(int posX, int posY, int conc) : Entite(posX, posY), concentration(conc) {}

char ReserveOxygene::getSymbole() const {
    return 'O';
}

int ReserveOxygene::getConcentration() const {
    return concentration;
}

void ReserveOxygene::consommer() {
    if (concentration > 0) {
        concentration = 0;
    }
}

void ReserveOxygene::agir(Station& station) {
    // Si un astronaute est sur la case, la réserve est consommée
    Entite* occupant = station.getEntite(x, y);
    if (occupant != nullptr && occupant->getSymbole() == 'S') {
        concentration = 0;
        return;
    }

    // L'oxygène se régénère doucement tant qu'il n'est pas épuisé
    if (concentration > 0 && concentration < 5) {
        concentration += 1;
    }

    // Si la réserve est pleine (5), elle s'étend sur une case vide voisine
    if (concentration == 5) {
        if (station.estDansGrille(x, y + 1) && station.getEntite(x, y + 1) == nullptr) {
            station.ajouterEntite(new ReserveOxygene(x, y + 1, 2));
        }
    }
}