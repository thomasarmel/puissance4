#include "jeu.h"

void partie(char choix_bot, FILE *fichier_sauvegarde) // Fonction princpale, qui gere toute la partie
{
    srand(time(NULL)); // Initialise l'aleatoire
    short (*fonctions_joueurs[2])(Jeu*, unsigned char, int); // Des pointeurs sur des fonctions. Le joueur 1 a sa fonction, le joueur 2 aussi
    switch(choix_bot)
    {
    case 'A':
        fonctions_joueurs[0]=jouer_bot;
        fonctions_joueurs[1]=jouer_bot;
        break;
    case 'B':
        fonctions_joueurs[0]=jouer_bot;
        fonctions_joueurs[1]=jouer_joueur;
        break;
    case 'J':
        fonctions_joueurs[0]=jouer_joueur;
        fonctions_joueurs[1]=jouer_bot;
        break;
    default:
        fonctions_joueurs[0]=jouer_joueur;
        fonctions_joueurs[1]=jouer_joueur;
        break;
    }
    Jeu jeu=creer_jeu(); // On cree un jeu
    int _gagnant=0;
    int joueur=0; // joueur varie entre 0 et 1. On ajoute 1 ensuite pour qu'il varie entre 1 et 2
    if(fichier_sauvegarde) // Si on a demande a lire une sauvegarde
    {
        unsigned char i=0, j=0;
        int c;
        while(!feof(fichier_sauvegarde)) // On remplit le jeu avec les caracteres qu'on trouve. Pas de verification que le fichier est bon
        {
            c=fgetc(fichier_sauvegarde);
            if(c=='0' || c=='1' || c=='2')
            {
                if(i==TAILLE_JEU)
                {
                    joueur=c-'1';
                    break;
                }
                jeu.p[i][j]=c-'0';
                j++;
                if(j==TAILLE_JEU)
                {
                    i++;
                    j=0;
                }
            }
        }
        fclose(fichier_sauvegarde);
    }
    int derniere_action=0;
    while((_gagnant=gagnant(jeu)) == 0) // Tant que personne n'a gagne
    {
        afficher_jeu(jeu); // Afficher le jeu
        derniere_action = (*fonctions_joueurs[joueur])(&jeu, (unsigned char)(joueur+1), (derniere_action != RETOURNER)); // Jouer la fonction associee au joueur courant
        joueur=1-joueur; // Changer de joueur
    }
    printf("Jeu termine\n");
    afficher_jeu(jeu);
    if(_gagnant == -1) // Si la grille est pleine et que personne n'a aligne 4 pions
    {
        printf("Egalite\n");
    }
    else
    {
        printf("Gagnant: %c\n", jeu.car_joueurs[_gagnant]);
    }
    supprimer_jeu(&jeu); // On nettoie le tas
}

Jeu creer_jeu()
{
    Jeu jeu;
    jeu.p=malloc(TAILLE_JEU*sizeof(unsigned char*)); // Cree la premiere dimension
    jeu.car_joueurs=(char*)malloc(4*sizeof(char)); // Pour donner les bons symboles aux bons joueurs
    jeu.car_joueurs[0]=CAR_VIDE;
    jeu.car_joueurs[1]=CAR_J1;
    jeu.car_joueurs[2]=CAR_J2;
    jeu.car_joueurs[3]='\0'; // Sinon erreur avec strcpy
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        jeu.p[i]=(unsigned char*)malloc(TAILLE_JEU*sizeof(unsigned char)); // Cree la 2nde dimension
        memset(jeu.p[i], 0, TAILLE_JEU); // Initialise toutes les cases a vide
    }
    jeu.prof=PROFONDEUR_MIN_MAX;
    return jeu;
}

void afficher_jeu(Jeu jeu)
{
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        printf(" _");
    }
    printf("\n\n");
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        for(unsigned char j=0; j<TAILLE_JEU; j++)
        {
            printf("|%c", jeu.car_joueurs[jeu.p[i][j]]); // Affiche les symboles correspondant a chaque joueur (J1, J2 ou vide)
        }
        printf("|\n");
    }
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        printf(" %d", (int)(i+1));
    }
    printf("\n");
}

