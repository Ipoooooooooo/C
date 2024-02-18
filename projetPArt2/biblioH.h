#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct livreh{
    int clef;
    int num;
    char *titre;
    char*auteur;
    struct livreh * suiv;
}LivreH;


typedef struct table { 
    int nE;/*Nombre d'élements contenus dans la table de hachage*/
    int m;/*taille de la table de hachage*/
    LivreH **T;/*table de hachage avec resolution des colisions*/
}BiblioH;
