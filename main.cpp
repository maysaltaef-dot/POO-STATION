#include <ctime>
#include <iostream>
#include <string>
#include "Station.h"

int main(int argc, char* argv[]) {
    // Vérifie si le fichier texte a bien été fourni au lancement
    if (argc < 2) {
        std::cout << "Erreur : tu as oublié de mettre le fichier en argument." << std::endl;
        return 1; // Arrête le programme en cas d'erreur
    }

    srand(time(0)); // Initialise le générateur de nombres aléatoires

    // Récupère le nom du fichier passé dans la commande
    std::string nomFichier = argv[1];
    
    // Crée la station en lisant les données du fichier
    Station maStation(nomFichier);
    
    // Lance la simulation pour 10 tours
    maStation.simuler(10); 
    
    // Sauvegarde la grille finale dans un nouveau fichier texte
    maStation.sauvegarderFichier(nomFichier + "_result.txt");

    return 0;
}