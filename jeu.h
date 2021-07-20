#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define GAUCHE -1 // Ne pas modifier
#define DROITE -2 // Ne pas modifier
#define RETOURNER -3 // Ne pas modifier
#ifndef TAILLE_JEU
    #define TAILLE_JEU 7
#endif // TAILLE_JEU
#define CAR_VIDE ' '
#define CAR_J1 'O'
#define CAR_J2 'X'
#ifndef PROFONDEUR_MIN_MAX
    #define PROFONDEUR_MIN_MAX 4
#endif // PROFONDEUR_MIN_MAX
#if TAILLE_JEU < 4
    #error Le jeu ne peut pas avoir une taille inferieure a 4
#endif // TAILLE_JEU
#if PROFONDEUR_MIN_MAX < 0
    #error La profondeur du jeu ne peut pas etre negative
#endif // PROFONDEUR_MIN_MAX
#ifdef __GNUC__
    #if TAILLE_JEU > 10
        #warning La taille du jeu est assez grande : le bot sera plus lent
    #endif // TAILLE_JEU
    #if PROFONDEUR_MIN_MAX > 5
        #warning La profondeur min - max est assez elevee : le bot sera plus lent
    #endif // PROFONDEUR_MIN_MAX
    #if defined(__i386__)
        #warning Compilation en 32 bits detectee : il pourrait y avoir des erreurs si la profondeur min - max est trop elevee
    #endif
#endif // __GNUC__

typedef unsigned char **Plateau;
struct _Jeu
{
    Plateau p; // Pointeur sur pointeur sur unsigned char: tableau en 2D
    char *car_joueurs; // Les caracteres pour les joueurs: O, X et ' ' par exemple
    unsigned short prof;
};
typedef struct _Jeu Jeu;

void partie(char choix_bot, FILE *fichier_sauvegarde);
Jeu creer_jeu();
void afficher_jeu(Jeu jeu);
int jouer_colonne(Jeu jeu, unsigned char joueur, unsigned short num_colonne);
void pivoter_jeu(Jeu *jeu, short direction);
Jeu clone_jeu(Jeu jeu);
void supprimer_jeu(Jeu *jeu);
int gagnant(Jeu jeu);
int gagnant_simple(Jeu jeu, unsigned char *pos_x, unsigned char *pos_y);
short jouer_bot(Jeu *jeu, unsigned char joueur, int peut_retourner);
short jouer_joueur(Jeu *jeu, unsigned char joueur, int peut_retourner);
long long evaluer_action_min_max(Jeu jeu, short action, unsigned char joueur, unsigned char joueur_tour, unsigned short n);
void sauvegarder_et_quitter(Jeu *jeu, unsigned char joueur_tour);
int sauvegarder(Jeu jeu, unsigned char joueur_tour, char *nom_fichier);
void quitter(Jeu *jeu);
void decrire_action(short action);

long long MIN_PTS_POSSIBLE(Jeu jeu);

void viderBuffer();
int lire_str(char *chaine, int longueur);

#endif // JEU_H_INCLUDED
