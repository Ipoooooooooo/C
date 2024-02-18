#include "biblioH.h"


int fonctionClef(char *auteur){
    int sum=0;
    int i=0;
    while(auteur[i]!='\0')  // while loop  
    {
        sum=sum+auteur[i];  
        i++;  
    }
    return sum;

}

LivreH * creer_livre(int num , char * titre ,char * auteur){
    LivreH * res = (LivreH *)malloc(sizeof(LivreH));
    res->clef=fonctionClef(auteur);
    res->num = num;
    res->titre = strdup(titre);
    res->auteur = strdup(auteur);
    res->suiv = NULL;
    return res;
}
void liberer_livre(LivreH *l) {
    if (l == NULL) {
        return; // Handle NULL pointer gracefully
    }

    LivreH *tmp;
    while (l != NULL) {
        tmp = l->suiv;
        free(l->titre);
        free(l->auteur);
        free(l);
        l = tmp;
    }
}
#include <stdlib.h>

BiblioH *creer_biblio(int m) {
    BiblioH *biblio = (BiblioH *)malloc(sizeof(BiblioH));

    if (!biblio) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la bibliothèque.\n");
        exit(1);  // Ou gérer l'erreur de manière appropriée
    }

    biblio->nE = 0;  // Initialiser le nombre d'éléments à 0
    biblio->m = m;   // Initialiser la taille de la table de hachage
    biblio->T = (LivreH **)malloc(m * sizeof(LivreH *));

    if (!biblio->T) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la table de hachage.\n");
        free(biblio);
        exit(1);  // Ou gérer l'erreur de manière appropriée
    }
    // Initialiser la table de hachage à NULL
    for (int i = 0; i < m; i++) {
        biblio->T[i] = NULL;
    }
    return biblio;
}


// Libère la mémoire allouée pour une table de hachage de livres (BiblioH)

void liberer_biblio(BiblioH *b) {
    // Vérifier si la structure BiblioH existe
    if (b == NULL) {
        return; // Rien à libérer si l'entrée est NULL
    }

    // Libérer les listes chaînées dans chaque emplacement du tableau
    for (int i = 0; i < b->m; i++) {
        LivreH *actuel = b->T[i];
        while (actuel != NULL) {
            LivreH *temp = actuel;
            actuel = actuel->suiv;
            // Libérer la mémoire allouée pour les chaînes de caractères titre et auteur
            free(temp->titre);
            free(temp->auteur);
            // Libérer la mémoire du livre lui-même
            free(temp);
        }
    }
    // Libérer le tableau de pointeurs
    free(b->T);
    // Libérer la structure BiblioH
    free(b);
}


int fonctionHachage(int cle , int m){
    int res = (int)floor(m*(cle*(sqrt(5)-1/2)-floor(cle*(sqrt(5)-1/2))));
    return res;

}
//Fonctionh(clef)= i 
//i est l'indice ou ce situe la clef
//si à tab[i] il ya un élément alors on l'ajoute a suite 
// sinon on ajpute à tab[i] notre clef avec sa valeur evidement
void inserer(BiblioH *b, int num, char *titre, char *auteur) {
    // Déterminer l'indice de la case de hachage
    int cle = fonctionClef(auteur);
    int indice = fonctionHachage(cle, b->m);

    // Créer un nouveau livre
    LivreH *livre = creer_livre(num, titre, auteur);

    // Insérer le livre en tête de la liste chaînée
    b->nE+=1;
    LivreH *tmp = b->T[indice];
    livre->suiv = tmp;
    b->T[indice] = livre;
}
void afficherLH(LivreH *l){

    if (l != NULL) {
        printf("Numéro : %d Titre : %s Auteur : %s\n", l->num, l->titre, l->auteur);
    } 
    else {
        printf("Il n'y a pas de livre\n");
    }

}
void afficherBH(BiblioH *b) {
    // Vérification des erreurs et cas particuliers :
    if (b == NULL) {
        printf("**Erreur**: Bibliothèque est vide.\n");
        return; // Quitte la fonction si la bibliothèque est vide
    } else if (b->nE == 0) {
        printf("**Info**: Bibliothèque ne contient aucun livre.\n");
        return; // Quitte la fonction s'il n'y a aucun livre
    }

    // Parcours de tous les livres de la bibliothèque :
    for (int i = 0; i < b->m; i++) {
        // Obtention du premier livre dans le bucket actuel :
        LivreH *current = b->T[i];

        // Gestion des buckets vides :
        if (current == NULL) {
            continue; // Passe au bucket suivant s'il est vide
        }
        // Parcours de la liste chaînée de livres dans le bucket :
        while (current != NULL) {
            // Affichage des informations du livre :
            printf("--------------------\n");
            printf("Clé : %d\n", current->clef);
            printf("Numéro : %d\n", current->num);
            printf("Titre : %s\n", current->titre);
            printf("Auteur : %s\n", current->auteur);
            printf("--------------------\n");

            // Passage au livre suivant dans la liste :
            current = current->suiv;
        }
    }

    printf("Fin de l'affichage.\n");
}



