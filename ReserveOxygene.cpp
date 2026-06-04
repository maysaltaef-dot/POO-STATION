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
    // 1. Vérifier s'il y a un astronaute sur la case
    Entite* occupant = station.getEntite(x, y);
    if (occupant != nullptr && occupant->getSymbole() == 'S') {
        // Un astronaute est présent : la réserve disparaît (concentration = 0)
        concentration = 0;
        return;  // Ne pas régénérer ni se diffuser
    }

    // 2. Régénération : si concentration > 0 et < 5, augmente de 1
    if (concentration > 0 && concentration < 5) {
        concentration += 1;
    }

    // 3. Diffusion : si concentration = 5, crée une réserve sur case adjacente vide
    if (concentration == 5) {
        if (station.estDansGrille(x, y + 1) && station.getEntite(x, y + 1) == nullptr) {
            station.ajouterEntite(new ReserveOxygene(x, y + 1, 2));
        }
    }
}