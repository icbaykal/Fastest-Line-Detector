#define NeedFunctionPrototypes
#include <stdio.h>
#include <stdlib.h>
#include "ListeMacros.h"
#include "Liste.h"
#include "ListeP.h"
#include <conio.h>


long DestList(Liste *s_idlist /* address of the list to process indentifier */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_next, *p_curent;

#ifdef debug
	(void) fprintf(stderr, " Function : DestList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)* s_idlist;

	/* Prarameters checking */
	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Deletion of all the elements of the list */
	p_curent = p_firstelem->p_first;
	free(p_firstelem);
	*s_idlist = NULL;		/* Update identifier */

	while (p_curent != NULL) {
		p_next = p_curent->p_suiv;
		free(p_curent);
		p_curent = p_next;
	};

#ifdef debug
	(void) fprintf(stderr, " End of : DestList \n");
#endif

	return (PASERR);

}				/* End of DestList */

long DestElmList(Liste s_idlist,		/* list to process indentifier */
	long el			/* element to remove from the list */
)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent, *p_next;

#ifdef debug
	(void) fprintf(stderr, " Function : DestElmList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;
	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Test if the list is empty */
	Lst_TestPar(p_firstelem->p_first, "EMPTY LIST \n", ERRVI);

#ifdef debug
	(void) fprintf(stderr, " DestElmList : all right none empty list \n");
#endif

	p_next = p_firstelem->p_first;
	if (p_next->s_elem == el) {	/* case of the first element */
		p_firstelem->p_first = p_next->p_suiv;
		if (p_firstelem->p_cur == p_next) p_firstelem->p_cur = p_next->p_suiv;
		if (p_firstelem->p_last == p_next) p_firstelem->p_last = NULL;
		free(p_next);
		p_firstelem->num--;

#ifdef debug
		(void) fprintf(stderr, " none empty list, DestElm \n ");
		(void)fprintf(stderr, " End of : DestElmList \n");
#endif

		return (PASERR);
	};				/* End of the if p_next->s_elem == el) */

					/* case of any element */
	while ((p_next != NULL) && (p_next->s_elem != el)) {

#ifdef debug
		(void) fprintf(stderr, " none empty list, DestElm \n ");
#endif

		p_curent = p_next;
		p_next = p_next->p_suiv;
	};				/* End of the while ((p_next != NULL) && (elem
					* != el) */
	if (p_next == NULL) {		/* The element isn't in the list */

#ifdef debug
		(void) fprintf(stderr, " End of : DestElmList (erreur empty list or unfound el.)\n");
#endif

		/* update error unfound element */
		return (ERRVI);
	};				/* End of the if p_next == NULL */

					/* element is found */
	p_curent->p_suiv = p_next->p_suiv;	/* chain updating */
	if (p_firstelem->p_cur == p_next) p_firstelem->p_cur = p_curent;
	if (p_firstelem->p_last == p_next) p_firstelem->p_last = p_curent;
	free(p_next);
	p_firstelem->num--;

#ifdef debug
	(void) fprintf(stderr, " End of : DestElmList \n");
#endif

	return (PASERR);

}				/* End of DestElmList */


long CreatList()
{
	ty_list *p_pteur;

#ifdef debug
	(void) fprintf(stderr, " Function : CreatList \n");
#endif

	/* Bloc allocation and fields initialisation */
	p_pteur = (ty_list *)malloc(sizeof(ty_list));

	/* Allocation checking */
	Lst_TestPar(p_pteur, "NO MORE MEMORY AVAILABLE \n", ERRNU);

	/* initialisation */
	p_pteur->p_first = NULL;
	p_pteur->p_cur = NULL;
	p_pteur->p_last = NULL;
	p_pteur->num = 0;

#ifdef debug
	(void) fprintf(stderr, " End of : CreatList \n");
#endif

	return ((long)p_pteur);

}				/* End of CreatList */

Liste CopyList(Liste s_idlist		/* list to process indentifier */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent,		/* Working pointer in the original list */
		*p_curentbis,		/* Working pointer in the new list */
		*p_nextbis;			/* Working pointer in the new list */
	ty_list *p_newprem;		/* obtained list identifier */

#ifdef debug
	(void) fprintf(stderr, " Function : CopyList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Bloc allocation and fields initialisation */
	p_newprem = (ty_list *)malloc(sizeof(ty_list));

	/* Allocation checking */
	Lst_TestPar(p_newprem, "NO MORE MEMORY AVAILABLE \n", ERRNU);

	p_newprem->num = p_firstelem->num;	/* initialisation */
	p_curent = p_firstelem->p_first;	/* First el. of the list to Copy */
	if (p_curent == NULL) {	/* empty list */
		p_newprem->p_first = NULL;	/* initialisation */
		p_newprem->p_cur = NULL;	/* initialisation */
		p_newprem->p_last = NULL;	/* initialisation */
	}
	else {			/* none empty list */
					/* case of the first element */
		p_curentbis = (ty_node *)malloc(sizeof(ty_node));

		/* Allocation checking */
		Lst_TestPar(p_curentbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);

		p_newprem->p_first = p_curentbis;	/* initialisation */
		p_newprem->p_cur = NULL;	/* initialisation */
		p_curentbis->s_elem = p_curent->s_elem;
		p_curent = p_curent->p_suiv;

		/* case of the next elements */
		while (p_curent != NULL) {
			p_nextbis = (ty_node *)malloc(sizeof(ty_node));

			/* Allocation checking */
			Lst_TestPar(p_nextbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);

			p_nextbis->s_elem = p_curent->s_elem;	/* copie de l'el. courant */
			p_curentbis->p_suiv = p_nextbis;	/* Chain updating */
			p_curent = p_curent->p_suiv;
			p_curentbis = p_nextbis;
		}				/* End of the while (p_curent != NULL)  */

		p_newprem->p_last = p_curentbis;	/* initialisation */
		p_curentbis->p_suiv = NULL;	/* initialisation */
	};				/* End of the if p_curent == NULL */


#ifdef debug
	(void) fprintf(stderr, " End of : CopyList \n");
#endif

	return ((Liste)p_newprem);

}				/* End of CopyList */

long AddElmList(
	Liste s_idlist,		/* Identifier of the list to process */
	long el			/* Element to insert */
)

{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent;

#ifdef debug
	(void) fprintf(stderr, " Function : AddElmList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;
	Lst_TestPar(p_firstelem, "AddElmList : NULL POINTER AS PARAMETER \n", ERRNU);

	/* Bloc allocation and fields initialisation */
	p_curent = (ty_node *)malloc(sizeof(ty_node));

	/* Allocation checking */
	Lst_TestPar(p_curent, "AddElmList : NO MORE MEMORY AVAILABLE \n", ERRNU);

	p_curent->p_suiv = NULL;	/* Chain updating */
	p_curent->s_elem = el;	/* Elem value updating */

							/* Bloc allocation and fields initialisation */
	if (p_firstelem->num == 0) {	/* empty list */

#ifdef debug
		(void) fprintf(stderr, " empty list, AddElm \n ");
#endif

		p_firstelem->p_first = p_curent;
		p_firstelem->p_cur = p_curent;
	}
	else {			/* none empty list */
		p_firstelem->p_last->p_suiv = p_curent;
	};				/* End of the if p_firstelem->num == 0 */

	p_firstelem->p_last = p_curent;	/* End of chain updating */
	p_firstelem->num++;		/* Number of element updating */

#ifdef debug
	(void) fprintf(stderr, " End of : AddElmList \n");
#endif

	return (PASERR);

}				/* End of AddElmList */

long PrintList(Liste s_idlist,		/* List to process indentifier */
	FILE *fptr			/* File where to print */
)

{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_next;

#ifdef debug
	(void) fprintf(stderr, " Function : PrintList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	/* Test if existing list */
	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Test if empty list */
	Lst_TestPar(p_firstelem->p_first, "EMPTY LIST \n", ERRVI);

	(void)fprintf(fptr, "\n-> List defined by %d : \n", s_idlist);
	(void)fprintf(fptr, "\n   There is %d in the list : \n", p_firstelem->num);
	(void)fprintf(fptr, "\n   first one : %d \n", p_firstelem->p_first->s_elem);
	if (p_firstelem->p_cur == NULL)
		(void) fprintf(fptr, "\n   curent one : NIL \n");
	else
		fprintf(fptr, "\n   curent one : %d \n", p_firstelem->p_cur->s_elem);
	(void)fprintf(fptr, "\n   last one : %d\n", p_firstelem->p_last->s_elem);
	p_next = p_firstelem->p_first;
	while (p_next != NULL) {
		(void)fprintf(fptr, "%d; ", p_next->s_elem);
		p_next = p_next->p_suiv;
	};				/* End of the while */
	(void)fprintf(fptr, "\n");

	return (PASERR);
}				/* End of PrintList */

long AppendList(Liste s_idlist1, Liste s_idlist2 )
{
	ty_list *p_premlist1,
		*p_premlist2;      /* pointer to the originals lists */
	ty_list *p_newprem;         /* pointer to the new list */
	ty_node *p_curent,          /* Working pointer in the original list */
		*p_curentbis,       /* Working pointer in the new list */
		*p_nextbis;         /* Working pointer in the new list */

#ifdef debug
	(void)fprintf(stderr, " Function : AppendList \n");
#endif

	/* Parameters transformation */
	p_premlist1 = (ty_list *)s_idlist1;
	p_premlist2 = (ty_list *)s_idlist2;

	/* Prarameters checking */
	Lst_TestPar(p_premlist1, "NULL POINTER AS 1st PARAMETER \n", ERRNU);
	Lst_TestPar(p_premlist2, "NULL POINTER AS 2nd PARAMETER \n", ERRNU);

	/* Bloc allocation and fields initialisation */

	/* List identifier */
	p_newprem = (ty_list *)malloc(sizeof(ty_list));

	/* Allocation checking */
	Lst_TestPar(p_newprem, "NO MORE MEMORY AVAILABLE \n", ERRNU);

	p_newprem->num = p_premlist1->num + p_premlist2->num;  /* initialisation */
	p_curent = p_premlist1->p_first;

	if (p_curent == NULL)   /* the first list is empty */
	{
		p_newprem->p_first = NULL;        /* initialisation */
		p_newprem->p_cur = NULL;         /* initialisation */
		p_newprem->p_last = NULL;         /* initialisation */
	}
	else                    /* the first list isn't empty */
	{

		/* first element */
		p_curentbis = (ty_node *)malloc(sizeof(ty_node));

		/* Allocation checking */
		Lst_TestPar(p_curentbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);

		p_newprem->p_first = p_curentbis;         /* initialisation */
		p_newprem->p_cur = NULL;           /* initialisation */
		p_curentbis->s_elem = p_curent->s_elem;
		p_curent = p_curent->p_suiv;

		/* next elements */
		while (p_curent != NULL)
		{
			p_nextbis = (ty_node *)malloc(sizeof(ty_node));

			/* Allocation checking */
			Lst_TestPar(p_nextbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);

			p_nextbis->s_elem = p_curent->s_elem;  /* curent elm. copy */
			p_curentbis->p_suiv = p_nextbis;       /* chain updating */
			p_curent = p_curent->p_suiv;
			p_curentbis = p_nextbis;
		} /* End of the while (p_curent != NULL)  */

		  /* last element initialisation */
		p_newprem->p_last = p_curentbis;
		p_curentbis->p_suiv = NULL;
	};  /* End of the if p_curent == NULL */


		/* second list */

	p_curent = p_premlist2->p_first;
	if (p_curent != NULL)  /* the second list isn't empty */
	{
		if (p_newprem->p_first == NULL) /* 1st list was empty */
		{
			/* first element of the new list */
			p_curentbis = (ty_node *)malloc(sizeof(ty_node));

			/* Allocation checking */
			Lst_TestPar(p_curentbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);
			p_newprem->p_first = p_curentbis;
			p_newprem->p_cur = NULL;
			p_curentbis->s_elem = p_curent->s_elem;
			p_curent = p_curent->p_suiv;
		} /* End of the if p_newprem->p_first == NULL */

		  /* next elements of the new list */
		while (p_curent != NULL)
		{
			p_nextbis = (ty_node *)malloc(sizeof(ty_node));

			/* Allocation checking */
			Lst_TestPar(p_nextbis, "NO MORE MEMORY AVAILABLE \n", ERRNU);
			p_nextbis->s_elem = p_curent->s_elem;  /* copie de l'el. courant */
			p_curentbis->p_suiv = p_nextbis;       /* m.a.j. du chainage */
			p_curent = p_curent->p_suiv;
			p_curentbis = p_nextbis;
		} /* End of the while (p_curent != NULL)  */

		  /* last element initialisation */
		p_newprem->p_last = p_curentbis;
		p_curentbis->p_suiv = NULL;
	};  /* End of the if p_curent != NULL */


#ifdef debug
	(void)fprintf(stderr, " End of : AppendList \n");
#endif

	return ((long)p_newprem);

} /* End of AppendList */

long SortList(Liste s_idlist,	int function(const void *, const void *)		)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent;
	int *table;			/* table where to put the list to sort */
	int i;			/* Table index */

#ifdef debug
	(void) fprintf(stderr, " Function : SortList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	if (p_firstelem->num == 0) {	/* empty list */

#ifdef debug
		(void) fprintf(stderr, " End of : SortList \n");
#endif

		return (PASERR);
	}

	/* Creation of the table and fill it */
	table = (int *)calloc(p_firstelem->num, sizeof(long));
	i = 0;
	p_curent = p_firstelem->p_first;
	do {
		table[i++] = p_curent->s_elem;
		p_curent = p_curent->p_suiv;
	} while (p_curent != NULL);
	// int values[] = { 40, 10, 100, 90, 20, 25 };
	// qsort (values, 6, sizeof(int), compare); compare is a funtion pointer
	qsort(table, p_firstelem->num, sizeof(int), function);

	/* Uptadte the list */
	p_curent = p_firstelem->p_first;
	p_curent->s_elem = table[0];

	/* case of the next elements */
	for (i = 1; i < p_firstelem->num; i++) {
		p_curent = p_curent->p_suiv;/* Next element */
		p_curent->s_elem = table[i];
	}				/* End of the for (i=1 ; i==p_firstelem->num ;
					* i++) */

#ifdef debug
	(void) fprintf(stderr, " End of : SortList \n");
#endif

	return (PASERR);

}				/* End of SortList */

long FirstElmList(Liste s_idlist			/* list to process indentifier */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */

#ifdef debug
	(void) fprintf(stderr, " Function : FirstElmList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Next element acces */
	Lst_TestPar(p_firstelem->p_first, "EMPTY LIST \n", ERRVI);

	p_firstelem->p_cur = p_firstelem->p_first;	/* Update curent element */

#ifdef debug
	(void) fprintf(stderr, " End of : FirstElmList \n");
#endif

	return (p_firstelem->p_first->s_elem);

}				/* End of FirstElmList */

long NextElmList(Liste s_idlist		/* List identifier */)

{
	ty_list *p_firstelem;		/* pointer to the list */
	ty_node *p_curent;		/* Pointer to curent node */

#ifdef debug
	(void) fprintf(stderr, " Function : NextElmList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;
	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Acces to the next element */
	Lst_TestPar(p_firstelem->p_first, "EMPTY LIST \n", ERRVI);
	p_curent = p_firstelem->p_cur;
	Lst_TestPar(p_curent->p_suiv, "NO MORE ELEMENTS \n", ERRVI);

	/* Tail none empty list */
	p_firstelem->p_cur = p_curent->p_suiv;	/* Update curent elemnt */

#ifdef debug
	(void) fprintf(stderr, " End of : NextElmList \n");
#endif

	return (p_firstelem->p_cur->s_elem);

}				/* End of NextElmList */

long ElmNumList(Liste s_idlist,		/* list to process indentifier */
	int j				/* looked for element */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent;
	int cpteur;


#ifdef debug
	(void) fprintf(stderr, " Function : ElmNumList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);
	Lst_TestPar(p_firstelem->p_first, "EMPTY LIST \n", ERRVI);

	if (p_firstelem->num < j || j == 0) {	/* Unexisting element */

#ifdef debug
		(void) fprintf(stderr, " ERREUR, Innexisting element \n ");
		(void)fprintf(stderr, "         J was : %d \n", j);
		(void)fprintf(stderr, " And there is  %d elements only in the list \n",
			p_firstelem->num);
		(void)fprintf(stderr, " End of : ElmNumList \n");
#endif

		return (ERRVI);
	};

	if (p_firstelem->num == j) {	/* last element */

#ifdef debug
		(void) fprintf(stderr, " End of : ElmNumList \n");
#endif

		return (p_firstelem->p_last->s_elem);
	};

	p_curent = p_firstelem->p_first;
	for (cpteur = 1; cpteur < j; cpteur++)
		p_curent = p_curent->p_suiv;

#ifdef debug
	(void) fprintf(stderr, " End of : ElmNumList \n");
#endif

	return (p_curent->s_elem);

}				/* End of ElmNumList */

long EmptyList(	Liste s_idlist		/* list to process indentifier */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */

#ifdef debug
	(void) fprintf(stderr, " Function : EmptyList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	/* Prarameters checking */
	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* Test si la list est vide */
	if (p_firstelem->num == 0) {

#ifdef debug
		(void) fprintf(stderr, " End of : EmptyList  : TRUE \n");
#endif

		return (1);
	}
	else {

#ifdef debug
		(void) fprintf(stderr, " End of : EmptyList  : FALSE  \n");
#endif

		return (0);
	};				/* End of the if (p_firstelem == NULL) */

}				/* End of EmptyList */

long InsertSortList(Liste s_idlist,		/* list to process indentifier */
	long el,			/* element to insert */
	long(*function) (long *,long *)		/* Acces function */)

{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent, *p_next, *p_intermede;	/* Working pointers */
	int found;			/* To leave the loop */

#ifdef debug
	(void) fprintf(stderr, " Function : InsertSortList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);


	/* Empty list ---------- */
	if (p_firstelem->num == 0) {

#ifdef debug
		(void) fprintf(stderr, " empty list, InsertSortList \n ");
#endif

		p_curent = (ty_node *)malloc(sizeof(ty_node));

		/* Allocation checking */
		Lst_TestPar(p_curent, "NO MORE MEMORY AVAILABLE \n", ERRNU);

		p_curent->p_suiv = NULL;	/* chain updating */
		p_curent->s_elem = el;	/* elem value updating */
		p_firstelem->p_first = p_curent;
		p_firstelem->p_cur = NULL;
		p_firstelem->p_last = p_curent;
	}

	/* None empty list --------------- */

	else {
		/* Scan the list until insertion or end of list */
		found = 0;
		p_next = p_firstelem->p_first;
		p_curent = NULL;
		while ((found == 0) && (p_next != NULL)) {
			if ((function)(&el, &(p_next->s_elem)) < 0) {
				found = 1;		/* Position of the element found */

								/* Creation and intialisation of the bloc */
				p_intermede = (ty_node *)malloc(sizeof(ty_node));

				/* Allocation checking */
				Lst_TestPar(p_intermede, "NO MORE MEMORY AVAILABLE \n", ERRNU);

				if (p_curent == NULL)	/* insertion en tete */
					p_firstelem->p_first = p_intermede;
				else			/* insertion au milieu */
					p_curent->p_suiv = p_intermede;
				p_intermede->p_suiv = p_next;
				p_intermede->s_elem = el;
			}				/* End of the if ((function)(&el,
							* &(p_next->s_elem)) == -1) */

			p_curent = p_next;
			p_next = p_next->p_suiv;
		}				/* End of the while (p_next != NULL) && (
						* p_next->s_elem != el) */


						/* Insertion at the end -------------------- */
		if (found == 0) {
			p_intermede = (ty_node *)malloc(sizeof(ty_node));

			Lst_TestPar(p_curent, "NO MORE MEMORY AVAILABLE \n", ERRNU);

			p_curent->p_suiv = p_intermede;
			p_firstelem->p_last = p_intermede;
			p_intermede->p_suiv = NULL;
			p_intermede->s_elem = el;
		}				/* End of the if  found == 0 */

	}				/* End of the else p_firstelem->num == 0 */

#ifdef debug
	(void) fprintf(stderr, " End of : InsertSortList \n");
#endif

	p_firstelem->num++;
	return (PASERR);

}				/* End of InsertSortList */


long MapList(Liste s_idlist,		/* list to process indentifier */
	void(*function) (long)		/* Called function */
)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */
	ty_node *p_curent;

#ifdef debug
	(void) fprintf(stderr, " Function : MapList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

	/* parcours de la list */
	p_curent = p_firstelem->p_first;
	while (p_curent != NULL) {
		/* Function call */
		(*function) (p_curent->s_elem);
		p_curent = p_curent->p_suiv;
	}				/* End of the while (p_curent != NULL)  */

#ifdef debug
	(void) fprintf(stderr, " End of : MapList \n");
#endif

	return (PASERR);

}				/* End of MapList */

long SizeList(Liste s_idlist			/* list to process indentifier */)
{
	ty_list *p_firstelem;		/* Pointer to the list to process */


#ifdef debug
	(void) fprintf(stderr, " Function : SizeList \n");
#endif

	/* Parameter transformation */
	p_firstelem = (ty_list *)s_idlist;

	Lst_TestPar(p_firstelem, "NULL POINTER AS PARAMETER \n", ERRNU);

#ifdef debug
	(void) fprintf(stderr, " End of : SizeList \n");
#endif

	return (p_firstelem->num);

}				/* End of SizeList */
