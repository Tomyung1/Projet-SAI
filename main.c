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
<<<<<<< Updated upstream
    
    
=======
>>>>>>> Stashed changes
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
    int i;
    
    // Déplacer tous les poissons
    for (i = 0; i < NB_POISSONS; i++) {
        deplacer_poisson(&poissons[i]);
        
        // Vérifier si un poisson est proche d'un bateau
        // Si oui, le mettre en état de fuite
        for (int j = 0; j < NB_BATEAUX; j++) {
            // Récupérer les positions
            double poisson_x = get_mat(poissons[i].o.modele, 0, 0);
            double poisson_y = get_mat(poissons[i].o.modele, 1, 0);
            double poisson_z = get_mat(poissons[i].o.modele, 2, 0);
            
            double bateau_x = get_mat(bateaux[j].o.modele, 0, 0);
            double bateau_y = get_mat(bateaux[j].o.modele, 1, 0);
            double bateau_z = get_mat(bateaux[j].o.modele, 2, 0);
            
            // Calculer la distance
            double distance = sqrt(pow(poisson_x - bateau_x, 2) + 
                                  pow(poisson_y - bateau_y, 2) + 
                                  pow(poisson_z - bateau_z, 2));
            
            // Si un bateau est proche, le poisson fuit
            if (distance < 8.0) {
                mettre_en_fuite(&poissons[i]);
                
                // Direction opposée au bateau
                poissons[i].direction_x = poisson_x - bateau_x;
                poissons[i].direction_y = poisson_y - bateau_y;
                poissons[i].direction_z = poisson_z - bateau_z;
                
                // Normaliser
                double longueur = sqrt(poissons[i].direction_x * poissons[i].direction_x + 
                                     poissons[i].direction_y * poissons[i].direction_y + 
                                     poissons[i].direction_z * poissons[i].direction_z);
                
                if (longueur > 0) {
                    poissons[i].direction_x /= longueur;
                    poissons[i].direction_y /= longueur;
                    poissons[i].direction_z /= longueur;
                }
                
                // Rotation pour faire face à la nouvelle direction
                double angle = atan2(poissons[i].direction_y, poissons[i].direction_x);
                rotation_z(&poissons[i].o.modele, angle - M_PI/2); 
                
                break; // Un seul bateau suffit pour fuir
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
    int dx = x - souris_x_prec;
    int dy = y - souris_y_prec;
    
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

<<<<<<< Updated upstream
/* Fonction pour gérer les mouvements de la souris */
void GererMouvementSouris(int x, int y) {
    /* Calcul du déplacement de la souris */
    int dx = x - souris_x_prec;
    int dy = y - souris_y_prec;
    
    /* Si le bouton gauche est pressé pendant le déplacement */
    if (bouton_presse == GLUT_LEFT_BUTTON) {
        /* Rotation de la caméra */
        angle_y += dx * 0.5;
        angle_z += dy * 0.5;
        
        /* Limiter l'angle Z pour éviter de se retourner */
        if (angle_z > 60.0) angle_z = 60.0;
        if (angle_z < -60.0) angle_z = -60.0;
        
        /* Calculer la nouvelle position en fonction des angles */
        if (test == 0) {
            /* Mode "regarde en (0,0,0)" */
            float distance = sqrt(p_x*p_x + p_y*p_y + p_z*p_z);
            p_x = distance * sin(angle_y * M_PI/180.0);
            p_y = -distance * cos(angle_y * M_PI/180.0);
            p_z = distance * sin(angle_z * M_PI/180.0);
        } else {
        }
    }
    
    /* Si le bouton droit est pressé pendant le déplacement */
    if (bouton_presse == GLUT_RIGHT_BUTTON) {
        /* Déplacement latéral (gauche/droite) */
        p_x += dx * 0.1;
        
        /* Déplacement avant/arrière */
        p_y -= dy * 0.1;
    }
    
    /* Si le bouton du milieu est pressé pendant le déplacement */
    if (bouton_presse == GLUT_MIDDLE_BUTTON) {
        /* Déplacement vertical (haut/bas) */
        p_z -= dy * 0.1;
    }
    
    /* Mise à jour de la position précédente */
    souris_x_prec = x;
    souris_y_prec = y;
}

=======
>>>>>>> Stashed changes

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
    glutMotionFunc(GererMouvementSouris); 

    // Position initiale
    p_x = 0;
    p_y = -30;
    p_z = 0;
    angle_y = 0.0; // Initialisation des angles
    angle_z = 0.0;
    test = 1; // Mode première personne par défaut

    // Masquer le curseur pour l'immersion
    glutSetCursor(GLUT_CURSOR_NONE);

    /* couleur */
    init_affichage();

    /* aléatoire */
    srand(time(NULL));

<<<<<<< Updated upstream
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
    
  
=======
    /* génération */
    generer_monde();
    
>>>>>>> Stashed changes
    glutMainLoop();
    return 0;
}
