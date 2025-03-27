#ifndef _MATRICE_H_
#define _MATRICE_H_

typedef struct {
    int n;  /* nombre de ligne */
    int m;  /* nombre de colonne */
    double** mat;
} matrice_s;

typedef matrice_s* matrice;


matrice creer_matrice(int n, int m);
void liberer_matrice(matrice m);
void set_mat(matrice m, int l, int c, double v);
double get_mat(matrice m, int l, int c);
matrice mult_matrice(matrice m1, matrice m2);
void afficher_matrice(matrice m);

void translation(matrice modele, double dx, double dy, double dz);

    
#endif /* _MATRICE_H_ */