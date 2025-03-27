#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"



/********************************************************
 *                Organisation                          *
 *                                                      *
 * - définir un repère pour le joueur                   *
 *   -> rotation / déplacement du repère, camera dessus *
 *      déplacement de la souris pour tourné            *
 *                                                      *
 *                                                      *
 ********************************************************/



int p_x=0, p_y= -10, p_z=0; /* position de l'observateur */

int test = 0; /* quel vu choisir : 0 = regarde en (0, 0, 0)
                                   1 = regarde devant */

/* x : largeur, y : profondeur, z : hauteur */
/* Variables pour la gestion de la souris */
int dernierX = -1, dernierY = -1;
int boutonSourisPresse = 0;
float angleRotationX = 0.0f;
float angleRotationY = 0.0f;

void affiche_cube(int x1, int y1, int z1, int x2, int y2, int z2, couleur c1, couleur c2, couleur c3){
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

void Affichage(){
    couleur blanc, gris_c, gris, bleu, bleu_c, bleu_cc;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.2, 1.2, -0.675, 0.675, 1, VISION_MAX);

    if (test == 0){
        gluLookAt(p_x, p_y, p_z, 0, 0, 0, 0, 0, 1);
    }
    else {
        float targetX = p_x + cos(angleRotationY) * sin(angleRotationX);
        float targetY = p_y + cos(angleRotationY) * cos(angleRotationX);
        float targetZ = p_z + sin(angleRotationY);
        
        gluLookAt(p_x, p_y, p_z, targetX, targetY, targetZ, 0, 0, 1);
    }
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    blanc.r  = 0.9;  blanc.g  = 0.9;  blanc.b  = 0.9;
    gris_c.r = 0.75; gris_c.g = 0.75; gris_c.b = 0.75;
    gris.r   = 0.6;  gris.g   = 0.6;  gris.b   = 0.6;
    
    bleu.r    = 0.13; bleu.g    = 0.5;  bleu.b    = 0.9;
    bleu_c.r  = 0.35; bleu_c.g  = 0.6;  bleu_c.b  = 0.9;
    bleu_cc.r = 0.6;  bleu_cc.g = 0.75; bleu_cc.b = 0.95;
    
    affiche_cube(0, 0, 0, 1, 1, 1, blanc, gris_c, gris);
    affiche_cube(-128, -128, -128, 128, 128, 128, bleu, bleu_c, bleu_cc);
    
    glFlush();
    glutSwapBuffers(); 
}


void Animer(){
    glutPostRedisplay(); 
}

void GererClavier(unsigned char touche, int x, int y){
    float vitesse = 1.0f;
    
    // Déplacement avec les touches ZQSD
    switch(touche) {
    case 'q':  // gauche
        p_x -= vitesse * cos(angleRotationX);
        p_y -= vitesse * sin(angleRotationX);
        break;
    case 'd':  // droite
        p_x += vitesse * cos(angleRotationX);
        p_y += vitesse * sin(angleRotationX);
        break;
    case 'z':  // avant
        p_x += vitesse * sin(angleRotationX);
        p_y += vitesse * cos(angleRotationX);
        break;
    case 's':  // arrière
        p_x -= vitesse * sin(angleRotationX);
        p_y -= vitesse * cos(angleRotationX);
        break;
    case 'c':  // changer de vue
        test = !test;
        break;
    case 27:   // quitter avec Echap
        exit(EXIT_SUCCESS);
    }
    glutPostRedisplay();
}
void GererSouris(int bouton, int etat, int x, int y){
    if (bouton == GLUT_LEFT_BUTTON) {
        if (etat == GLUT_DOWN) {
            boutonSourisPresse = 1;
            dernierX = x;
            dernierY = y;
        } else if (etat == GLUT_UP) {
            boutonSourisPresse = 0;
            dernierX = -1;
            dernierY = -1;
        }
    }
}
void GererMouvementSouris(int x, int y){
    // Seulement si un bouton est pressé
    if (boutonSourisPresse) {
           if (dernierX != -1 && dernierY != -1) {
            float sensibilite = 0.005f;
            
            // Calculer le changement de position
            angleRotationX += (x - dernierX) * sensibilite;
            angleRotationY += (y - dernierY) * sensibilite;
            
            // Limiter la rotation verticale
            if (angleRotationY > M_PI/2) angleRotationY = M_PI/2;
            if (angleRotationY < -M_PI/2) angleRotationY = -M_PI/2;
           
        }
        
        // Mettre à jour les dernières positions
        dernierX = x;
        dernierY = y;
        glutPostRedisplay();
    }
}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(LARGUEUR, HAUTEUR);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Projet SAI");
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(Affichage);
    glutIdleFunc(Animer);
    glutKeyboardFunc(GererClavier);
    glutMouseFunc(GererSouris);
    glutMotionFunc(GererMouvementSouris);


    /* position initial */
    p_x = 0;
    p_y = -10;  /* distance du centre, négatif pour reculer */
    p_z = 0;
    
    glutMainLoop();
    return 0;
}