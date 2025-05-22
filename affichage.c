#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/affichage.h"
#include "headers/matrice.h"
#include "headers/poisson.h"
#include "headers/bateau.h"
#include "headers/obstacle.h"
#include "headers/collision.h"

extern poisson poissons[NB_POISSONS];
extern bateau bateaux[NB_BATEAUX];
extern obstacle obstacles[NB_OBSTACLES];

/* palette de couleur */
couleur rouge, rose, vert, blanc, gris_c, gris, poisson1, eau1, eau2, eau3, ciel1, ciel2, ciel3, bateau1, bateau2;

int (*regle_poisson)[3];   /* Pointeur de tableau de 3 entiers (indice */
int (*regle_bateau)[3];    /* de sommet) utiliser pour définir les     */
int (*regle_obstacle)[3];  /* facettes des modèle 3d                   */


static int (*charger_regles(char* fichier))[3]{
    FILE* f;
    int i;
    int n;
    double a1, a2, a3;
    int f1, f2, f3;
    int (*regles)[3];

    // Chargement des règles
    if ((f = fopen(fichier, "r")) == NULL){
        fprintf(stderr, "Erreur lecture modèle 3D de %s\n", fichier);
        exit(EXIT_FAILURE);
    }

    // dèplacement jusqu'au règle
    if (fscanf(f, "%d\n", &n) != 1){
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++){
        if (fscanf(f, "%lf,%lf,%lf\n", &a1, &a2, &a3) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    // lecture des règles
    if (fscanf(f, "%d\n", &n) != 1){  // nombre de facette
        fprintf(stderr, "Erreur lecture modèle 3D\n");
        fclose(f);
        exit(EXIT_FAILURE);        
    }
    /* allocation */
    regles = malloc(n * sizeof(int[3]));
    for (i = 0; i < n; i++){
        if (fscanf(f, "%d,%d,%d\n", &f1, &f2, &f3) != 3){
            fprintf(stderr, "Erreur lecture modèle 3D\n");
            free(regles);
            fclose(f);
            exit(EXIT_FAILURE);
        }
        regles[i][0] = f1;
        regles[i][1] = f2;
        regles[i][2] = f3;
    }

    fclose(f);

    return regles;
}

/* initialise l'affichage, en particuler les valeurs pour les couleurs
   appeler 1 seul fois */
void init_affichage(){
    rouge.r  = 1;    rouge.g  = 0;    rouge.b  = 0;
    rose.r   = 1;    rose.g   = 0;    rose.b   = 0.58;
    vert.r   = 0;    vert.g   = 1;    vert.b   = 0;
    blanc.r  = 0.9;  blanc.g  = 0.9;  blanc.b  = 0.9;
    gris_c.r = 0.75; gris_c.g = 0.75; gris_c.b = 0.75;
    gris.r   = 0.6;  gris.g   = 0.6;  gris.b   = 0.6;
    
    poisson1.r = 0.01; poisson1.g = 0.02; poisson1.b = 0.37;
    eau1.r     = 0.00; eau1.g     = 0.47; eau1.b     = 0.71;
    eau2.r     = 0.00; eau2.g     = 0.59; eau2.b     = 0.78;
    eau3.r     = 0.00; eau3.g     = 0.71; eau3.b     = 0.85;
    ciel1.r    = 0.28; ciel1.g    = 0.79; ciel1.b    = 0.89;
    ciel2.r    = 0.56; ciel2.g    = 0.88; ciel2.b    = 0.94;
    ciel3.r    = 0.68; ciel3.g    = 0.91; ciel3.b    = 0.96;
    bateau1.r  = 0.60; bateau1.g  = 0.35; bateau1.b  = 0.16;
    bateau2.r  = 0.47; bateau2.g  = 0.27; bateau2.b  = 0.14;

    regle_poisson = charger_regles("objets/poisson.obj");
    regle_bateau = charger_regles("objets/bateau.obj");
    regle_obstacle = charger_regles("objets/obstacle.obj");
}

/* affiche un pavé droit, du point en bas à gauche devant au point en haut à droite derrière */
void affiche_cube(double x1, double y1, double z1, double x2, double y2, double z2, couleur c1, couleur c2, couleur c3){

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


void afficher_hitbox_pave(matrice hitbox, couleur c){
    int i;
    double x0, y0, z0, xi, yi, zi;


    glBegin(GL_LINES);

    glColor3f(c.r, c.g, c.b);
    
    x0 = get_mat(hitbox, 0, 1);
    y0 = get_mat(hitbox, 1, 1);
    z0 = get_mat(hitbox, 2, 1);
    for (i = 2; i <= 4; i++){
        xi = get_mat(hitbox, 0, i);
        yi = get_mat(hitbox, 1, i);
        zi = get_mat(hitbox, 2, i);

        glVertex3f(x0, y0, z0); glVertex3f(xi, yi, zi);
    }
    
    x0 = get_mat(hitbox, 0, 8);
    y0 = get_mat(hitbox, 1, 8);
    z0 = get_mat(hitbox, 2, 8);
    for (i = 5; i <= 7; i++){
        xi = get_mat(hitbox, 0, i);
        yi = get_mat(hitbox, 1, i);
        zi = get_mat(hitbox, 2, i);

        glVertex3f(x0, y0, z0); glVertex3f(xi, yi, zi);
    }

    xi = get_mat(hitbox, 0, 2);
    yi = get_mat(hitbox, 1, 2);
    zi = get_mat(hitbox, 2, 2);

    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 6), get_mat(hitbox, 1, 6), get_mat(hitbox, 2, 6));
    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 7), get_mat(hitbox, 1, 7), get_mat(hitbox, 2, 7));
    
    xi = get_mat(hitbox, 0, 3);
    yi = get_mat(hitbox, 1, 3);
    zi = get_mat(hitbox, 2, 3);
    
    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 5), get_mat(hitbox, 1, 5), get_mat(hitbox, 2, 5));
    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 7), get_mat(hitbox, 1, 7), get_mat(hitbox, 2, 7));
                                        
    xi = get_mat(hitbox, 0, 4);
    yi = get_mat(hitbox, 1, 4);
    zi = get_mat(hitbox, 2, 4);
    
    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 5), get_mat(hitbox, 1, 5), get_mat(hitbox, 2, 5));
    glVertex3f(xi, yi, zi); glVertex3f(get_mat(hitbox, 0, 6), get_mat(hitbox, 1, 6), get_mat(hitbox, 2, 6));
    
    
    glEnd();

    /*
    double x1, y1, z1;
    
    x0 = get_mat(hitbox, 0, 0);
    y0 = get_mat(hitbox, 1, 0);
    z0 = get_mat(hitbox, 2, 0);
    x1 = get_mat(hitbox, 0, 1);
    y1 = get_mat(hitbox, 1, 1);
    z1 = get_mat(hitbox, 2, 1);
    
    glBegin(GL_LINES);

    glColor3f(c.r, c.g, c.b);
    
    glVertex3f(x0, y0, z0); glVertex3f(x1, y0, z0);
    glVertex3f(x1, y0, z0); glVertex3f(x1, y1, z0);
    glVertex3f(x1, y1, z0); glVertex3f(x0, y1, z0);
    glVertex3f(x0, y1, z0); glVertex3f(x0, y0, z0);

    glVertex3f(x0, y0, z1); glVertex3f(x1, y0, z1);
    glVertex3f(x1, y0, z1); glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x0, y1, z1);
    glVertex3f(x0, y1, z1); glVertex3f(x0, y0, z1);

    glVertex3f(x0, y0, z0); glVertex3f(x0, y0, z1);
    glVertex3f(x1, y0, z0); glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z0); glVertex3f(x1, y1, z1);
    glVertex3f(x0, y1, z0); glVertex3f(x0, y1, z1);

    glEnd();
    */
}

