#ifndef _BATEAU_H_
#define _BATEAU_H_

#include "objet.h"

/* MACRO etat */


typedef struct {
    objet o;
    /* plus ? */
} bateau;

bateau creer_bateau();
void liberer_bateau(bateau b);

#endif /* _BATEAU_H_ */