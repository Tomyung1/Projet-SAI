#ifndef _OBJET_H_
#define _OBJET_H_

#include "matrice.h"

/* états possibles */
#define E_INITIAL 0
#define E_NORMAL 1
#define E_ACTION 2
#define E_FIN 3

typedef struct {
    matrice modele;
    int etat;
} objet;

matrice lire_modele(char * chemin);

#endif /* _OBJET_H_ */
