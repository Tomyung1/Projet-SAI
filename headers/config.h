#ifndef _CONFIG_H_
#define _CONFIG_H_


#define VISION_MAX 300  /* distance maximal dont le personne peut voir */

/* écran */
#define LARGUEUR 1600
#define HAUTEUR 900

/* niveau de la mer (fixe, peut représenter la moyenne s'il y a des vagues) */
#define NIVEAU_MER -5


/* le faire avec opengl ? */
/* couleur normalisée red/green/blue */
typedef struct {
    double r;  
    double g;
    double b;
} couleur;

#endif /* _CONFIG_H_ */
