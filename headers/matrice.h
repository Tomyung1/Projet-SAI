#ifndef _MATRICE_H_
#define _MATRICE_H_

typedef struct {
    int n;
    int m;
    double** mat;
} matrice_s;

typedef matrice_s* matrice;


matrice creer_matrice(int n, int m);
void afficher_matrice(matrice m);
    
#endif /* _MATRICE_H_ */