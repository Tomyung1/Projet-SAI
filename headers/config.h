#ifndef _CONFIG_H_
#define _CONFIG_H_

/*  MODIFICATION ICI = make clean  */


#define VISION_MAX 1000  /* distance maximal dont le personne peut voir */

/* écran */
#define LARGUEUR 1600
#define HAUTEUR 900

/* limite du monde */
#define LIMITE_MIN_X -50
#define LIMITE_MIN_Y -50
#define LIMITE_MIN_Z -50
#define LIMITE_MAX_X 50
#define LIMITE_MAX_Y 50
#define LIMITE_MAX_Z 512

/* niveau de la mer (fixe, peut représenter la moyenne s'il y a des vagues) */
#define NIVEAU_MER -5

#define NB_POISSONS 100
#define NB_BATEAUX 20
#define NB_OBSTACLES 4    // /!\ NB_OBSTACLES = NB_OBST_PAR_LIGNE²

#define NB_OBST_PAR_LIGNE 2   // modification = make clean

/* calcul */
#define DIST_CALCUL_COLLISION_CARRE 400 // distance au carré entre 2 objet avant de calculer leur collision

/* Paramètres d'évitement pour les bateaux */
#define DISTANCE_DETECTION_BATEAU 20.0
#define MARGE_LIMITE_MONDE 10.0
#define DUREE_EVITEMENT_OBSTACLE 90
#define ANGLE_EVITEMENT (M_PI/3)  // 60 degrés
/* debug */
#define AFF_HITBOX 1 // si != 0, on affiche les hitbox
#define AFF_DIR 1    // si != 0, on affiche les directions
#define EVITEMENT 0  // si != 0, les bateaux évite les obstacles progressivement...
#define FIN_PARTIE 1 // si != 0, met fin brutalement à la partie en cas de collision avec un obstacle

/* distance de détection pour la fuite des poissons */
#define DISTANCE_FUITE_CARRE 64.0  // distance au carré de 8 unités

/* Mode de jeu */
#define MODE_JEU 1  // si != 0, alors on lance en mode jeu

/* Contrôles du jeu */
#define NB_TOUCHES 256  // Nombre de touches du clavier
#define TOUCHE_FLECHE_HAUT 101
#define TOUCHE_FLECHE_BAS 103
#define TOUCHE_FLECHE_GAUCHE 100
#define TOUCHE_FLECHE_DROITE 102
#define TOUCHE_ESPACE 32

/* Gameplay */
#define VITESSE_BATEAU_JOUEUR 0.01
#define VITESSE_ROTATION_BATEAU 0.003

/* le faire avec opengl ? */
/* couleur normalisée red/green/blue */
typedef struct {
    double r;  
    double g;
    double b;
} couleur;

#endif /* _CONFIG_H_ */