int jouer_colonne(Jeu jeu, unsigned char joueur, unsigned short num_colonne)
{
    for(unsigned char i=0; i<TAILLE_JEU; i++) // Pour chaque ligne
    {
        if(jeu.p[i][num_colonne] != 0) // Si la case n'est pas vide
        {
            if(i==0) // Si c'est la premiere case, alors la colonne est pleine
            {
                return 0; // Donc coup impossible
            }
            else // Sinon
            {
                jeu.p[i-1][num_colonne]=joueur; // On joue sur la case au dessus qui est forcement vide
                return 1;
            }
        }
    }
    jeu.p[TAILLE_JEU-1][num_colonne]=joueur; // Si aucune case n'est pleine, on joue sur la case tout en bas
    return 1;
}

/*
Travailler sur le meme jeu aurait ete fastidieux
On cree donc un nouveau jeu, en remettant tous les pions dans l'ordre aux nouveaux emplacements
Enfin on libere la memoire de l'ancien jeu
*/
void pivoter_jeu(Jeu *jeu, short direction)
{
    Jeu nouveau_jeu=creer_jeu();
    Jeu ancien_jeu=*jeu;
    *jeu=nouveau_jeu;
    if(direction == GAUCHE)
    {
        for(unsigned char i=0; i<TAILLE_JEU; i++)
        {
            for(unsigned char j=0; j<TAILLE_JEU; j++)
            {
                jouer_colonne(*jeu, ancien_jeu.p[i][j], i);
            }
        }
    }
    else if(direction == DROITE)
    {
        for(unsigned char i=0; i<TAILLE_JEU; i++)
        {
            for(short j=(TAILLE_JEU-1); j>=0; j--)
            {
                jouer_colonne(*jeu, ancien_jeu.p[i][j], (TAILLE_JEU-1)-i);
            }
        }
    }
    else if(direction == RETOURNER)
    {
        for(unsigned char i=0; i<TAILLE_JEU; i++)
        {
            for(unsigned char j=0; j<TAILLE_JEU; j++)
            {
                jouer_colonne(*jeu, ancien_jeu.p[i][j], j);
            }
        }
    }
    supprimer_jeu(&ancien_jeu);
}

Jeu clone_jeu(Jeu jeu) // Utile pour min-max: cree un nouveau jeu et remet les memes valeurs aux cases
{
    Jeu nouveau_jeu=creer_jeu();
    strcpy(nouveau_jeu.car_joueurs, jeu.car_joueurs);
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        for(unsigned char j=0; j<TAILLE_JEU; j++)
        {
            nouveau_jeu.p[i][j]=jeu.p[i][j];
        }
    }
    return nouveau_jeu;
}


void supprimer_jeu(Jeu *jeu)
{
    for(int i=0; i<TAILLE_JEU; i++)
    {
        free(jeu->p[i]); // Supprime chaque ligne
    }
    free(jeu->p); // Suprime le tableau de lignes
    free(jeu->car_joueurs); // Suprime les caracteres
} // NB: on suppose ici que jeu a ete instancie sur la pile. Si ce n'est pas le cas, dans la fonction appelante, ajouter free(jeu);

int gagnant(Jeu jeu)
{
    unsigned char pos_x, pos_y;
    int _gagnant=gagnant_simple(jeu, &pos_x, &pos_y);
    if(_gagnant == 1 || _gagnant==2)
    {
        jeu.p[pos_x][pos_y]=0;
        unsigned char tmp;
        int g=_gagnant-1;
        int _gagnant2=gagnant_simple(jeu, &tmp, &tmp);
        jeu.p[pos_x][pos_y]=_gagnant;
        int g2=_gagnant2-1;
        if(1-g == g2)
        {
            return -1;
        }
    }
    return _gagnant;
}

