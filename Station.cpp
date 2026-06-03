#include "Station.h"
#include "Astronaute.h"
#include "ReserveOxygene.h"
#include <iostream>
#include <fstream>

// Constructeur
Station::Station(const std::string& cheminFichier) : lignes(0), colonnes(0) {
    initialiserGrille(cheminFichier);
}

// Destructeur pour libérer la mémoire proprement
Station::~Station() {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            delete grille[i][j];
        }
    }
}

// Lecture du fichier pour initialiser la grille
void Station::initialiserGrille(const std::string& cheminFichier) {
    std::ifstream fichier(cheminFichier);
    if (!fichier) {
        std::cerr << "Erreur : fichier invalide ou introuvable." << std::endl;
        exit(1); // Arrêt si fichier invalide
    }

    fichier >> lignes >> colonnes;
    if (lignes <= 0 || colonnes <= 0) {
        std::cerr << "Erreur : grille vide ou dimensions invalides." << std::endl;
        exit(1);
    }

    // On redimensionne le vecteur 2D et on le remplit de "nullptr" (cases vides)
    grille.resize(lignes, std::vector<Entite*>(colonnes, nullptr));

    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            char symbole;
            fichier >> symbole;
            if (symbole == 'S') {
                grille[i][j] = new Astronaute(i, j); // astronaute
            } else if (symbole == 'O') {
                grille[i][j] = new ReserveOxygene(i, j); // réserve d'oxygène
            }
            // Si c'est '0', on laisse nullptr (module vide)
        }
    }
}

// Mode console : affichage de la grille à chaque itération
void Station::afficherConsole() const {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            if (grille[i][j] != nullptr) {
                std::cout << grille[i][j]->getSymbole() << " ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}

// Boucle principale de la simulation
void Station::simuler(int nbIterations) {
    std::cout << "ETAT INITIAL" << std::endl;
    afficherConsole();

    for (int tour = 1; tour <= nbIterations; ++tour) {
        int nbAstronautesEnVie = 0;

        // 1. On parcourt toute la grille case par case
        for (int i = 0; i < lignes; ++i) {
            for (int j = 0; j < colonnes; ++j) {
                
                // Si la case n'est pas vide
                if (grille[i][j] != nullptr) {
                    
                    // L'entité fait son action
                    grille[i][j]->agir(*this); 

                    // Si l'entité est un astronaute et qu'il n'a plus d'oxygène, on le supprime 
                    if (grille[i][j]->getSymbole() == 'S') {
                        Astronaute* astro = dynamic_cast<Astronaute*>(grille[i][j]);
                        if (astro != nullptr && astro->estMort()) {
                            supprimerEntite(i, j);
                            continue; // On passe à la case suivante
                        }
                    
                    // On compte les astronautes pour la condition d'arrêt
                    if (grille[i][j]->getSymbole() == 'S') {
                        nbAstronautesEnVie++;
                    }
                }
            }
        }
        
        std::cout << "ITERATION " << tour << std::endl;
        afficherConsole();

        // 2. Condition d'arrêt : si tout le monde est mort
        if (nbAstronautesEnVie == 0) {
            std::cout << "Tous les astronautes sont morts. Arret de la simulation." << std::endl;
            break; 
        }
    }
}

// Mode fichier : écriture de l'état final
void Station::sauvegarderFichier(const std::string& nomFichierResultat) const {
    std::ofstream fichier(nomFichierResultat);
    if (!fichier) {
        std::cerr << "Erreur d'ecriture du fichier de sortie." << std::endl;
        return;
    }
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            if (grille[i][j] != nullptr) {
                fichier << grille[i][j]->getSymbole() << " ";
            } else {
                fichier << "0 ";
            }
        }
        fichier << std::endl;
    }
}
// Vérifie si les coordonnées ne sortent pas de la grille
bool Station::estDansGrille(int x, int y) const {
    return (x >= 0 && x < lignes && y >= 0 && y < colonnes);
}

// Renvoie l'entité sur une case (ou nullptr si c'est vide)
Entite* Station::getEntite(int x, int y) const {
    if (estDansGrille(x, y)) {
        return grille[x][y];
    }
    return nullptr;
}

// Déplace une entité d'une case à une autre
void Station::deplacerEntite(int ancienX, int ancienY, int nouvX, int nouvY) {
    if (estDansGrille(ancienX, ancienY) && estDansGrille(nouvX, nouvY)) {
        grille[nouvX][nouvY] = grille[ancienX][ancienY];
        grille[ancienX][ancienY] = nullptr;
    }
}

// Supprime définitivement une entité de la mémoire
void Station::supprimerEntite(int x, int y) {
    if (estDansGrille(x, y) && grille[x][y] != nullptr) {
        delete grille[x][y];
        grille[x][y] = nullptr;
    }
}

// Ajoute une nouvelle entité sur la grille
void Station::ajouterEntite(Entite* e) {
    if (e != nullptr && estDansGrille(e->getX(), e->getY())) {
        grille[e->getX()][e->getY()] = e;
    }
}