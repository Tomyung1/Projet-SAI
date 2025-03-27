#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/poisson.h"
#include "headers/matrice.h"

poisson creer_poisson(){
    poisson p;
    
    p.o.etat = E_INITIAL;

    p.o.modele = lire_modele("objets/poisson.obj");
    
    return p;
}


void liberer_poisson(poisson p){
    liberer_matrice(p.o.modele);
}