int gagnant_simple(Jeu jeu, unsigned char *pos_x, unsigned char *pos_y)
{
    for(unsigned char i=0; i<TAILLE_JEU; i++) // lignes
    {
        for(unsigned char j=3; j<TAILLE_JEU; j++)
        {
            if(jeu.p[i][j] != 0 && jeu.p[i][j] == jeu.p[i][j-1] && jeu.p[i][j] == jeu.p[i][j-2] && jeu.p[i][j] == jeu.p[i][j-3])
            {
                *pos_x=i;
                *pos_y=j;
                return jeu.p[i][j];
            }
        }
    }
    for(unsigned char i=0; i<TAILLE_JEU; i++) // colonnes
    {
        for(unsigned char j=3; j<TAILLE_JEU; j++)
        {
            if(jeu.p[j][i] != 0 && jeu.p[j][i] == jeu.p[j-1][i] && jeu.p[j][i] == jeu.p[j-2][i] && jeu.p[j][i] == jeu.p[j-3][i])
            {
                *pos_x=j;
                *pos_y=i;
                return jeu.p[j][i];
            }
        }
    }
    for(unsigned char i=3; i<TAILLE_JEU; i++) // diagonale descendante
    {
        for(unsigned char j=3; j<TAILLE_JEU; j++)
        {
            if(jeu.p[i][j] != 0 && jeu.p[i][j] == jeu.p[i-1][j-1] && jeu.p[i][j] == jeu.p[i-2][j-2] && jeu.p[i][j] == jeu.p[i-3][j-3])
            {
                *pos_x=i;
                *pos_y=j;
                return jeu.p[i][j];
            }
        }
    }
    for(unsigned char i=3; i<TAILLE_JEU; i++) // diagonale montante
    {
        for(unsigned char j=0; j<TAILLE_JEU-3; j++)
        {
            if(jeu.p[i][j] != 0 && jeu.p[i][j] == jeu.p[i-1][j+1] && jeu.p[i][j] == jeu.p[i-2][j+2] && jeu.p[i][j] == jeu.p[i-3][j+3])
            {
                *pos_x=i;
                *pos_y=j;
                return jeu.p[i][j];
            }
        }
    }
    for(unsigned char i=0; i<TAILLE_JEU; i++) // Il reste de la place ?
    {
        if(jeu.p[0][i] == 0)
        {
            return 0;
        }
    }
    return -1; // Jeu rempli
}

short jouer_bot(Jeu *jeu, unsigned char joueur, int peut_retourner)
{
    printf("Au tour du bot: ");
    short max_action=0;
    long long max_points=MIN_PTS_POSSIBLE(*jeu); // max vaut avant la boucle le nombre minimum que evaluer_action_min_max() pourra renvoyer
    for(short i=-3; i<TAILLE_JEU; i++) // Pour chaque action (RETOURNER, GAUCHE, DROITE, 0, 1, 2...)
    {
        if(!peut_retourner && i == RETOURNER)
        {
            continue;
        }
        long long pts_action=evaluer_action_min_max(*jeu, i, joueur, joueur, jeu->prof); // Combien de "points" vaut l'action
        if(pts_action > max_points || (pts_action == max_points && rand()%2 == 0)) // Pour avoir la meilleure action possible. On ajoute une petite part d'aleatoire si 2 actions sont equivalentes
        {
            max_points=pts_action;
            max_action=i;
        }
    }
    decrire_action(max_action);
    printf("\n");
    switch(max_action) // Le bot joue la meilleure action determinee precedemment
    {
    case GAUCHE:
        pivoter_jeu(jeu, GAUCHE);
        break;
    case DROITE:
        pivoter_jeu(jeu, DROITE);
        break;
    case RETOURNER:
        pivoter_jeu(jeu, RETOURNER);
        break;
    default:
        if(max_action >=0 && max_action < TAILLE_JEU)
        {
            jouer_colonne(*jeu, joueur, (unsigned short)max_action);
        }
        break;
    }
    return max_action;
} // NB: La valeur de chaque action est codee sur un long long. Theoriquement, sur un systeme 32 bits, avec une trop grande profondeur, cela poserait probleme. Cependant, comme il est difficile de joueur avec une profondeur >=6, on peut considerer que ce n'est pas un probleme

