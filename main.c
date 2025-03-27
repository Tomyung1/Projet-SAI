#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/action.h"



/********************************************************
 *                Organisation                          *
 *                                                      *
 * - définir un repère pour le joueur                   *
 *   -> rotation / déplacement du repère, camera dessus *
 *      déplacement de la souris pour tourné            *
 * - définir des structure pour les objets              *
 * - TAD matrice pour les transformations des objets    *
 *                                                      *
 ********************************************************/



int p_x, p_y, p_z; /* position de l'observateur */

int test = 0; /* quel vu choisir : 0 = regarde en (0, 0, 0)
                                   1 = regarde devant */


couleur blanc, gris_c, gris, poisson, eau1, eau2, eau3, ciel1, ciel2, ciel3, bateau1, bateau2;


/* x : largeur, y : profondeur, z : hauteur */
void affiche_cube(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3){

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
    affiche_cube(-128, -128, -128, 128, 128, 128, ciel1, ciel2, ciel3);
}

void affiche_bateau(double x, double y, double z){
    double tx = 2,
           ty = 1,
           tz = 0.2,
           epaisseur = 0.1,
           pos_canne = 1.5,
           hauteur_canne = 1,
           prof_canne = -1;
    affiche_cube(x - tx, y - ty, z - tz, x + tx, y + ty, z + tz, bateau2, bateau2, bateau1);

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
    affiche_cube(x - tx, y - ty, z - tz, x + tx, y + ty, z + tz, poisson, poisson, poisson);
}

void affiche_eau() {
    glBegin(GL_TRIANGLES);

    glBegin(GL_TRIANGLES);

    glColor4f(eau1.r, eau1.g, eau1.b, 0.5);
    glVertex3f(-128, 128, NIVEAU_MER);
    glVertex3f(-128, -128, NIVEAU_MER);
    glVertex3f(128, -128, NIVEAU_MER);
    
    glVertex3f(-128, 128, NIVEAU_MER);
    glVertex3f(128, 128, NIVEAU_MER);
    glVertex3f(128, -128, NIVEAU_MER);
        
    glEnd();

    glEnd();
}



void Affichage(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.2, 1.2, -0.675, 0.675, 1, VISION_MAX);

    if (test == 0){
        gluLookAt(p_x, p_y, p_z, 0, 0, 0, 0, 0, 1);
    }
    else {
        gluLookAt(p_x, p_y, p_z, p_x, p_y + 1, p_z, 0, 0, 1);
    }
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    affiche_ciel();
    affiche_bateau(0, 0, NIVEAU_MER);
    affiche_bateau(5, 5, NIVEAU_MER);
    affiche_poisson(-1, 0, NIVEAU_MER - 2);
    affiche_eau();
    
    glFlush();
    glutPostRedisplay();
}


void Animer(){
    
    /* glutPostRedisplay(); */
}

void GererClavier(unsigned char touche, int x, int y){
    if (touche == 'q'){  /* gauche */
        p_x -= 1;
    } else if (touche == 'd'){  /* droite */
        p_x += 1;
    } else if (touche == 'z'){  /* haut */
        p_z += 1;
    } else if (touche == 's'){  /* bas */
        p_z -= 1;
    } else if (touche == 'c'){  /* changer de vu */
        test = !test;
    } else if (touche == 27){  /* quitté brutalement avec echap */
        exit(EXIT_FAILURE);
    }
}

void GererSouris(int bouton, int etat, int x, int y){

}

void GererMouvementSouris(int x, int y){

}


int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE);

    glutInitWindowSize(LARGUEUR, HAUTEUR);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Projet SAI");
    glEnable(GL_DEPTH_TEST);
    /* transparence */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutDisplayFunc(Affichage);
    glutIdleFunc(Animer);
    glutKeyboardFunc(GererClavier);
    glutMouseFunc(GererSouris);
    glutPassiveMotionFunc(GererMouvementSouris);


    /* position initial */
    p_x = 0;
    p_y = -10;  /* distance du centre, négatif pour reculer */
    p_z = 0;


    /* couleurs */
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
    
    glutMainLoop();
    return 0;
}