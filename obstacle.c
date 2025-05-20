#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/obstacle.h"
#include "headers/matrice.h"

obstacle creer_obstacle(){
    obstacle ob;
    
    ob.o.etat = E_INITIAL;

    lire_modele("objets/obstacle.obj", &ob.o.modele, &ob.o.hitbox, NULL);
    
    return ob;
}


void liberer_obstacle(obstacle ob){
    liberer_matrice(ob.o.modele);
}