short jouer_joueur(Jeu *jeu, unsigned char joueur, int peut_retourner)
{
    printf("Au tour du joueur %d:\n", (int)joueur);
    int mouvement_valide=0;
    short action=0;
    while(!mouvement_valide) // Tant que le joueur n'a pas entre un mouvement valide
    {
        mouvement_valide=1;
        char choix_joueur;
        printf("S: sauvegarder et quitter\nQ: Quitter\n");
        printf("+/-: Augmenter/Diminuer la difficulte\n");
        printf("Entrer la colonne (entre 1 et %d), G, D ou R: ", TAILLE_JEU);
        scanf(" %c", &choix_joueur);
        if(choix_joueur=='Q' || choix_joueur=='q') // Quitter
        {
            quitter(jeu);
        }
        if(choix_joueur=='S' || choix_joueur=='s') // Sauvegarder et quitter
        {
            sauvegarder_et_quitter(jeu, joueur);
        }
        if(choix_joueur=='G' || choix_joueur=='g') // Pivoter a gauche
        {
            pivoter_jeu(jeu, GAUCHE);
            action=GAUCHE;
        }
        else if(choix_joueur=='D' || choix_joueur=='d') // Pivoter a droite
        {
            pivoter_jeu(jeu, DROITE);
            action=DROITE;
        }
        else if(choix_joueur=='R' || choix_joueur=='r') // Retourner
        {
            if(!peut_retourner)
            {
                printf("Impossible de retourner 2 fois de suite\n");
                mouvement_valide=0;
            }
            else
            {
                pivoter_jeu(jeu, RETOURNER);
                action=RETOURNER;
            }
        }
        else if(choix_joueur > '0' && choix_joueur <= TAILLE_JEU+'0') // Mettre un pion dans une colonne
        {
            action=choix_joueur-'0'-1;
            if(!jouer_colonne(*jeu, joueur, (unsigned short)action))
            {
                printf("Cette colonne est deja pleine\n");
                mouvement_valide=0;
            }
        }
        else if(choix_joueur == '+') // Augmenter la difficulte
        {
            if(jeu->prof == TAILLE_JEU*TAILLE_JEU)
            {
                printf("Vous avez atteint la profondeur maximale\n");
            }
            else
            {
                jeu->prof=jeu->prof+1;
                printf("Profondeur: %hu\n", jeu->prof);
            }
            mouvement_valide=0;
        }
        else if(choix_joueur == '-') // Diminuer la difficulte
        {
            if(jeu->prof == 0)
            {
                printf("Vous avez atteint la profondeur minimale\n");
            }
            else
            {
                jeu->prof=jeu->prof-1;
                printf("Profondeur: %hu\n", jeu->prof);
            }
            mouvement_valide=0;
        }
        else
        {
            printf("Veuillez entrer un mouvement valide.\n");
            mouvement_valide=0;
        }
    }
    return action;
}

