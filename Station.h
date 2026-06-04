#ifndef STATION_H
#define STATION_H

#include <vector>
#include <string>
#include "Entite.h"

class Station {
private:
    std::vector<std::vector<Entite*>> grille;
    int lignes, colonnes;
    std::vector<Entite*> aNettoyer;
public:
    Station(const std::string& cheminFichier);
    ~Station();
    void initialiserGrille(const std::string& cheminFichier);
    void simuler(int nbIterations);
    void afficherConsole() const;
    void sauvegarderFichier(const std::string& nomFichierResultat) const;
    bool estDansGrille(int x, int y) const;
    Entite* getEntite(int x, int y) const;
    void deplacerEntite(int ancienX, int ancienY, int nouvX, int nouvY);
    void supprimerEntite(int x, int y);
    void ajouterEntite(Entite* e);
};

#endif
