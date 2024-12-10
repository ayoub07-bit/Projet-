#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct article {
    char nom[50];
    int quantite;
    float prix;
} article;

article stock[50];
int nbarticles = 0;

typedef struct noeud {
    int val;
    struct noeud* suiv;
} noeud;

typedef struct file {
    noeud* tete;
    noeud* queue;
} file;


int main() {
    file fileclients;
    initialiserfile(&fileclients);
    menuPrincipal(&fileclients);
    return 0;
}

void menuPrincipal(file* file) {
    int choix;
    do {
        printf("\n--- Application Café ---\n");
        printf("1. Administration\n");
        printf("2. Client\n");
        printf("3. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                gestionadministration(file);
                break;
            case 2:
                gestionclient(file);
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 3);
}

void gestionadministration(file* file) {
    int choix;
    char code[4];
    printf("Entrer le code: ");
    scanf("%s",&code);
    while(strcmp(code, "0000") != 0){
        printf("Code erroné, réssayer une autre fois: ");
        scanf("%s",&code);
    }
    do {
        printf("\n--- Administration ---\n");
        printf("1. Afficher le stock\n");
        printf("2. Ajouter un article\n");
        printf("3. Mettre à jour un article\n");
        printf("4. Afficher la file des clients\n");
        printf("5. Servir un client (FIFO)\n");
        printf("6. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherstock();
                break;
            case 2:
                ajouterarticle();
                break;
            case 3:
                mettreajourstock();
                break;
            case 4:
                afficherfile(file);
                break;
            case 5:
                defiler(file);
                break;
            case 6:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 6);
}

void afficherstock() {
    printf("\n--- Stock Actuel ---\n");
    for (int i = 0; i < nbarticles; i++) {
        printf("%d. %s - Quantité : %d - Prix : %.2f\n", i + 1, stock[i].nom, stock[i].quantite, stock[i].prix);
    }
}

void ajouterarticle() {
    if (nbarticles >= 50) {
        printf("Le stock est plein !\n");
        return;
    }

    printf("Nom de l'article : ");
    scanf("%s", stock[nbarticles].nom);
    printf("Quantité initiale : ");
    scanf("%d", &stock[nbarticles].quantite);
    printf("Prix unitaire : ");
    scanf("%f", &stock[nbarticles].prix);

    nbarticles++;
    printf("article ajouté avec succès !\n");
}

void mettreajourstock() {
    int choix, quantite;
    afficherstock();
    printf("Choisissez un article à mettre à jour (numéro) : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > nbarticles) {
        printf("Numéro invalide.\n");
        return;
    }

    printf("Nouvelle quantité pour %s : ", stock[choix - 1].nom);
    scanf("%d", &quantite);
    stock[choix - 1].quantite = quantite;
    printf("Mise à jour réussie !\n");
}

void gestionclient(file* file) {
    int choix, quantite, numTable;
    afficherstock();
    printf("Entrer votre numéro de table: ");
    scanf("%d", &numTable);
    printf("Choisissez un article à commander (numéro) : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > nbarticles) {
        printf("Numéro invalide.\n");
        return;
    }

    printf("Quantité pour %s : ", stock[choix - 1].nom);
    scanf("%d", &quantite);

    if (quantite > stock[choix - 1].quantite) {
        printf("Désolé, stock insuffisant.\n");
        return;
    }
    stock[choix - 1].quantite -= quantite;
    printf("Commande enregistrée avec succès !\n");

    enfiler(file, numTable);
}

void initialiserfile(file* file) {
    file->tete = NULL;
    file->queue = NULL;
}

void enfiler(file* file, int numtable) {
    noeud* nouveauClient = (noeud*)malloc(sizeof(noeud));
    nouveauClient->val = numtable;
    nouveauClient->suiv = NULL;

    if (file->queue == NULL) {
        file->tete = file->queue = nouveauClient;
    } else {
        file->queue->suiv = nouveauClient;
        file->queue = nouveauClient;
    }

}

void defiler(file* file) {
    if (file->tete == NULL) {
        printf("Aucun client à servir.");
        return;
    }

    noeud* clientServi = file->tete;
    printf("Client %d est servi.\n", clientServi->val);

    file->tete = clientServi->suiv;

    if (file->tete == NULL) {
        file->queue = NULL;
    }

    free(clientServi);
}

void afficherfile(file* file) {
    if (file->tete == NULL) {
        printf("La file est vide.\n");
        return;
    }

    noeud* courant = file->tete;
    printf("\n--- file des Clients ---\n");
    while (courant != NULL) {
        printf("Client à table %d\n", courant->val);
        courant = courant->suiv;
    }
}
