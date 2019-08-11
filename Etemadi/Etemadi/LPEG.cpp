#include <math.h>      /* Standard C mathematics library */
#include <errno.h>     /* Standard C error handling routines */
/* #include <strings.h> */   /* Standard C string handling routines */
#include <ctype.h>     /* Standard C type identification routines */
#include "FEX.h"
#define PI     3.141593
#define PIBY2  1.570796

#ifndef HUGE
#define HUGE 999999999.0
#endif

int  SortByThetaAscending(const void *a, const void *b)
{
	struct ORTLine *element1;
	struct ORTLine *element2;

	/*
	First we must cast the dummy variables into ORTLine structures

	*/

	element1 = ((ORTLine *)a);
	element2 = ((ORTLine *)b);

	/*
	First we must cast the dummy variables into ORTLine structures

	*/

	if (element1->Theta > element2->Theta) {
		return(1);
	}
	else {
		return(0);
	}
}


int LineSegRowIntercept(double StartCol, double	StartRow, double EndCol,double	EndRow, double *RowIntercept)
{

#ifdef debug
	fprintf(stderr, " Start of function LineSegRowIntercept \n");
#endif

	if (EndCol != StartCol) {	/* Line is not vertical */
		*RowIntercept = (EndCol*StartRow - StartCol*EndRow) /
			(EndCol - StartCol);
		return(0);
	}

	*RowIntercept = HUGE;	/* Line is vertical */
	return(-1);

#ifdef debug
	fprintf(stderr, " End of function LineSegRowIntercept \n");
#endif

}

int LineSegColIntercept(double StartCol, double	StartRow, double EndCol, double	EndRow, double	*ColIntercept)
{

#ifdef debug
	fprintf(stderr, " Start of function LineSegColIntercept \n");
#endif

	if (EndCol == StartCol) { 	/* Line is vertical */
		*ColIntercept = StartCol;
		return(0);
	}

	if (EndRow != StartRow) {	/* Line is not horizontal */
		*ColIntercept = (StartCol*EndRow - EndCol*StartRow) /
			(EndRow - StartRow);
		return(0);
	}

	*ColIntercept = HUGE;     /* Line is horizontal */
	return(-1);

#ifdef debug
	fprintf(stderr, " End of function LineSegColIntercept \n");
#endif

}
/* Compare with PIBY2 with a little leaway to avoid infinities */
#define MACCompareWithPIBY2(a) (a > (PIBY2-0.001) && a < (PIBY2+0.001) )

int PtLinePerpIntercept(double PtCol, double PtRow, double	ColIntercept, double	RowIntercept,
	double Theta, double *InterceptPtCol, double *InterceptPtRow)
{
	double Grad;
	double GradSquared;
	double OnePlusGradSquared;

#ifdef debug
	fprintf(stderr, " Start of function PtLinePerpIntercept \n");
#endif


	if (MACCompareWithPIBY2(Theta)) { /* Line is vertical */
		*InterceptPtRow = PtRow;
		*InterceptPtCol = ColIntercept;
	}

	if (Theta < 0.001 || Theta >(PI - 0.001)) { /*Line is horizontal */
		*InterceptPtRow = RowIntercept;
		*InterceptPtCol = PtCol;
	}

	/*
	Otherwise we have to work things out...

	*/
	if (!MACCompareWithPIBY2(Theta) && Theta >= 0.001 && Theta <= (PI - 0.001)) {

		Grad = tan(Theta);
		GradSquared = Grad*Grad;
		OnePlusGradSquared = 1.0 + GradSquared;

		*InterceptPtRow = (RowIntercept / (OnePlusGradSquared)) +
			(Grad*PtCol / (OnePlusGradSquared)) +
			(GradSquared*PtRow / (OnePlusGradSquared));

		*InterceptPtCol = (*InterceptPtRow - RowIntercept) / Grad;
	}

	return(0);

#ifdef debug
	fprintf(stderr, " End of function PtLinePerpIntercept \n");
#endif

}

double LineSegTheta(double StartCol, double	StartRow, double EndCol, double	EndRow)
{

	double dx, dy; 			/* Buffers */

	double Orientation;

#ifdef debug
	fprintf(stderr, " Start of function LineSegTheta \n");
#endif

	dx = EndCol - StartCol;
	dy = EndRow - StartRow;

	if (dx == 0.0) return(PIBY2); 	/* Line is vertical */
	if (dy == 0.0) return(0.0);     	/* Line is Horizontal */

	Orientation = atan2(dy, dx);

	/*
	Make it so line orientation is always between 0 and pi.

	*/

	if (Orientation < 0.0)
		Orientation = Orientation + PI;

	if (Orientation > PI)
		Orientation = Orientation - PI;

	return(Orientation);

#ifdef debug
	fprintf(stderr, " End of function LineSegTheta \n");
#endif

}

double LineLength(double StartCol, double StartRow, double	EndCol, double	EndRow)
{

	double dx, dy;

	double Length;

#ifdef debug
	fprintf(stderr, " Start of function LineLength \n");
#endif

	dx = EndCol - StartCol;
	dy = EndRow - StartRow;

	Length = hypot(dx, dy);

	return(Length);

#ifdef debug
	fprintf(stderr, " End of function LineLength \n");
#endif

}

int SkipLines(FILE *(*p_InFile), int LineLength, int LinesToSkip, int PrintFlag)
{
	int i;                       /* Loop index */
	char LineBuffer[100]; /* Just a buffer to store the characters */

#ifdef debug
	fprintf(stderr, " Start of function SkipLines \n");
#endif

	/*
	Skip LinesToSkip lines of Lines

	*/
	for (i = 0;i<LinesToSkip;i++) {

		if ((fgets(LineBuffer, LineLength, *p_InFile)) != NULL) {
			if (PrintFlag == 0)
				fprintf(stderr, "-> %s", LineBuffer);
		}
		else {
			return(-1);
		}
	}

	return(0);

#ifdef debug
	fprintf(stderr, " End of function SkipLines \n");
#endif

}
