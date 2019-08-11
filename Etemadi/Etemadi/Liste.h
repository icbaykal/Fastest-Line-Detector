/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ORT - Object Recognition Toolkit

Copyright (C) 1991,1992,1993

	Ataollah Etemadi,
	Space and Atmospheric Physics Group,
	Blackett Laboratory,
	Imperial College of Science, Technology and Medicine
	Prince Consort Road,
	London SW7 2BZ.

    Software Contributors:

	J-P. Schmidt,				(Liste, and Xwindows interface)
	G.A.W. West and P.L. Rosin,		(Pixel Chainning S/W)
	G. A. Jones,				(Least-squares circular-arc fitting)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 1, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Note: This program has been exempted from the requirement of
	  paragraph 2c of the General Public License.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* ****************************************************************** */
/** Bibliotheque libList
 * 
 * Auteur : J.Paul Schmidt.
 * Version : 1.1            date : 88/04/14
 * 
 * Modifications : 
 * Auteur : 
 * Reason : 
 * Implemented functionality : Lists handler.
 *
 Interface functions : CreatList
 *                     AddElmList
 *                     AppendList
 *                     CopyList
 *                     CreatList
 *                     DestElmList
 *                     DestList
 *                     ElmNumList
 *                     <PrintList> (pour traces)
 *                     InsertSortList
 *                     EmptyList
 *                     MapList
 *                     FirstElmList
 *                     NextElmList
 *                     SizeList 
 *                     SortList
 **/  
/* ****************************************************************** */

#ifndef _EngListe
#define _EngListe
#include <stdio.h>

/* Constantes */
#define ERRVI -999999990
#define ERRNU -999999991
#define PASERR 0

typedef long Liste ;

#ifdef _Cplus_plus

#else

/* Availlable functions */
extern long AddElmList (Liste s_idlist,                /* Identifier of the list to process */
  long el                       /* Element to insert */
);

extern Liste AppendList (Liste s_idlist1, Liste s_idlist2   /* list identifiers */);

extern Liste CopyList (Liste s_idlist         /* list to process indentifier */);

extern Liste CreatList ();

extern long DestElmList (Liste s_idlist,  /* list to process indentifier */
  long el                       /* element to remove from the list */
);

extern long DestList (long *s_idlist /* address of the list to process indentifier */
/* RETOUR */
);

extern long ElmNumList (Liste s_idlist,                /* list to process indentifier */
  int j                         /* looked for element */
);

extern long EmptyList (Liste s_idlist         /* list to process indentifier */
);

extern long FirstElmList (
  Liste s_idlist                 /* list to process indentifier */
);

extern long InsertSortList ( Liste s_idlist,                /* list to process indentifier */
  long el,                      /* element to insert */
  long (*function) ()           /* Acces function */
);

extern long MapList (
  Liste s_idlist,                /* list to process indentifier */
  void (*function) ()           /* Called function */
);

extern long NextElmList (
  Liste s_idlist         /* List identifier */
);

extern long PrintList (
  Liste s_idlist,                /* List to process indentifier */
  FILE *fptr                    /* File where to print */
);

extern long SizeList (
  Liste s_idlist                 /* list to process indentifier */
);

extern long SortList (Liste s_idlist,                /* list to process indentifier */
  int (*function) (const void *, const void *)           /* function used to compar the elements */
);
#endif  /* End of  _Cplus_plus */
#endif  /* End of  _EngListe */
