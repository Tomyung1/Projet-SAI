#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/action.h"
#include "headers/poisson.h"
#include "headers/bateau.h"
#include "headers/obstacle.h"



/* x : largeur, y : profondeur, z : hauteur */



/********************************************************
 *                Organisation                          *
 *                                                      *
 * - définir un repère pour le joueur                   *
 *   -> rotation / déplacement du repère, camera dessus *
 *      déplacement de la souris pour tourné            *
 * - ajout libération de la mémoire à la fermeture de   *
 *   la fenêtre                                         *
 *                                                      *
 ********************************************************/



int p_x, p_y, p_z; /* position de l'observateur */

int test = 0; /* quel vu choisir : 0 = regarde en (0, 0, 0)
                                   1 = regarde devant */

poisson poissons[NB_POISSONS];
bateau bateaux[NB_BATEAUX];
obstacle obstacles[NB_OBSTACLES];

/* x : largeur, y : profondeur, z : hauteur */



void Affichage(){
    int i;
    
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

    for (i = 0; i < NB_POISSONS; i++){
        afficher_poisson(poissons[i]);
    }
    
    for (i = 0; i < NB_BATEAUX; i++){
        afficher_bateau(bateaux[i]);
    }

    for (i = 0; i < NB_OBSTACLES; i++){
        afficher_obstacle(obstacles[i]);
    }
    
    /* afficher les transparent à la fin */
    affiche_eau();
    
    glFlush();
    glutPostRedisplay();
}


void Animer(){
    
    /* glutPostRedisplay(); */
}

void GererClavier(unsigned char touche, int x, int y){
    int i;
    
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

        for (i = 0; i < NB_POISSONS; i++){
            liberer_poisson(poissons[i]);
        }
        for (i = 0; i < NB_BATEAUX; i++){
            liberer_bateau(bateaux[i]);
        }
        for (i = 0; i < NB_OBSTACLES; i++){
            liberer_obstacle(obstacles[i]);
        }
        
        exit(EXIT_SUCCESS);
    }
}

void GererSouris(int bouton, int etat, int x, int y){

}

void GererMouvementSouris(int x, int y){

}


int main(int argc, char *argv[]){
    int i;
    
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
    p_y = -30;  /* distance du centre, négatif pour reculer */
    p_z = 0;

    /* couleur */
    init_affichage();

    /* aléatoire */
    srand(time(NULL));

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

    trans_rot_z_alea(&bateaux[0].o.modele, 5, 15, 5, 15, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea(&bateaux[1].o.modele, -15, -5, 5, 15, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea(&bateaux[2].o.modele, 5, 15, -15, -5, NIVEAU_MER, NIVEAU_MER);
    trans_rot_z_alea(&bateaux[3].o.modele, -15, -5, -15, -5, NIVEAU_MER, NIVEAU_MER);

    translation(&obstacles[0].o.modele, 0, 0, NIVEAU_MER);
    
  
    glutMainLoop();
    return 0;
}
