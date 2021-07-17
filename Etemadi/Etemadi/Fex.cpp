#include<stdio.h>
#include<stdlib.h>
# include <math.h>
#include "ListeMacros.h"
#include "Liste.h"
#include "FEX.h"
#include "LPEG.h"
#include "Basic_image.h"

extern Message msg;

Liste *FEX(Liste &LineListID, int noof_segments, Liste *StringListIDL,int width,int height)
{

	/*
	=======================================================================
	===================== START OF DECLARATION ============================
	=======================================================================
	*/
	int i, NoOfRows, NoOfCols;
	int MaxJumpPixels;
	int Next;
	int LineCounter;
	int CurveCounter;

	FILE *p_InFile;		/* Pointers to start of files fo I/O */
	FILE *p_OutFile;

	/*
	Number of lines in corresponding lists

	*/
	int StringListSize;	  /* Number of points in string */
	int SegmentListSize;	  /* Number of segments found along string */
	int StringID;
	/*
	ID numbers of appropriate lists

	*/
	Liste *StringListID;
	Liste SegmentListID;
	
	Liste CircularArcListID;

	/*
	Pointer to list

	*/
	struct ORTPoint          *StringList;
	struct ORTSegment        *SegmentList;
	struct ORTLine    	     *Line;
	struct ORTCircularArc    *CircularArc;

	/*
	Buffers for outputting result
	*/

	int semicircle;
	int direction;

	long Buffer;
	long Segment;

	struct ORTSegment *SegmentBuffer;
	struct ORTPoint   *PointBuffer;

	double Length;
	double LengthParlVar;
	double LengthPerpVar;
	double ThetaVar;
	double Height;
	double Width;

	double StartCol, EndCol;
	double StartRow, EndRow;

	double side;
	double x1, x2, x3, x4;
	double y1, y2, y3, y4;
	double MidToVL;
	double MidToJct;

	/*
	=======================================================================
	===================== START OF INITIALISATION =========================
	=======================================================================
	*/


	/*
	Set defaults, process command line options, and initialise lists
	*/

	Next = 0;
	MaxJumpPixels = 2;

	

	/*
	Use standard input output I/O

	*/
	//p_InFile = fopen("myfile.txt", "r");;
	//p_OutFile = fopen("lines.txt", "w");;

	/*
	=======================================================================
	============================ MAIN LOOP ================================
	=======================================================================
	*/

	/*
	Get one string from string file and store it in a linked list for processing
	*/

	LineListID = CreatList();
	CircularArcListID = CreatList();

	LineCounter = 0;
	CurveCounter = 0;
	StringID = 0;
	int tt;
	NoOfCols = width; NoOfRows = height;
	for(tt=1;tt<noof_segments;tt++)
	{
		StringListSize= (int)SizeList(StringListIDL[tt]);
		StringListID = &StringListIDL[tt];		
		
		//StringListID = &StringListIDL[tt];
		StringID += 1;
		/*
		Find segments along the string using Strider algorithm and store them in
		a list for linking
		*/
		if (StringToSegments(MaxJumpPixels,
			StringListSize,
			*StringListID,
			StringList,
			&SegmentListSize,
			&SegmentListID,
			&SegmentList) == 0) {
			
			if (LinkSegments(StringListSize,
				*StringListID,
				StringList,
				&SegmentListSize,
				&SegmentListID,
				&SegmentList) == 0) {

				for (i = 1; i <= SegmentListSize; i++) {

					Segment = ElmNumList(SegmentListID, (long)i);
					SegmentBuffer = MACCast(ORTSegment, Segment);

					/*
					Now classify the bloody things and output to appropriate files
					*/

					Height = LineLength(SegmentBuffer->MidPoint.Col,
						SegmentBuffer->MidPoint.Row,
						SegmentBuffer->VLPoint.Col,
						SegmentBuffer->VLPoint.Row);

					Length = (double)(SegmentBuffer->StringEnd -
						SegmentBuffer->StringStart + 1);
					/*
					Due to ambiguity at end points
					*/
					LengthParlVar = sqrt(2.0);
					LengthPerpVar = sqrt(2.0);
					ThetaVar = (2.0*LengthPerpVar) / Length;
					/*
					This is the condition for a segment to be classified as a straight-line
					*/
					if (Height <= 2.0*LengthPerpVar || Height >= HUGE ||
						SegmentBuffer->JctPoint.Col >= HUGE ||
						SegmentBuffer->JctPoint.Row >= HUGE) {

						LineCounter++;
						/*
						Add these segments to the line list, removing ambiguous end points.
						*/
						Line = MACAllocateMem(ORTLine);

						StringList = MACCast(ORTPoint,
							(ElmNumList(*StringListID, (long)SegmentBuffer->StringStart + 1)));

						Line->Start.Col = StringList->Col;
						Line->Start.Row = StringList->Row;

						StringList = MACCast(ORTPoint,
							(ElmNumList(*StringListID, (long)SegmentBuffer->StringEnd - 1)));

						Line->End.Col = StringList->Col;
						Line->End.Row = StringList->Row;

						Line->ID = LineCounter;
						Line->StringID = StringID;

						/*Line->MidPoint.Col = SegmentBuffer->MidPoint.Col;
						Line->MidPoint.Row = SegmentBuffer->MidPoint.Row;
						Line->Length = LineLength(Line->Start.Col,
							Line->Start.Row,
							Line->End.Col,
							Line->End.Row);

						Line->LengthParlVar = 2.0*LengthParlVar;
						Line->LengthPerpVar = LengthPerpVar;
						Line->Theta = LineSegTheta(Line->Start.Col,
							Line->Start.Row,
							Line->End.Col,
							Line->End.Row);
						Line->ThetaVar = ThetaVar;*/
						AddElmList(LineListID, (long)Line);

					}
					else {

						/*CurveCounter++;
						CircularArc = MACAllocateMem(ORTCircularArc);
						CircularArc->ID = CurveCounter;
						CircularArc->StringID = StringID;
						CircularArc->Start.Col = SegmentBuffer->Start.Col;
						CircularArc->Start.Row = SegmentBuffer->Start.Row;
						CircularArc->End.Col = SegmentBuffer->End.Col;
						CircularArc->End.Row = SegmentBuffer->End.Row;
						CircularArc->MidPoint.Col = SegmentBuffer->MidPoint.Col;
						CircularArc->MidPoint.Row = SegmentBuffer->MidPoint.Row;
						CircularArc->VLPoint.Col = SegmentBuffer->VLPoint.Col;
						CircularArc->VLPoint.Row = SegmentBuffer->VLPoint.Row;
						CircularArc->Origin.Col = SegmentBuffer->JctPoint.Col;
						CircularArc->Origin.Row = SegmentBuffer->JctPoint.Row;
						CircularArc->Radius = (1.0 / 3.0) * (
							LineLength(CircularArc->Origin.Col,
								CircularArc->Origin.Row,
								CircularArc->Start.Col,
								CircularArc->Start.Row) +
							LineLength(CircularArc->Origin.Col,
								CircularArc->Origin.Row,
								CircularArc->MidPoint.Col,
								CircularArc->MidPoint.Row) +
							LineLength(CircularArc->Origin.Col,
								CircularArc->Origin.Row,
								CircularArc->End.Col,
								CircularArc->End.Row));
						CircularArc->Length = Length;
						CircularArc->LengthParlVar = 2.0*LengthParlVar;
						CircularArc->LengthPerpVar = LengthPerpVar;
						CircularArc->Theta = LineSegTheta(CircularArc->Origin.Col,
							CircularArc->Origin.Row,
							CircularArc->MidPoint.Col,
							CircularArc->MidPoint.Row);
						CircularArc->Height = Height;
						CircularArc->Width = LineLength(SegmentBuffer->Start.Col,
							SegmentBuffer->Start.Row,
							SegmentBuffer->End.Col,
							SegmentBuffer->End.Row);
						
						//First figure out whether curve is bigger or smaller than a semicircle
						

						MidToVL = LineLength(CircularArc->MidPoint.Col,
							CircularArc->MidPoint.Row,
							CircularArc->VLPoint.Col,
							CircularArc->VLPoint.Row);

						MidToJct = LineLength(CircularArc->MidPoint.Col,
							CircularArc->MidPoint.Row,
							CircularArc->Origin.Col,
							CircularArc->Origin.Row);
						semicircle = 0;
						if (MidToVL >= MidToJct)  semicircle = 1;
						
						//Now figure out whether to draw clockwise or anticlockwise
						
						x1 = CircularArc->Start.Col;
						y1 = CircularArc->Start.Row;
						x2 = CircularArc->MidPoint.Col;
						y2 = CircularArc->MidPoint.Row;
						x3 = CircularArc->End.Col;
						y3 = CircularArc->End.Row;
						x4 = CircularArc->Origin.Col;
						y4 = CircularArc->Origin.Row;

						side = -(x4 - x3)*(y1 - y3) + (y4 - y3)*(x1 - x3);

						if (side <= 0.0 && semicircle != 1)
							direction = -1;

						if (side <= 0.0 && semicircle == 1)
							direction = 1;

						if (side  > 0.0 && semicircle != 1)
							direction = 1;

						if (side  > 0.0 && semicircle == 1)
							direction = -1;

						CircularArc->Direction = direction;

						AddElmList(CircularArcListID, (long)CircularArc);
						*/
					}
				}
			}

			/*
			Free memory used by buffer lists
			*/
			DestList(StringListID);
			DestList(&SegmentListID);

		}

	}
	//--------------------Bu noktada hesaplama bitiyor galiba -------------
	return(&LineListID);
	/*
	OutPut Results. First the line Segments.
	*/
	LineCounter = (int)SizeList(LineListID);

	


	/*
	Sort by theta and reassign ID numbers
	*/
	/*SortList(LineListID, SortByThetaAscending);

	Buffer = FirstElmList(LineListID);
	Line = MACCast(ORTLine, Buffer);
	Line->ID = 1;
	//line numaralarýný atýyor gereksiz
	for (i = 2; i <= LineCounter;i++) {
		Buffer = NextElmList(LineListID);
		Line = MACCast(ORTLine, Buffer);
		Line->ID = i;
	}*/

	/*for (i = 1; i <= LineCounter; i++) {
		Buffer = ElmNumList(LineListID, (long)i);
		Line = MACCast(ORTLine, Buffer);
		
		fprintf(p_OutFile, "%d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
			Line->ID,
			Line->StringID,
			Line->Start.Col,
			Line->Start.Row,
			Line->End.Col,
			Line->End.Row,
			Line->MidPoint.Col,
			Line->MidPoint.Row,
			Line->Length,
			Line->LengthParlVar,
			Line->LengthPerpVar,
			Line->Theta,
			Line->ThetaVar);
	}*/
	
	/*
	Now the circular Arcs
	*/

	/*CurveCounter = (int)SizeList(CircularArcListID);

	fprintf(p_OutFile, "#\n");
	fprintf(p_OutFile, "#CircularArcs\n");
	fprintf(stderr, "Number of CircularArcs = %d\n", CurveCounter);

	for (i = 1; i <= CurveCounter; i++) {
		Buffer = ElmNumList(CircularArcListID, (long)i);
		CircularArc = MACCast(ORTCircularArc, Buffer);

		fprintf(p_OutFile, "%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
			CircularArc->ID,
			CircularArc->StringID,
			CircularArc->Direction,
			CircularArc->Origin.Col,
			CircularArc->Origin.Row,
			CircularArc->Start.Col,
			CircularArc->Start.Row,
			CircularArc->MidPoint.Col,
			CircularArc->MidPoint.Row,
			CircularArc->End.Col,
			CircularArc->End.Row,
			CircularArc->VLPoint.Col,
			CircularArc->VLPoint.Row,
			CircularArc->Radius,
			CircularArc->Length,
			CircularArc->LengthParlVar,
			CircularArc->LengthPerpVar,
			CircularArc->Width,
			CircularArc->Height,
			CircularArc->Theta);
	}*/

	/*
	Free memory used by remaining lists
	*/
	
	DestList(&CircularArcListID);

	/*
	Close all files
	*/
	//fclose(p_InFile);
	//fclose(p_OutFile);

	return(0);
}