long long evaluer_action_min_max(Jeu jeu, short action, unsigned char joueur, unsigned char joueur_tour, unsigned short n)
{
    long long min_possible=MIN_PTS_POSSIBLE(jeu);
    Jeu jeu_clone=clone_jeu(jeu); // On "travaille" sur un clone du jeu passe en parametre. On aurait tout aussi bien pu annuler l'action mais vu qu'il est possible de pivoter le jeu ca aurait ete complique
    int nb=TAILLE_JEU+4;
    switch(action) // Action passee en parametre
    {
    case GAUCHE:
        pivoter_jeu(&jeu_clone, GAUCHE);
        break;
    case DROITE:
        pivoter_jeu(&jeu_clone, DROITE);
        break;
    case RETOURNER:
        pivoter_jeu(&jeu_clone, RETOURNER);
        break;
    default:
        if(action>=0 && action < TAILLE_JEU)
        {
            if(!jouer_colonne(jeu_clone, joueur_tour, (unsigned short)action)) // Si la colonne est deja pleine
            {
                supprimer_jeu(&jeu_clone);
                return min_possible; // Renvoyer -infini: ce coup n'est pas possible
            }
        }
        break;
    }
    int _gagnant=gagnant(jeu_clone); // On teste les resultats du coup simule
    if(_gagnant == -1) // Egalite ?
    {
        supprimer_jeu(&jeu_clone);
        return 0;
    }
    if(_gagnant == 0) // Si le jeu n'est pas fini apres la simulation
    {
        if(n==0) // Si on arrive a la profondeur max des simulations
        {
            supprimer_jeu(&jeu_clone);
            return 0; // On considere ca comme une egalite pour le moment
        } // Sinon
        long long total=(joueur == joueur_tour) ? -min_possible : 0;
        int j=joueur_tour-1;
        j=1-j;
        j=j+1;
        for(short i=-3; i<TAILLE_JEU; i++) // On lance une nouvelle simulation de tous les coups possibles avec le joueur adverse
        {
            if(action == RETOURNER && i == RETOURNER)
            {
                continue;
            }
            long long eval=evaluer_action_min_max(jeu_clone, i, joueur, j, n-1);
            if(eval != min_possible) // On n'evalue pas les coups impossible
            {
                if(joueur == joueur_tour)
                {
                    if(eval < total)
                    {
                        total=eval;
                    }
                }
                else
                {
                    total+=eval;
                }
            }
        }
        supprimer_jeu(&jeu_clone);
        return total; // On renvoie la "somme" de tout ce qui peut arriver apres notre coup simule
    }
    supprimer_jeu(&jeu_clone);
    if((unsigned char)_gagnant == joueur) // On a gagne sur le coup simule ?
    {
        return (long long)pow((double)nb, (double)n);
    }
    return -(long long)pow((double)nb, (double)n); // On a perdu sur le coup simule ?
}

int sauvegarder(Jeu jeu, unsigned char joueur_tour, char *nom_fichier)
{
    FILE *fp=NULL;
    fp=fopen(nom_fichier, "w+");
    if(fp == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return 0;
    }
    for(unsigned char i=0; i<TAILLE_JEU; i++)
    {
        for(unsigned char j=0; j<TAILLE_JEU; j++)
        {
            fprintf(fp, "%c", jeu.p[i][j]+'0');
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "%c\n", joueur_tour+'0');
    fclose(fp);
    printf("Sauvegarde reussie\n");
    return 1;
}

/*
Le fichier ressemblera a:
0000000
0000000
0001000
0002000
1001000
2022200
1122101
2
Si le prochain joueur qui devra jouer sera la joueur 2
*/

void quitter(Jeu *jeu)
{
    printf("Au revoir\n");
    supprimer_jeu(jeu);
    exit(0);
}

void sauvegarder_et_quitter(Jeu *jeu, unsigned char joueur_tour)
{
    char nom_fichier[100];
    do
    {
        memset(nom_fichier, 0, 100);
        printf("Fichier: ");
        while(nom_fichier[0] == '\0') // Eviter les erreur de lecture du stdin
        {
            lire_str(nom_fichier, 100);
        }
    } while(!sauvegarder(*jeu, joueur_tour, nom_fichier));
    quitter(jeu);
}

void decrire_action(short action)
{
    if(action >= 0)
    {
        printf("Pion dans la colonne %d", (action+1));
        return;
    }
    switch(action)
    {
    case GAUCHE:
        printf("Pivoter le plateau vers la gauche");
        break;
    case DROITE:
        printf("Pivoter le plateau vers la droite");
        break;
    case RETOURNER:
        printf("Retourner le plateau");
        break;
    default:
        return;
    }
}

inline long long MIN_PTS_POSSIBLE(Jeu jeu) // Le minimum absolu de points que pourra renvoyer evaluer_action_min_max()
{
    long long min=-(const long long)pow((double)(TAILLE_JEU+4), (double)(jeu.prof + 1));
    return min;
}

/* https://openclassrooms.com/fr/courses/19980-apprenez-a-programmer-en-c/16993-la-saisie-de-texte-securisee */
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int lire_str(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}
