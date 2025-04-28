#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

// Structure pour représenter une règle de grammaire
typedef struct {
    char gauche; // Non-terminal à gauche de la règle
    char droite[20]; // Partie droite de la règle
} Regle;

Regle grammaire[MAX]; // Tableau pour stocker les règles
char first[26][20]; // Tableau pour stocker les ensembles PREMIER
char follow[26][20]; // Tableau pour stocker les ensembles SUIVANT
int nbRegles = 0; // Nombre de règles dans la grammaire

// Vérifie si un caractère est un non-terminal (majuscule)
int estNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

// Ajoute un symbole dans un ensemble (sans doublons)
void ajouter(char *ensemble, char symbole) {
    if (!strchr(ensemble, symbole)) { // Vérifie si le symbole est déjà présent
        int len = strlen(ensemble);
        ensemble[len] = symbole;
        ensemble[len + 1] = '\0';
    }
}

// Calcule l'ensemble PREMIER pour un symbole donné
void calculerPremier(char *ensemble, char symbole) {
    if (!estNonTerminal(symbole)) { // Si le symbole est un terminal
        ajouter(ensemble, symbole);
        return;
    }

    for (int i = 0; i < nbRegles; i++) {
        if (grammaire[i].gauche == symbole) { // Si la règle commence par le symbole
            char *rhs = grammaire[i].droite;

            // Si la production est epsilon seule
            if (rhs[0] == '#' && rhs[1] == '\0') {
                ajouter(ensemble, '#');
                continue;
            }

            int j = 0;
            int peutEtreEpsilon = 1;

            // Parcourt les symboles de la partie droite
            while (rhs[j] && peutEtreEpsilon) {
                char temp[20] = "";
                calculerPremier(temp, rhs[j]);

                peutEtreEpsilon = 0;
                for (int k = 0; temp[k]; k++) {
                    if (temp[k] == '#') {
                        peutEtreEpsilon = 1; // Continue si epsilon est trouvé
                    } else {
                        ajouter(ensemble, temp[k]);
                    }
                }
                j++;
            }

            if (peutEtreEpsilon) {
                ajouter(ensemble, '#');
            }
        }
    }
}

// Calcule l'ensemble SUIVANT pour un symbole donné
void calculerSuivant(char *ensemble, char symbole) {
    if (symbole == grammaire[0].gauche) { // Ajoute $ pour le symbole de départ
        ajouter(ensemble, '$');
    }

    for (int i = 0; i < nbRegles; i++) {
        char *rhs = grammaire[i].droite;
        for (int j = 0; rhs[j]; j++) {
            if (rhs[j] == symbole) { // Si le symbole est trouvé dans la partie droite
                if (rhs[j + 1]) { // Si un symbole suit
                    char temp[20] = "";
                    calculerPremier(temp, rhs[j + 1]);

                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '#') { // Ajoute les symboles sauf epsilon
                            ajouter(ensemble, temp[k]);
                        }
                    }

                    if (strchr(temp, '#')) { // Si epsilon est présent
                        if (grammaire[i].gauche != symbole) { // Évite les boucles infinies
                            char temp2[20] = "";
                            calculerSuivant(temp2, grammaire[i].gauche);
                            for (int k = 0; temp2[k]; k++) {
                                ajouter(ensemble, temp2[k]);
                            }
                        }
                    }
                } else { // Si le symbole est en fin de production
                    if (grammaire[i].gauche != symbole) { // Évite les boucles infinies
                        char temp2[20] = "";
                        calculerSuivant(temp2, grammaire[i].gauche);
                        for (int k = 0; temp2[k]; k++) {
                            ajouter(ensemble, temp2[k]);
                        }
                    }
                }
            }
        }
    }
}

// Ajoute une règle à la grammaire, même si elle contient des |
void ajouterRegle(char *ligne) {
    char gauche = ligne[0]; // Non-terminal à gauche
    char *rhs = strchr(ligne, '>') + 1; // Partie droite après >

    char *token = strtok(rhs, "|"); // Découpe les alternatives
    while (token) {
        grammaire[nbRegles].gauche = gauche;
        strcpy(grammaire[nbRegles].droite, token);
        nbRegles++;
        token = strtok(NULL, "|");
    }
}

// Programme principal
int main() {
    int r;
    printf("Nombre de règles : ");
    scanf("%d", &r);
    getchar(); // Consomme le \n après scanf

    // Lecture des règles
    for (int i = 0; i < r; i++) {
        char ligne[100];
        printf("Règle %d (ex : E->TR) : ", i + 1);
        fgets(ligne, sizeof(ligne), stdin);
        ligne[strcspn(ligne, "\n")] = '\0'; // Supprime le \n
        ajouterRegle(ligne);
    }

    // Calcul des ensembles PREMIER
    for (int i = 0; i < nbRegles; i++) {
        char nt = grammaire[i].gauche;
        if (first[nt - 'A'][0] == '\0') { // Si PREMIER(nt) n'est pas encore calculé
            calculerPremier(first[nt - 'A'], nt);
        }
    }

    // Calcul des ensembles SUIVANT
    for (int i = 0; i < nbRegles; i++) {
        char nt = grammaire[i].gauche;
        if (follow[nt - 'A'][0] == '\0') { // Si SUIVANT(nt) n'est pas encore calculé
            calculerSuivant(follow[nt - 'A'], nt);
        }
    }

    // Affichage des ensembles PREMIER
    printf("\n--- PREMIER ---\n");
    int deja[26] = {0};
    for (int i = 0; i < nbRegles; i++) {
        char nt = grammaire[i].gauche;
        if (!deja[nt - 'A']) { // Évite les doublons
            printf("PREMIER(%c) = { %s }\n", nt, first[nt - 'A']);
            deja[nt - 'A'] = 1;
        }
    }

    // Affichage des ensembles SUIVANT
    printf("\n--- SUIVANT ---\n");
    memset(deja, 0, sizeof(deja));
    for (int i = 0; i < nbRegles; i++) {
        char nt = grammaire[i].gauche;
        if (!deja[nt - 'A']) { // Évite les doublons
            printf("SUIVANT(%c) = { %s }\n", nt, follow[nt - 'A']);
            deja[nt - 'A'] = 1;
        }
    }

    return 0;
}
