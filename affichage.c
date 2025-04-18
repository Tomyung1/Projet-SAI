#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/matrice.h"
#include "headers/poisson.h"
#include "headers/bateau.h"
#include "headers/obstacle.h"

extern poisson poissons[NB_POISSONS];
extern bateau bateaux[NB_BATEAUX];
extern obstacle obstacles[NB_OBSTACLES];

/* palette de couleur */
couleur blanc, gris_c, gris, poisson1, eau1, eau2, eau3, ciel1, ciel2, ciel3, bateau1, bateau2;

int (*regle_poisson)[3];   /* Pointeur de tableau de 3 entiers (indice */
int (*regle_bateau)[3];    /* de sommet) utiliser pour définir les     */
int (*regle_obstacle)[3];  /* facettes des modèle 3d                   */


static int (*charger_regles(char* fichier))[3]{
    FILE* f;
    int i;
    int n;
    double a1, a2, a3;
    int f1, f2, f3;
    int (*regles)[3];

    // Chargement des règles
    if ((f = fopen(fichier, "r")) == NULL){
        fprintf(stderr, "Erreur lecture modèle 3D de %s\n", fichier);
        exit(EXIT_FAILURE);
    }

    // dèplacement jusqu'au règle
    if (fscanf(f, "%d\n", &n) != 1){
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++){
        if (fscanf(f, "%lf,%lf,%lf\n", &a1, &a2, &a3) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    // lecture des règles
    if (fscanf(f, "%d\n", &n) != 1){  // nombre de facette
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        fclose(f);
        exit(EXIT_FAILURE);        
    }
    /* allocation */
    regles = malloc(n * sizeof(int[3]));
    for (i = 0; i < n; i++){
        if (fscanf(f, "%d,%d,%d\n", &f1, &f2, &f3) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            free(regles);
            fclose(f);
            exit(EXIT_FAILURE);
        }
        regles[i][0] = f1;
        regles[i][1] = f2;
        regles[i][2] = f3;
    }

    fclose(f);

    return regles;
}

/* initialise l'affichage, en particuler les valeurs pour les couleurs
   appeler 1 seul fois */
void init_affichage(){
    blanc.r  = 0.9;  blanc.g  = 0.9;  blanc.b  = 0.9;
    gris_c.r = 0.75; gris_c.g = 0.75; gris_c.b = 0.75;
    gris.r   = 0.6;  gris.g   = 0.6;  gris.b   = 0.6;
    
    poisson1.r = 0.01; poisson1.g = 0.02; poisson1.b = 0.37;
    eau1.r = 0.00; eau1.g = 0.47; eau1.b = 0.71;
    eau2.r = 0.00; eau2.g = 0.59; eau2.b = 0.78;
    eau3.r = 0.00; eau3.g = 0.71; eau3.b = 0.85;
    ciel1.r = 0.28; ciel1.g = 0.79; ciel1.b = 0.89;
    ciel2.r = 0.56; ciel2.g = 0.88; ciel2.b = 0.94;
    ciel3.r = 0.68; ciel3.g = 0.91; ciel3.b = 0.96;
    bateau1.r = 0.60; bateau1.g = 0.35; bateau1.b = 0.16;
    bateau2.r = 0.47; bateau2.g = 0.27; bateau2.b = 0.14;

    regle_poisson = charger_regles("objets/poisson.obj");
    regle_bateau = charger_regles("objets/bateau.obj");
    regle_obstacle = charger_regles("objets/obstacle.obj");
    
}

/* affiche un pavé droit, du point en bas à gauche devant au point en haut à droite derrière */
void affiche_cube(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3){

    glBegin(GL_QUADS);

    /* devant -> tour */
    glColor3f(c1.r, c1.g, c1.b);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);
    
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    glColor3f(c2.r, c2.g, c2.b);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y1, z1);

    glColor3f(c3.r, c3.g, c3.b);
    /* dessous */
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    /* dessus */
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);
    
    glEnd();
}

/* dessin d'une facette triangulaire avec les numéro des points dans une matrice */
void dessiner_facette_triangle(matrice m, int p1, int p2, int p3, couleur c){
    glBegin(GL_TRIANGLES);

    glColor3f(c.r, c.g, c.b);

    glVertex3f(get_mat(m, 0, p1), get_mat(m, 1, p1), get_mat(m, 2, p1));
    glVertex3f(get_mat(m, 0, p2), get_mat(m, 1, p2), get_mat(m, 2, p2));
    glVertex3f(get_mat(m, 0, p3), get_mat(m, 1, p3), get_mat(m, 2, p3));

    glEnd();
}

