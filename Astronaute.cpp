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
    // À chaque tour, l'astronaute vieillit et consomme de l'oxygène
    oxygene -= 1;
    age += 1;

    // S'il n'a plus d'oxygène, il meurt (il sera effacé plus tard)
    if (oxygene <= 0) {
        return; 
    }

    // Choisit une direction au hasard (0: Haut, 1: Bas, 2: Gauche, 3: Droite)
    int direction = rand() % 4;
    int nouvX = x;
    int nouvY = y;

    if (direction == 0) nouvX -= 1; // Haut
    else if (direction == 1) nouvX += 1; // Bas
    else if (direction == 2) nouvY -= 1; // Gauche
    else if (direction == 3) nouvY += 1; // Droite

    // Regarde ce qu'il y a sur la case d'arrivée
    if (station.estDansGrille(nouvX, nouvY)) {
        Entite* cible = station.getEntite(nouvX, nouvY);

        if (cible == nullptr) {
            // Si la case est vide, il s'y déplace
            station.deplacerEntite(x, y, nouvX, nouvY);
            x = nouvX;
            y = nouvY;
        } 
        else if (cible->getSymbole() == 'O') {
            // S'il trouve de l'oxygène, il le consomme et prend sa place
            oxygene += 3;
            station.supprimerEntite(nouvX, nouvY);
            
            station.deplacerEntite(x, y, nouvX, nouvY);
            x = nouvX;
            y = nouvY;
        }
        // S'il y a déjà un autre astronaute, il ne bouge pas ce tour-ci
    }

    // L'astronaute se reproduit s'il est assez vieux et a assez d'oxygène
    if (age >= 3 && oxygene >= 6) {
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        
        // Cherche une case vide autour pour placer un nouvel astronaute
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (station.estDansGrille(nx, ny) && station.getEntite(nx, ny) == nullptr) {
                station.ajouterEntite(new Astronaute(nx, ny));
                oxygene -= 4; // L'effort lui coûte de l'oxygène
                break; // Un seul enfant par tour
            }
        }
    }
}