#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/matrice.h"
#include "headers/poisson.h"
#include "headers/bateau.h"

/* palette de couleur */
couleur blanc, gris_c, gris, poisson1, eau1, eau2, eau3, ciel1, ciel2, ciel3, bateau1, bateau2;

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
}

/* affiche un pavé droit, du point en bas à gauche devant au point en haut à droite derrière */
void affiche_pave(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3){

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
    affiche_pave(-128, -128, -128, 128, 128, 128, ciel1, ciel2, ciel3);
}

/* règle de dessin */
void afficher_bateau(bateau b){

    /* bateau */
    /* coque */
    dessiner_facette_carre(b.o.modele, 0, 1, 2, 3, bateau2);
    dessiner_facette_triangle(b.o.modele, 1, 5, 2, bateau2);
    dessiner_facette_carre(b.o.modele, 5, 4, 3, 2, bateau2);
    dessiner_facette_triangle(b.o.modele, 0, 3, 4, bateau2);
    dessiner_facette_carre(b.o.modele, 4, 5, 1, 0, bateau1);
    
    /* cabine */
    dessiner_facette_carre(b.o.modele, 6, 7, 8, 9, gris_c);
    dessiner_facette_carre(b.o.modele, 7, 11, 12, 8, gris_c);
    dessiner_facette_carre(b.o.modele, 10, 11, 12, 13, gris_c);
    dessiner_facette_carre(b.o.modele, 10, 6, 9, 13, gris_c);
    dessiner_facette_carre(b.o.modele, 10, 11, 7, 6, gris_c);

    /* canne */
    dessiner_facette_carre(b.o.modele, 14, 16, 18, 19, gris);
    dessiner_facette_carre(b.o.modele, 16, 15, 17, 18, gris);
    dessiner_facette_carre(b.o.modele, 14, 15, 17, 19, gris);
    dessiner_facette_triangle(b.o.modele, 17, 18, 20, gris);
    dessiner_facette_triangle(b.o.modele, 18, 19, 20, gris);
    dessiner_facette_triangle(b.o.modele, 19, 17, 20, gris);
    
    /* /\* radeau *\/ */
    /* /\* bord *\/ */
    /* dessiner_facette_carre(b.o.modele, 0, 1, 2, 3, bateau1); */
    /* dessiner_facette_carre(b.o.modele, 1, 5, 6, 2, bateau1); */
    /* dessiner_facette_carre(b.o.modele, 4, 5, 6, 7, bateau1); */
    /* dessiner_facette_carre(b.o.modele, 4, 0, 3, 7, bateau1); */
    /* /\* haut / bas *\/ */
    /* dessiner_facette_carre(b.o.modele, 0, 1, 5, 4, bateau2); */
    /* dessiner_facette_carre(b.o.modele, 7, 6, 2, 3, bateau2); */
    
    /* /\* canne *\/ */
    /* dessiner_facette_carre(b.o.modele, 8, 9, 10, 11, gris); */
    /* dessiner_facette_carre(b.o.modele, 10, 11, 12, 13, gris); */
    
}

/* règle de dessin */
void afficher_poisson(poisson p){
    /* corp */
    dessiner_facette_carre(p.o.modele, 0, 1, 2, 3, poisson1);
    dessiner_facette_carre(p.o.modele, 1, 5, 6, 2, poisson1);
    dessiner_facette_carre(p.o.modele, 5, 4, 2, 6, poisson1);
    dessiner_facette_carre(p.o.modele, 4, 5, 1, 0, poisson1);
    dessiner_facette_carre(p.o.modele, 7, 6, 2, 3, poisson1);

    /* nez */
    dessiner_facette_triangle(p.o.modele, 0, 8, 3, poisson1);
    dessiner_facette_triangle(p.o.modele, 3, 8, 7, poisson1);
    dessiner_facette_triangle(p.o.modele, 7, 8, 4, poisson1);
    dessiner_facette_triangle(p.o.modele, 4, 8, 0, poisson1);


    /* queue */
    dessiner_facette_triangle(p.o.modele, 10, 9, 11, poisson1);
    dessiner_facette_triangle(p.o.modele, 11, 9, 12, poisson1);
    dessiner_facette_triangle(p.o.modele, 12, 9, 13, poisson1);
    dessiner_facette_triangle(p.o.modele, 13, 9, 10, poisson1);
    dessiner_facette_carre(p.o.modele, 10, 11, 12, 13, poisson1);
}

/* affiche l'eau */
void affiche_eau() {
    glBegin(GL_TRIANGLES);

    glColor4f(eau1.r, eau1.g, eau1.b, 0.5);
    glVertex3f(-128, 128, NIVEAU_MER);
    glVertex3f(-128, -128, NIVEAU_MER);
    glVertex3f(128, -128, NIVEAU_MER);
    
    glVertex3f(-128, 128, NIVEAU_MER);
    glVertex3f(128, 128, NIVEAU_MER);
    glVertex3f(128, -128, NIVEAU_MER);
        
    glEnd();
}
