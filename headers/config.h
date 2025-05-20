#ifndef _CONFIG_H_
#define _CONFIG_H_


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
#define NB_BATEAUX 10
#define NB_OBSTACLES 4    // /!\ NB_OBSTACLES = NB_OBST_PAR_LIGNE²

#define NB_OBST_PAR_LIGNE 2   // modification = make clean 

/* le faire avec opengl ? */
/* couleur normalisée red/green/blue */
typedef struct {
    double r;  
    double g;
    double b;
} couleur;

#endif /* _CONFIG_H_ */