static void afficher_direction(matrice modele, matrice direction){
    glBegin(GL_LINES);
    glColor3f(rouge.r, rouge.g, rouge.b);
    glVertex3f(get_mat(modele, 0, 0),
               get_mat(modele, 1, 0),
               get_mat(modele, 2, 0));
    glVertex3f(get_mat(modele, 0, 0) + 1 * get_mat(direction, 0, 0),
               get_mat(modele, 1, 0) + 1 * get_mat(direction, 1, 0),
               get_mat(modele, 2, 0) + 1 * get_mat(direction, 2, 0));
    glEnd();
}

/* affiche le ciel */
void affiche_ciel(){
    affiche_cube(LIMITE_MIN_X, LIMITE_MIN_Y, LIMITE_MIN_Z,
                 LIMITE_MAX_X, LIMITE_MAX_Y, LIMITE_MAX_Z,
                 ciel1, ciel2, ciel3);
}

/* règle de dessin */
void afficher_bateau(bateau b,int est_joueur){
    int i;
    couleur couleur_coque = est_joueur ? vert : bateau2;
  
    /* coque */
    for (i = 0; i < 6; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  couleur_coque);
    }
    
    for (i = 6; i < 8; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  est_joueur ? vert : bateau1);
    }
    
    /* cabine */
    for (i = 8; i < 18; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  gris_c);
    }
    
    /* canne */
    for (i = 18; i < 27; i++){
        dessiner_facette_triangle(b.o.modele,
                                  regle_bateau[i][0],
                                  regle_bateau[i][1],
                                  regle_bateau[i][2],
                                  gris);
    }

    // affichage des hitbox
    if (AFF_HITBOX){
        afficher_hitbox_pave(b.o.hitbox, rouge);
        afficher_hitbox_pave(b.hitbox_canne, rose);
    }
    // affichage de la direction
    if (AFF_DIR){
        afficher_direction(b.o.modele, b.direction);
    }
}



