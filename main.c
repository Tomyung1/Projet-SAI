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

#define JOUEUR bateaux[0]


/* x : largeur, y : profondeur, z : hauteur */



/********************************************************
 *                    Organisation                      *
 *                                                      *
 *                                                      *
 * - collisions entre bateaux                           *
 *                                                      *
 *                                                      *
 *                                                      *
 ********************************************************/



double p_x, p_y, p_z; /* position de l'observateur */

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

/* Variables de jeu */
int touches_pressees[NB_TOUCHES] = {0};  /* Tableau de bits pour les touches */
int bateau_joueur = 0;  /* Index du bateau contrôlé par le joueur */
double hauteur = 4; /* hauteur par rapport au centre du bateau pour la position de l'observateur */

/* x : largeur, y : profondeur, z : hauteur */


// Déclarations de fonctions
void Affichage();
void Animer();
void GererClavier(unsigned char touche, int x, int y);
void GererMouvementSouris(int x, int y);
void GererSouris(int bouton, int etat, int x, int y);
void controler_bateau_joueur();
void pecher_poisson(int index_poisson);
void basculer_mode();
void GererToucheNormale(unsigned char touche, int x, int y);
void GererToucheSpeciale(int touche, int x, int y);
void GererToucheSpecialeRelachee(int touche, int x, int y);
void GererToucheNormaleRelachee(unsigned char touche, int x, int y);
void afficher_score_console();

