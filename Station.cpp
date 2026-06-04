#include "Astronaute.h"
#include "ReserveOxygene.h"
#include "Station.h"
#include <iostream>
#include <fstream>

// Constructeur
Station::Station(const std::string& cheminFichier) : lignes(0), colonnes(0) {
    initialiserGrille(cheminFichier);
}

// Détruit la station et vide la mémoire de toutes les cases
Station::~Station() {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            delete grille[i][j];
        }
    }
}

// Lit le fichier texte pour créer la grille de départ
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

    // Prépare la grille avec la bonne taille, remplie de cases vides (nullptr)
    grille.resize(lignes, std::vector<Entite*>(colonnes, nullptr));

    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            char symbole;
            fichier >> symbole;
            if (symbole == 'S') {
                grille[i][j] = new Astronaute(i, j);
            } else if (symbole == 'O') {
                grille[i][j] = new ReserveOxygene(i, j);
            }
        }
    }
}

// Affiche la grille actuelle dans le terminal
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

// Gère l'évolution de la station sur plusieurs tours
void Station::simuler(int nbIterations) {
    std::cout << "ETAT INITIAL" << std::endl;
    afficherConsole();

    for (int tour = 1; tour <= nbIterations; ++tour) {
        int nbAstronautesEnVie = 0;

        // Fait la liste de tous les éléments avant de commencer le tour
        std::vector<Entite*> entites;
        for (int i = 0; i < lignes; ++i) {
            for (int j = 0; j < colonnes; ++j) {
                if (grille[i][j] != nullptr) {
                    entites.push_back(grille[i][j]);
                }
            }
        }

        // Fait agir chaque élément un par un
        for (Entite* entite : entites) {
            if (entite == nullptr) {
                continue;
            }

            // Vérifie si l'élément n'a pas été supprimé ou mangé pendant ce tour
            bool estPresent = false;
            for (int i = 0; i < lignes; ++i) {
                for (int j = 0; j < colonnes; ++j) {
                    if (grille[i][j] == entite) {
                        estPresent = true;
                        break;
                    }
                }
                if (estPresent) break;
            }
            if (!estPresent) {
                continue;
            }

            entite->agir(*this);

            if (entite->estMort()) {
                supprimerEntite(entite->getX(), entite->getY());
            }
        }

        // Supprime définitivement les éléments morts de la mémoire
        for (Entite* e : aNettoyer) {
            delete e;
        }
        aNettoyer.clear();

        // Vérifie combien d'astronautes sont encore en vie
        for (int i = 0; i < lignes; ++i) {
            for (int j = 0; j < colonnes; ++j) {
                if (grille[i][j] != nullptr && grille[i][j]->getSymbole() == 'S') {
                    nbAstronautesEnVie++;
                }
            }
        }

        std::cout << "ITERATION " << tour << std::endl;
        afficherConsole();

        if (nbAstronautesEnVie == 0) {
            std::cout << "Tous les astronautes sont morts. Arret de la simulation." << std::endl;
            break;
        }
    }
}

// Sauvegarde la grille actuelle dans un fichier texte
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
// Vérifie si les coordonnées demandées sont bien dans la grille
bool Station::estDansGrille(int x, int y) const {
    return (x >= 0 && x < lignes && y >= 0 && y < colonnes);
}

// Donne l'élément qui se trouve sur la case (ou rien si elle est vide)
Entite* Station::getEntite(int x, int y) const {
    if (estDansGrille(x, y)) {
        return grille[x][y];
    }
    return nullptr;
}

// Déplace un élément d'une case vers une autre
void Station::deplacerEntite(int ancienX, int ancienY, int nouvX, int nouvY) {
    if (estDansGrille(ancienX, ancienY) && estDansGrille(nouvX, nouvY)
        && grille[ancienX][ancienY] != nullptr && grille[nouvX][nouvY] == nullptr) {
        grille[nouvX][nouvY] = grille[ancienX][ancienY];
        grille[ancienX][ancienY] = nullptr;
    }
}

// Retire un élément de la grille et prépare sa destruction
void Station::supprimerEntite(int x, int y) {
    if (estDansGrille(x, y) && grille[x][y] != nullptr) {
        aNettoyer.push_back(grille[x][y]);
        grille[x][y] = nullptr;
    }
}

// Place un nouvel élément dans la grille
void Station::ajouterEntite(Entite* e) {
    if (e != nullptr && estDansGrille(e->getX(), e->getY())) {
        grille[e->getX()][e->getY()] = e;
    }
}