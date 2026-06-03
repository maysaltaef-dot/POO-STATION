#include <iostream>
#include <string>
#include "Station.h"

int main(int argc, char* argv[]) {
    // 1. Sécurité : on vérifie que tu as bien donné le fichier au lancement
    if (argc < 2) {
        std::cout << "Erreur : tu as oublié de mettre le fichier en argument." << std::endl;
        return 1; // On arrête le programme
    }

    // 2. On récupère le nom du fichier (qui est stocké dans argv[1])
    std::string nomFichier = argv[1];
    
    // 3. On crée la station à partir de ce fichier
    Station maStation(nomFichier);
    
    // 4. On simule 10 itérations (comme demandé dans le sujet)
    maStation.simuler(10); 
    
    // 5. On sauvegarde l'état final dans un nouveau fichier texte
    maStation.sauvegarderFichier(nomFichier + ".result.txt");

    return 0; // Fin du programme
}