#include <stdio.h>

BiblioH *charger_n_entrees(char *nomfic, int n) {
    // Vérification des entrées
    if (!nomfic || n <= 0) {
        fprintf(stderr, "**Erreur**: Arguments invalides.\n");
        return NULL;
    }

    // Ouverture du fichier
    FILE *fp = fopen(nomfic, "r");
    if (!fp) {
        fprintf(stderr, "**Erreur**: Impossible d'ouvrir le fichier '%s'.\n", nomfic);
        return NULL;
    }

    // Création de la BiblioH et allocation des structures
    BiblioH *b = creer_biblio(n);

    // Allocation du tableau de pointeurs
    b->T = (LivreH **)malloc(b->m * sizeof(LivreH *));
    if (!b->T) {
        fprintf(stderr, "**Erreur**: Allocation mémoire échouée.\n");
        liberer_biblio(b);
        fclose(fp);
        return NULL;
    }

    // Initialisation de la table de hachage
    for (int i = 0; i < b->m; i++) {
        b->T[i] = NULL;
    }

    // Lecture et insertion des entrées
    int ent = 0;
    while (ent < n) {
        // Lecture des données du fichier
        int num;
        char titre[256], auteur[256];

        if (fscanf(fp,"%d %s %s", &num, titre, auteur) != 3) {
            fprintf(stderr, "Erreur de lecture des données du fichier.\n");
            return NULL;
        }
        ent++;
    }
    fclose(fp);
    return b;
}
#include <stdio.h>

void enregistrer_biblio(BiblioH *b, char *nomfic) {
    // Vérification des erreurs
    if (b == NULL || nomfic == NULL) {
        fprintf(stderr, "Erreur : Bibliothèque ou nom de fichier invalide.\n");
        return;
    }
    FILE *fp = fopen(nomfic, "w");
    if (!fp) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s' en écriture.\n", nomfic);
        return;
    }
    // Parcours de tous les livres de la bibliothèque
    for (int i = 0; i < b->m; i++) {
        LivreH *current = b->T[i];
        while (current != NULL) {
            fprintf(fp, "%d %d %s %s\n", current->clef, current->num, current->titre, current->auteur);
            current = current->suiv;
        }
    }
    fclose(fp);
}

LivreH *rechercher_par_numero(BiblioH *b, int numero) {
    // Vérification des erreurs
    if (b == NULL) {
        fprintf(stderr, "Erreur : Bibliothèque invalide.\n");
        return NULL;
    }
    for (int i = 0; i < b->m; i++) {
        LivreH *current = b->T[i];
        while (current != NULL) {
            if (current->num == numero) {
                return current;
            }
            current = current->suiv;
        }
    }
    return NULL;
}











int main(){

    BiblioH *biblio = charger_n_entrees("Biblio.txt", 6);
    BiblioH *b=  creer_biblio(12);
    inserer(b,0,"EcceHomo","Nieztche");
    inserer(b,1,"Terre des hommes","Saint-Exupery");
    afficherBH(biblio);
    enregistrer_biblio(b,"test.txt");
    //afficherBH(b);
    liberer_biblio(b);
    liberer_biblio(biblio);
    //BiblioH *b = charger_n_entrees("Biblio.txt",3);
    return 0;
}