Liste *FEX_Modified(Liste &LineListID, int noof_segments, Liste *StringListIDL, int width, int height)
{

	/*
	=======================================================================
	===================== START OF DECLARATION ============================
	=======================================================================
	*/
	int i, NoOfRows, NoOfCols;
	int MaxJumpPixels;
	int Next;
	int LineCounter;
	int CurveCounter;

	FILE *p_InFile;		/* Pointers to start of files fo I/O */
	FILE *p_OutFile;

	/*
	Number of lines in corresponding lists

	*/
	int StringListSize;	  /* Number of points in string */
	int SegmentListSize;	  /* Number of segments found along string */
	int StringID;
	/*
	ID numbers of appropriate lists

	*/
	Liste *StringListID;
	Liste SegmentListID;

	Liste CircularArcListID;

	/*
	Pointer to list

	*/
	struct ORTPoint          *StringList;
	struct ORTSegment        *SegmentList;
	struct ORTLine    	     *Line;
	struct ORTCircularArc    *CircularArc;

	/*
	Buffers for outputting result
	*/

	int semicircle;
	int direction;

	long Buffer;
	long Segment;

	struct ORTSegment *SegmentBuffer;
	struct ORTPoint   *PointBuffer;

	double Length;
	double LengthParlVar;
	double LengthPerpVar;
	double ThetaVar;
	double Height;
	double Width;

	double StartCol, EndCol;
	double StartRow, EndRow;

	double side;
	double x1, x2, x3, x4;
	double y1, y2, y3, y4;
	double MidToVL;
	double MidToJct;
	double linelen;
	/*
	=======================================================================
	===================== START OF INITIALISATION =========================
	=======================================================================
	*/


	/*
	Set defaults, process command line options, and initialise lists
	*/

	Next = 0;
	MaxJumpPixels = 2;



	/*
	Use standard input output I/O

	*/
	//p_InFile = fopen("myfile.txt", "r");;
	//p_OutFile = fopen("lines.txt", "w");;

	/*
	=======================================================================
	============================ MAIN LOOP ================================
	=======================================================================
	*/

	/*
	Get one string from string file and store it in a linked list for processing
	*/

	LineListID = CreatList();
	CircularArcListID = CreatList();

	LineCounter = 0;
	CurveCounter = 0;
	StringID = 0;
	int tt;
	NoOfCols = width; NoOfRows = height;
	for (tt = 1;tt<noof_segments;tt++)
	{
		StringListSize = (int)SizeList(StringListIDL[tt]);
		StringListID = &StringListIDL[tt];
		//Line length limit
		if (StringListSize < 7) continue;
		//StringListID = &StringListIDL[tt];
		StringID += 1;
		/*
		Find segments along the string using Strider algorithm and store them in
		a list for linking
		*/
		if (StringToSegments(MaxJumpPixels,
			StringListSize,
			*StringListID,
			StringList,
			&SegmentListSize,
			&SegmentListID,
			&SegmentList) == 0) {

			if (LinkSegments(StringListSize,
				*StringListID,
				StringList,
				&SegmentListSize,
				&SegmentListID,
				&SegmentList) == 0) {

				for (i = 1; i <= SegmentListSize; i++) {

					Segment = ElmNumList(SegmentListID, (long)i);
					SegmentBuffer = MACCast(ORTSegment, Segment);

					/*
					Now classify the bloody things and output to appropriate files
					*/
					linelen=LineLength(SegmentBuffer->Start.Col,SegmentBuffer->Start.Row,
						SegmentBuffer->End.Col,SegmentBuffer->End.Row);
					//Line length limit
					if (linelen < 6.0)continue;
					Height = LineLength(SegmentBuffer->MidPoint.Col,
						SegmentBuffer->MidPoint.Row,
						SegmentBuffer->VLPoint.Col,
						SegmentBuffer->VLPoint.Row);

					Length = (double)(SegmentBuffer->StringEnd -
						SegmentBuffer->StringStart + 1);
					/*
					Due to ambiguity at end points
					*/
					LengthParlVar = sqrt(2.0);
					LengthPerpVar = sqrt(2.0);
					ThetaVar = (2.0*LengthPerpVar) / Length;
					/*
					This is the condition for a segment to be classified as a straight-line
					*/
					if (Height <= 2.0*LengthPerpVar || Height >= HUGE ||
						SegmentBuffer->JctPoint.Col >= HUGE ||
						SegmentBuffer->JctPoint.Row >= HUGE) {

						LineCounter++;
						/*
						Add these segments to the line list, removing ambiguous end points.
						*/
						Line = MACAllocateMem(ORTLine);

						StringList = MACCast(ORTPoint,
							(ElmNumList(*StringListID, (long)SegmentBuffer->StringStart + 1)));

						Line->Start.Col = StringList->Col;
						Line->Start.Row = StringList->Row;

						StringList = MACCast(ORTPoint,
							(ElmNumList(*StringListID, (long)SegmentBuffer->StringEnd - 1)));

						Line->End.Col = StringList->Col;
						Line->End.Row = StringList->Row;

						Line->ID = LineCounter;
						Line->StringID = StringID;

						/*Line->MidPoint.Col = SegmentBuffer->MidPoint.Col;
						Line->MidPoint.Row = SegmentBuffer->MidPoint.Row;
						Line->Length = LineLength(Line->Start.Col,
							Line->Start.Row,
							Line->End.Col,
							Line->End.Row);

						Line->LengthParlVar = 2.0*LengthParlVar;
						Line->LengthPerpVar = LengthPerpVar;
						Line->Theta = LineSegTheta(Line->Start.Col,
							Line->Start.Row,
							Line->End.Col,
							Line->End.Row);
						Line->ThetaVar = ThetaVar;*/
						AddElmList(LineListID, (long)Line);

					}
					else {

						/*CurveCounter++;
						CircularArc = MACAllocateMem(ORTCircularArc);
						CircularArc->ID = CurveCounter;
						CircularArc->StringID = StringID;
						CircularArc->Start.Col = SegmentBuffer->Start.Col;
						CircularArc->Start.Row = SegmentBuffer->Start.Row;
						CircularArc->End.Col = SegmentBuffer->End.Col;
						CircularArc->End.Row = SegmentBuffer->End.Row;
						CircularArc->MidPoint.Col = SegmentBuffer->MidPoint.Col;
						CircularArc->MidPoint.Row = SegmentBuffer->MidPoint.Row;
						CircularArc->VLPoint.Col = SegmentBuffer->VLPoint.Col;
						CircularArc->VLPoint.Row = SegmentBuffer->VLPoint.Row;
						CircularArc->Origin.Col = SegmentBuffer->JctPoint.Col;
						CircularArc->Origin.Row = SegmentBuffer->JctPoint.Row;
						CircularArc->Radius = (1.0 / 3.0) * (
						LineLength(CircularArc->Origin.Col,
						CircularArc->Origin.Row,
						CircularArc->Start.Col,
						CircularArc->Start.Row) +
						LineLength(CircularArc->Origin.Col,
						CircularArc->Origin.Row,
						CircularArc->MidPoint.Col,
						CircularArc->MidPoint.Row) +
						LineLength(CircularArc->Origin.Col,
						CircularArc->Origin.Row,
						CircularArc->End.Col,
						CircularArc->End.Row));
						CircularArc->Length = Length;
						CircularArc->LengthParlVar = 2.0*LengthParlVar;
						CircularArc->LengthPerpVar = LengthPerpVar;
						CircularArc->Theta = LineSegTheta(CircularArc->Origin.Col,
						CircularArc->Origin.Row,
						CircularArc->MidPoint.Col,
						CircularArc->MidPoint.Row);
						CircularArc->Height = Height;
						CircularArc->Width = LineLength(SegmentBuffer->Start.Col,
						SegmentBuffer->Start.Row,
						SegmentBuffer->End.Col,
						SegmentBuffer->End.Row);

						//First figure out whether curve is bigger or smaller than a semicircle


						MidToVL = LineLength(CircularArc->MidPoint.Col,
						CircularArc->MidPoint.Row,
						CircularArc->VLPoint.Col,
						CircularArc->VLPoint.Row);

						MidToJct = LineLength(CircularArc->MidPoint.Col,
						CircularArc->MidPoint.Row,
						CircularArc->Origin.Col,
						CircularArc->Origin.Row);
						semicircle = 0;
						if (MidToVL >= MidToJct)  semicircle = 1;

						//Now figure out whether to draw clockwise or anticlockwise

						x1 = CircularArc->Start.Col;
						y1 = CircularArc->Start.Row;
						x2 = CircularArc->MidPoint.Col;
						y2 = CircularArc->MidPoint.Row;
						x3 = CircularArc->End.Col;
						y3 = CircularArc->End.Row;
						x4 = CircularArc->Origin.Col;
						y4 = CircularArc->Origin.Row;

						side = -(x4 - x3)*(y1 - y3) + (y4 - y3)*(x1 - x3);

						if (side <= 0.0 && semicircle != 1)
						direction = -1;

						if (side <= 0.0 && semicircle == 1)
						direction = 1;

						if (side  > 0.0 && semicircle != 1)
						direction = 1;

						if (side  > 0.0 && semicircle == 1)
						direction = -1;

						CircularArc->Direction = direction;

						AddElmList(CircularArcListID, (long)CircularArc);
						*/
					}
				}
			}

			/*
			Free memory used by buffer lists
			*/
			DestList(StringListID);
			DestList(&SegmentListID);

		}

	}
	//--------------------Bu noktada hesaplama bitiyor galiba -------------
	return(&LineListID);
	/*
	OutPut Results. First the line Segments.
	*/
	LineCounter = (int)SizeList(LineListID);




	/*
	Sort by theta and reassign ID numbers
	*/
	SortList(LineListID, SortByThetaAscending);

	Buffer = FirstElmList(LineListID);
	Line = MACCast(ORTLine, Buffer);
	Line->ID = 1;
	//line numaralarýný atýyor gereksiz
	for (i = 2; i <= LineCounter;i++) {
		Buffer = NextElmList(LineListID);
		Line = MACCast(ORTLine, Buffer);
		Line->ID = i;
	}

	for (i = 1; i <= LineCounter; i++) {
		Buffer = ElmNumList(LineListID, (long)i);
		Line = MACCast(ORTLine, Buffer);

		/*fprintf(p_OutFile, "%d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
		Line->ID,
		Line->StringID,
		Line->Start.Col,
		Line->Start.Row,
		Line->End.Col,
		Line->End.Row,
		Line->MidPoint.Col,
		Line->MidPoint.Row,
		Line->Length,
		Line->LengthParlVar,
		Line->LengthPerpVar,
		Line->Theta,
		Line->ThetaVar);*/
	}

	/*
	Now the circular Arcs
	*/

	/*CurveCounter = (int)SizeList(CircularArcListID);

	fprintf(p_OutFile, "#\n");
	fprintf(p_OutFile, "#CircularArcs\n");
	fprintf(stderr, "Number of CircularArcs = %d\n", CurveCounter);

	for (i = 1; i <= CurveCounter; i++) {
		Buffer = ElmNumList(CircularArcListID, (long)i);
		CircularArc = MACCast(ORTCircularArc, Buffer);

		fprintf(p_OutFile, "%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
			CircularArc->ID,
			CircularArc->StringID,
			CircularArc->Direction,
			CircularArc->Origin.Col,
			CircularArc->Origin.Row,
			CircularArc->Start.Col,
			CircularArc->Start.Row,
			CircularArc->MidPoint.Col,
			CircularArc->MidPoint.Row,
			CircularArc->End.Col,
			CircularArc->End.Row,
			CircularArc->VLPoint.Col,
			CircularArc->VLPoint.Row,
			CircularArc->Radius,
			CircularArc->Length,
			CircularArc->LengthParlVar,
			CircularArc->LengthPerpVar,
			CircularArc->Width,
			CircularArc->Height,
			CircularArc->Theta);
	}*/

	/*
	Free memory used by remaining lists
	*/

	DestList(&CircularArcListID);

	/*
	Close all files
	*/
	fclose(p_InFile);
	fclose(p_OutFile);

	return(0);
}


