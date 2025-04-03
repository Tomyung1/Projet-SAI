#include <stdio.h>
#include <stdlib.h>

#include "headers/config.h"
#include "headers/objet.h"
#include "headers/bateau.h"
#include "headers/matrice.h"

bateau creer_bateau(){
    bateau b;
    
    b.o.etat = E_INITIAL;

    b.o.modele = lire_modele("objets/bateau.obj");
    
    return b;
}


void liberer_bateau(bateau b){
    liberer_matrice(b.o.modele);
}

