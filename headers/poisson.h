#ifndef _POISSON_H_
#define _POISSON_H_

#include "objet.h"

/* MACRO etat */


typedef struct {
    objet o;
    /* plus ? */
} poisson;

poisson creer_poisson();
void liberer_poisson(poisson p);

#endif /* _POISSON_H_ */