/* ===================================================== */
/* 	Beginning of PSEUDO INVERSE subroutine		 */
/* ===================================================== */


void PseudoInverse3n(int StringStart, int StringEnd, int *det_flag,
	double a1[MAXPOINT][3],	double b1[MAXPOINT],	double c[3])
{
	int ii, jj, n;
	double aa[4][4], bb[4][4];
	double cc[3];
	double det;

	for (ii = 0; ii < 3; ii++) {
		cc[ii] = 0.0;
		for (jj = 0; jj < 3; jj++) {
			aa[ii][jj] = 0.0;
			bb[ii][jj] = 0.0;
		}
	}

	for (ii = 0; ii < 3; ii++) {
		for (jj = 0; jj < 3; jj++) {
			for (n = StringStart; n <= StringEnd; n++) {
				aa[ii][jj] = aa[ii][jj] + a1[n][ii] * a1[n][jj];
			} /* end for n */
		} /* end for jj */
	} /* end for ii */

	det = aa[0][0] * aa[1][1] * aa[2][2] -
		aa[0][0] * aa[1][2] * aa[2][1] +
		aa[0][1] * aa[1][2] * aa[2][0] -
		aa[0][1] * aa[1][0] * aa[2][2] +
		aa[0][2] * aa[1][0] * aa[2][1] -
		aa[0][2] * aa[1][1] * aa[2][0];

	if (det == 0.0) {
		*det_flag = 1;

	}
	else {

		bb[0][0] = (aa[1][1] * aa[2][2] - aa[2][1] * aa[1][2]) / det;
		bb[0][1] = (aa[2][1] * aa[0][2] - aa[0][1] * aa[2][2]) / det;
		bb[0][2] = (aa[0][1] * aa[1][2] - aa[1][1] * aa[0][2]) / det;
		bb[1][0] = (aa[2][0] * aa[1][2] - aa[1][0] * aa[2][2]) / det;
		bb[1][1] = (aa[0][0] * aa[2][2] - aa[2][0] * aa[0][2]) / det;
		bb[1][2] = (aa[1][0] * aa[0][2] - aa[0][0] * aa[1][2]) / det;
		bb[2][0] = (aa[1][0] * aa[2][1] - aa[2][0] * aa[1][1]) / det;
		bb[2][1] = (aa[2][0] * aa[0][1] - aa[0][0] * aa[2][1]) / det;
		bb[2][2] = (aa[0][0] * aa[1][1] - aa[1][0] * aa[0][1]) / det;

		for (ii = 0; ii < 3; ii++) {
			for (n = StringStart; n <= StringEnd; n++) {
				cc[ii] = cc[ii] + a1[n][ii] * b1[n];
			}
		}

		for (ii = 0; ii < 3; ii++) {
			for (jj = 0; jj < 3; jj++) {
				c[ii] = c[ii] + bb[jj][ii] * cc[jj];
			}
		}

	} /* end of else */

}


