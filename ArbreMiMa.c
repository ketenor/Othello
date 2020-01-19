#include <stdlib.h>
#include <stdio.h>
#include "ArbreMiMa.h"
#include "ListePos.h"

int EvaluerPlateau_0(int plateau[H][H])
	{
	int i,j;
	int res = 0;
	/*A Completer*/
	for (i = 0; i < H; j++)
		for (j = 0; j < H; j++)
		{
			if (plateau[i][j] == NOIR)
				res++;
			if (plateau[i][j] == BLANC)
				res--;
		}
	return (res);
	}

NdMiMa_t *Construire_arbre(int plateau[H][H], int prof, int couleurQuiJoue)
	{
	/*A Completer*/
	int plateau_copie[H][H];
	NdMiMa_t *curr;

	if (!(curr = malloc (sizeof(NdMiMa_t*))))
		return (NULL);
	if (Partie_terminee(plateau_copie))
		return (NULL);
	if (!prof)
		return (NULL);
	if (!(curr->liste_pos = Trouver_liste_pos_jouables(plateau, couleurQuiJoue)))
		curr -> JoueurBloque = Construire_arbre(plateau, --prof, Autre_joueur(couleurQuiJoue));
	while (curr->liste_pos)
		{
		Copier_plateau(plateau_copie, plateau);
		Jouer_pion(plateau_copie, curr->liste_pos->i, curr->liste_pos->j, couleurQuiJoue);
		curr->liste_pos->Nd = Construire_arbre(plateau_copie, --prof, Autre_joueur(couleurQuiJoue));
		curr->liste_pos = curr->liste_pos->suiv;
		}
	return (curr);
	}

int MinMax(NdMiMa_t *arbre, int plateau[H][H], int (*EvaluerPlateau)(int plateau[H][H]))
	{
	/*A Completer*/
	int res;
        int couleur;
	int n;
	int i;
	int new[H][H];
        NdMiMa_t *tmp;
	PosJouable_t *l;

        if(arbre == NULL)
		return EvaluerPlateau(plateau);
	tmp = arbre;
	couleur = arbre->Couleur;
	res = 0;
	n = 0;
	i = 0;
	l = tmp->liste_pos;
	while(l){
		n++;
		l=l->suiv;
	}
	int m[n];
	l = tmp->liste_pos;
        while(l){
		Copier_plateau(new,plateau);
                Jouer_pion(new, l->i, l->j, tmp->Couleur);
                m[i] = MinMax(l->Nd,new,EvaluerPlateau);
                i++;
                l=l->suiv;
	}
        if (couleur == NOIR){
		for(int j=0;j<n;j++){
			if(m[j] > res)
				res = m[j];
		}
	}
	if(couleur == BLANC){
		for (int j=0; j<n; j++){
			if (m[j] < res)
				res = m[j];
		}
	}
        if(couleur != VIDE)
		return res;
        return 0;
        }

int MeilleurPos(NdMiMa_t *arbre, int plateau[H][H], int (*EvaluerPlateau)(int plateau[H][H]),int *pi, int *pj)
	{
	/*A Completer*/
	*pi = -1;
        *pj = -1;
        NdMiMa_t *a = arbre;
        PosJouable_t *l = a->liste_pos;
        int new[H][H];
        while(l){
                Copier_plateau(new,plateau);
                Jouer_pion(new, l->i, l->j, a->Couleur);
                if(MinMax(l->Nd,new,EvaluerPlateau)==MinMax(a,plateau,EvaluerPlateau)){
                        *pi = l->i;
                        *pj = l->j;
                        break;
                }
                l = l->suiv;
        }
        return MinMax(arbre, plateau, EvaluerPlateau);
	}

NdMiMa_t *Detruire_arbre(NdMiMa_t *arbre)
	{
	if (arbre)
		{
		Detruire_liste(arbre->liste_pos);
		free(arbre);
		}
	return NULL;
	}

