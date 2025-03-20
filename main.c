#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

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



int p_x, p_y, p_z; /* position de l'observateur */

int test = 0; /* quel vu choisir : 0 = regarde en (0, 0, 0)
                                   1 = regarde devant */

/* x : largeur, y : profondeur, z : hauteur */

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
        gluLookAt(p_x, p_y, p_z, p_x, p_y + 1, p_z, 0, 0, 1);
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
    
    glutDisplayFunc(Affichage);
    glutIdleFunc(Animer);
    glutKeyboardFunc(GererClavier);
    glutMouseFunc(GererSouris);
    glutPassiveMotionFunc(GererMouvementSouris);


    /* position initial */
    p_x = 0;
    p_y = -10;  /* distance du centre, négatif pour reculer */
    p_z = 0;
    
    glutMainLoop();
    return 0;
}