/* =============================================== */
/* 							  */
/* 	Circle Parameter Finding Algorithm		  */
/* 	----------------------------------		  */
/* 	using * Least Squared Error Method		  */
/* 							  */
/* ----------------------------------------------- */
/* 							  */
/*     Author    : Graeme A. Jones 	         */
/* 	Created   : 27th July '89			  */
/* 	Modified  : 10th Oct  '91	GAJ		  */
/* 							  */
/* 	Least Squared Error Subroutine		  */
/* =============================================== */

int CircleLeastSq(double StringCol[MAXPOINT], double StringRow[MAXPOINT],int StringStart,int StringEnd,
double  *a, double*b, double*r)
/*
Variable Definitions:
---------------------
StringCol,StringRow  : String arrays
StringStart,StringEnd: Start/end points of string
a,b,r	         : input  : initial values of center pt and radius
: output : final values of center pt and radius

*/

{
	int e, det_flag;
	double xx, yy;
	double xmatrix[MAXPOINT][3];
	double zmatrix[MAXPOINT];
	double parameters[3];
	double init_x, init_y;

	init_x = (*a);
	init_y = (*b);

	for (e = StringStart; e <= StringEnd; e++) {

		xx = StringCol[e] - init_x;
		yy = StringRow[e] - init_y;

		xmatrix[e][0] = xx;
		xmatrix[e][1] = yy;
		xmatrix[e][2] = 1.0;
		zmatrix[e] = 0.5*(xx*xx + yy*yy);
	}

	det_flag = 0;
	for (e = 0; e < 3; e++) {
		parameters[e] = 0.0;
	}

	PseudoInverse3n(StringStart, StringEnd,
		&det_flag, xmatrix, zmatrix, parameters);

	if (det_flag == 0) {
		*a = parameters[0];
		*b = parameters[1];
		*r = parameters[2];
		*r = 2.0*(*r) + (*a)*(*a) + (*b)*(*b);
		if (*r <= 0.0) {
			return(-1);
		}
		else {
			*r = sqrt((*r));
			*a = *a + init_x;
			*b = *b + init_y;
		}
	}
	else {
		return(-1);
	}
	return(0);
}

/* ===================================================== */
/* 	End of LEAST SQ ERROR subroutine			 */
/* ===================================================== */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Name : CreateStringList
Type : int
Written on   : 8-Mar-90     By : A. Etemadi
Modified on  : 16-Feb-93    By : A. Etemadi

Added support for rectangular images
Removed print flag option

Directory    : ~atae/ORT/FEX/src

==============================================================================

Input parameters    :

p_InFile 		-- Pointer to data file

Output parameters   :

StringListSize	-- Number of pixels in string
StringListID		-- ID of Linked List of pixels belonging to string
StringList		-- Linked List of pixels belonging to string

Output Result :

0  Successful
1  Error
-1  EOF

Calling procedure:

FILE *p_InFile;

int StringListSize;

Liste StringListID;

struct ORTPoint *StringList;

CreateStringList( &p_InFile,
&NoOfCols,
&NoOfRows,
&StringListSize,
&StringListID,
&StringList);

Functionality:

This function reads the positions of the pixels in the strings produced by the
CreateString program, stores them in a linked list and passes them back.
One string is read at a time (hence the **p_InFile buisness to keep track of
the position in the file).

******************
COORDINATE SYSTEM OF ROSIN'S STRING DATA IS NOT THE SAME AS THE ORT.
A) ROWS AND COLS SHOULD BE SWAPPED
B) WE SHOULD CENTER THINGS AT 0,0 BY SUBTACTING HALF THE SIZE OF THE IMAGE
THESE HAVE TO BE CHANGED IF WE USE ANOTHER STRING FINDER INSTEAD OF ROSIN'S
******************
----------------------------------------------------------------------------*/
int  CreateStringList(FILE *(*p_InFile),int *NoOfCols,
	int *NoOfRows,	int *StringListSize, Liste *StringListID,
	ORTPoint *(*StringList))
{

	int flag;
	int Row, Col;

#ifdef debug
	fprintf(stderr, " Start of function CreateStringList \n");
#endif

	/*
	Read Image dimensions
	*/

	fscanf((*p_InFile), "%d %d\n", NoOfCols, NoOfRows);

	if (SkipLines(&(*p_InFile),100, 1, 1) != 0) {
		fprintf(stderr, "Reached the end of the file...\n");
		return(-1);
	}

	*StringListID = CreatList();
	*StringListSize = 0;

	flag = 1;
	while (flag > 0) {

		/*
		Allocate some memory for the StringList structure

		*/

		*StringList = MACAllocateMem(ORTPoint);

		/*
		Now read a String record

		*/

		flag = fscanf((*p_InFile), "%4d %4d\n", &Row, &Col);

		if (flag == 0)
			return(1);

		if (flag == EOF)
			return(-1);

		if (Row == -1)
			return(0);

		/*
		******************
		COORDINATE SYSTEM OF ROSIN'S STRING DATA IS NOT THE SAME AS THE ORT.
		A) ROWS AND COLS SHOULD BE SWAPPED
		B) WE SHOULD CENTER THINGS AT 0,0 BY SUBTACTING HALF THE SIZE OF THE IMAGE
		THESE HAVE TO BE CHANGED IF WE USE ANOTHER STRING FINDER INSTEAD OF ROSIN'S
		*****************
		*/

		(*StringList)->Col = (double)Row - ((double)(*NoOfCols)) / 2.0;
		(*StringList)->Row = (double)Col - ((double)(*NoOfRows)) / 2.0;
		/*
		Now add this element to the list

		*/
		AddElmList(*StringListID, (long)*StringList);
		//(*StringListSize)++;

	}
}


int DrawBezierLine(int x1, int x2, int y1, int y2,int LineIntensity,
int LineThickness, int EndPointIntensity, int xsize,int ysize, char Image[HEIGHT][WIDTH])
{
	int temp, deltax, deltay, xincrement, yincrement, e, a, b, i;
	int k, l;

	for (k = (x1 - 1); k <= (x1 + 1); k++) {
		for (l = (y1 - 1); l <= (y1 + 1); l++) {
			if (l <= ysize && k <= xsize && l >= 0 && k >= 0) {
				Image[l][k] = (EndPointIntensity & 0377);
			}
		}
	}

	for (k = (x2 - 1); k <= (x2 + 1); k++) {
		for (l = (y2 - 1); l <= (y2 + 1); l++) {
			if (l <= ysize && k <= xsize && l >= 0 && k >= 0) {
				Image[l][k] = (EndPointIntensity & 0377);
			}
		}
	}

	if (abs(x2 - x1) > abs(y2 - y1)) {
		if (x1 > x2) {
			temp = x1; x1 = x2; x2 = temp;
			temp = y1; y1 = y2; y2 = temp;
		}
		deltax = x2 - x1;
		deltay = abs(y2 - y1);
		e = (2 * deltay) - deltax;
		a = (2 * deltay) - (2 * deltax);
		b = (2 * deltay);
		if (y2 > y1) {
			yincrement = 1;
		}
		else {
			yincrement = -1;
		}
		for (i = 0;i <= deltax;i++) {

			if ((x1>0) && (x1 <= xsize) && (y1>0) && (y1 <= ysize)) {
				for (k = (x1 - LineThickness); k <= (x1 + LineThickness); k++) {
					for (l = (y1 - LineThickness); l <= (y1 + LineThickness); l++) {
						Image[l][k] = (LineIntensity & 0377);
					}
				}
			}
			if (e > 0) {
				y1 = y1 + yincrement;
				e = e + a;
			}
			else {
				e = e + b;
			}
			x1 = x1 + 1;
		}
	}
	else {
		if (y1 > y2) {
			temp = x1; x1 = x2; x2 = temp;
			temp = y1; y1 = y2; y2 = temp;
		}
		deltay = y2 - y1;
		deltax = abs(x2 - x1);
		e = (2 * deltax) - deltay;
		a = (2 * deltax) - (2 * deltay);
		b = (2 * deltax);
		if (x2 > x1) {
			xincrement = 1;
		}
		else {
			xincrement = -1;
		}
		for (i = 0;i <= deltay;i++) {

			if ((x1>0) && (x1 <= xsize) && (y1>0) && (y1 <= ysize)) {
				for (k = (x1 - LineThickness); k <= (x1 + LineThickness); k++) {
					for (l = (y1 - LineThickness); l <= (y1 + LineThickness); l++) {
						Image[l][k] = (LineIntensity & 0377);
					}
				}
			}
			if (e > 0) {
				x1 = x1 + xincrement;
				e = e + a;
			}
			else {
				e = e + b;
			}
			y1 = y1 + 1;
		}
	}
	return(0);
}

