#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/collision.h"
#include "headers/matrice.h"

/*
Renvoie 1 si les 2 hitbox se chevauche
*/
int collisions_AABB(matrice hitbox1, matrice hitbox2){
    double x_min_1 = get_mat(hitbox1, 0, 0);
    double y_min_1 = get_mat(hitbox1, 1, 0);
    double z_min_1 = get_mat(hitbox1, 2, 0);
    double x_max_1 = get_mat(hitbox1, 0, 1);
    double y_max_1 = get_mat(hitbox1, 1, 1);
    double z_max_1 = get_mat(hitbox1, 2, 1);

    double x_min_2 = get_mat(hitbox2, 0, 0);
    double y_min_2 = get_mat(hitbox2, 1, 0);
    double z_min_2 = get_mat(hitbox2, 2, 0);
    double x_max_2 = get_mat(hitbox2, 0, 1);
    double y_max_2 = get_mat(hitbox2, 1, 1);
    double z_max_2 = get_mat(hitbox2, 2, 1);

    return x_max_1 >= x_min_2 && x_min_1 <= x_max_2 &&
           y_max_1 >= y_min_2 && y_min_1 <= y_max_2 &&
           z_max_1 >= z_min_2 && z_min_1 <= z_max_2;
}


// calcul le produit croisé de deux axe, et le range dans le 3 ème
static void prod_croise(double axeA[3], double axeB[3], double axes[3]){
    axes[0] = axeA[1] * axeB[2] - axeA[2] * axeB[1];
    axes[1] = axeA[2] * axeB[0] - axeA[0] * axeB[2];
    axes[2] = axeA[0] * axeB[1] - axeA[1] * axeB[0];
}


// renvoie les 3 axes de chaque hitbox en pavé
static void recup_axes(double axes[15][3], matrice hitboxA, matrice hitboxB){
    int i, j, n = 0;
    double x0, y0, z0;
    
    // 3 axes de hitboxA
    x0 = get_mat(hitboxA, 0, 0);
    y0 = get_mat(hitboxA, 1, 0);
    z0 = get_mat(hitboxA, 2, 0);
    
    for (i = 1; i <= 3; i++){
        axes[n][0] = get_mat(hitboxA, 0, i) - x0;
        axes[n][1] = get_mat(hitboxA, 1, i) - y0;
        axes[n][2] = get_mat(hitboxA, 2, i) - z0;

        n++;
    }

    
    // 3 axes de hitboxA
    x0 = get_mat(hitboxB, 0, 0);
    y0 = get_mat(hitboxB, 1, 0);
    z0 = get_mat(hitboxB, 2, 0);
    
    for (i = 1; i <= 3; i++){
        axes[n][0] = get_mat(hitboxB, 0, i) - x0;
        axes[n][1] = get_mat(hitboxB, 1, i) - y0;
        axes[n][2] = get_mat(hitboxB, 2, i) - z0;

        n++;
    }

    // 9 axes croisés
    for (i = 0; i <= 2; i++){  // Pour A
        
        for (j = 3; j <= 5; j++){  // Pour B

            prod_croise(axes[i], axes[j], axes[n]);

            n++;
        }
    }
}


// optimisation : ne pas calculer tout les points pour hitboxA et interchanger A et B dans collisions_OBB
// renvoie s'il y a collision entre les deux hitbox projeter sur axe
static int collision_projection(double axe[3], matrice hitboxA, matrice hitboxB){
    double min1, max1, min2, max2;
    double x1, y1, z1, x2, y2, z2;
    double prod_scalaire;
    int i;
    
    x1 = axe[0];
    y1 = axe[1];
    z1 = axe[2];
    
    // calcul du produit scalaire de tout les sommets de la hitboxA
    // premier
    x2 = get_mat(hitboxA, 0, 0);
    y2 = get_mat(hitboxA, 1, 0);
    z2 = get_mat(hitboxA, 2, 0);

    prod_scalaire = x1 * x2 + y1 * y2 + z1 * z2;
    min1 = prod_scalaire;
    max1 = prod_scalaire;

    // autres
    for (i = 1; i < 8; i++){
        x2 = get_mat(hitboxA, 0, i);
        y2 = get_mat(hitboxA, 1, i);
        z2 = get_mat(hitboxA, 2, i);

        prod_scalaire = x1 * x2 + y1 * y2 + z1 * z2;

        if (prod_scalaire > max1){
            max1 = prod_scalaire;
        }
        else if (prod_scalaire < min1){
            min1 = prod_scalaire;
        }
    }

    // calcul du produit scalaire de tout les sommets de la hitboxB
    x2 = get_mat(hitboxB, 0, 0);
    y2 = get_mat(hitboxB, 1, 0);
    z2 = get_mat(hitboxB, 2, 0);

    prod_scalaire = x1 * x2 + y1 * y2 + z1 * z2;
    min2 = prod_scalaire;
    max2 = prod_scalaire;

    for (i = 1; i < 8; i++){
        x2 = get_mat(hitboxB, 0, i);
        y2 = get_mat(hitboxB, 1, i);
        z2 = get_mat(hitboxB, 2, i);

        prod_scalaire = x1 * x2 + y1 * y2 + z1 * z2;

        if (prod_scalaire > max2){
            max2 = prod_scalaire;
        }
        else if (prod_scalaire < min2){
            min2 = prod_scalaire;
        }
    }
    

    // Est se qu'ils se superpose ?
    return max1 >= min2 && min1 <= max2;
}

int collisions_OBB(matrice hitboxA, matrice hitboxB){
    double axes[15][3];
    int axe = 0;

    recup_axes(axes, hitboxA, hitboxB);
    
    while (axe < 15 && collision_projection(axes[axe], hitboxA, hitboxB)){
        axe++;
    }

    return axe >= 15;
}