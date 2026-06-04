# Simulateur de Station Spatiale 

Ce projet est une simulation orientée objet d'une station spatiale contenant des astronautes (S) et des réserves d'oxygène (O) .

## Comment compiler le projet

Ouvrez le terminal dans le dossier du projet et tapez la commande suivante pour compiler tous les fichiers .cpp :


g++ *.cpp -o simulation.exe


## Comment lancer la simulation

Une fois le projet compilé exécutez le programme  :

.\simulation.exe station_initialisation.txt


## Affichage et Résultats
- Mode console : L'évolution de la grille est affichée étape par étape dans le terminal.
- Mode fichier : À la fin de la simulation, la grille finale est automatiquement sauvegardée dans un fichier texte.(Ce fichier est automatiquement recréé et mis à jour à chaque nouvelle exécution du programme)

---

## Modélisation (UML)

### Diagramme de Classes

```mermaid
classDiagram
    class Entite {
        <<abstract>>
        #int x
        #int y
        +Entite(int posX, int posY)
        +virtual ~Entite()
        +virtual char getSymbole() const*
        +virtual void agir(Station& station)*
        +virtual bool estMort() const
        +int getX() const
        +int getY() const
    }
    
    class Astronaute {
        -int oxygene
        -int age
        +Astronaute(int posX, int posY)
        +char getSymbole() const
        +bool estMort() const
        +void agir(Station& station)
    }
    
    class ReserveOxygene {
        -int concentration
        +ReserveOxygene(int posX, int posY, int conc)
        +char getSymbole() const
        +int getConcentration() const
        +void consommer()
        +void agir(Station& station)
    }
    
    class Station {
        -int lignes
        -int colonnes
        -vector~vector~Entite*~~ grille
        -vector~Entite*~ aNettoyer
        +Station(string cheminFichier)
        +~Station()
        +void initialiserGrille(string cheminFichier)
        +void afficherConsole() const
        +void simuler(int nbIterations)
        +void sauvegarderFichier(string nomFichierResultat) const
        +bool estDansGrille(int x, int y) const
        +Entite* getEntite(int x, int y) const
        +void deplacerEntite(int ancienX, int ancienY, int nouvX, int nouvY)
        +void supprimerEntite(int x, int y)
        +void ajouterEntite(Entite* e)
    }

    Entite <|-- Astronaute
    Entite <|-- ReserveOxygene
    Station "1" o-- "*" Entite : contient
```

### Diagramme de Séquence (Une Itération de la Simulation)

```mermaid
sequenceDiagram
    participant Station
    participant Entite
    participant Astronaute

    Station->>Station: Récupération des entités présentes
    loop Pour chaque entité
        Station->>Entite: agir(station)
        alt Si c'est un Astronaute
            Entite->>Astronaute: agir()
            Astronaute->>Station: estDansGrille(nouvX, nouvY)
            Astronaute->>Station: getEntite(nouvX, nouvY)
            alt Case vide
                Astronaute->>Station: deplacerEntite()
            else Réserve d'oxygène trouvée
                Astronaute->>Station: supprimerEntite()
                Astronaute->>Station: deplacerEntite()
            end
        end
        Station->>Entite: estMort()
        opt Si l'entité n'a plus d'oxygène
            Station->>Station: supprimerEntite() (ajout à aNettoyer)
        end
    end
    Station->>Station: Libération mémoire (delete aNettoyer)
    Station->>Station: afficherConsole()
```

## Rapport Court

**1. Choix de conception :**
- **Polymorphisme :** Utilisation d'une classe abstraite `Entite` regroupant les propriétés communes (x , y) et une méthode agir(). Cela permet à la station de gérer n'importe quel type d'objet via un  std::vector<std::vector<Entite*>>.
- **Nettoyage différé :** Plutôt que de détruire les entités immédiatement pendant la boucle elles sont placées dans un vecteur aNettoyer puis détruites à la fin du tour pour éviter les erreurs de modification en pleine itération.

**2. Difficultés rencontrées :**
La principale difficulté a été de bien gérer la mémoire et les cases vides pour éviter que le programme ne plante. Il fallait toujours s'assurer qu'on ne faisait pas interagir un personnage qui venait juste de disparaître. Un autre problème était lié aux déplacements : au début, un astronaute qui descendait dans la grille pouvait parfois jouer une deuxième fois de suite dans le même tour. Pour régler ça, j'ai choisi de faire une liste de tout le monde au tout début du tour, pour m'assurer que chaque élément ne bouge qu'une seule fois.