int sgn(int x)

{
	if (x <  0) return(-1);
	if (x == 0) return(0);
	return(1);
}

int sgn2(int x, int y)

{
	if (x != 0) {
		return(sgn(x));
	}
	else {
		return(sgn(y));
	}

}


void DrawCircularArc(int x2, int y2, int x1, int y1, int x0, int y0, int dir, int rsq,
	int ArcIntensity, int ArcThickness, int	EndPointIntensity, int xsize, int ysize, char Image[HEIGHT][WIDTH])
/* procedure for drawing arc of a circle
x0,y0 : circle centre
x1,y1 : start point of arc
x2,y2 : end point of arc
rsq   : radius squared
source of algorithm is the optimal integer lattice formulation in
McIllroy paper on circle drawing  ACM Trans on Graphics, Vol 2, No 4,
October 1983. pp 237-263
*/

{
	int e, ex, ey, exy, dxe, dye, d2e, dx, dy, xpoint, ypoint;
	int dr;
	int done;
	int end_of_arc;
	int k, l;



	for (k = (x1 - 2); k <= (x1 + 2); k++) {
		for (l = (y1 - 2); l <= (y1 + 2); l++) {
			if (k >= 0 && l >= 0 && k <= xsize && l <= ysize)
				Image[l][k] = (EndPointIntensity & 0377);
		}
	}

	for (k = (x2 - 2); k <= (x2 + 2); k++) {
		for (l = (y2 - 2); l <= (y2 + 2); l++) {
			if (k >= 0 && l >= 0 && k <= xsize && l <= ysize)
				Image[l][k] = (EndPointIntensity & 0377);
		}
	}

	x1 = x1 - x0; y1 = y1 - y0;
	e = x1*x1 + y1*y1 - rsq;
	dr = -sgn(e);

	done = 0;
	while (done != 1) {

		if (abs(y1) > abs(x1)) {
			dx = 0;
			dy = sgn(y1)*dr;
		}
		else {
			if (x1 != 0) {
				dx = sgn(x1)*dr;
				dy = 0;
			}
			else {
				dx = 1;
				dy = 0;
			}
		}

		exy = e + 2 * (x1*dx + y1*dy) + 1;
		done = (abs(exy) > abs(e));

		if (done == 0) {
			x1 = x1 + dx;
			y1 = y1 + dy;
			e = exy;
		}
	}

	x2 = x2 - x0; y2 = y2 - y0;
	e = x2*x2 + y2*y2 - rsq;
	dr = -sgn(e);
	done = 0;
	while (done != 1) {

		if (abs(y2) > abs(x2)) {
			dx = 0;
			dy = sgn(y2)*dr;
		}
		else {
			if (x2 != 0) {
				dx = sgn(x2)*dr;
				dy = 0;
			}
			else {
				dx = 1;
				dy = 0;
			}
		}

		exy = e + 2 * (x2*dx + y2*dy) + 1;
		done = (abs(exy) > abs(e));

		if (done == 0) {
			x2 = x2 + dx;
			y2 = y2 + dy;
			e = exy;
		}
	}

	dx = sgn2(-y1*dir, -x1);
	dy = sgn2(x1*dir, -y1);
	e = x1*x1 + y1*y1 - rsq;
	dxe = 2 * x1*dx;
	dye = 2 * y1*dy;
	d2e = 2;

	if (dx*dy*dir > 0) {
		e = -e;
		dxe = -dxe;
		dye = -dye;
		d2e = -d2e;
	}

	end_of_arc = 0;
	while (end_of_arc != 1) {

		xpoint = x0 + x1;
		ypoint = y0 + y1;
		if ((xpoint >= 0) && (xpoint <= xsize) &&
			(ypoint >= 0) && (ypoint <= ysize)) {

			for (k = (xpoint - ArcThickness);k <= (xpoint + ArcThickness);k++) {
				for (l = (ypoint - ArcThickness);l <= (ypoint + ArcThickness);l++) {
					if (k >= 0 && l >= 0 && k <= xsize && l <= ysize)
						Image[l][k] = (ArcIntensity & 0377);
				}
			}

		}

		ex = e + dxe;
		ey = e + dye;
		exy = e + dxe + dye;

		if (-exy < ey) {
			x1 = x1 + dx;
			e = e + dxe;
			dxe = dxe + d2e;
		}

		if (exy < -ex) {
			y1 = y1 + dy;
			e = e + dye;
			dye = dye + d2e;
		}

		if (x1 == 0) {
			dy = -dy;
			dye = -dye + d2e;
			e = -e;
			dxe = -dxe;
			dye = -dye;
			d2e = -d2e;
		}

		if (y1 == 0) {
			dx = -dx;
			dxe = -dxe + d2e;
			e = -e;
			dxe = -dxe;
			dye = -dye;
			d2e = -d2e;
		}
		if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1)
			end_of_arc = 1;
	}
}

#define MaxDisparity 1.4142137

