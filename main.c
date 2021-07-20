#include "jeu.h"

/**
ATTENTION !!!
Utilise la bibliotheque math.h
Sous gcc compiler avec -lm
Pour rendre le joueur autonome plus ou moins "intelligent", utiliser l'ooption -DPROFONDEUR_MIN_MAX=[profondeur]
Pour modifier la taille du jeu: -DTAILLE_JEU=[taille] (7 par defaut)
*/

int main()
{
    char choix_bot='\0';
    printf("PUISSANCE 4\n\nProfondeur: %d\n\n", PROFONDEUR_MIN_MAX);
    while(choix_bot != 'B' && choix_bot != 'b' && choix_bot != 'J' && choix_bot != 'j' && choix_bot != 'P' && choix_bot != 'p' && choix_bot != 'A' && choix_bot != 'a') // Choisir entre Le bot commence, le joueur commence ou joueur vs joueur
    {
        printf("B: Le bot commence\nJ: Le joueur commence\nP: Pas de bot\nA: Bot vs bot\nChoix: ");
        scanf(" %c", &choix_bot);
        if(choix_bot != 'B' && choix_bot != 'J' && choix_bot != 'P' && choix_bot != 'A')
        {
            choix_bot=choix_bot - 'a' + 'A';
        }
    }
    printf("Lire sauvegarde ? (Y/N): "); // Doit-on aller chercher une sauvegarde ?
    char choix_sauvegarde='\0';
    scanf(" %c", &choix_sauvegarde);
    FILE *fichier_sauvegarde=NULL; // NULL par defaut
    if(choix_sauvegarde == 'Y' || choix_sauvegarde == 'y')
    {
        do
        {
            char nom_fichier[100]={0};
            printf("Fichier: ");
            while(nom_fichier[0] == '\0')
            {
                lire_str(nom_fichier, 100);
            }
            fichier_sauvegarde=fopen(nom_fichier, "r");
        } while(!fichier_sauvegarde); // Tant qu'on n'arrive pas à ouvrir le fichier
    }
    partie(choix_bot, fichier_sauvegarde); // On lance la partie
    return 0;
}