/* règle de dessin */
void afficher_poisson(poisson p){
    int i;
    
    for (i = 0; i < 20; i++){
        dessiner_facette_triangle(p.o.modele,
                                  regle_poisson[i][0],
                                  regle_poisson[i][1],
                                  regle_poisson[i][2],
                                  poisson1);
    }

    // affichage de la hitbox
    if (AFF_HITBOX){
        afficher_hitbox_pave(p.o.hitbox, rouge);
    }
    // affichage de la direction
    if (AFF_DIR){
        afficher_direction(p.o.modele, p.direction);
    }
}

/* règle de dessin */
void afficher_obstacle(obstacle ob){
    int i;

    for (i = 0; i < 24; i++){
        dessiner_facette_triangle(ob.o.modele,
                                  regle_obstacle[i][0],
                                  regle_obstacle[i][1],
                                  regle_obstacle[i][2],
                                  blanc);
    }

    if (AFF_HITBOX){
        afficher_hitbox_pave(ob.o.hitbox, rouge);
    }
}


/* affiche l'eau */
void affiche_eau() {
    glBegin(GL_TRIANGLES);

    glColor4f(eau1.r, eau1.g, eau1.b, 0.5);
    glVertex3f(LIMITE_MIN_X, LIMITE_MAX_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MAX_X, LIMITE_MAX_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MAX_X, LIMITE_MIN_Y, NIVEAU_MER);
    
    glVertex3f(LIMITE_MAX_X, LIMITE_MIN_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MIN_X, LIMITE_MIN_Y, NIVEAU_MER);
    glVertex3f(LIMITE_MIN_X, LIMITE_MAX_Y, NIVEAU_MER);
     
    glEnd();

    /* debug zone iceberg (si le code ne change pas dans genere_monde
    int x, y, num_obstacle;
    int dx, dy, larg, decalage;
    double pourcentage;

    dx = (abs(LIMITE_MIN_X) + abs(LIMITE_MAX_X)) / NB_OBST_PAR_LIGNE;
    dy = (abs(LIMITE_MIN_Y) + abs(LIMITE_MAX_Y)) / NB_OBST_PAR_LIGNE;
    decalage = dx / 2;
    pourcentage = 0.7;
    larg = decalage * pourcentage;
    
    num_obstacle = 0;
    for (x = 0; x < NB_OBST_PAR_LIGNE; x++){
        for (y = 0; y < NB_OBST_PAR_LIGNE; y++){
            affiche_cube(LIMITE_MIN_X+x*dx+decalage-larg,
                         LIMITE_MIN_Y+y*dy+decalage-larg,
                         NIVEAU_MER + 0.1,
                         LIMITE_MIN_X+x*dx+decalage+larg,
                         LIMITE_MIN_Y+y*dy+decalage+larg,
                         NIVEAU_MER + 0.2,
                         poisson1, poisson1, poisson1);
            num_obstacle++;
        }
    }
    */
}




