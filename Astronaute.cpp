#include "Astronaute.h"
#include "Station.h"
#include "ReserveOxygene.h"
#include <cstdlib> // Pour utiliser rand() (aléatoire)

Astronaute::Astronaute(int posX, int posY) : Entite(posX, posY), oxygene(5), age(0) {}

char Astronaute::getSymbole() const {
    return 'S';
}

bool Astronaute::estMort() const {
    return oxygene <= 0;
}

void Astronaute::agir(Station& station) {
    // 1. Il perd 1 point d'oxygène et vieillit
    oxygene -= 1;
    age += 1;

    // S'il n'a plus d'oxygène, il meurt (la station s'occupera de le nettoyer)
    if (oxygene <= 0) {
        return; 
    }

    // 2. Déplacement aléatoire (Haut, Bas, Gauche, Droite)
    int direction = rand() % 4; // Génère un nombre entre 0 et 3
    int nouvX = x;
    int nouvY = y;

    if (direction == 0) nouvX -= 1; // Haut
    else if (direction == 1) nouvX += 1; // Bas
    else if (direction == 2) nouvY -= 1; // Gauche
    else if (direction == 3) nouvY += 1; // Droite

    // 3. Vérifier ce qu'il se passe sur la nouvelle case
    if (station.estDansGrille(nouvX, nouvY)) {
        Entite* cible = station.getEntite(nouvX, nouvY);

        if (cible == nullptr) {
            // Case vide : il se déplace simplement
            station.deplacerEntite(x, y, nouvX, nouvY);
            x = nouvX;
            y = nouvY;
        } 
        else if (cible->getSymbole() == 'O') {
            // Il y a une réserve d'oxygène : il la mange !
            oxygene += 3;
            station.supprimerEntite(nouvX, nouvY); // La réserve disparaît
            
            // Et il prend sa place
            station.deplacerEntite(x, y, nouvX, nouvY);
            x = nouvX;
            y = nouvY;
        }
        // S'il y a un autre astronaute sur la case, il ne bouge pas ce tour-ci (pour faire simple)
    }
}