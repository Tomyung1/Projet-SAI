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