int LinkSegments(int StringListSize,
	Liste StringListID,
	ORTPoint *StringList,
	int *SegmentListSize,
	Liste *SegmentListID,
	ORTSegment *(*SegmentList))
{

	int i;		/* Dummy variables */
	int JoinSegments;

	int OldSegmentID;
	int NewSegmentID;
	int OldNoOfSegments;
	int NewNoOfSegments;
	int OldSegmentCurvature;
	int NewSegmentCurvature;

	struct ORTSegment *OldSegment;
	struct ORTSegment *NewSegment;

	double OldSegmentTheta;
	double NewSegmentTheta;
	double OldGrad;
	double NewGrad;

	double JunctionPtCol;
	double JunctionPtRow;
	double InitJctCol;
	double InitJctRow;
	double Radius;

	double OldMidLength;
	double NewMidLength;
	double OldEndLength;
	double NewEndLength;
	double OldStartLength;
	double NewStartLength;

	double StringRow[MAXPOINT];
	double StringCol[MAXPOINT];

	double VLColIntercept;
	double VLRowIntercept;

	double OldHeight;
	double NewHeight;
	double OldEndHeight;

	int Length;
	double NewStartHeight;
	double Height;
	double Row, Col;
	double VLTheta;
	double MidPtCol;
	double MidPtRow;

	int LinkedSegmentLength;
	Liste LinkedSegmentListID;
	Liste BufferListID;
	struct ORTSegment *LinkedSegment;

#ifdef debug
	fprintf(stderr, " Start of function LinkSegments \n");
#endif

	/*
	String is too small to process
	*/

	if (StringListSize < 3)
		return(-1);
	/*
	Only one segment so no point linking it..
	*/
	if ((*SegmentListSize) == 1)
		return(0);

	LinkedSegmentListID = CreatList();
	/*
	Store string data in array
	*/

	for (i = 1;i <= StringListSize;i++) {
		StringList = MACCast(ORTPoint, (ElmNumList(StringListID, (long)i)));
		StringRow[i] = StringList->Row;
		StringCol[i] = StringList->Col;
	}

	/*
	Initialise variables
	*/
	OldNoOfSegments = (*SegmentListSize);
	NewNoOfSegments = 2;
	OldSegmentID = 1;
	NewSegmentID = 2;
	/*
	Loop until we can't link any more segments together
	*/
	while (NewNoOfSegments < OldNoOfSegments &&
		NewNoOfSegments > 1 && OldNoOfSegments > 1) {
		/*
		Loop until we've joined all segments we could in this pass
		*/
		while (NewSegmentID <= (*SegmentListSize) &&
			(*SegmentListSize) > 1) {

			/*
			Find the junction point (if any) between lines passing through the VL point
			and the midpoint of the segment, which should be perpendicular to the segment.
			This will
			a) give us an idea of the relative curvature of the two segments
			b) allow us to check if the two segments are co-curvilinear.
			*/
			OldSegment = MACCast(ORTSegment,
				(ElmNumList((*SegmentListID), (long)OldSegmentID)));

			NewSegment = MACCast(ORTSegment,
				(ElmNumList((*SegmentListID), (long)NewSegmentID)));

			/*
			First find the orientation of the lines joining the midpoint to the VL point.
			by rotating the VL by PIBY2.
			*/

			OldSegmentTheta = (OldSegment->VLTheta) + PIBY2;
			if (OldSegmentTheta > PI)
				OldSegmentTheta = OldSegmentTheta - PI;

			NewSegmentTheta = (NewSegment->VLTheta) + PIBY2;
			if (NewSegmentTheta > PI)
				NewSegmentTheta = NewSegmentTheta - PI;
			/*
			Now we got the orientations of the perps to the segments we can find the
			junction point. We check first though to see if the segments are not collinear.
			*/

			JoinSegments = 1;

			/*
			Find the junction point
			*/

			OldGrad = tan(OldSegmentTheta);
			NewGrad = tan(NewSegmentTheta);

			if (fabs(OldSegmentTheta - NewSegmentTheta) < 0.001) {
				JunctionPtCol = HUGE;
				JunctionPtRow = HUGE;

			}
			else {

				InitJctCol = JunctionPtCol = ((NewSegment->MidPoint.Row) -
					(OldSegment->MidPoint.Row) -
					(NewGrad * (NewSegment->MidPoint.Col)) +
					(OldGrad * (OldSegment->MidPoint.Col))) /
					(OldGrad - NewGrad);

				InitJctRow = JunctionPtRow = (OldSegment->MidPoint.Row) +
					(OldGrad * (JunctionPtCol - (OldSegment->MidPoint.Col)));

				Radius = LineLength(JunctionPtCol, JunctionPtRow,
					StringCol[OldSegment->StringEnd],
					StringRow[OldSegment->StringEnd]);

				if (CircleLeastSq(StringCol, StringRow,
					OldSegment->StringStart,
					NewSegment->StringEnd,
					&JunctionPtCol, &JunctionPtRow, &Radius) != 0) {
					JunctionPtCol = InitJctCol;
					JunctionPtRow = InitJctRow;
				}

			}
			/*
			Find the heights associated with the segments
			*/
			OldHeight = LineLength(OldSegment->VLPoint.Col,
				OldSegment->VLPoint.Row,
				OldSegment->MidPoint.Col,
				OldSegment->MidPoint.Row);

			NewHeight = LineLength(NewSegment->VLPoint.Col,
				NewSegment->VLPoint.Row,
				NewSegment->MidPoint.Col,
				NewSegment->MidPoint.Row);

			/*
			Now find the curvatures of the segments relative to the junction point
			*/

			OldMidLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				OldSegment->MidPoint.Col,
				OldSegment->MidPoint.Row);

			NewMidLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				NewSegment->MidPoint.Col,
				NewSegment->MidPoint.Row);

			OldEndLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				OldSegment->End.Col,
				OldSegment->End.Row);

			NewEndLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				NewSegment->End.Col,
				NewSegment->End.Row);

			OldStartLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				OldSegment->Start.Col,
				OldSegment->Start.Row);

			NewStartLength = LineLength(JunctionPtCol,
				JunctionPtRow,
				NewSegment->Start.Col,
				NewSegment->Start.Row);
			if (OldMidLength >
				LineLength(JunctionPtCol,
					JunctionPtRow,
					OldSegment->VLPoint.Col,
					OldSegment->VLPoint.Row) ||
				OldHeight <= MaxDisparity) {
				OldSegmentCurvature = 0;
			}
			else {
				OldSegmentCurvature = 1;
			}

			if (NewMidLength >
				LineLength(JunctionPtCol,
					JunctionPtRow,
					NewSegment->VLPoint.Col,
					NewSegment->VLPoint.Row) ||
				NewHeight <= MaxDisparity) {
				NewSegmentCurvature = 0;
			}
			else {
				NewSegmentCurvature = 1;
			}

			/*
			Check if segments are co-curvilinear and if so join them. The criterion is:

			1. They have same curvature which is not infinity (HUGE)
			2. The end point of the old segment is 3 pixels or less away from
			the start point of the new segment
			3. The difference between any pair of the distances between
			the points along the suspected curve and the center of curvature
			of the combined segments is less than MaxDisparity
			*/

			if ((NewSegmentCurvature == OldSegmentCurvature &&
				abs(OldSegment->StringEnd - NewSegment->StringStart) <= 3 &&
				fabs(OldMidLength - NewMidLength) <= MaxDisparity &&
				fabs(OldMidLength - NewStartLength) <= MaxDisparity &&
				fabs(OldMidLength - NewEndLength) <= MaxDisparity &&
				fabs(OldMidLength - OldEndLength) <= MaxDisparity &&
				fabs(OldMidLength - OldStartLength) <= MaxDisparity &&
				fabs(NewMidLength - NewStartLength) <= MaxDisparity &&
				fabs(NewMidLength - NewEndLength) <= MaxDisparity &&
				fabs(NewMidLength - OldEndLength) <= MaxDisparity &&
				fabs(NewMidLength - OldStartLength) <= MaxDisparity))
				JoinSegments = 0;
			/*
			Check if the segments are collinear, and if so join them. The criterion is :

			1. The end point of the old segment is 2 pixels or less away from
			the start point of the new segment
			2. They have the same curvature
			3. The joined segment is linear ie the heights of perps. dropped
			onto the line joining the endpts of the joined segment is less
			than 2.0 (otherwise they'll be classified as curves)
			*/

			LineSegColIntercept(OldSegment->Start.Col,
				OldSegment->Start.Row,
				NewSegment->End.Col,
				NewSegment->End.Row,
				&VLColIntercept);

			LineSegRowIntercept(OldSegment->Start.Col,
				OldSegment->Start.Row,
				NewSegment->End.Col,
				NewSegment->End.Row,
				&VLRowIntercept);

			VLTheta = LineSegTheta(OldSegment->Start.Col,
				OldSegment->Start.Row,
				NewSegment->End.Col,
				NewSegment->End.Row);

			PtLinePerpIntercept(OldSegment->End.Col,
				OldSegment->End.Row,
				VLColIntercept,
				VLRowIntercept,
				VLTheta,
				&Col, &Row);

			OldEndHeight = LineLength(Col, Row,
				OldSegment->End.Col,
				OldSegment->End.Row);

			PtLinePerpIntercept(NewSegment->Start.Col,
				NewSegment->Start.Row,
				VLColIntercept,
				VLRowIntercept,
				VLTheta,
				&Col, &Row);

			NewStartHeight = LineLength(Col, Row,
				NewSegment->Start.Col,
				NewSegment->Start.Row);

			Length = NewSegment->StringEnd - OldSegment->StringStart + 1;

			MidPtCol =
				(StringCol[OldSegment->StringStart + (int)(Length / 2)] +
					StringCol[NewSegment->StringEnd - (int)(Length / 2)]) / 2.0;

			MidPtRow =
				(StringRow[OldSegment->StringStart + (int)(Length / 2)] +
					StringRow[NewSegment->StringEnd - (int)(Length / 2)]) / 2.0;

			PtLinePerpIntercept(MidPtCol,
				MidPtRow,
				VLColIntercept,
				VLRowIntercept,
				VLTheta,
				&Col, &Row);

			Height = LineLength(Col, Row,
				MidPtCol,
				MidPtRow);

			if (abs(OldSegment->StringEnd - NewSegment->StringStart) <= 3 &&
				NewSegmentCurvature == OldSegmentCurvature &&
				Height         < 2.0 &&
				OldEndHeight   < 2.0 &&
				NewStartHeight < 2.0)
				JoinSegments = 0;

			/*
			If everything is OK then delete the segments from the List and replace them
			with one segment
			*/

			if (JoinSegments == 0) {

				LinkedSegment = MACAllocateMem(ORTSegment);

				LinkedSegment->StringStart = OldSegment->StringStart;
				LinkedSegment->StringEnd = NewSegment->StringEnd;
				LinkedSegment->Start.Col = OldSegment->Start.Col;
				LinkedSegment->Start.Row = OldSegment->Start.Row;
				LinkedSegment->End.Col = NewSegment->End.Col;
				LinkedSegment->End.Row = NewSegment->End.Row;

				LinkedSegmentLength = LinkedSegment->StringEnd -
					LinkedSegment->StringStart + 1;

				LinkedSegment->MidPoint.Col =
					(StringCol[LinkedSegment->StringStart + (int)(LinkedSegmentLength / 2)] +
						StringCol[LinkedSegment->StringEnd - (int)(LinkedSegmentLength / 2)]) / 2.0;

				LinkedSegment->MidPoint.Row =
					(StringRow[LinkedSegment->StringStart + (int)(LinkedSegmentLength / 2)] +
						StringRow[LinkedSegment->StringEnd - (int)(LinkedSegmentLength / 2)]) / 2.0;

				/*
				Compute the virtual line parameters
				*/
				LineSegColIntercept(LinkedSegment->Start.Col,
					LinkedSegment->Start.Row,
					LinkedSegment->End.Col,
					LinkedSegment->End.Row,
					&VLColIntercept);

				LineSegRowIntercept(LinkedSegment->Start.Col,
					LinkedSegment->Start.Row,
					LinkedSegment->End.Col,
					LinkedSegment->End.Row,
					&VLRowIntercept);

				LinkedSegment->VLTheta = LineSegTheta(LinkedSegment->Start.Col,
					LinkedSegment->Start.Row,
					LinkedSegment->End.Col,
					LinkedSegment->End.Row);

				PtLinePerpIntercept(LinkedSegment->MidPoint.Col,
					LinkedSegment->MidPoint.Row,
					VLColIntercept,
					VLRowIntercept,
					LinkedSegment->VLTheta,
					&LinkedSegment->VLPoint.Col,
					&LinkedSegment->VLPoint.Row);

				Height = LineLength(LinkedSegment->VLPoint.Col,
					LinkedSegment->VLPoint.Row,
					LinkedSegment->MidPoint.Col,
					LinkedSegment->MidPoint.Row);

				/*
				This is to get over problems with determining the center of
				complete circle, since the estimate of the center turn out to
				be infinity in this case
				*/

				InitJctCol = JunctionPtCol = (LinkedSegment->VLPoint.Col +
					LinkedSegment->MidPoint.Col) / 2.0;
				InitJctRow = JunctionPtRow = (LinkedSegment->VLPoint.Row +
					LinkedSegment->MidPoint.Row) / 2.0;

				Radius = LineLength(JunctionPtCol, JunctionPtRow,
					LinkedSegment->MidPoint.Col,
					LinkedSegment->MidPoint.Row);

				if (CircleLeastSq(StringCol, StringRow,
					LinkedSegment->StringStart,
					LinkedSegment->StringEnd,
					&JunctionPtCol, &JunctionPtRow, &Radius) != 0) {
					JunctionPtCol = HUGE;
					JunctionPtRow = HUGE;
				}

				LinkedSegment->JctPoint.Col = JunctionPtCol;
				LinkedSegment->JctPoint.Row = JunctionPtRow;
				/*
				This is a botch to get around the fact that the Liste library adds elements to
				the end of a list and not where you want them. What I do is essentially
				recreate the list and replaced the two linked segments by one segment.
				*/
				if (EmptyList(LinkedSegmentListID) != 1)
					DestList(&LinkedSegmentListID);

				LinkedSegmentListID = CreatList();

				for (i = 1; i < OldSegmentID; i++) {
					NewSegment = MACCast(ORTSegment,
						(ElmNumList((*SegmentListID), (long)i)));
					AddElmList(LinkedSegmentListID, (long)NewSegment);
				}

				AddElmList(LinkedSegmentListID, (long)LinkedSegment);

				for (i = (NewSegmentID + 1); i <= (*SegmentListSize); i++) {
					NewSegment = MACCast(ORTSegment,
						(ElmNumList((*SegmentListID), (long)i)));
					AddElmList(LinkedSegmentListID, (long)NewSegment);
				}

				BufferListID = CopyList(LinkedSegmentListID);
				(*SegmentListSize) = (int)SizeList(BufferListID);
				NewNoOfSegments = (*SegmentListSize);
				(*SegmentListID) = BufferListID;
				(*SegmentList) = LinkedSegment;

				OldSegmentID = 1;
				NewSegmentID = 2;

			}
			else {

				OldSegmentID = NewSegmentID;
				NewSegmentID = NewSegmentID + 1;

			}

		} /* while there are still segments pairs to join */

		OldNoOfSegments = NewNoOfSegments;
		OldSegmentID = 1;
		NewSegmentID = 2;

	} /* while the list is still shrinking */
	return(0);

