typedef struct t_salle {
    int ID;                  // ID/Numéro de la salle
    struct t_salle *droite;  // Porte droite
    struct t_salle *gauche;  // Porte gauche
    struct t_salle *haut;    // Porte haut
    struct t_salle *bas;     // Porte bas
} t_salle;

t_salle *initRoom(); // Fonction pour initialiser une salle
int genMap(int stage); // Fonction pour générer la carte (les salles) d'un étage (stage)