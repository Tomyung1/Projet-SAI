#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/collision.h"
#include "headers/poisson.h"
#include "headers/bateau.h"
#include "headers/obstacle.h"



/* x : largeur, y : profondeur, z : hauteur */



/********************************************************
 *                    Organisation                      *
 *                                                      *
 * - Calcul et stratégie de collisions                  *
 * - Collisions OBB + rotation des hitbox               *
 * - Contrôler un bateau                                *
 * - Commencer le jeu avec un score par bateau          *
 *                                                      *
 ********************************************************/



int p_x, p_y, p_z; /* position de l'observateur */

int test = 0; /* quel vu choisir : 0 = regarde en (0, 0, 0)
                                   1 = regarde devant */

poisson poissons[NB_POISSONS];
bateau bateaux[NB_BATEAUX];
obstacle obstacles[NB_OBSTACLES];
int souris_x_prec = -1;  /* Position précédente de la souris en X */
int souris_y_prec = -1;  /* Position précédente de la souris en Y */
float angle_y = 0.0;     /* Angle de rotation autour de l'axe Y */
float angle_z = 0.0;     /* Angle de rotation autour de l'axe Z */
int bouton_presse = 0;   /* État des boutons de la souris */



/* x : largeur, y : profondeur, z : hauteur */




void Affichage(){
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.12, 0.12, -0.0675, 0.0675, 0.1, VISION_MAX);

    if (test == 0){
        // Vue centrée
        gluLookAt(p_x, p_y, p_z, 0, 0, NIVEAU_MER, 0, 0, 1);
    } else {
        // Vue en première personne avec rotation de la souris
        // Calculer le point visé à partir des angles de rotation
        double lookX = p_x + cos(angle_y) * cos(angle_z);
        double lookY = p_y + sin(angle_y) * cos(angle_z);
        double lookZ = p_z + sin(angle_z);
        gluLookAt(p_x, p_y, p_z, lookX, lookY, lookZ, 0, 0, 1);
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

void Animer() {
    int i, j;
    /*
    double dir_pois_x, dir_pois_y, dir_pois_z;
    */
    
    // Les poissons
    for (i = 0; i < NB_POISSONS; i++) {

        // déplacement des poissons
        deplacer_poisson(&poissons[i]);

        // Collisions poissons - obstacles
        for (j = 0; j < NB_OBSTACLES; j++){
            if (distance_carre_modele(poissons[i].o.modele, obstacles[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_AABB(poissons[i].o.hitbox, obstacles[j].o.hitbox)){
                    printf("collision poisson %d et obstacle %d\n", i, j);
                }
            }
        }

        // Collisions poissons - canne à pêche bateaux
        for (j = 0; j < NB_BATEAUX; j++){
            if (distance_carre_modele(poissons[i].o.modele, bateaux[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_AABB(poissons[i].o.hitbox, bateaux[j].hitbox_canne)){
                    printf("collisions poisson %d et obstacle %d\n", i, j);
                }
            }
        }
        
        /*
        // Vérifier si un poisson est proche d'un bateau
        // Si oui, le mettre en état de fuite
        for (j = 0; j < NB_BATEAUX; j++) {
            // Récupérer les positions
            double poisson_x = get_mat(poissons[i].o.modele, 0, 0);
            double poisson_y = get_mat(poissons[i].o.modele, 1, 0);
            double poisson_z = get_mat(poissons[i].o.modele, 2, 0);
            
            double bateau_x = get_mat(bateaux[j].o.modele, 0, 0);
            double bateau_y = get_mat(bateaux[j].o.modele, 1, 0);
            double bateau_z = get_mat(bateaux[j].o.modele, 2, 0);
            
            // Calculer la distance
            double distance = pow(poisson_x - bateau_x, 2) + 
                              pow(poisson_y - bateau_y, 2) + 
                              pow(poisson_z - bateau_z, 2);
            
            // Si un bateau est proche, le poisson fuit
            if (distance < 8.0 * 8.0) {
                mettre_en_fuite(&poissons[i]);
                
                // Direction opposée au bateau
                set_mat(poissons[i].direction, 0, 0, poisson_x - bateau_x);
                set_mat(poissons[i].direction, 1, 0, poisson_y - bateau_y);
                set_mat(poissons[i].direction, 2, 0, poisson_z - bateau_z);
                
                dir_pois_x = get_mat(poissons[i].direction, 0, 0);
                dir_pois_y = get_mat(poissons[i].direction, 1, 0);
                dir_pois_z = get_mat(poissons[i].direction, 2, 0);
                
                // Normaliser
                double longueur = sqrt(dir_pois_x * dir_pois_x +
                                       dir_pois_y * dir_pois_y +
                                       dir_pois_z * dir_pois_z);
                
                if (longueur > 0) {
                    set_mat(poissons[i].direction, 0, 0, dir_pois_x / longueur);
                    set_mat(poissons[i].direction, 1, 0, dir_pois_y / longueur);
                    set_mat(poissons[i].direction, 2, 0, dir_pois_z / longueur);

                    // mise à jour des variables de direction
                    dir_pois_x = get_mat(poissons[i].direction, 0, 0);
                    dir_pois_y = get_mat(poissons[i].direction, 1, 0);
                    dir_pois_z = get_mat(poissons[i].direction, 2, 0);
                }
                
                // Rotation pour faire face à la nouvelle direction
                double angle = atan2(dir_pois_y, dir_pois_x);
                rotation_z(&poissons[i].o.modele, angle - M_PI/2); 
                
                break; // Un seul bateau suffit pour fuir
            }
        }
        */
    }

    for (i = 0; i < NB_BATEAUX; i++) {

        // déplacement des bateaux
        deplacer_bateau(&bateaux[i]);
        tourner_bateau(&bateaux[i], M_PI / 4096, 'd');

        // collisions bateaux - obstacles
        for (j = 0; j < NB_OBSTACLES; j++){
            if (distance_carre_modele(bateaux[i].o.modele, obstacles[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_AABB(bateaux[i].o.hitbox, obstacles[j].o.hitbox)){
                    printf("collisions bateaux %d et obstacle %d\n", i, j);
                }
            }
        }
    }
    
    glutPostRedisplay();
}

void GererMouvementSouris(int x, int y) {
    // Si c'est le premier appel ou le curseur a été recentré, ignorer
    if (souris_x_prec == -1 || souris_y_prec == -1 || 
        (x == LARGUEUR/2 && y == HAUTEUR/2)) {
        souris_x_prec = x;
        souris_y_prec = y;
        return;
    }
    
    // Calculer les déplacements relatifs
    int dx = souris_x_prec - x;
    int dy = souris_y_prec - y;
    
    // Sensibilité de la souris
    float sensibilite = 0.002;
    
    // Mise à jour des angles de rotation
    angle_y += dx * sensibilite;
    angle_z += dy * sensibilite;
    
    // Limiter l'angle vertical pour éviter de se retourner complètement
    if (angle_z > M_PI/2 - 0.1)
        angle_z = M_PI/2 - 0.1;
    if (angle_z < -M_PI/2 + 0.1)
        angle_z = -M_PI/2 + 0.1;
    
    // Recentrer le curseur
    glutWarpPointer(LARGUEUR/2, HAUTEUR/2);
    
    // Mémoriser la position centrée comme précédente
    souris_x_prec = LARGUEUR/2;
    souris_y_prec = HAUTEUR/2;
}

void GererClavier(unsigned char touche, int x, int y){
    int i;
    float vitesse_deplacement = 1.0;
    
    // Calculer les vecteurs de déplacement en fonction de l'orientation
    float frontal_x = cos(angle_y) * vitesse_deplacement;
    float frontal_y = sin(angle_y) * vitesse_deplacement;
    float lateral_x = cos(angle_y + M_PI/2) * vitesse_deplacement;
    float lateral_y = sin(angle_y + M_PI/2) * vitesse_deplacement;
    
    if (touche == 'q'){  /* gauche */
        if (test == 1) { // Mode première personne
            p_x += lateral_x;
            p_y += lateral_y;
        } else {
            p_x -= 1;
        }
    } else if (touche == 'd'){  /* droite */
        if (test == 1) { // Mode première personne
            p_x -= lateral_x;
            p_y -= lateral_y;
        } else {
            p_x += 1;
        }
    } else if (touche == 'z'){  /* avancer */
        if (test == 1) { // Mode première personne
            p_x += frontal_x;
            p_y += frontal_y;
        } else {
            p_z += 1;
        }
    } else if (touche == 's'){  /* reculer */
        if (test == 1) { // Mode première personne
            p_x -= frontal_x;
            p_y -= frontal_y;
        } else {
            p_z -= 1;
        }
    } else if (touche == 'r'){
        p_y += 1;
    } else if (touche == 'f'){
        p_y -= 1;
    } else if (touche == 'c'){  /* changer de vue */
        test = !test;
        if (test) {
            // Passer en mode première personne
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(LARGUEUR/2, HAUTEUR/2);
        } else {
            // Passer en mode vue centrée
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    } else if (touche == 'm'){  /* toggle mode souris */
        static int souris_active = 1;
        souris_active = !souris_active;
        if (souris_active) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutPassiveMotionFunc(GererMouvementSouris);
            glutMotionFunc(GererMouvementSouris);
        } else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
            glutPassiveMotionFunc(NULL);
            glutMotionFunc(NULL);
        }
    } else if (touche == 27){  /* quitter avec echap */
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
    } else if (touche == 't'){
        rotation_sur_place(&bateaux[0].o.modele, M_PI / 4, 'z');
        translation(&bateaux[1].o.modele, 1, 0, 0);
    }
}

void GererSouris(int bouton, int etat, int x, int y) {
    /* Sauvegarde de la position actuelle de la souris */
    souris_x_prec = x;
    souris_y_prec = y;
    
    /* Mémorisation de l'état du bouton */
    if (etat == GLUT_DOWN) {
        bouton_presse = bouton;
    } else {
        bouton_presse = -1;  /* Aucun bouton pressé */
    }
    
    /* Zoom avec la molette de la souris */
    if (etat == GLUT_DOWN) {
        switch (bouton) {
            case 3:  /* Molette vers le haut: zoom avant */
                p_y += 2;
                break;
            case 4:  /* Molette vers le bas: zoom arrière */
                p_y -= 2;
                break;
        }
    }
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
    glutMotionFunc(GererMouvementSouris); 

    // Position initiale
    p_x = 0;
    p_y = -30;
    p_z = 20;
    angle_y = 0.0; // Initialisation des angles
    angle_z = 0.0;
    test = 1; // Mode première personne par défaut

    // Masquer le curseur pour l'immersion
    glutSetCursor(GLUT_CURSOR_NONE);

    /* couleur */
    init_affichage();

    /* aléatoire */
    srand(time(NULL));

    /* génération */
    generer_monde();
    
    glutMainLoop();
    return 0;
}
