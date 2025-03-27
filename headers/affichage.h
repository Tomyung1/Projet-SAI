#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

/* x : largeur, y : profondeur, z : hauteur */
void init_affichage();
void affiche_cube(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3);
void affiche_ciel();
void affiche_bateau(double x, double y, double z);
void affiche_poisson(double x, double y, double z);
void affiche_eau();


#endif /* _AFFICHAGE_H_ */
