# Simulateur de Station Spatiale 

Ce projet est une simulation orientée objet d'une station spatiale contenant des astronautes (S) et des réserves d'oxygène (O) .

## Comment compiler le projet

Ouvrez le terminal dans le dossier du projet et tapez la commande suivante pour compiler tous les fichiers .cpp :


g++ *.cpp -o simulation.exe


## Comment lancer la simulation

Une fois le projet compilé exécutez le programme en lui passant votre fichier de grille initiale en argument :

.\simulation.exe station_initialisation.txt


## Affichage et Résultats
- Mode console : L'évolution de la grille est affichée étape par étape dans le terminal.
- Mode fichier : À la fin de la simulation, la grille finale est automatiquement sauvegardée dans un fichier texte.(Ce fichier est automatiquement recréé et mis à jour à chaque nouvelle exécution du programme)
