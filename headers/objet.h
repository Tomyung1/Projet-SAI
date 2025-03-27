#ifndef _OBJET_H_
#define _OBJET_H_

#include "matrice.h"

/* MACRO etat */
#define E_INITIAL 0

typedef struct {
    int etat;        /* état de l'objet (comportement) */
    matrice modele;  /* matrice 4 * nb_sommet */
} objet;

matrice lire_modele(char * chemin);

#endif /* _OBJET_H_ */