void Affichage(){
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.12, 0.12, -0.0675, 0.0675, 0.1, VISION_MAX);

    if (MODE_JEU){
        p_x = get_mat(JOUEUR.o.modele, 0, 0);
        p_y = get_mat(JOUEUR.o.modele, 1, 0);
        p_z = get_mat(JOUEUR.o.modele, 2, 0) + hauteur;
    }
    
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
        afficher_bateau(bateaux[i], (MODE_JEU && i == bateau_joueur));
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
    double dir_pois_x, dir_pois_y, dir_pois_z;
    
    // Contrôler le bateau du joueur
    if (MODE_JEU){
        controler_bateau_joueur();
    }
    
    // Les poissons
    for (i = 0; i < NB_POISSONS; i++) {

        // Vérifier si un poisson est proche d'un bateau pour le mettre en fuite
        for (j = 0; j < NB_BATEAUX; j++) {
            // Utiliser la fonction de distance au carré pour optimiser
            if (distance_carre_modele(poissons[i].o.modele, bateaux[j].o.modele) < 64.0) { // distance de 8 unités
                // Récupérer les positions
                double poisson_x = get_mat(poissons[i].o.modele, 0, 0);
                double poisson_y = get_mat(poissons[i].o.modele, 1, 0);
                double poisson_z = get_mat(poissons[i].o.modele, 2, 0);
                
                double bateau_x = get_mat(bateaux[j].o.modele, 0, 0);
                double bateau_y = get_mat(bateaux[j].o.modele, 1, 0);
                double bateau_z = get_mat(bateaux[j].o.modele, 2, 0);
                
                // Mettre le poisson en état de fuite
                mettre_en_fuite(&poissons[i]);
                
                // Calculer la direction de fuite (opposée au bateau)
                dir_pois_x = poisson_x - bateau_x;
                dir_pois_y = poisson_y - bateau_y;
                dir_pois_z = poisson_z - bateau_z;

                // Normaliser le vecteur de direction
                double longueur = sqrt(dir_pois_x * dir_pois_x + dir_pois_y * dir_pois_y + dir_pois_z * dir_pois_z);

                if (longueur > 0) {
                    // Calculer l'angle de rotation nécessaire
                    double nouvel_angle = atan2(dir_pois_y, dir_pois_x);
                    double ancien_angle = atan2(get_mat(poissons[i].direction, 1, 0), get_mat(poissons[i].direction, 0, 0));
                    double rotation_necessaire = nouvel_angle - ancien_angle;
    
                    // Faire tourner le poisson pour qu'il "regarde" dans la bonne direction
                    tourner_poisson(&poissons[i], rotation_necessaire, 'z'); // Rotation autour de Z
                }
                break;
            }
        }

        // déplacement des poissons
        deplacer_poisson(&poissons[i]);

        // Collisions poissons - poissons
        for (j = i+1; j < NB_POISSONS; j++){
            if (distance_carre_modele(poissons[i].o.modele, poissons[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_OBB(poissons[i].o.hitbox, poissons[j].o.hitbox)){
                    tourner_poisson(&poissons[i], M_PI, 'z');
                }
            }
        }
        
        // Collisions poissons - obstacles
        for (j = 0; j < NB_OBSTACLES; j++){
            if (distance_carre_modele(poissons[i].o.modele, obstacles[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_OBB(poissons[i].o.hitbox, obstacles[j].o.hitbox)){
                    tourner_poisson(&poissons[i], M_PI, 'z'); // Demi-tour
                }
            }
        }

        // Collisions poissons - canne à pêche bateaux
        for (j = 0; j < NB_BATEAUX; j++){
            if (distance_carre_modele(poissons[i].o.modele, bateaux[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_OBB(poissons[i].o.hitbox, bateaux[j].hitbox_canne)){
                    if (MODE_JEU && j == bateau_joueur) {
                        // Le joueur pêche un poisson !
                        pecher_poisson(i);
                    }
                }
            }
        }
    }

    // Les bateaux
    for (i = 0; i < NB_BATEAUX; i++) {
        if (!MODE_JEU || i != bateau_joueur) {
            // Évitement d'obstacles AVANT le déplacement
            eviter_obstacles_bateau(&bateaux[i], obstacles, NB_OBSTACLES);
            
            // déplacement des bateaux
            deplacer_bateau(&bateaux[i]);
        }

        // collisions bateaux - bateaux
        for (j = i+1; j < NB_BATEAUX; j++){
            if (distance_carre_modele(bateaux[i].o.modele, bateaux[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_OBB(bateaux[i].o.hitbox, bateaux[j].o.hitbox)){
                    tourner_bateau(&bateaux[i], M_PI, 'z'); // Demi-tour
                }
            }
        }
        
        // collisions bateaux - obstacles
        for (j = 0; j < NB_OBSTACLES; j++){
            if (distance_carre_modele(bateaux[i].o.modele, obstacles[j].o.modele) < DIST_CALCUL_COLLISION_CARRE){
                if (collisions_OBB(bateaux[i].o.hitbox, obstacles[j].o.hitbox)){
                    
                    tourner_bateau(&bateaux[i], M_PI, 'z'); // Demi-tour
                }
            }
        }
    }
    
    glutPostRedisplay();
}

void GererClavier(unsigned char touche, int x, int y){
    int i;
    float vitesse_deplacement = 1.0;
    
    // Calculer les vecteurs de déplacement en fonction de l'orientation
    float frontal_x = cos(angle_y) * vitesse_deplacement;
    float frontal_y = sin(angle_y) * vitesse_deplacement;
    float lateral_x = cos(angle_y + M_PI/2) * vitesse_deplacement;
    float lateral_y = sin(angle_y + M_PI/2) * vitesse_deplacement;

    if (!MODE_JEU){
        if (touche == 'q'){  /* gauche */
            if (test == 1) { // Mode première personne
                p_x += lateral_x;
                p_y += lateral_y;
            }
            else {
                p_x -= 1;
            }
        
        } else if (touche == 'd'){  /* droite */
            if (test == 1) { // Mode première personne
                p_x -= lateral_x;
                p_y -= lateral_y;
            }
            else {
                p_x += 1;
            }
        
        } else if (touche == 'z'){  /* avancer */
            if (test == 1) { // Mode première personne
                p_x += frontal_x;
                p_y += frontal_y;
            }
            else {
                p_z += 1;
            }
        
        } else if (touche == 's'){  /* reculer */
            if (test == 1) { // Mode première personne
                p_x -= frontal_x;
                p_y -= frontal_y;
            }
            else {
                p_z -= 1;
            }
        }
    }
    
    /* else if (touche == 'r'){
        p_y += 1;
    } else if (touche == 'f'){
        p_y -= 1;
    } */
    if (touche == 'c'){  /* changer de vue */
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
    } else if (touche == 'i' || touche == 'I'){  /* informations */
        printf("\n╔════════════════════════════════════╗\n");
        printf("║            INFORMATIONS            ║\n");
        printf("╠════════════════════════════════════╣\n");
        printf("║ Mode actuel : %-20s ║\n", (MODE_JEU) ? "JEU" : "LIBRE");
        if (MODE_JEU) {
            printf("║ Score : %-26d ║\n", JOUEUR.score_poissons);
            printf("║ Bateau : VERT (contrôlable)       ║\n");
        }
        printf("╠════════════════════════════════════╣\n");
        printf("║ Contrôles :                        ║\n");
        if (MODE_JEU) {
            printf("║ • Flèches : Contrôler le bateau   ║\n");
        }
        printf("║ • C : Changer de vue               ║\n");
        printf("║ • I : Afficher ces informations    ║\n");
        printf("║ • M : Toggle contrôle souris       ║\n");
        printf("╚════════════════════════════════════╝\n");
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
    
    // Gérer les touches normales pour le tableau de bits
    GererToucheNormale(touche, x, y);
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
    if (etat == GLUT_DOWN && !MODE_JEU) {
        double lookX = cos(angle_y) * cos(angle_z);
        double lookY = sin(angle_y) * cos(angle_z);
        double lookZ = sin(angle_z);
        
        switch (bouton) {
            case 3:  /* Molette vers le haut: zoom avant */
                p_x += lookX;
                p_y += lookY;
                p_z += lookZ;
                break;
            case 4:  /* Molette vers le bas: zoom arrière */
                p_x -= lookX;
                p_y -= lookY;
                p_z -= lookZ;
                break;
        }
    }
}

/* Fonction appelée quand une touche spéciale est pressée */
void GererToucheSpeciale(int touche, int x, int y) {
    if (touche < NB_TOUCHES) {
        touches_pressees[touche] = 1;
    }
}

/* Fonction appelée quand une touche spéciale est relâchée */
void GererToucheSpecialeRelachee(int touche, int x, int y) {
    if (touche < NB_TOUCHES) {
        touches_pressees[touche] = 0;
    }
}

/* Fonction appelée quand une touche normale est pressée */
void GererToucheNormale(unsigned char touche, int x, int y) {
    if (touche < NB_TOUCHES) {
        touches_pressees[touche] = 1;
    }
}

/* Fonction appelée quand une touche normale est relâchée */
void GererToucheNormaleRelachee(unsigned char touche, int x, int y) {
    if (touche < NB_TOUCHES) {
        touches_pressees[touche] = 0;
    }
}

/* Contrôler le bateau du joueur */
void controler_bateau_joueur() {
    
    bateau *b = &bateaux[bateau_joueur];
    int i, test_coll = 0, coll_obst = 0;
    
    for (i = 1; i < NB_BATEAUX && !test_coll; i++){
        test_coll = test_coll || collisions_OBB(JOUEUR.o.hitbox, bateaux[i].o.hitbox);
    }
    for (i = 0; i < NB_OBSTACLES && !test_coll; i++){
        test_coll = test_coll || collisions_OBB(JOUEUR.o.hitbox, obstacles[i].o.hitbox);
        if (test_coll){
            coll_obst = 1;
        }
    }
    

    
    if (!test_coll){
    
        /* Rotation */
        if (touches_pressees[TOUCHE_FLECHE_GAUCHE]) {
            tourner_bateau(b, VITESSE_ROTATION_BATEAU, 'g');
        }
        if (touches_pressees[TOUCHE_FLECHE_DROITE]) {
            tourner_bateau(b, VITESSE_ROTATION_BATEAU, 'd');
        }
    
        /* Mouvement avant/arrière */
        if (touches_pressees[TOUCHE_FLECHE_HAUT]) {
            // Avancer dans la direction actuelle
            double dx = get_mat(b->direction, 0, 0) * VITESSE_BATEAU_JOUEUR;
            double dy = get_mat(b->direction, 1, 0) * VITESSE_BATEAU_JOUEUR;
            double dz = get_mat(b->direction, 2, 0) * VITESSE_BATEAU_JOUEUR;
        
            translation(&(b->o.modele), dx, dy, dz);
            translation(&(b->o.hitbox), dx, dy, dz);
            translation(&(b->hitbox_canne), dx, dy, dz);
        }
        if (touches_pressees[TOUCHE_FLECHE_BAS]) {
            // Reculer
            double dx = get_mat(b->direction, 0, 0) * (-VITESSE_BATEAU_JOUEUR);
            double dy = get_mat(b->direction, 1, 0) * (-VITESSE_BATEAU_JOUEUR);
            double dz = get_mat(b->direction, 2, 0) * (-VITESSE_BATEAU_JOUEUR);
        
            translation(&(b->o.modele), dx, dy, dz);
            translation(&(b->o.hitbox), dx, dy, dz);
            translation(&(b->hitbox_canne), dx, dy, dz);
        }
    }
    else if (coll_obst){
        printf("*** VOUS AVEZ COULÉ ***\n");

        if (FIN_PARTIE){
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
}

/* Fonction de pêche */
void pecher_poisson(int index_poisson) {
    if (index_poisson < 0 || index_poisson >= NB_POISSONS) return;
    
    JOUEUR.score_poissons++;
    printf("*** POISSON PECHE ! Score : %d ***\n", JOUEUR.score_poissons);
    
    // Repositionner le poisson ailleurs 
    double nouveau_x = (rand() % 80) - 40;  // Position aléatoire entre -40 et 40
    double nouveau_y = (rand() % 80) - 40;
    double nouveau_z = NIVEAU_MER - (rand() % 5) - 1;  // Sous l'eau
    
    // Calculer le déplacement nécessaire
    double pos_actuelle_x = get_mat(poissons[index_poisson].o.modele, 0, 0);
    double pos_actuelle_y = get_mat(poissons[index_poisson].o.modele, 1, 0);
    double pos_actuelle_z = get_mat(poissons[index_poisson].o.modele, 2, 0);
    
    double dx = nouveau_x - pos_actuelle_x;
    double dy = nouveau_y - pos_actuelle_y;
    double dz = nouveau_z - pos_actuelle_z;
    
    // Déplacer le poisson et sa hitbox
    translation(&poissons[index_poisson].o.modele, dx, dy, dz);
    translation(&poissons[index_poisson].o.hitbox, dx, dy, dz);
    
    // Remettre le poisson en état normal
    mettre_en_normal(&poissons[index_poisson]);
}

void afficher_score_console() {
    if (MODE_JEU) {
        printf("\r[PECHE] Score: %d poissons peches\n", JOUEUR.score_poissons);
        fflush(stdout);  
    }
}

/* Basculer entre mode libre et mode jeu */
void basculer_mode() {
    if (MODE_JEU) {
        JOUEUR.score_poissons = 0;
        printf("\n╔══════════════════════════════════╗\n");
        printf("║        MODE JEU ACTIVÉ           ║\n");
        printf("╠══════════════════════════════════╣\n");
        printf("║ Votre bateau: VERT               ║\n");
        printf("║ Contrôles:                       ║\n");
        printf("║ • Flèches: Se déplacer           ║\n");
        printf("║ • Objectif: Pêcher des poissons! ║\n");
        printf("║ • Attention aux iceberg !!       ║\n");
        printf("╚══════════════════════════════════╝\n\n");
        
        // Positionner la caméra sur le bateau du joueur
        p_x = get_mat(bateaux[bateau_joueur].o.modele, 0, 0);
        p_y = get_mat(bateaux[bateau_joueur].o.modele, 1, 0);
        p_z = get_mat(bateaux[bateau_joueur].o.modele, 2, 0) + hauteur;
    } else {
        printf("\n=== MODE LIBRE ACTIVÉ ===\n");
    }
}

int main(int argc, char *argv[]){
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE);

    glutInitWindowSize(LARGUEUR, HAUTEUR);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Projet SAI");
    glEnable(GL_DEPTH_TEST);

    /* transparence */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutDisplayFunc(Affichage);
    glutIdleFunc(Animer);
    glutKeyboardFunc(GererClavier);
    glutKeyboardUpFunc(GererToucheNormaleRelachee);  
    glutSpecialFunc(GererToucheSpeciale);            
    glutSpecialUpFunc(GererToucheSpecialeRelachee);  
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

    /* Jeu */
    basculer_mode();
    
    glutMainLoop();
    return 0;
}