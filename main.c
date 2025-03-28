
#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/action.h"



/* x : largeur, y : profondeur, z : hauteur */



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

poisson poiss_test;
bateau bat_test;

/* x : largeur, y : profondeur, z : hauteur */



void Affichage(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.12, 0.12, -0.0675, 0.0675, 0.1, VISION_MAX);

    if (test == 0){
        gluLookAt(p_x, p_y, p_z, 0, 0, NIVEAU_MER, 0, 0, 1);
    }
    else {
        gluLookAt(p_x, p_y, p_z, p_x, p_y + 1, p_z, 0, 0, 1);
    }
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    affiche_ciel();
    
    afficher_poisson(poiss_test);
    afficher_bateau(bat_test);
    
    /* afficher les transparent à la fin */
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
    } else if (touche == 'r'){
        p_y += 1;
    } else if (touche == 'f'){
        p_y -= 1;
    } else if (touche == 'c'){  /* changer de vu */
        test = !test;
    } else if (touche == 27){  /* quitté avec echap */
        
        liberer_poisson(poiss_test);
        liberer_bateau(bat_test);
        
        exit(EXIT_SUCCESS);
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

    /* couleur */
    init_affichage();

    
    poiss_test = creer_poisson();
    bat_test = creer_bateau();
    
    translation(&poiss_test.o.modele, -4, -2, NIVEAU_MER);
    translation(&bat_test.o.modele, 0, 0, NIVEAU_MER);
    
    glutMainLoop();
    return 0;
}