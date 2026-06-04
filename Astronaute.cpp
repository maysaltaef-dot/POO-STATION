#include "Station.h"
#include "Astronaute.h"
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

    // 4. Reproduction : si âge >= 3 et oxygène >= 6
    if (age >= 3 && oxygene >= 6) {
        // Cherche une case adjacente vide pour créer un nouvel astronaute
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (station.estDansGrille(nx, ny) && station.getEntite(nx, ny) == nullptr) {
                // Case vide trouvée : crée un nouvel astronaute
                station.ajouterEntite(new Astronaute(nx, ny));
                oxygene -= 4;  // Le parent perd 4 points d'oxygène
                break;  // Un seul enfant par itération
            }
        }
    }
}