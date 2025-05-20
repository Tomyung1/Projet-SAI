#ifndef _MATRICE_H_
#define _MATRICE_H_

typedef struct {
    int n;  /* nombre de ligne */
    int m;  /* nombre de colonne */
    double** mat;
} matrice_s;

typedef matrice_s* matrice;


matrice creer_matrice(int n, int m);
matrice copier_matrice(matrice m);
matrice creer_identite(int n);
void liberer_matrice(matrice m);
    
void set_mat(matrice m, int l, int c, double v);
double get_mat(matrice m, int l, int c);
matrice mult_matrice(matrice m1, matrice m2);
void afficher_matrice(matrice m);

void translation(matrice* modele, double dx, double dy, double dz);
void rotation_x(matrice* modele, double theta);
void rotation_y(matrice* modele, double theta);
void rotation_z(matrice* modele, double theta);
/* vvvvvvvvvv NE MARCHE PAS vvvvvvvvvv */
void octant_negatif(matrice* modele, matrice* direction, matrice* hitbox, int* negatif_x, int* negatif_y, int* negatif_z);
void octant_negatif_inverse(matrice* modele, matrice* direction, matrice* hitbox, int negatif_x, int negatif_y, int negatif_z);
void alignement_x(matrice* modele, matrice* direction, matrice* hitbox, double *theta_x, double *theta_y, int* negatif_x, int* negatif_y, int* negatif_z);
void alignement_x_inverse(matrice* modele, matrice* direction, matrice* hitbox, double theta_x, double theta_y, int negatif_x, int negatif_y, int negatif_z);
/* ^^^^^^^^^^ NE MARCHE PAS ^^^^^^^^^^ */
void rotation_sur_place(matrice* modele, double theta, char axe);
void agrandissement(matrice* modele, double facteur_x, double facteur_y, double facteur_z);
void trans_rot_z_alea_tout(matrice* modele, matrice* direction, matrice* hitbox, double x_min, double x_max, double y_min, double y_max, double z_min, double z_max);
    
#endif /* _MATRICE_H_ */