/* dessin d'une facette rectangulaire avec les numéro des points dans une matrice */
void dessiner_facette_carre(matrice m, int p1, int p2, int p3, int p4, couleur c){
    glBegin(GL_QUADS);
    
    glColor3f(c.r, c.g, c.b);

    glVertex3f(get_mat(m, 0, p1), get_mat(m, 1, p1), get_mat(m, 2, p1));
    glVertex3f(get_mat(m, 0, p2), get_mat(m, 1, p2), get_mat(m, 2, p2));
    glVertex3f(get_mat(m, 0, p3), get_mat(m, 1, p3), get_mat(m, 2, p3));
    glVertex3f(get_mat(m, 0, p4), get_mat(m, 1, p4), get_mat(m, 2, p4));

    glEnd();
}


/* affiche le ciel */
void affiche_ciel(){
    affiche_cube(LIMITE_MIN_X, LIMITE_MIN_Y, LIMITE_MIN_Z,
                 LIMITE_MAX_X, LIMITE_MAX_Y, LIMITE_MAX_Z,
                 ciel1, ciel2, ciel3);
}

/* règle de dessin */
void afficher_bateau(bateau b){
    int i;

    /* coque */
    for (i = 0; i < 6; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  bateau2);
    }
    
    for (i = 6; i < 8; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  bateau1);
    }
    
    /* cabine */
    for (i = 8; i < 18; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  gris_c);
    }
    
    /* canne */
    for (i = 18; i < 27; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  gris);
    }
}

/* règle de dessin */
void afficher_poisson(poisson p){
    int i;
    
    for (i = 0; i < 20; i++){
        dessiner_facette_triangle(p.o.modele,
                                  regle_poisson[i][0],
                                  regle_poisson[i][1],
                                  regle_poisson[i][2],
                                  poisson1);
    }
}

/* règle de dessin */
void afficher_obstacle(obstacle ob){
    int i;

    for (i = 0; i < 12; i++){
        dessiner_facette_triangle(ob.o.modele,
                                  regle_obstacle[i][0],
                                  regle_obstacle[i][1],
                                  regle_obstacle[i][2],
                                  gris);
    }
}


/* affiche l'eau */
void affiche_eau() {
    glBegin(GL_TRIANGLES);

    glColor4f(eau1.r, eau1.g, eau1.b, 0.5);
    glVertex3f(LIMITE_MIN_X, LIMITE_MAX_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MAX_X, LIMITE_MAX_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MAX_X, LIMITE_MIN_Y, NIVEAU_MER);
    
    glVertex3f(LIMITE_MAX_X, LIMITE_MIN_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MIN_X, LIMITE_MIN_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MIN_X, LIMITE_MAX_Y, NIVEAU_MER);
     
    glEnd();
}




void generer_monde(){
    int i;
    
    for (i = 0; i < NB_POISSONS; i++){
        poissons[i] = creer_poisson();
    }

    for (i = 0; i < NB_BATEAUX; i++){
        bateaux[i] = creer_bateau();
    }

    for (i = 0; i < NB_OBSTACLES; i++){
        obstacles[i] = creer_obstacle();
    }


    /* fonction... */
    trans_rot_z_alea(&poissons[0].o.modele, 5, 15, 5, 15, NIVEAU_MER - 1, NIVEAU_MER - 6);
    trans_rot_z_alea(&poissons[1].o.modele, -15, -5, 5, 15, NIVEAU_MER - 1, NIVEAU_MER - 6);
    trans_rot_z_alea(&poissons[2].o.modele, 5, 15, -15, -5, NIVEAU_MER - 1, NIVEAU_MER - 6);
    trans_rot_z_alea(&poissons[3].o.modele, -15, -5, -15, -5, NIVEAU_MER - 1, NIVEAU_MER - 6);

    trans_rot_z_alea_2(&bateaux[0].o.modele, &bateaux[0].direction, 5, 15, 5, 15, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea_2(&bateaux[1].o.modele, &bateaux[1].direction, -15, -5, 5, 15, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea_2(&bateaux[2].o.modele, &bateaux[2].direction, 5, 15, -15, -5, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea_2(&bateaux[3].o.modele, &bateaux[3].direction, -15, -5, -15, -5, NIVEAU_MER, NIVEAU_MER);

    translation(&obstacles[0].o.modele, 0, 0, NIVEAU_MER);
}