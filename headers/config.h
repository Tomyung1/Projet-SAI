#ifndef _CONFIG_H_
#define _CONFIG_H_


#define VISION_MAX 450  /* distance maximal dont le personne peut voir */

/* écran */
#define LARGUEUR 1600
#define HAUTEUR 900

/* limite du monde */
#define LIMITE_MIN_X -128
#define LIMITE_MIN_Y -128
#define LIMITE_MIN_Z -128
#define LIMITE_MAX_X 128
#define LIMITE_MAX_Y 128
#define LIMITE_MAX_Z 128

/* niveau de la mer (fixe, peut représenter la moyenne s'il y a des vagues) */
#define NIVEAU_MER -5

#define NB_POISSONS 4
#define NB_BATEAUX 4
#define NB_OBSTACLES 25   // /!\ NB_OBSTACLES = NB_OBST_PAR_LIGNE²

#define NB_OBST_PAR_LIGNE 5   // modification = make clean 

/* le faire avec opengl ? */
/* couleur normalisée red/green/blue */
typedef struct {
    double r;  
    double g;
    double b;
} couleur;

#endif /* _CONFIG_H_ */