#ifdef debug
	fprintf(stderr, " End of function LinkSegments \n");
#endif

}

int ReadRawImageData(FILE *(*p_InFile),	int Rows,
	int Cols, char Image[HEIGHT][WIDTH])
{

	int i, j;

#ifdef debug
	fprintf(stderr, " Start of function ReadRawImageData \n");
#endif

	for (i = 0;i<Rows;i++) {
		for (j = 0;j<Cols;j++) {
			Image[i][j] = getc((*p_InFile));
		}
	}

#ifdef debug
	fprintf(stderr, " End of function ReadRawImageData \n");
#endif
	return(0);

}


#define MinSegmentLength 3

int StringToSegments(int MaxJumpPixels,
	int StringListSize,
	Liste StringListID,
	ORTPoint *StringList,
	int *SegmentListSize,
	Liste *SegmentListID,
	ORTSegment *(*SegmentList))
{

	int i;		/* Dummy variables */
	int StartPosition;
	int EndPosition;
	int FoundSegment;
	int AsymmetryFlag;
	int Step;

	double StringRow[10000];
	double StringCol[10000];

	double BodyCol;
	double BodyRow;
	double StartCol;
	double StartRow;
	double EndCol;
	double EndRow;

	double LeftArmLength;
	double RightArmLength;
	double Length;

	double LeftArmTheta;
	double RightArmTheta;
	double LeftArmGrad;
	double RightArmGrad;
	double LeftArmCol;
	double LeftArmRow;
	double RightArmCol;
	double RightArmRow;
	double JctPtCol;
	double JctPtRow;
	double InitJctCol;
	double InitJctRow;
	double Radius;
	double CRight;
	double CLeft;

	double VLColIntercept;
	double VLRowIntercept;
	double VLPtCol;
	double VLPtRow;
	double VLTheta;

	double OldTotalArmLength;
	double CurrentTotalArmLength;
	double SegmentLength;

	double Disparity;

#ifdef debug
	fprintf(stderr, " Start of function StringToSegments \n");
#endif
	/*
	String is too small to process
	*/

	if (StringListSize < 3)
		return(-1);

	/*
	Create linked list to put segments in
	*/
	*SegmentListID = CreatList();
	*SegmentListSize = 0;

	/*
	Store string data in array
	*/

	for (i = 1;i <= StringListSize;i++) {
		StringList = MACCast(ORTPoint, (ElmNumList(StringListID, (long)i)));
		StringRow[i] = StringList->Row;
		StringCol[i] = StringList->Col;
	}

	/*
	Can start anywhere along the Segment as long as the position is more than
	1 pixel from end points. This point will be treated as an end point. Since
	we set the min Segmentlength to 3 we start at the 2nd pixel along.
	*/

	StartPosition = 1;
	EndPosition = 3;
	AsymmetryFlag = 0;
	FoundSegment = 0;
	LeftArmTheta = 0.0;
	RightArmTheta = 0.0;
	CurrentTotalArmLength = 0.0;
	OldTotalArmLength = 0.0;

	/*
	Loop until the whole Segment is processed or we have only 2 pixel bits left
	*/

	while (EndPosition <= StringListSize) {

		Step = 0;
		while (FoundSegment == 0) {

			/*
			Now use the end points of the line segments forming the arms, and the position
			of the body to find the the virtual line parameters
			*/
			LineSegColIntercept(StringCol[StartPosition],
				StringRow[StartPosition],
				StringCol[EndPosition],
				StringRow[EndPosition],
				&VLColIntercept);

			LineSegRowIntercept(StringCol[StartPosition],
				StringRow[StartPosition],
				StringCol[EndPosition],
				StringRow[EndPosition],
				&VLRowIntercept);

			VLTheta = LineSegTheta(StringCol[StartPosition],
				StringRow[StartPosition],
				StringCol[EndPosition],
				StringRow[EndPosition]);

			BodyCol = (StringCol[StartPosition + (Step / 2)] +
				StringCol[EndPosition - (Step / 2)]) / 2.0;
			BodyRow = (StringRow[StartPosition + (Step / 2)] +
				StringRow[EndPosition - (Step / 2)]) / 2.0;

			PtLinePerpIntercept(BodyCol,
				BodyRow,
				VLColIntercept,
				VLRowIntercept,
				VLTheta,
				&VLPtCol,
				&VLPtRow);
			/*
			Find if we are roughly symmetric by comparing arm lengths (could compare diff.
			in Heights of arms below intercept point)
			*/

			LeftArmLength = LineLength(VLPtCol, VLPtRow,
				StringCol[StartPosition],
				StringRow[StartPosition]);

			RightArmLength = LineLength(VLPtCol, VLPtRow,
				StringCol[EndPosition],
				StringRow[EndPosition]);

			CurrentTotalArmLength = LeftArmLength + RightArmLength;

			Length = (double)(EndPosition - StartPosition + 1);
			Disparity = (sqrt(2.0) / (sqrt(Length*Length + 1.0)));

			if (fabs(RightArmLength - LeftArmLength) >= Disparity ||
				CurrentTotalArmLength < OldTotalArmLength) {
				AsymmetryFlag++;
			}
			else {
				AsymmetryFlag = 0;
			}

			OldTotalArmLength = CurrentTotalArmLength;
			/*
			If still near-symmetric move body by one position, and extend the arms
			*/

			if (AsymmetryFlag > MaxJumpPixels || EndPosition == StringListSize) {
				EndPosition = EndPosition - AsymmetryFlag;
				FoundSegment = 1;
			}
			else {
				EndPosition++;
				Step++;
			}
		} /* While segment not found */

		  /*
		  If we reached an end point then stop, store the result and continue
		  */
		SegmentLength = EndPosition - StartPosition + 1;
		if (SegmentLength > 2) {

			if (AsymmetryFlag == 0) {

				BodyCol = (StringCol[StartPosition + (int)(SegmentLength / 2)] +
					StringCol[EndPosition - (int)(SegmentLength / 2)]) / 2.0;
				BodyRow = (StringRow[StartPosition + (int)(SegmentLength / 2)] +
					StringRow[EndPosition - (int)(SegmentLength / 2)]) / 2.0;

				StartCol = StringCol[StartPosition];
				StartRow = StringRow[StartPosition];
				EndCol = StringCol[EndPosition];
				EndRow = StringRow[EndPosition];
				/*
				Compute the virtual line parameters
				*/

				LineSegColIntercept(StartCol,
					StartRow,
					EndCol,
					EndRow,
					&VLColIntercept);

				LineSegRowIntercept(StartCol,
					StartRow,
					EndCol,
					EndRow,
					&VLRowIntercept);

				VLTheta = LineSegTheta(StartCol,
					StartRow,
					EndCol,
					EndRow);

				PtLinePerpIntercept(BodyCol,
					BodyRow,
					VLColIntercept,
					VLRowIntercept,
					VLTheta,
					&VLPtCol,
					&VLPtRow);

				/*
				Now find the rough position of the center of curvature
				*/

				LeftArmTheta = LineSegTheta(StartCol,
					StartRow,
					BodyCol,
					BodyRow);

				RightArmTheta = LineSegTheta(EndCol,
					EndRow,
					BodyCol,
					BodyRow);

				LeftArmCol = (StartCol + BodyCol) / 2.0;
				LeftArmRow = (StartRow + BodyRow) / 2.0;
				RightArmCol = (EndCol + BodyCol) / 2.0;
				RightArmRow = (EndRow + BodyRow) / 2.0;

				RightArmTheta = RightArmTheta + PIBY2;
				if (RightArmTheta > PI)
					RightArmTheta = RightArmTheta - PI;

				LeftArmTheta = LeftArmTheta + PIBY2;
				if (LeftArmTheta > PI)
					LeftArmTheta = LeftArmTheta - PI;

				RightArmGrad = tan(RightArmTheta);
				LeftArmGrad = tan(LeftArmTheta);

				CRight = RightArmRow - RightArmGrad*RightArmCol;
				CLeft = LeftArmRow - LeftArmGrad*LeftArmCol;

				if (fabs(LeftArmTheta - RightArmTheta) < 0.001) {
					JctPtCol = HUGE;
					JctPtRow = HUGE;
				}
				else {

					if (MACCompareWithPIBY2(LeftArmTheta)) {
						InitJctCol = JctPtCol = LeftArmCol;
						InitJctRow = JctPtRow = RightArmGrad*LeftArmCol + CRight;
						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}

					}

					if (MACCompareWithPIBY2(RightArmTheta)) {
						InitJctCol = JctPtCol = RightArmCol;
						InitJctRow = JctPtRow = LeftArmGrad*RightArmCol + CLeft;

						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}

					}

					if (!MACCompareWithPIBY2(RightArmTheta) &&
						!MACCompareWithPIBY2(LeftArmTheta)) {
						InitJctCol = JctPtCol = (CRight - CLeft) / (LeftArmGrad - RightArmGrad);
						InitJctRow = JctPtRow = RightArmGrad*JctPtCol + CRight;
						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}
					}
				}
				/*
				Allocate memory for the segment
				*/

				*SegmentList = MACAllocateMem(ORTSegment);

				(*SegmentList)->StringStart = StartPosition;
				(*SegmentList)->StringEnd = EndPosition;
				(*SegmentList)->Start.Col = StartCol;
				(*SegmentList)->Start.Row = StartRow;
				(*SegmentList)->End.Col = EndCol;
				(*SegmentList)->End.Row = EndRow;
				(*SegmentList)->MidPoint.Col = BodyCol;
				(*SegmentList)->MidPoint.Row = BodyRow;
				(*SegmentList)->JctPoint.Col = JctPtCol;
				(*SegmentList)->JctPoint.Row = JctPtRow;
				(*SegmentList)->VLPoint.Col = VLPtCol;
				(*SegmentList)->VLPoint.Row = VLPtRow;
				(*SegmentList)->VLTheta = VLTheta;
				/*
				Add element to list
				*/

				AddElmList(*SegmentListID, (long)*SegmentList);
				(*SegmentListSize)++;
			}
			else {

				BodyCol = (StringCol[StartPosition + (int)(SegmentLength / 2)] +
					StringCol[EndPosition - (int)(SegmentLength / 2)]) / 2.0;
				BodyRow = (StringRow[StartPosition + (int)(SegmentLength / 2)] +
					StringRow[EndPosition - (int)(SegmentLength / 2)]) / 2.0;

				StartCol = StringCol[StartPosition];
				StartRow = StringRow[StartPosition];
				EndCol = StringCol[EndPosition];
				EndRow = StringRow[EndPosition];
				/*
				Compute the virtual line parameters
				*/

				LineSegColIntercept(StartCol,
					StartRow,
					EndCol,
					EndRow,
					&VLColIntercept);

				LineSegRowIntercept(StartCol,
					StartRow,
					EndCol,
					EndRow,
					&VLRowIntercept);

				VLTheta = LineSegTheta(StartCol,
					StartRow,
					EndCol,
					EndRow);

				PtLinePerpIntercept(BodyCol,
					BodyRow,
					VLColIntercept,
					VLRowIntercept,
					VLTheta,
					&VLPtCol,
					&VLPtRow);
				/*
				Now find the rough position of the center of curvature
				*/

				LeftArmTheta = LineSegTheta(StartCol,
					StartRow,
					BodyCol,
					BodyRow);

				RightArmTheta = LineSegTheta(EndCol,
					EndRow,
					BodyCol,
					BodyRow);

				LeftArmCol = (StartCol + BodyCol) / 2.0;
				RightArmCol = (EndCol + BodyCol) / 2.0;
				LeftArmRow = (StartRow + BodyRow) / 2.0;
				RightArmRow = (EndRow + BodyRow) / 2.0;

				RightArmTheta = RightArmTheta + PIBY2;
				if (RightArmTheta > PI)
					RightArmTheta = RightArmTheta - PI;

				LeftArmTheta = LeftArmTheta + PIBY2;
				if (LeftArmTheta > PI)
					LeftArmTheta = LeftArmTheta - PI;

				RightArmGrad = tan(RightArmTheta);
				LeftArmGrad = tan(LeftArmTheta);

				CRight = RightArmRow - RightArmGrad*RightArmCol;
				CLeft = LeftArmRow - LeftArmGrad*LeftArmCol;

				if (fabs(LeftArmTheta - RightArmTheta) < 0.001) {
					JctPtCol = HUGE;
					JctPtRow = HUGE;
				}
				else {

					if (MACCompareWithPIBY2(LeftArmTheta)) {
						InitJctCol = JctPtCol = LeftArmCol;
						InitJctRow = JctPtRow = RightArmGrad*LeftArmCol + CRight;
						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}
					}

					if (MACCompareWithPIBY2(RightArmTheta)) {
						InitJctCol = JctPtCol = RightArmCol;
						InitJctRow = JctPtRow = LeftArmGrad*RightArmCol + CLeft;
						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}

					}

					if (!MACCompareWithPIBY2(RightArmTheta) &&
						!MACCompareWithPIBY2(LeftArmTheta)) {
						InitJctCol = JctPtCol = (CRight - CLeft) / (LeftArmGrad - RightArmGrad);
						InitJctRow = JctPtRow = RightArmGrad*JctPtCol + CRight;
						Radius = LineLength(JctPtCol, JctPtRow,
							StringCol[(int)((EndPosition - StartPosition) / 2)],
							StringRow[(int)((EndPosition - StartPosition) / 2)]);

						if (CircleLeastSq(StringCol, StringRow,
							StartPosition, EndPosition,
							&JctPtCol, &JctPtRow, &Radius) != 0) {
							JctPtCol = InitJctCol;
							JctPtRow = InitJctRow;
						}
					}
				}
				/*
				Allocate memory for the segment
				*/

				*SegmentList = MACAllocateMem(ORTSegment);

				(*SegmentList)->StringStart = StartPosition;
				(*SegmentList)->StringEnd = EndPosition;
				(*SegmentList)->Start.Col = StartCol;
				(*SegmentList)->Start.Row = StartRow;
				(*SegmentList)->End.Col = EndCol;
				(*SegmentList)->End.Row = EndRow;
				(*SegmentList)->MidPoint.Col = BodyCol;
				(*SegmentList)->MidPoint.Row = BodyRow;
				(*SegmentList)->JctPoint.Col = JctPtCol;
				(*SegmentList)->JctPoint.Row = JctPtRow;
				(*SegmentList)->VLPoint.Col = VLPtCol;
				(*SegmentList)->VLPoint.Row = VLPtRow;
				(*SegmentList)->VLTheta = VLTheta;
				/*
				Add element to list
				*/
				AddElmList(*SegmentListID, (long)*SegmentList);
				(*SegmentListSize)++;

			} /* depending on asymmetry make correction and add segment to the list */
		}
		StartPosition = EndPosition + 1;
		EndPosition = StartPosition + 2;
		OldTotalArmLength = 0.0;
		CurrentTotalArmLength = 0.0;
		AsymmetryFlag = 0;
		FoundSegment = 0;

	} /* while the string is not completely processed */

	if ((*SegmentListSize) > 0) {
		return(0);
	}
	else {
		return(-1);
	}
#ifdef debug
	fprintf(stderr, " End of function StringToSegments \n");
#endif
}