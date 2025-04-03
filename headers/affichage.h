#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include "poisson.h"
#include "bateau.h"
#include "obstacle.h"

void init_affichage();
void affiche_cube(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3);
void affiche_ciel();
void afficher_bateau(bateau b);
void afficher_poisson(poisson p);
void afficher_obstacle(obstacle ob);
void affiche_eau();


#endif /* _AFFICHAGE_H_ */
