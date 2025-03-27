#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"

couleur blanc, gris_c, gris, poisson, eau1, eau2, eau3, ciel1, ciel2, ciel3, bateau1, bateau2;

void init_affichage(){
    blanc.r  = 0.9;  blanc.g  = 0.9;  blanc.b  = 0.9;
    gris_c.r = 0.75; gris_c.g = 0.75; gris_c.b = 0.75;
    gris.r   = 0.6;  gris.g   = 0.6;  gris.b   = 0.6;
    
    poisson.r = 0.01; poisson.g = 0.02; poisson.b = 0.37;
    eau1.r = 0.00; eau1.g = 0.47; eau1.b = 0.71;
    eau2.r = 0.00; eau2.g = 0.59; eau2.b = 0.78;
    eau3.r = 0.00; eau3.g = 0.71; eau3.b = 0.85;
    ciel1.r = 0.28; ciel1.g = 0.79; ciel1.b = 0.89;
    ciel2.r = 0.56; ciel2.g = 0.88; ciel2.b = 0.94;
    ciel3.r = 0.68; ciel3.g = 0.91; ciel3.b = 0.96;
    bateau1.r = 0.60; bateau1.g = 0.35; bateau1.b = 0.16;
    bateau2.r = 0.47; bateau2.g = 0.27; bateau2.b = 0.14;
}

/* x : largeur, y : profondeur, z : hauteur */
void affiche_pave(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3){

    glBegin(GL_QUADS);

    /* devant -> tour */
    glColor3f(c1.r, c1.g, c1.b);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    glColor3f(c2.r, c2.g, c2.b);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    glColor3f(c1.r, c1.g, c1.b);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    glColor3f(c2.r, c2.g, c2.b);
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

void affiche_ciel(){
    affiche_pave(-128, -128, -128, 128, 128, 128, ciel1, ciel2, ciel3);
}

void affiche_bateau(double x, double y, double z){
    double tx = 2,
           ty = 1,
           tz = 0.2,
           epaisseur = 0.1,
           pos_canne = 1.5,
           hauteur_canne = 1,
           prof_canne = -1;
    affiche_pave(x - tx, y - ty, z - tz, x + tx, y + ty, z + tz, bateau2, bateau2, bateau1);

    glBegin(GL_QUADS);

    glColor3f(gris.r, gris.g, gris.b);
    
    glVertex3f(x + pos_canne, y - epaisseur, z + tz);
    glVertex3f(x + pos_canne, y + epaisseur, z + tz);
    glVertex3f(x + pos_canne + 1, y + epaisseur, z + hauteur_canne);
    glVertex3f(x + pos_canne + 1, y - epaisseur, z + hauteur_canne);
    
    glVertex3f(x + pos_canne + 1.5, y - epaisseur, z + tz + prof_canne);
    glVertex3f(x + pos_canne + 1.5, y + epaisseur, z + tz + prof_canne);
    glVertex3f(x + pos_canne + 1, y + epaisseur, z + hauteur_canne);
    glVertex3f(x + pos_canne + 1, y - epaisseur, z + hauteur_canne);
    
    glEnd();
}

void affiche_poisson(double x, double y, double z){
    double tx = 0.5,
           ty = 0.2,
           tz = 0.2;
    affiche_pave(x - tx, y - ty, z - tz, x + tx, y + ty, z + tz, poisson, poisson, poisson);
}

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
