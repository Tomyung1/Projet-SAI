#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/obstacle.h"
#include "headers/matrice.h"

obstacle creer_obstacle(){
    obstacle ob;
    
    ob.o.etat = E_INITIAL;

    ob.o.modele = lire_modele("objets/obstacle.obj");
    
    return ob;
}


void liberer_obstacle(obstacle ob){
    liberer_matrice(ob.o.modele);
}