void generer_monde(){
    int i, j, x, y, num_obstacle;
    int nb_tentative, test_collision, tentative_max = 15;
    int dx, dy, larg, decalage, taille;
    double pourcentage;
    matrice copie_modele, copie_direction, copie_hitbox, copie_hitbox2;

    for (i = 0; i < NB_POISSONS; i++){
        poissons[i] = creer_poisson();
    }

    for (i = 0; i < NB_BATEAUX; i++){
        bateaux[i] = creer_bateau();
    }

    for (i = 0; i < NB_OBSTACLES; i++){
        obstacles[i] = creer_obstacle();
    }

    // variable de génération
    dx = (abs(LIMITE_MIN_X) + abs(LIMITE_MAX_X)) / NB_OBST_PAR_LIGNE;
    dy = (abs(LIMITE_MIN_Y) + abs(LIMITE_MAX_Y)) / NB_OBST_PAR_LIGNE;
    decalage = dx / 2;
    pourcentage = 0.7;
    larg = decalage * pourcentage; // pour assurer qu'il ne se chevauche pas
    printf("Facteur maximal pour les icebergs : %d\n", decalage - larg);
    
    /* parcours de zone carré de largeur larg pour placer un obstacle par zone */
    num_obstacle = 0;
    for (x = 0; x < NB_OBST_PAR_LIGNE; x++){
        for (y = 0; y < NB_OBST_PAR_LIGNE; y++){
            taille = rand() % 6 + 3; // entre 4 et 8
            agrandissement(&obstacles[num_obstacle].o.modele, taille, taille, taille);
            agrandissement(&obstacles[num_obstacle].o.hitbox, taille, taille, taille);
            trans_rot_z_alea_tout(&obstacles[num_obstacle].o.modele,
                                  NULL,
                                  &obstacles[num_obstacle].o.hitbox,
                                  NULL,
                                  LIMITE_MIN_X + x*dx + decalage - larg,
                                  LIMITE_MIN_X + x*dx + decalage + larg,
                                  LIMITE_MIN_Y + y*dy + decalage - larg,
                                  LIMITE_MIN_Y + y*dy + decalage + larg,
                                  NIVEAU_MER, NIVEAU_MER);
            num_obstacle++;
        }
    }
    
    
    // bateaux (ne rentre pas en collisions entre eux et avec les obstacles)
    for (i = 0; i < NB_BATEAUX; i++){
        
        nb_tentative = 0;
        while (1){
            copie_modele = copier_matrice(bateaux[i].o.modele);
            copie_direction = copier_matrice(bateaux[i].direction);
            copie_hitbox = copier_matrice(bateaux[i].o.hitbox);
            copie_hitbox2 = copier_matrice(bateaux[i].hitbox_canne);
            
            nb_tentative += 1;
            x = rand() % (LIMITE_MAX_X - LIMITE_MIN_X - 20) + LIMITE_MIN_X + 10;
            y = rand() % (LIMITE_MAX_Y - LIMITE_MIN_Y - 20) + LIMITE_MIN_Y + 10;
            trans_rot_z_alea_tout(&copie_modele, &copie_direction, &copie_hitbox, &copie_hitbox2, x, x, y, y, NIVEAU_MER, NIVEAU_MER);

            test_collision = 0;
            // collisions bateau - bateau
            for (j = 0; j < i && !test_collision; j++){
                test_collision = test_collision || collisions_AABB(copie_hitbox, bateaux[j].o.hitbox);
            }
            // collision bateau - obstacle
            for (j = 0; j < NB_OBSTACLES && !test_collision; j++){
                test_collision = test_collision || collisions_AABB(copie_hitbox, obstacles[j].o.hitbox);
            }
            
            // cas d'arrêt
            if (nb_tentative >= tentative_max || !test_collision){
                break;
            }
            else {
                liberer_matrice(copie_modele);
                liberer_matrice(copie_direction);
                liberer_matrice(copie_hitbox);
                liberer_matrice(copie_hitbox2);
            }
        } 
        
        liberer_matrice(bateaux[i].o.modele);
        bateaux[i].o.modele = copie_modele;
        liberer_matrice(bateaux[i].direction);
        bateaux[i].direction = copie_direction;
        liberer_matrice(bateaux[i].o.hitbox);
        bateaux[i].o.hitbox = copie_hitbox;
        liberer_matrice(bateaux[i].hitbox_canne);
        bateaux[i].hitbox_canne = copie_hitbox2;
    }

    
    // poissons, gérérer sans orientation vertical (ne rentre pas en collisions entre eux et avec les obstacles)
    for (i = 0; i < NB_POISSONS; i++){
        
        nb_tentative = 0;
        while (1) {
            copie_modele = copier_matrice(poissons[i].o.modele);
            copie_direction = copier_matrice(poissons[i].direction);
            copie_hitbox = copier_matrice(poissons[i].o.hitbox);
            
            nb_tentative += 1;
            x = rand() % (LIMITE_MAX_X - LIMITE_MIN_X - 20) + LIMITE_MIN_X + 10;
            y = rand() % (LIMITE_MAX_Y - LIMITE_MIN_Y - 20) + LIMITE_MIN_Y + 10;
            trans_rot_z_alea_tout(&copie_modele, &copie_direction, &copie_hitbox, NULL, x, x, y, y, NIVEAU_MER - 1, NIVEAU_MER - 6);

            test_collision = 0;
            // collision poisson - poisson
            for (j = 0; j < i && !test_collision; j++){
                test_collision = test_collision || collisions_AABB(poissons[i].o.hitbox, poissons[j].o.hitbox);
            }
            // collision poisson - obstacle
            for (j = 0; j < NB_OBSTACLES && !test_collision; j++){
                test_collision = test_collision || collisions_AABB(copie_hitbox, obstacles[j].o.hitbox);
            }
            // collision poisson - canne
            for (j = 0; j < NB_BATEAUX && !test_collision; j++){
                test_collision = test_collision || collisions_OBB(poissons[i].o.hitbox, bateaux[j].hitbox_canne);
            }
            
            // cas d'arrêt
            if (nb_tentative >= tentative_max || !test_collision){
                break;
            }
            else {
                liberer_matrice(copie_modele);
                liberer_matrice(copie_direction);
                liberer_matrice(copie_hitbox);
            }
        }
        
        liberer_matrice(poissons[i].o.modele);
        poissons[i].o.modele = copie_modele;
        liberer_matrice(poissons[i].direction);
        poissons[i].direction = copie_direction;
        liberer_matrice(poissons[i].o.hitbox);
        poissons[i].o.hitbox = copie_hitbox;
    }
}