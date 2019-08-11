/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%% SEGMENTATION and LINES C FILE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* StickMan source codes.*/
/* To convert 'stickman_extend.c' file to 'stickman_extend.mexw64' file,  do below steps on Matlab R2012/2013a */
/* 1) Write mex -setup */
/* 2) Select Microsoft Visual C++ 2010 in c:\Program Files (x86)\Microsoft Visual Studio 10.0  */
/* 3) Write mex stickman_extend.c */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 

#include "mex.h"
#include <math.h>
#include "matrix.h"
#include "stickman_main.h"

#define	S_OUT_CNT		plhs[0]  /* Total Segmented Line numbers */
#define	S_OUT_SX	  		plhs[1]  /* Start Points X Coordinate of Segmented Lines */
#define	S_OUT_SY	  		plhs[2]  /* Start Points Y Coordinate of Segmented Lines */
#define	S_OUT_EX	  		plhs[3]  /* End Points X Coordinate of Segmented Lines */
#define	S_OUT_EY	  		plhs[4]  /* End Points Y Coordinate of Segmented Lines */



/* %%%%%%%%%%%%%%%% Segmentation 4x4 line pieces and Count the Lines  %%%%%%%%%%%%%%%% */
static void seg_and_count_lines(
			
		   unsigned char	yp[], /* Recognized 4x4 line pieces' Slopes */
		   unsigned char	yp_st[], /* Recognized 4x4 line pieces' Start points  */
		   unsigned char	yp_end[], /* Recognized 4x4 line pieces' End points   */
		   unsigned int	yp_cnt[], /* Recognized 4x4 line pieces' COUNT  */
		   unsigned int	yp_cnt1[], /* Recognized 4x4 line pieces' COUNT  */
		   unsigned  int yp_xy[],	/* Location of 4x4 line pieces in the array */
		   unsigned  int  dynamicData[],	    /*Segmentation numbers */	
		   unsigned  int  dynamicData0[],	     /*X Coordinate Of Segmented 4x4 line pieces */	
		   unsigned  int  dynamicData1[],	     /*Y Coordinate Of Segmented 4x4 line pieces */		   
		   unsigned  int  dynamicData_z[],   /*Corner Points */	
		   unsigned  int  dynamicData_s[],	 	/*Recognized 4x4 line pieces */	
		   unsigned  int  dynamicData_x[],   /*X Coordinate Of Recognized 4x4 line pieces */	
		   unsigned  int  dynamicData_y[],       /*Y Coordinate Of Recognized 4x4 line pieces */	
		   unsigned  int  dynamicData_es1[],  	 /*Recognized 4x4 line pieces, the line Start&End Points */		
		   unsigned  int  dynamicData_es[],  	 /*Recognized 4x4 line pieces, the line Start&End Points */		   
		   unsigned  int yp_sx_ext0[],
           unsigned  int yp_sy_ext0[],
           unsigned  int yp_ex_ext0[],
		   unsigned  int yp_ey_ext0[],		 
		   unsigned  char  yp_ext[],  	 /*Recognized 4x4 line pieces, the line Start&End Points will be extended */
		   size_t  	mp, /*Height of the divided input image to 4x4 pixel windows*/
		   size_t  	np, /*Width of the divided input image to 4x4 pixel windows*/
		   size_t  	m_in, /*Height of the input image*/
		   bool b_in[], /* Input Binary Edge image */	
		   bool b_in_seg[],/* Segmentation Rule */
		   double b_in_H[] /* Choose Segmentation Rule */
		   )
{
   
		size_t  index_n, index_m;		
		size_t   a00;
		bool a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
		size_t   b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;
		size_t  sum;
		size_t  r;
		size_t t_rem,c_rem,t_rem_s,c_rem_s,tem_p;
		size_t c_sl,c_s,c_e,t_s,t_e;
		size_t i_cp;
		size_t i_pw;
		size_t i_sg;		
		size_t i_dz;
		size_t i_ds;
		size_t i_sg1;
		size_t i_sg2;
		size_t i_es;
		
		size_t i_es1;
		size_t i_es2;
		size_t i_es3;
		size_t i_es4;
		
		
		
		
		
		

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PiecewiseMatch %%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	/* 4x4 edge pixel window of the input image*/	
	/* a3   		a2    		a1   		a0    */	
	/* a7   		a6    		a5   		a4    */	
	/* a11 		a10  		a9   		a8    */	
	/* a15 		a14  		a13 		a12  */	
	
	/* Conversion weight of 4x4 edge pixels to 16-bit unsigned integer */
	/* 8         		4          		 2        		 1         */	
	/* 128     		64        		 32      		 16       */	
	/* 2048    		1024     		 512    		 256     */	
	/* 32768  		16384   		 8192  		4096    */
	
	/* PiecewiseMatch starts from the top left corner of the image and proceeds top to bottom and continues left to right  */
		 i_dz=0;
		 i_ds=0;
   	for ( index_n = 1; index_n < np-1; index_n++ ) {			
        for ( index_m = 1; index_m < mp-1; index_m++ ) {
		
		a00=m_in*index_n*4+index_m*4; /* Taking edge pixels from a3 down to a15  in first column of the 4x4 input window */
		a3=b_in[a00++];
		a7=b_in[a00++];
		a11=b_in[a00++];
		a15=b_in[a00];
		
		a00=a00+m_in; /* Taking edge pixels continues from a14 up to a2 in second column of the 4x4 input window*/
		a14=b_in[a00--];
		a10=b_in[a00--];
		a6=b_in[a00--];
		a2=b_in[a00];
		
		a00=a00+m_in; /* Taking edge pixels continues from a1 down  to a13 in third column of the 4x4 input window*/
		a1=b_in[a00++];
		a5=b_in[a00++];
		a9=b_in[a00++];
		a13 =b_in[a00];
		
		a00=a00+m_in;/* Taking edge pixels continues from a14 up to a2 in last column of the 4x4 input window*/
		a12=b_in[a00--];
		a8=b_in[a00--];
		a4=b_in[a00--];
		a0=b_in[a00];
		
		if(a0||a1||a2||a3||a7||a11||a15||a14||a13||a12)  /* 110 different patterns have common 10 pixels */
		{
			if(a0+a1+a2+a3+a4+a5+a6+a7+a8+a9+a10+a11+a12+a13+a14+a15<5) /* If total pixel numbers of the 4x4 window is greater than 4, it is unrecognized 4x4 edge patterns  */
			{					
					b0=a0*1;
					b1 =a1*2;
					b2 =a2*4;
					b3 =a3*8;
					b4 =a4*16;
					b5 =a5*32;
					b6 =a6*64;
					b7 =a7*128;
					b8 =a8*256;
					b9 =a9*512;
					b10=a10*1024;
					b11=a11*2048;
					b12=a12*4096;
					b13=a13*8192;
					b14=a14*16384;
					b15=a15*32768;
					sum=b0+b1+b2+b3+b4+b5+b6+b7+b8+b9+b10+b11+b12+b13+b14+b15; /* Conversion of 4x4 edge pixels to 16-bit unsigned integer */
					r=b_in_fs[sum]; /* One of the 110 lines patterns and 4 corner points OR it is zero */
				
				if(r>0)	/* Recognized 4x4 edge patterns */
				{
				i_pw=mp*index_n+index_m;	
				yp[i_pw]=b_in_slp[r];   /* Slope angle of the recognized 4x4 line pattern */
						if (r<63)  /* One of the 110 lines patterns */
						{				
							yp_st[i_pw]=b_in_st[r];  /* Start  point of the recognized 4x4 line pattern */
							yp_end[i_pw]=b_in_end[r]; /* End  point of the recognized 4x4 line pattern */
							yp_xy[i_pw]=i_pw; /* Location of  the recognized 4x4 line pattern in the array */
							dynamicData_s[i_ds]=i_pw;	     /*Recognized 4x4 line pieces */	
							dynamicData_x[i_ds]=index_n;     /*X Coordinate Of Recognized 4x4 line pieces */
							dynamicData_y[i_ds]=index_m;    /*Y Coordinate Of Recognized 4x4 line pieces */	
							yp_ext[i_pw]=1; 							
							i_ds++;
						}
						else
						{	
							dynamicData_z[i_dz]=i_pw;/*Corner Points */	
							i_dz++;
						}	
											
				}
			}
		}				
	  }	
	}
	
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Corner Points %%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	/*Corner Points  update the Start&End of the recognized Neighbor 4x4 line patterns */
	
	/* Remove Bug Corner*/
			for ( i_cp= 0; i_cp< i_dz; i_cp++ )
		{	
			if (yp[dynamicData_z[i_cp]]==10) /* Corner point 8*/
			{
				if (yp[dynamicData_z[i_cp]-1]==12)  /* Corner point 32768 */
				{
				yp[dynamicData_z[i_cp]]=0;
				}
			}
		}
		
	for ( i_cp= 0; i_cp< i_dz; i_cp++ )
		{		
				if (yp[dynamicData_z[i_cp]]==9) /* Corner point 1*/
				{	
			
					if (yp_end[dynamicData_z[i_cp]-1]==8) /* Corner point 1 updates the end point 8 to 7 of the Up neighbor recognized 4x4 line pattern if its slope angle is 2, 3, 4, 5 or 6.*/
					{
						if ((yp[dynamicData_z[i_cp]-1]==2)||(yp[dynamicData_z[i_cp]-1]==3)||(yp[dynamicData_z[i_cp]-1]==4)||(yp[dynamicData_z[i_cp]-1]==5)||(yp[dynamicData_z[i_cp]-1]==6))
						{	
							yp_end[dynamicData_z[i_cp]-1]=7;
						}
					}
					
					if (yp_st[dynamicData_z[i_cp]+mp]==12) /* Corner point 1 updates the start point 12 to 1 of the Right neighbor recognized 4x4 line pattern if its slope angle is 1, 2, 3, 4 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]+mp]==1)||(yp[dynamicData_z[i_cp]+mp]==2)||(yp[dynamicData_z[i_cp]+mp]==3)||(yp[dynamicData_z[i_cp]+mp]==4)||(yp[dynamicData_z[i_cp]+mp]==8))
						{	
							yp_st[dynamicData_z[i_cp]+mp]=1;
						}
					}		
				}
				
				else if (yp[dynamicData_z[i_cp]]==10) /* Corner point 8*/
				{	
			
					if (yp_st[dynamicData_z[i_cp]-mp]==5) /* Corner point 8 updates the start point 5 to 4 of the Left neighbor recognized 4x4 line pattern if its slope angle is 6.*/
					{
						if (yp[dynamicData_z[i_cp]-mp]==6)
						{	
							yp_st[dynamicData_z[i_cp]-mp]=4;
						}
					}					
					else if(yp_end[dynamicData_z[i_cp]-mp]==5) /* Corner point 8 updates the end point 5 to 4 of the Left neighbor recognized 4x4 line pattern if its slope angle is 1, 2, 7 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]-mp]==1)||(yp[dynamicData_z[i_cp]-mp]==2)||(yp[dynamicData_z[i_cp]-mp]==7)||(yp[dynamicData_z[i_cp]-mp]==8))
						{	
							yp_end[dynamicData_z[i_cp]-mp]=4;
						}
					}
					
					if (yp_st[dynamicData_z[i_cp]-1]==9) /* Corner point 8 updates the start point 9 to 10 of the Up neighbor recognized 4x4 line pattern if its slope angle is 7 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]-1]==7)||(yp[dynamicData_z[i_cp]-1]==8))
						{	
							yp_st[dynamicData_z[i_cp]-1]=10;
						}
					}					
					else if(yp_end[dynamicData_z[i_cp]-1]==9) /* Corner point 8 updates the end point 9 to 10 of the Up neighbor recognized 4x4 line pattern if its slope angle is 4, 5 or 6.*/
					{
						if ((yp[dynamicData_z[i_cp]-1]==4)||(yp[dynamicData_z[i_cp]-1]==5)||(yp[dynamicData_z[i_cp]-1]==6))
						{	
							yp_end[dynamicData_z[i_cp]-1]=10;
						}
					}		
				}
				
				else if (yp[dynamicData_z[i_cp]]==11) /* Corner point binary 4096 */
				{	
			
					if (yp_st[dynamicData_z[i_cp]+1]==3) /* Corner point 4096 updates the start point 3 to 4 of the Down neighbor recognized 4x4 line pattern if its slope angle is 4, 5 or 6.*/
					{
						if ((yp[dynamicData_z[i_cp]+1]==4)||(yp[dynamicData_z[i_cp]+1]==5)||(yp[dynamicData_z[i_cp]+1]==6))
						{	
							yp_st[dynamicData_z[i_cp]+1]=4;
						}
					}					
					else if(yp_end[dynamicData_z[i_cp]+1]==3) /* Corner point 4096 updates the end point 3 to 4 of the Down neighbor recognized 4x4 line pattern if its slope angle is 7 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]+1]==7)||(yp[dynamicData_z[i_cp]+1]==8))
						{	
							yp_end[dynamicData_z[i_cp]+1]=4;
						}
					}
					
					if (yp_st[dynamicData_z[i_cp]+mp]==11) /* Corner point 4096 updates the start point 11 to 10 of the Right neighbor recognized 4x4 line pattern if its slope angle is 1, 2, 7 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]+mp]==1)||(yp[dynamicData_z[i_cp]+mp]==2)||(yp[dynamicData_z[i_cp]+mp]==7)||(yp[dynamicData_z[i_cp]+mp]==8))
						{	
							yp_st[dynamicData_z[i_cp]+mp]=10;
						}
					}					
					else if(yp_end[dynamicData_z[i_cp]+mp]==11) /* Corner point 4096 updates the end point 11 to 10 of the Right neighbor recognized 4x4 line pattern if its slope angle is 6.*/
					{
						if (yp[dynamicData_z[i_cp]+mp]==6)
						{	
							yp_end[dynamicData_z[i_cp]+mp]=10;
						}
					}		
				}
				
				else if (yp[dynamicData_z[i_cp]]==12)  /* Corner point 32768 */
				{	
			
					if (yp_end[dynamicData_z[i_cp]-mp]==6) /* Corner point 32768 updates the end point 6 to 7 of the Left neighbor recognized 4x4 line pattern if its slope angle is 1, 2, 3, 4 or 8.*/
					{
						if ((yp[dynamicData_z[i_cp]-mp]==1)||(yp[dynamicData_z[i_cp]-mp]==2)||(yp[dynamicData_z[i_cp]-mp]==3)||(yp[dynamicData_z[i_cp]-mp]==4)||(yp[dynamicData_z[i_cp]-mp]==8))
						{	
							yp_end[dynamicData_z[i_cp]-mp]=7;
						}
					}
					
					if (yp_st[dynamicData_z[i_cp]+1]==2) /* Corner point 32768 updates the start point 2 to 1 of the Down neighbor recognized 4x4 line pattern if its slope angle is 2, 3, 4, 5 or 6.*/
					{
						if ((yp[dynamicData_z[i_cp]+1]==2)||(yp[dynamicData_z[i_cp]+1]==3)||(yp[dynamicData_z[i_cp]+1]==4)||(yp[dynamicData_z[i_cp]+1]==5)||(yp[dynamicData_z[i_cp]+1]==6))
						{	
							yp_st[dynamicData_z[i_cp]+1]=1;
						}
					}		
				}			
				
				/* Set Corner point to 0.*/
				yp[dynamicData_z[i_cp]]=0;
			}
			
		
	
			
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Segmentation %%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	/*Segmentation checks target Up, DownLeft, Left and UpLeft Neighbors of the recognized 4x4 line pattern */
	for ( i_sg= 0; i_sg< i_ds; i_sg++ )
		{		
			
			if ((yp[dynamicData_s[i_sg]-1]>0)&&(yp[dynamicData_s[i_sg]]!=1))	/*Check Up Neighbor target if it and current window whose slope angle is not 1, are recognized */
			{			
					c_s=yp_st[dynamicData_s[i_sg]];  /* Start point of the current window */
					c_e=yp_end[dynamicData_s[i_sg]];  /* End point of the current window */
					t_s=yp_st[dynamicData_s[i_sg]-1];  /* Start point of the Up target */
					t_e=yp_end[dynamicData_s[i_sg]-1]; /* End point of the Up target  */
					
				if (b_up_tp[13*c_s+t_e]|| b_up_tp[13*c_s+t_s]||b_up_tp[13*c_e+t_e]|| b_up_tp[13*c_e+t_s])
				{
					t_rem=yp_xy[dynamicData_s[i_sg]-1]; /* Location of the Up target  */
					c_rem=yp_xy[dynamicData_s[i_sg]]; /* Location of the current window */
					c_rem_s=yp[c_rem]; /* Segmentation Slope of the current window */
					t_rem_s=yp[t_rem]; /* Segmentation Slope  of the Up target  */
				
				if (b_in_seg[c_rem_s+(t_rem_s-1)*8])  /* Segmentation rule Based on Their Slope */
					{
						tem_p=yp_xy[t_rem];/* Location of the Up target  */
						yp_xy[c_rem]=tem_p;/* Update Location of the current window as Location of the Up target  for Segmentation  */
						
						if(((c_e==2)||(c_e==3)||(c_e==4))&& ((yp[dynamicData_s[i_sg]]==7)||(yp[dynamicData_s[i_sg]]==8)))
						{
							yp_end[dynamicData_s[i_sg]]=0;  /* Set End point of the current window 0 if it is connected.*/
						}
						else
						{
							yp_st[dynamicData_s[i_sg]]=0;   /* Set Start  point of the current window 0 if it is connected.*/
						}
						
						if((t_s==8)||(t_s==9)||(t_s==10))
						{
							yp_st[dynamicData_s[i_sg]-1]=0;   /* Set Start point of the Up target 0 if it is connected.*/
						}
						else
						{
							yp_end[dynamicData_s[i_sg]-1]=0;   /* Set End point of the Up target 0 if it is connected.*/
						}
					}
				}
			/* End of Up Neighbor*/			
			}
			
			if ((yp[dynamicData_s[i_sg]-mp+1]>0)&&(yp[dynamicData_s[i_sg]]!=3))	/*Check DownLeft Neighbor target */
			{	
				if (((yp_st[dynamicData_s[i_sg]]==10)&&(yp_end[dynamicData_s[i_sg]-mp+1]==4))||((yp_st[dynamicData_s[i_sg]]==10)&&(yp_st[dynamicData_s[i_sg]-mp+1]==4))||((yp_end[dynamicData_s[i_sg]]==10)&&(yp_end[dynamicData_s[i_sg]-mp+1]==4))||((yp_end[dynamicData_s[i_sg]]==10)&&(yp_st[dynamicData_s[i_sg]-mp+1]==4)))
				{
					t_rem=yp_xy[dynamicData_s[i_sg]+1-mp];  /* Location of the DownLeft  target  */
					c_rem=yp_xy[dynamicData_s[i_sg]]; /* Location of the current window */
					c_rem_s=yp[c_rem];  /* Segmentation Slope of the current window */
					t_rem_s=yp[t_rem]; /* Segmentation Slope  of the DownLeft target  */
					
					if (b_in_seg[c_rem_s+(t_rem_s-1)*8]) /* Segmentation rule Based on Their Slope */
					{					
							tem_p=yp_xy[t_rem]; /* Location of the DownLeft  target  */
							yp_xy[c_rem]=tem_p; /* Update Location of the current window as Location of the DownLeft  target  for Segmentation  */
						
						if(yp_end[dynamicData_s[i_sg]]==10)
						{
							yp_end[dynamicData_s[i_sg]]=0;  /* Set End point of the current window 0 if it is connected.*/
						}
						else
						{
							yp_st[dynamicData_s[i_sg]]=0;  /* Set Start  point of the current window 0 if it is connected.*/
						}
						
						if(yp_st[dynamicData_s[i_sg]+1-mp]==4)
						{
							yp_st[dynamicData_s[i_sg]+1-mp]=0; /* Set Start point of the DownLeft  target 0 if it is connected.*/
						}
						else
						{
							yp_end[dynamicData_s[i_sg]+1-mp]=0; /* Set End point of the DownLeft  target 0 if it is connected.*/
						}				
					}
				}
			/* End of DownLeft Neighbor*/			
			}
			
			if ((yp[dynamicData_s[i_sg]-mp]>0)&&(yp[dynamicData_s[i_sg]]!=5)) 	/*Check Left Neighbor target if it and current window whose slope angle is not 5, are recognized */
			{
				c_sl=yp[dynamicData_s[i_sg]]; /* Slope of the current window */
				c_s=yp_st[dynamicData_s[i_sg]];  /* Start point of the current window */
				c_e=yp_end[dynamicData_s[i_sg]]; /* End point of the current window */
				t_s=yp_st[dynamicData_s[i_sg]-mp]; /* Start point of the Left target */
				t_e=yp_end[dynamicData_s[i_sg]-mp];  /* End point of the Left target  */
				
				if ((b_left_tp[13*c_s+t_e])|| (b_left_tp[13*c_s+t_s])||(b_left_tp[13*c_e+t_e])|| (b_left_tp[13*c_e+t_s]))
				{
				t_rem=yp_xy[dynamicData_s[i_sg]-mp];/* Location of the Left  target  */
				c_rem=yp_xy[dynamicData_s[i_sg]]; /* Location of the current window */
				c_rem_s=yp[c_rem]; /* Segmentation Slope of the current window */
				t_rem_s=yp[t_rem]; /* Segmentation Slope  of the Left target  */
				
				if (b_in_seg[c_rem_s+(t_rem_s-1)*8])  /* Segmentation rule Based on Their Slope */
					{														
						tem_p=yp_xy[t_rem]; /* Location of the Left  target  */
						yp_xy[c_rem]=tem_p;  /* Update Location of the current window as Location of the Left  target  for Segmentation  */
						
						if(((c_e==10)||(c_e==11)||(c_e==12))&& (c_sl==6))
						{
							yp_end[dynamicData_s[i_sg]]=0; /* Set End point of the current window 0 if it is connected.*/
								
								if((t_s==5)||(t_s==6))
								{
									yp_st[dynamicData_s[i_sg]-mp]=0;  /* Set Start point of the Left  target 0 if it is connected.*/
								}
								else
								{
									yp_end[dynamicData_s[i_sg]-mp]=0; /* Set End point of the Left  target 0 if it is connected.*/
								}
						}
						else
						{
							yp_st[dynamicData_s[i_sg]]=0; /* Set Start  point of the current window 0 if it is connected.*/
							
							if((t_s==4)||(t_s==5)||(t_s==6))
								{
									yp_st[dynamicData_s[i_sg]-mp]=0;  /* Set Start point of the Left  target 0 if it is connected.*/
								}
								else
								{
									yp_end[dynamicData_s[i_sg]-mp]=0; /* Set End point of the Left  target 0 if it is connected.*/
								}
						}
					}
				}
			/* End of Left Neighbor*/	
			}
			
			if ((yp_st[dynamicData_s[i_sg]]==1)&&(yp_end[dynamicData_s[i_sg]-mp-1]==7)) /*UpLeft neighbor Segmentation Connection Points rule: Check UpLeft Neighbor target if its end point is 7 and current window's start point is 1.  */
			{
				t_rem=yp_xy[dynamicData_s[i_sg]-mp-1];  /* Location of the UpLeft  target  */
				c_rem=yp_xy[dynamicData_s[i_sg]];/* Location of the current window */
				c_rem_s=yp[c_rem]; /* Segmentation Slope of the current window */
				t_rem_s=yp[t_rem];/* Segmentation Slope  of the UpLeft target  */
				
				if (b_in_seg[c_rem_s+(t_rem_s-1)*8]) /* Segmentation rule Based on Their Slope */
				{
						tem_p=yp_xy[t_rem];/* Location of the UpLeft  target  */
						yp_xy[c_rem]=tem_p;	 /* Update Location of the current window as Location of the UpLeft  target  for Segmentation  */										
						yp_st[dynamicData_s[i_sg]]=0;	  /* Set Start  point of the current window 0 if it is connected.*/				
						yp_end[dynamicData_s[i_sg]-mp-1]=0;	/* Set End point of the UpLeft  target 0 if it is connected.*/		
				}
			}	/* End of UpLeft Neighbor*/
			
		/* End of check current 4x4 piece Segmentation*/			
		}	
	
    /* %%%%%%%%%%%%%%%% Count the Lines  %%%%%%%%%%%%%%%% */		
	i_es2=0;
		for ( i_sg1= 0; i_sg1< i_ds; i_sg1++ ) 	
		{		
			if (((yp_st[dynamicData_s[i_sg1]]>0)&&(yp_end[dynamicData_s[i_sg1]]==0))||((yp_st[dynamicData_s[i_sg1]]==0)&&(yp_end[dynamicData_s[i_sg1]]>0)))
			{			
				dynamicData_es1[i_es2]=i_sg1;	
					i_es2++;				
			}				
		}	
		
			i_es=0;
		for ( i_es4= 0; i_es4< i_es2-1; i_es4++ ) 	
		{		
				
			if ( dynamicData_es1[i_es4]<i_ds)
			{	
				for ( i_es3= i_es4+1; i_es3< i_es2; i_es3++ ) 	
				{	
				 if ( dynamicData_es1[i_es3]<i_ds)
				 {	
					if ( yp_xy[dynamicData_s[dynamicData_es1[i_es4]]]== yp_xy[dynamicData_s[dynamicData_es1[i_es3]]])
					{				
						dynamicData_es[i_es]=dynamicData_es1[i_es4];
						dynamicData_es1[i_es4]=i_ds;
						i_es++;	
						dynamicData_es[i_es]=dynamicData_es1[i_es3];
						dynamicData_es1[i_es3]=i_ds;
						i_es++;	
						break;	
					}			
				}
			  }				
				
			}		
		}
		
	
		
		i_es1=0;
		
	if( b_in_H[0]>0)
	{
		for ( i_sg1= 0; i_sg1< i_ds; i_sg1++ ) 	
		{		
			if ((yp_st[dynamicData_s[i_sg1]]>0)&&(yp_end[dynamicData_s[i_sg1]]>0))
			{			
											/* Start*/
							if (yp_st[dynamicData_s[i_sg1]]==1) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==2)
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+2;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}	
						else if (yp_st[dynamicData_s[i_sg1]]==3) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+3;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==4) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}	
						else if (yp_st[dynamicData_s[i_sg1]]==5) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+2;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==6) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+3;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==7) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==8) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+3;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==9) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+2;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==10) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==11) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+3;
							}
						else if (yp_st[dynamicData_s[i_sg1]]==12) 
							{	
									yp_sx_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_sy_ext0[i_es1] =(dynamicData_y[i_sg1])*4+2;
							}
							
														/* END*/
							if (yp_end[dynamicData_s[i_sg1]]==1)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==2)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+2;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}	
						else if (yp_end[dynamicData_s[i_sg1]]==3)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+3;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}
						else if  (yp_end[dynamicData_s[i_sg1]]==4)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+1;
							}	
						else if (yp_end[dynamicData_s[i_sg1]]==5)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+2;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==6)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+3;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==7)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+4;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==8)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+3;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==9)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+2;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==10)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+4;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==11)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+3;
							}
						else if (yp_end[dynamicData_s[i_sg1]]==12)
							{	
									yp_ex_ext0[i_es1] =(dynamicData_x[i_sg1])*4+1;
									yp_ey_ext0[i_es1] =(dynamicData_y[i_sg1])*4+2;
							}
					
				i_es1++;		
					
			}				
		}
	}		
	
	
	yp_cnt1[0]=(i_es/2);/*Total segmented line numbers */

	yp_cnt[0]=((i_es/2)+i_es1);/*Total segmented line numbers */
	

	
	
	/* %%%%%%%%%%%%%%%% Calculate X and Y Coordinates Of Segmented 4x4 line pieces   %%%%%%%%%%%%%%%% */	
		for ( i_sg2= 0; i_sg2< i_es; i_sg2++ ) 	
		{		
				dynamicData[i_sg2] =yp_xy[dynamicData_s[dynamicData_es[i_sg2]]];
				
						if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==1)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==1))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+1;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+1;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==2)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==2))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+2;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+1;
							}	
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==3)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==3))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+3;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+1;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==4)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==4))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+4;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+1;
							}	
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==5)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==5))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+4;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+2;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==6)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==6))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+4;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+3;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==7)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==7))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+4;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+4;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==8)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==8))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+3;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+4;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==9)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==9))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+2;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+4;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==10)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==10))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+1;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+4;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==11)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==11))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+1;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+3;
							}
						else if((yp_st[dynamicData_s[dynamicData_es[i_sg2]]]==12)||(yp_end[dynamicData_s[dynamicData_es[i_sg2]]]==12))
							{	
									dynamicData0[i_sg2] =(dynamicData_x[dynamicData_es[i_sg2]])*4+1;
									dynamicData1[i_sg2] =(dynamicData_y[dynamicData_es[i_sg2]])*4+2;
							}
						
						
		}
		
		
					

return;
}

/* %%%%%%%%%%%%%%%% Total segmented line numbers with Start and End Points  %%%%%%%%%%%%%%%% */	
static void segment_lines(	
	    unsigned  int yp_sx_ext[],
        unsigned  int yp_sy_ext[],
        unsigned  int yp_ex_ext[],
		unsigned  int yp_ey_ext[],
		unsigned  int  dynamicData[],				
		unsigned  int  dynamicData0[],	
		unsigned  int  dynamicData1[],
		unsigned  int yp_sx_ext0[],
        unsigned  int yp_sy_ext0[],
        unsigned  int yp_ex_ext0[],
		unsigned  int yp_ey_ext0[],	
		size_t  	m_no,
	    size_t  	m_no1,
		double b_in_H[] /* Choose Segmentation Rule */	
		)
{
		size_t i_lsn, i_lsn1,j_lsn, i_lsm;
		
		
		
/* Segmented line numbers with X and Y Coordinates Of Start and End Points*/	
 i_lsm=0; 
 for ( i_lsn = 0; i_lsn < (m_no1*2)-1; i_lsn ++ )
	 { 
		
					yp_sx_ext[i_lsm]=dynamicData0[i_lsn];
					yp_sy_ext[i_lsm]=dynamicData1[i_lsn];
					yp_ex_ext[i_lsm]=dynamicData0[i_lsn+1];
					yp_ey_ext[i_lsm]=dynamicData1[i_lsn+1];	
					i_lsm++;
					i_lsn ++;

	}
	

if( b_in_H[0]>0)
{
	
		for ( i_lsn1 = m_no1; i_lsn1 < m_no; i_lsn1 ++ ) 
	{
					yp_sx_ext[i_lsn1]=yp_sx_ext0[i_lsn1-m_no1];
					yp_sy_ext[i_lsn1]=yp_sy_ext0[i_lsn1-m_no1];
					yp_ex_ext[i_lsn1]=yp_ex_ext0[i_lsn1-m_no1];
					yp_ey_ext[i_lsn1]=yp_ey_ext0[i_lsn1-m_no1];
						
	}
}
	
    return;
}	

/* %%%%%%%%%%%%%%%% EXTENDED Total segmented line numbers with Start and End Points  %%%%%%%%%%%%%%%% */	
static void extend_segment_lines(	
	    unsigned  int yp_sx_ext[],
        unsigned  int yp_sy_ext[],
        unsigned  int yp_ex_ext[],
		unsigned  int yp_ey_ext[],
		unsigned  int yp_sx_ext1[],
        unsigned  int yp_sy_ext1[],
        unsigned  int yp_ex_ext1[],
		unsigned  int yp_ey_ext1[],	
		unsigned  char  b_in_ext[],  	 /*ImgEDGE will be extended */
		unsigned  char  yp_ext[],  	 /*Recognized 4x4 line pieces, the line Start&End Points will be extended */
		bool b_in[], /* Input Binary Edge image */
		size_t  	mp, /*Height of the divided input image to 4x4 pixel windows*/		
		size_t  	m_in, /*Height of the input image*/
		size_t  	m_no		   
		)
{
		size_t i_lsn1;
		
		size_t sx_r1, sx_r2; 
		size_t sy_r1, sy_r2; 
		
		size_t sx_rem1, sx_rem2,sx_rem3, sx_rem4,sx_rem5, sx_rem6,sx_rem7, sx_rem8; 
		size_t sy_rem1, sy_rem2,sy_rem3, sy_rem4,sy_rem5, sy_rem6,sy_rem7, sy_rem8; 
		size_t bn_s1,bn_s2,bn_s3,bn_s4,bn_s5,bn_s6,bn_s7,bn_s8;
		size_t yn_s1,yn_s2,yn_s3,yn_s4,yn_s5,yn_s6,yn_s7,yn_s8;
		
		size_t ex_rem1, ex_rem2,ex_rem3, ex_rem4,ex_rem5, ex_rem6,ex_rem7, ex_rem8; 
		size_t ey_rem1, ey_rem2,ey_rem3, ey_rem4,ey_rem5, ey_rem6,ey_rem7, ey_rem8; 
		size_t bn_s12,bn_s22,bn_s32,bn_s42,bn_s52,bn_s62,bn_s72,bn_s82;
		size_t yn_s12,yn_s22,yn_s32,yn_s42,yn_s52,yn_s62,yn_s72,yn_s82;
		
/* Segmented line numbers with X and Y Coordinates Of Start and End Points*/
 
 for ( i_lsn1 = 0; i_lsn1 < m_no; i_lsn1++ )
	 { 			  
						/* x1 and x2 are equal. */
				if(yp_sx_ext[i_lsn1]== yp_ex_ext[i_lsn1]) /* (y1,x1) check 3,2 and 1 neighbor, (y2,x2) check 5,6 and 7 neighbor. */
				{	
						if(yp_sy_ext[i_lsn1]<yp_ey_ext[i_lsn1])
						{	
							sx_r1=yp_sx_ext[i_lsn1];
					        sy_r1=yp_sy_ext[i_lsn1];
					        sx_r2=yp_ex_ext[i_lsn1];
					        sy_r2=yp_ey_ext[i_lsn1];
						}
						else
						{	
							sx_r2=yp_sx_ext[i_lsn1];
					        sy_r2=yp_sy_ext[i_lsn1];
					        sx_r1=yp_ex_ext[i_lsn1];
					        sy_r1=yp_ey_ext[i_lsn1];
						}
						
					/* (y1,x1) check 3,2 and 1 neighbor */						
					
					
					sx_rem2=sx_r1;
					sy_rem2=sy_r1-1;
					bn_s2=m_in*(sx_rem2-1)+(sy_rem2-1);
					
					
					
					if ((b_in_ext[bn_s2]==0)&&(b_in[bn_s2]>0)) /* 1 pixel extended with 2 */
					{
						yn_s2=mp*((sx_rem2-1-((sx_rem2-1)%4))/4)+((sy_rem2-1-((sy_rem2-1)%4))/4);
					
					
							if (yp_ext[yn_s2]==0) /* 1 pixel extended with 2 */
							{
										/*2 pixels Extend with 2 */
										ex_rem2=sx_rem2;
										ey_rem2=sy_rem2-1;
										bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
										
										yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
										
										if ((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0)) /* 1 pixel extended with 2 */
										{
											yp_sx_ext1[i_lsn1]=ex_rem2;
											yp_sy_ext1[i_lsn1]=ey_rem2;
											b_in_ext[bn_s22]=1;
										}
										else
										{
										yp_sx_ext1[i_lsn1]=sx_rem2;
										yp_sy_ext1[i_lsn1]=sy_rem2;							
										}
										b_in_ext[bn_s2]=1;
							}
							else 
							{
								yp_sx_ext1[i_lsn1]=sx_r1;
								yp_sy_ext1[i_lsn1]=sy_r1;
							}	
					
					}					
					else 
					{
						yp_sx_ext1[i_lsn1]=sx_r1;
						yp_sy_ext1[i_lsn1]=sy_r1;
					}	
					
					/* (y2,x2) check 5,6 and 7 neighbor. */
					
										
					sx_rem6=sx_r2;
					sy_rem6=sy_r2+1;
					bn_s6=m_in*(sx_rem6-1)+(sy_rem6-1);
							
					
					if ((b_in_ext[bn_s6]==0)&&(b_in[bn_s6]>0)) /* 1 pixel extended with 6 */
					{
					yn_s6=mp*((sx_rem6-1-((sx_rem6-1)%4))/4)+((sy_rem6-1-((sy_rem6-1)%4))/4);			
					
							if  (yp_ext[yn_s6]==0) /* 1 pixel extended with 6 */
							{
										/*2 pixels Extend with 6 */
										ex_rem6=sx_rem6;
										ey_rem6=sy_rem6+1;
										bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
										yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
										if ((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0)) /* 1 pixel extended with 6 */
										{
											yp_ex_ext1[i_lsn1]=ex_rem6;
											yp_ey_ext1[i_lsn1]=ey_rem6;
											b_in_ext[bn_s62]=1;
										}
										else
										{
									yp_ex_ext1[i_lsn1]=sx_rem6;
									yp_ey_ext1[i_lsn1]=sy_rem6;
									}
									b_in_ext[bn_s6]=1;
							}
							else
							{
								yp_ex_ext1[i_lsn1]=sx_r2;
								yp_ey_ext1[i_lsn1]=sy_r2;
							}
					}
					else
					{
						yp_ex_ext1[i_lsn1]=sx_r2;
						yp_ey_ext1[i_lsn1]=sy_r2;
					}
					
					
					
				}
			/* y1 and y2 are equal. */
				else if(yp_sy_ext[i_lsn1]== yp_ey_ext[i_lsn1]) /* (y1,x1) check 3,4 and 5 neighbor, (y2,x2) check 1,7 and 8 neighbor. */
				{
						if(yp_sx_ext[i_lsn1]<yp_ex_ext[i_lsn1])
						{	
							sx_r1=yp_sx_ext[i_lsn1];
					        sy_r1=yp_sy_ext[i_lsn1];
					        sx_r2=yp_ex_ext[i_lsn1];
					        sy_r2=yp_ey_ext[i_lsn1];
						}
						else
						{	
							sx_r2=yp_sx_ext[i_lsn1];
					        sy_r2=yp_sy_ext[i_lsn1];
					        sx_r1=yp_ex_ext[i_lsn1];
					        sy_r1=yp_ey_ext[i_lsn1];
						}
						
						
					/* (y1,x1) check 3,4 and 5 neighbor. */
					
					
					sx_rem4=sx_r1-1;
					sy_rem4=sy_r1;
					bn_s4=m_in*(sx_rem4-1)+(sy_rem4-1);
					
					 if ((b_in_ext[bn_s4]==0)&&(b_in[bn_s4]>0))/* 1 pixel extended with 4 */
					 {
						yn_s4=mp*((sx_rem4-1-((sx_rem4-1)%4))/4)+((sy_rem4-1-((sy_rem4-1)%4))/4);					
										
						if (yp_ext[yn_s4]==0) /* 1 pixel extended with 4 */
						{
									/*2 pixels Extend with 4 */
										ex_rem4=sx_rem4-1;
										ey_rem4=sy_rem4;
										bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
										yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
										if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 1 pixel extended with 4 */
										{
											yp_sx_ext1[i_lsn1]=ex_rem4;
											yp_sy_ext1[i_lsn1]=ey_rem4;
											b_in_ext[bn_s42]=1;
										}
										else
										{
							yp_sx_ext1[i_lsn1]=sx_rem4;
							yp_sy_ext1[i_lsn1]=sy_rem4;
										}
							b_in_ext[bn_s4]=1;
						}
						else 
						{
							yp_sx_ext1[i_lsn1]=sx_r1;
							yp_sy_ext1[i_lsn1]=sy_r1;
						}
					 }
					else 
					{
						yp_sx_ext1[i_lsn1]=sx_r1;
						yp_sy_ext1[i_lsn1]=sy_r1;
					}
					
						
					/* (y2,x2) check 1,7 and 8 neighbor. */
					
					
					sx_rem8=sx_r2+1;
					sy_rem8=sy_r2;
					bn_s8=m_in*(sx_rem8-1)+(sy_rem8-1);
					 if ((b_in_ext[bn_s8]==0)&&(b_in[bn_s8]>0))  /* 1 pixel extended with 8*/
					{
						yn_s8=mp*((sx_rem8-1-((sx_rem8-1)%4))/4)+((sy_rem8-1-((sy_rem8-1)%4))/4);
					
						if (yp_ext[yn_s8]==0) /* 1 pixel extended with 8*/
						{
								/*2 pixels Extend with 8 */
										ex_rem8=sx_rem8+1;
										ey_rem8=sy_rem8;
										bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
										yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
										if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0)) /* 1 pixel extended with 8*/
										{
											yp_ex_ext1[i_lsn1]=ex_rem8;
											yp_ey_ext1[i_lsn1]=ey_rem8;
											b_in_ext[bn_s82]=1;
										}
										else
										{
								yp_ex_ext1[i_lsn1]=sx_rem8;
								yp_ey_ext1[i_lsn1]=sy_rem8;
										}
								b_in_ext[bn_s8]=1;
						}
						else
						{
							yp_ex_ext1[i_lsn1]=sx_r2;
							yp_ey_ext1[i_lsn1]=sy_r2;
						}
					}
					else
					{
						yp_ex_ext1[i_lsn1]=sx_r2;
						yp_ey_ext1[i_lsn1]=sy_r2;
					}
					
						
				}
				/* %%%%%%%%%%%%%%%% ALL CONDITION %%%%%%%%%%%%%%%%%%%%%%%%*/
	/* (x1 > x2  and [y1 < y2 OR y1 > y2]) OR(x1 < x2  and [y1 < y2 OR y1 > y2])   */
					/* (x1 is smaller than x2  and [(y1 is bigger than y2) OR  ( and y1 is smaller than y2)] */
					
					/* Condition 1 */
					/* (x1 is smaller than x2  and [(y1 is bigger than y2) */
				else if((yp_sx_ext[i_lsn1]<yp_ex_ext[i_lsn1])&&(yp_sy_ext[i_lsn1]>yp_ey_ext[i_lsn1])) 
				{						
							sx_r1=yp_sx_ext[i_lsn1];
					        sy_r1=yp_sy_ext[i_lsn1];
					        sx_r2=yp_ex_ext[i_lsn1];
					        sy_r2=yp_ey_ext[i_lsn1];
							
								
																		
									/* (y1,x1) check 4,5 and 6 neighbor. 1 pixel */
									sx_rem4=sx_r1-1;
									sy_rem4=sy_r1;
									bn_s4=m_in*(sx_rem4-1)+(sy_rem4-1);
									yn_s4=mp*((sx_rem4-1-((sx_rem4-1)%4))/4)+((sy_rem4-1-((sy_rem4-1)%4))/4);
									
									sx_rem5=sx_r1-1;
									sy_rem5=sy_r1+1;
									bn_s5=m_in*(sx_rem5-1)+(sy_rem5-1);
									yn_s5=mp*((sx_rem5-1-((sx_rem5-1)%4))/4)+((sy_rem5-1-((sy_rem5-1)%4))/4);
									
									sx_rem6=sx_r1;
									sy_rem6=sy_r1+1;
									bn_s6=m_in*(sx_rem6-1)+(sy_rem6-1);
									yn_s6=mp*((sx_rem6-1-((sx_rem6-1)%4))/4)+((sy_rem6-1-((sy_rem6-1)%4))/4);
									
									
									
									if (((b_in_ext[bn_s5]==0)&&(b_in[bn_s5]>0)&& (yp_ext[yn_s5]==0)) && (sy_r1-sy_r2>4)) /* 1 pixel extended with 5 */
									{
												/* (y1,x1) check 4,5 and 6 neighbor. Extend 2 pixels from 5*/
												ex_rem4=sx_rem5-1;
												ey_rem4=sy_rem5;
												bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
												yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
												
												ex_rem5=sx_rem5-1;
												ey_rem5=sy_rem5+1;
												bn_s52=m_in*(ex_rem5-1)+(ey_rem5-1);
												yn_s52=mp*((ex_rem5-1-((ex_rem5-1)%4))/4)+((ey_rem5-1-((ey_rem5-1)%4))/4);
												
												ex_rem6=sx_rem5;
												ey_rem6=sy_rem5+1;
												bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
												yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
												
												if ((b_in_ext[bn_s52]==0)&&(b_in[bn_s52]>0)&& (yp_ext[yn_s52]==0)) /* 2 pixels extended with 5 */
												{
														yp_sx_ext1[i_lsn1]=ex_rem5;
														yp_sy_ext1[i_lsn1]=ey_rem5;
														b_in_ext[bn_s52]=1;
												}
												else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 2 pixels extended with 4 */
												{
														yp_sx_ext1[i_lsn1]=ex_rem4;
														yp_sy_ext1[i_lsn1]=ey_rem4;
														b_in_ext[bn_s42]=1;
												}
												else if (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r1-sy_r2>4)) /* 2 pixels extended with 6 */
												{
													
														yp_sx_ext1[i_lsn1]=ex_rem6;
														yp_sy_ext1[i_lsn1]=ey_rem6;
														b_in_ext[bn_s62]=1;
													
												}
												else 
												{
														yp_sx_ext1[i_lsn1]=sx_rem5;
														yp_sy_ext1[i_lsn1]=sy_rem5;
												}
												b_in_ext[bn_s5]=1;
									}
									
									else if ((b_in_ext[bn_s4]==0)&&(b_in[bn_s4]>0)&& (yp_ext[yn_s4]==0)) /* 1 pixel extended with 4 */
									{
											/* (y1,x1) check 4,5 and 6 neighbor. Extend 2 pixels from 4*/
											ex_rem4=sx_rem4-1;
											ey_rem4=sy_rem4;
											bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
											yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
											
											ex_rem5=sx_rem4-1;
											ey_rem5=sy_rem4+1;
											bn_s52=m_in*(ex_rem5-1)+(ey_rem5-1);
											yn_s52=mp*((ex_rem5-1-((ex_rem5-1)%4))/4)+((ey_rem5-1-((ey_rem5-1)%4))/4);
											
											ex_rem6=sx_rem4;
											ey_rem6=sy_rem4+1;
											bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
											yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
											
											if ((b_in_ext[bn_s52]==0)&&(b_in[bn_s52]>0)&& (yp_ext[yn_s52]==0)) /* 2 pixels extended with 5 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem5;
													yp_sy_ext1[i_lsn1]=ey_rem5;
													b_in_ext[bn_s52]=1;
											}
											else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 2 pixels extended with 4 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem4;
													yp_sy_ext1[i_lsn1]=ey_rem4;
													b_in_ext[bn_s42]=1;
											}
											else if  (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r1-sy_r2>4)) /* 2 pixels extended with 6 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem6;
													yp_sy_ext1[i_lsn1]=ey_rem6;
													b_in_ext[bn_s62]=1;
											}
											else 
											{
													yp_sx_ext1[i_lsn1]=sx_rem4;
													yp_sy_ext1[i_lsn1]=sy_rem4;
											}
											b_in_ext[bn_s4]=1;
									}
									else if (((b_in_ext[bn_s6]==0)&&(b_in[bn_s6]>0)&& (yp_ext[yn_s6]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 6 */
									{
												/* (y1,x1) check 4,5 and 6 neighbor. Extend 2 pixels from 6*/
											ex_rem4=sx_rem6-1;
											ey_rem4=sy_rem6;
											bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
											yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
											
											ex_rem5=sx_rem6-1;
											ey_rem5=sy_rem6+1;
											bn_s52=m_in*(ex_rem5-1)+(ey_rem5-1);
											yn_s52=mp*((ex_rem5-1-((ex_rem5-1)%4))/4)+((ey_rem5-1-((ey_rem5-1)%4))/4);
											
											ex_rem6=sx_rem6;
											ey_rem6=sy_rem6+1;
											bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
											yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
											
											if ((b_in_ext[bn_s52]==0)&&(b_in[bn_s52]>0)&& (yp_ext[yn_s52]==0)) /* 2 pixels extended with 5 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem5;
													yp_sy_ext1[i_lsn1]=ey_rem5;
													b_in_ext[bn_s52]=1;
											}
											else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 2 pixels extended with 4 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem4;
													yp_sy_ext1[i_lsn1]=ey_rem4;
													b_in_ext[bn_s42]=1;
											}
											else if (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r1-sy_r2>4)) /* 2 pixels extended with 6 */
											 
											{
													yp_sx_ext1[i_lsn1]=ex_rem6;
													yp_sy_ext1[i_lsn1]=ey_rem6;
													b_in_ext[bn_s62]=1;
											}
											else 
											{
											yp_sx_ext1[i_lsn1]=sx_rem6;
											yp_sy_ext1[i_lsn1]=sy_rem6;
											}
											b_in_ext[bn_s6]=1;
									}
									 
									else 
									{
										yp_sx_ext1[i_lsn1]=sx_r1;
										yp_sy_ext1[i_lsn1]=sy_r1;
										
									}
									
																		/* (y2,x2) check 2,1 and 8 neighbor. */	
									sx_rem1=sx_r2+1;
									sy_rem1=sy_r2-1;
									bn_s1=m_in*(sx_rem1-1)+(sy_rem1-1);
									yn_s1=mp*((sx_rem1-1-((sx_rem1-1)%4))/4)+((sy_rem1-1-((sy_rem1-1)%4))/4);
					
									sx_rem8=sx_r2+1;
									sy_rem8=sy_r2;
									bn_s8=m_in*(sx_rem8-1)+(sy_rem8-1);
									yn_s8=mp*((sx_rem8-1-((sx_rem8-1)%4))/4)+((sy_rem8-1-((sy_rem8-1)%4))/4);
									
									sx_rem2=sx_r2;
									sy_rem2=sy_r2-1;
									bn_s2=m_in*(sx_rem2-1)+(sy_rem2-1);
									yn_s2=mp*((sx_rem2-1-((sx_rem2-1)%4))/4)+((sy_rem2-1-((sy_rem2-1)%4))/4);
									
									if (((b_in_ext[bn_s1]==0)&&(b_in[bn_s1]>0)&& (yp_ext[yn_s1]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 1 */
									{
												
											/* (y2,x2) check 2,1 and 8 neighbor. Extend 2 pixels from 1*/	
											ex_rem1=sx_rem1+1;
											ey_rem1=sy_rem1-1;
											bn_s12=m_in*(ex_rem1-1)+(ey_rem1-1);
											yn_s12=mp*((ex_rem1-1-((ex_rem1-1)%4))/4)+((ey_rem1-1-((ey_rem1-1)%4))/4);
							
											ex_rem8=sx_rem1+1;
											ey_rem8=sy_rem1;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											ex_rem2=sx_rem1;
											ey_rem2=sy_rem1-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											if ((b_in_ext[bn_s12]==0)&&(b_in[bn_s12]>0)&& (yp_ext[yn_s12]==0)) /* 1 pixel extended with 1 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem1;
													yp_ey_ext1[i_lsn1]=ey_rem1;
													b_in_ext[bn_s12]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 2 */
											{
														yp_ex_ext1[i_lsn1]=ex_rem2;
														yp_ey_ext1[i_lsn1]=ey_rem2;
														b_in_ext[bn_s22]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}
											else
											{
													yp_ex_ext1[i_lsn1]=sx_rem1;
													yp_ey_ext1[i_lsn1]=sy_rem1;
											}
											b_in_ext[bn_s1]=1;
									}
									else if (((b_in_ext[bn_s2]==0)&&(b_in[bn_s2]>0)&& (yp_ext[yn_s2]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 2 */
									{
												/* (y2,x2) check 2,1 and 8 neighbor. Extend 2 pixels from 2*/	
											ex_rem1=sx_rem2+1;
											ey_rem1=sy_rem2-1;
											bn_s12=m_in*(ex_rem1-1)+(ey_rem1-1);
											yn_s12=mp*((ex_rem1-1-((ex_rem1-1)%4))/4)+((ey_rem1-1-((ey_rem1-1)%4))/4);
							
											ex_rem8=sx_rem2+1;
											ey_rem8=sy_rem2;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											ex_rem2=sx_rem2;
											ey_rem2=sy_rem2-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											if ((b_in_ext[bn_s12]==0)&&(b_in[bn_s12]>0)&& (yp_ext[yn_s12]==0)) /* 1 pixel extended with 1 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem1;
													yp_ey_ext1[i_lsn1]=ey_rem1;
													b_in_ext[bn_s12]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 2 */
											{
														yp_ex_ext1[i_lsn1]=ex_rem2;
														yp_ey_ext1[i_lsn1]=ey_rem2;
														b_in_ext[bn_s22]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}
											else
											{
												yp_ex_ext1[i_lsn1]=sx_rem2;
												yp_ey_ext1[i_lsn1]=sy_rem2;
											}
												b_in_ext[bn_s2]=1;
									}
									else if ((b_in_ext[bn_s8]==0)&&(b_in[bn_s8]>0)&& (yp_ext[yn_s8]==0))  /* 1 pixel extended with 8*/
									{
											/* (y2,x2) check 2,1 and 8 neighbor. Extend 2 pixels from 8*/	
											ex_rem1=sx_rem8+1;
											ey_rem1=sy_rem8-1;
											bn_s12=m_in*(ex_rem1-1)+(ey_rem1-1);
											yn_s12=mp*((ex_rem1-1-((ex_rem1-1)%4))/4)+((ey_rem1-1-((ey_rem1-1)%4))/4);
							
											ex_rem8=sx_rem8+1;
											ey_rem8=sy_rem8;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											ex_rem2=sx_rem8;
											ey_rem2=sy_rem8-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											if ((b_in_ext[bn_s12]==0)&&(b_in[bn_s12]>0)&& (yp_ext[yn_s12]==0)) /* 1 pixel extended with 1 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem1;
													yp_ey_ext1[i_lsn1]=ey_rem1;
													b_in_ext[bn_s12]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r1-sy_r2>4))  /* 1 pixel extended with 2 */
											{
														yp_ex_ext1[i_lsn1]=ex_rem2;
														yp_ey_ext1[i_lsn1]=ey_rem2;
														b_in_ext[bn_s22]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}
											else
											{
											yp_ex_ext1[i_lsn1]=sx_rem8;
											yp_ey_ext1[i_lsn1]=sy_rem8;
											}
											b_in_ext[bn_s8]=1;
									}
									else
									{
										yp_ex_ext1[i_lsn1]=sx_r2;
										yp_ey_ext1[i_lsn1]=sy_r2;
									}
									
									
									
									
												
								
								
				}
				
				/* Condition 2 */
				/* (x1 is smaller than x2  and [(y1 is smaller  than y2) */
				else if((yp_sx_ext[i_lsn1]<yp_ex_ext[i_lsn1])&&(yp_sy_ext[i_lsn1]<yp_ey_ext[i_lsn1])) 
				{						
							sx_r1=yp_sx_ext[i_lsn1];
					        sy_r1=yp_sy_ext[i_lsn1];
					        sx_r2=yp_ex_ext[i_lsn1];
					        sy_r2=yp_ey_ext[i_lsn1];
							
								/* (y1,x1) check 3,2 and 4 neighbor. */
									sx_rem3=sx_r1-1;
									sy_rem3=sy_r1-1;
									bn_s3=m_in*(sx_rem3-1)+(sy_rem3-1);
									yn_s3=mp*((sx_rem3-1-((sx_rem3-1)%4))/4)+((sy_rem3-1-((sy_rem3-1)%4))/4);
									
									sx_rem4=sx_r1-1;
									sy_rem4=sy_r1;
									bn_s4=m_in*(sx_rem4-1)+(sy_rem4-1);
									yn_s4=mp*((sx_rem4-1-((sx_rem4-1)%4))/4)+((sy_rem4-1-((sy_rem4-1)%4))/4);
									
									sx_rem2=sx_r1;
									sy_rem2=sy_r1-1;
									bn_s2=m_in*(sx_rem2-1)+(sy_rem2-1);
									yn_s2=mp*((sx_rem2-1-((sx_rem2-1)%4))/4)+((sy_rem2-1-((sy_rem2-1)%4))/4);
									
									
									if (((b_in_ext[bn_s3]==0)&&(b_in[bn_s3]>0)&& (yp_ext[yn_s3]==0))&& (sy_r2-sy_r1>4))  /* 1 pixel extended with 3*/
									{									
									
											/* (y1,x1) check 3,2 and 4 neighbor. Extend 2 pixels from 3*/
											ex_rem3=sx_rem3-1;
											ey_rem3=sy_rem3-1;
											bn_s32=m_in*(ex_rem3-1)+(ey_rem3-1);
											yn_s32=mp*((ex_rem3-1-((ex_rem3-1)%4))/4)+((ey_rem3-1-((ey_rem3-1)%4))/4);
											
											ex_rem4=sx_rem3-1;
											ey_rem4=sy_rem3;
											bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
											yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
											
											ex_rem2=sx_rem3;
											ey_rem2=sy_rem3-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											
											if ((b_in_ext[bn_s32]==0)&&(b_in[bn_s32]>0)&& (yp_ext[yn_s32]==0)) /* 1 pixel extended with 3*/
											{	
													yp_sx_ext1[i_lsn1]=ex_rem3;
													yp_sy_ext1[i_lsn1]=ey_rem3;
													b_in_ext[bn_s32]=1;
											}
											else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 1 pixel extended with 4 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem4;
													yp_sy_ext1[i_lsn1]=ey_rem4;
													b_in_ext[bn_s42]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r2-sy_r1>4))  /* 1 pixel extended with 2 */
											
											{
													yp_sx_ext1[i_lsn1]=ex_rem2;
													yp_sy_ext1[i_lsn1]=ey_rem2;
													b_in_ext[bn_s22]=1;
											}
											else 
											{
										
											
													yp_sx_ext1[i_lsn1]=sx_rem3;
													yp_sy_ext1[i_lsn1]=sy_rem3;
											}
											b_in_ext[bn_s3]=1;
									}
									else if ((b_in_ext[bn_s4]==0)&&(b_in[bn_s4]>0)&& (yp_ext[yn_s4]==0)) /* 1 pixel extended with 4 */
									{
											/* (y1,x1) check 3,2 and 4 neighbor. Extend 2 pixels from 4*/
											ex_rem3=sx_rem4-1;
											ey_rem3=sy_rem4-1;
											bn_s32=m_in*(ex_rem3-1)+(ey_rem3-1);
											yn_s32=mp*((ex_rem3-1-((ex_rem3-1)%4))/4)+((ey_rem3-1-((ey_rem3-1)%4))/4);
											
											ex_rem4=sx_rem4-1;
											ey_rem4=sy_rem4;
											bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
											yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
											
											ex_rem2=sx_rem4;
											ey_rem2=sy_rem4-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											
											if ((b_in_ext[bn_s32]==0)&&(b_in[bn_s32]>0)&& (yp_ext[yn_s32]==0)) /* 1 pixel extended with 3*/
											{	
													yp_sx_ext1[i_lsn1]=ex_rem3;
													yp_sy_ext1[i_lsn1]=ey_rem3;
													b_in_ext[bn_s32]=1;
											}
											else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 1 pixel extended with 4 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem4;
													yp_sy_ext1[i_lsn1]=ey_rem4;
													b_in_ext[bn_s42]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r2-sy_r1>4))  /* 1 pixel extended with 2 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem2;
													yp_sy_ext1[i_lsn1]=ey_rem2;
													b_in_ext[bn_s22]=1;
											}
											else 
											{
											yp_sx_ext1[i_lsn1]=sx_rem4;
											yp_sy_ext1[i_lsn1]=sy_rem4;
											}
											b_in_ext[bn_s4]=1;
									}
									else if (((b_in_ext[bn_s2]==0)&&(b_in[bn_s2]>0)&& (yp_ext[yn_s2]==0))&& (sy_r2-sy_r1>4))  /* 1 pixel extended with 2 */
									{
											/* (y1,x1) check 3,2 and 4 neighbor. Extend 2 pixels from 2*/
											ex_rem3=sx_rem2-1;
											ey_rem3=sy_rem2-1;
											bn_s32=m_in*(ex_rem3-1)+(ey_rem3-1);
											yn_s32=mp*((ex_rem3-1-((ex_rem3-1)%4))/4)+((ey_rem3-1-((ey_rem3-1)%4))/4);
											
											ex_rem4=sx_rem2-1;
											ey_rem4=sy_rem2;
											bn_s42=m_in*(ex_rem4-1)+(ey_rem4-1);
											yn_s42=mp*((ex_rem4-1-((ex_rem4-1)%4))/4)+((ey_rem4-1-((ey_rem4-1)%4))/4);
											
											ex_rem2=sx_rem2;
											ey_rem2=sy_rem2-1;
											bn_s22=m_in*(ex_rem2-1)+(ey_rem2-1);
											yn_s22=mp*((ex_rem2-1-((ex_rem2-1)%4))/4)+((ey_rem2-1-((ey_rem2-1)%4))/4);
											
											
											if ((b_in_ext[bn_s32]==0)&&(b_in[bn_s32]>0)&& (yp_ext[yn_s32]==0)) /* 1 pixel extended with 3*/
											{	
													yp_sx_ext1[i_lsn1]=ex_rem3;
													yp_sy_ext1[i_lsn1]=ey_rem3;
													b_in_ext[bn_s32]=1;
											}
											else if ((b_in_ext[bn_s42]==0)&&(b_in[bn_s42]>0)&& (yp_ext[yn_s42]==0)) /* 1 pixel extended with 4 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem4;
													yp_sy_ext1[i_lsn1]=ey_rem4;
													b_in_ext[bn_s42]=1;
											}
											else if (((b_in_ext[bn_s22]==0)&&(b_in[bn_s22]>0)&& (yp_ext[yn_s22]==0))&& (sy_r2-sy_r1>4))  /* 1 pixel extended with 2 */
											{
													yp_sx_ext1[i_lsn1]=ex_rem2;
													yp_sy_ext1[i_lsn1]=ey_rem2;
													b_in_ext[bn_s22]=1;
											}
											else 
											{
											yp_sx_ext1[i_lsn1]=sx_rem2;
											yp_sy_ext1[i_lsn1]=sy_rem2;
											}
											b_in_ext[bn_s2]=1;
									}
									else 
									{
										yp_sx_ext1[i_lsn1]=sx_r1;
										yp_sy_ext1[i_lsn1]=sy_r1;
									}
									
									/*y1 is smaller than y2 && END Point*/
									/* (y2,x2) check 6,7 and 8 neighbor. */
									sx_rem7=sx_r2+1;
									sy_rem7=sy_r2+1;
									bn_s7=m_in*(sx_rem7-1)+(sy_rem7-1);
									yn_s7=mp*((sx_rem7-1-((sx_rem7-1)%4))/4)+((sy_rem7-1-((sy_rem7-1)%4))/4);
									
									sx_rem6=sx_r2;
									sy_rem6=sy_r2+1;
									bn_s6=m_in*(sx_rem6-1)+(sy_rem6-1);
									yn_s6=mp*((sx_rem6-1-((sx_rem6-1)%4))/4)+((sy_rem6-1-((sy_rem6-1)%4))/4);
									
									sx_rem8=sx_r2+1;
									sy_rem8=sy_r2;
									bn_s8=m_in*(sx_rem8-1)+(sy_rem8-1);
									yn_s8=mp*((sx_rem8-1-((sx_rem8-1)%4))/4)+((sy_rem8-1-((sy_rem8-1)%4))/4);
									
									 if (((b_in_ext[bn_s7]==0)&&(b_in[bn_s7]>0)&& (yp_ext[yn_s7]==0)) && (sy_r2-sy_r1>4))/* 1 pixel extended with 7 */
									{		
											/* (y2,x2) check 6,7 and 8 neighbor.  Extend 2 pixels from 7 */
											ex_rem7=sx_rem7+1;
											ey_rem7=sy_rem7+1;
											bn_s72=m_in*(ex_rem7-1)+(ey_rem7-1);
											yn_s72=mp*((ex_rem7-1-((ex_rem7-1)%4))/4)+((ey_rem7-1-((ey_rem7-1)%4))/4);
											
											ex_rem6=sx_rem7;
											ey_rem6=sy_rem7+1;
											bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
											yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
											
											ex_rem8=sx_rem7+1;
											ey_rem8=sy_rem7;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											if ((b_in_ext[bn_s72]==0)&&(b_in[bn_s72]>0)&& (yp_ext[yn_s72]==0)) /* 1 pixel extended with 7 */
											{		
													yp_ex_ext1[i_lsn1]=ex_rem7;
													yp_ey_ext1[i_lsn1]=ey_rem7;
													b_in_ext[bn_s72]=1;
											}
											else if (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r2-sy_r1>4)) /* 2 pixels extended with 6 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem6;
													yp_ey_ext1[i_lsn1]=ey_rem6;
													b_in_ext[bn_s62]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}	
											else
											{
													yp_ex_ext1[i_lsn1]=sx_rem7;
													yp_ey_ext1[i_lsn1]=sy_rem7;
											}
											b_in_ext[bn_s7]=1;
									}
									else if (((b_in_ext[bn_s6]==0)&&(b_in[bn_s6]>0)&& (yp_ext[yn_s6]==0))&& (sy_r2-sy_r1>4)) /* 1 pixel extended with 6 */
									{
											/* (y2,x2) check 6,7 and 8 neighbor.  Extend 2 pixels from 6 */
											ex_rem7=sx_rem6+1;
											ey_rem7=sy_rem6+1;
											bn_s72=m_in*(ex_rem7-1)+(ey_rem7-1);
											yn_s72=mp*((ex_rem7-1-((ex_rem7-1)%4))/4)+((ey_rem7-1-((ey_rem7-1)%4))/4);
											
											ex_rem6=sx_rem6;
											ey_rem6=sy_rem6+1;
											bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
											yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
											
											ex_rem8=sx_rem6+1;
											ey_rem8=sy_rem6;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											if ((b_in_ext[bn_s72]==0)&&(b_in[bn_s72]>0)&& (yp_ext[yn_s72]==0)) /* 1 pixel extended with 7 */
											{		
													yp_ex_ext1[i_lsn1]=ex_rem7;
													yp_ey_ext1[i_lsn1]=ey_rem7;
													b_in_ext[bn_s72]=1;
											}
											else if (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r2-sy_r1>4)) /* 2 pixels extended with 6 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem6;
													yp_ey_ext1[i_lsn1]=ey_rem6;
													b_in_ext[bn_s62]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}	
											else
											{
											yp_ex_ext1[i_lsn1]=sx_rem6;
											yp_ey_ext1[i_lsn1]=sy_rem6;
											}
											b_in_ext[bn_s6]=1;
									}
									else if ((b_in_ext[bn_s8]==0)&&(b_in[bn_s8]>0)&& (yp_ext[yn_s8]==0))  /* 1 pixel extended with 8*/
									{
											/* (y2,x2) check 6,7 and 8 neighbor.  Extend 2 pixels from 8 */
											ex_rem7=sx_rem8+1;
											ey_rem7=sy_rem8+1;
											bn_s72=m_in*(ex_rem7-1)+(ey_rem7-1);
											yn_s72=mp*((ex_rem7-1-((ex_rem7-1)%4))/4)+((ey_rem7-1-((ey_rem7-1)%4))/4);
											
											ex_rem6=sx_rem8;
											ey_rem6=sy_rem8+1;
											bn_s62=m_in*(ex_rem6-1)+(ey_rem6-1);
											yn_s62=mp*((ex_rem6-1-((ex_rem6-1)%4))/4)+((ey_rem6-1-((ey_rem6-1)%4))/4);
											
											ex_rem8=sx_rem8+1;
											ey_rem8=sy_rem8;
											bn_s82=m_in*(ex_rem8-1)+(ey_rem8-1);
											yn_s82=mp*((ex_rem8-1-((ex_rem8-1)%4))/4)+((ey_rem8-1-((ey_rem8-1)%4))/4);
											
											if ((b_in_ext[bn_s72]==0)&&(b_in[bn_s72]>0)&& (yp_ext[yn_s72]==0)) /* 1 pixel extended with 7 */
											{		
													yp_ex_ext1[i_lsn1]=ex_rem7;
													yp_ey_ext1[i_lsn1]=ey_rem7;
													b_in_ext[bn_s72]=1;
											}
											else if (((b_in_ext[bn_s62]==0)&&(b_in[bn_s62]>0)&& (yp_ext[yn_s62]==0))&& (sy_r2-sy_r1>4)) /* 2 pixels extended with 6 */
											{
													yp_ex_ext1[i_lsn1]=ex_rem6;
													yp_ey_ext1[i_lsn1]=ey_rem6;
													b_in_ext[bn_s62]=1;
											}
											else if ((b_in_ext[bn_s82]==0)&&(b_in[bn_s82]>0)&& (yp_ext[yn_s82]==0))  /* 1 pixel extended with 8*/
											{
													yp_ex_ext1[i_lsn1]=ex_rem8;
													yp_ey_ext1[i_lsn1]=ey_rem8;
													b_in_ext[bn_s82]=1;
											}	
											else
											{
											yp_ex_ext1[i_lsn1]=sx_rem8;
											yp_ey_ext1[i_lsn1]=sy_rem8;
											}
											b_in_ext[bn_s8]=1;
									}	
									else
									{
										yp_ex_ext1[i_lsn1]=sx_r2;
										yp_ey_ext1[i_lsn1]=sy_r2;
									}
							
				}
				
					
				else
				{
							yp_sx_ext1[i_lsn1]=yp_sx_ext[i_lsn1];
					        yp_sy_ext1[i_lsn1]=yp_sy_ext[i_lsn1];
					        yp_ex_ext1[i_lsn1]=yp_ex_ext[i_lsn1];
					        yp_ey_ext1[i_lsn1]=yp_ey_ext[i_lsn1];
				}
					
				
									
								
	}	
			
						
					
						
				
					
	
	
    return;
}	



/*mexFunction C Syntax */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    
	unsigned int *yp_xy;  /* Location of 4x4 line pieces in the array */
	unsigned char *yp_st; /* Recognized 4x4 line pieces' Start points if it is connected, it will be zero during segmentation */
	unsigned char *yp_end; /* Recognized 4x4 line pieces' End points if it is connected, it will be zero during segmentation  */
	unsigned char	*yp; /* Recognized 4x4 line pieces' Slopes */
	unsigned int	*yp_cnt; /* Recognized 4x4 line pieces' Count */	
	unsigned int	*yp_cnt1; /* Recognized 4x4 line pieces' Count */
    bool	*b_in; /* Input Binary Edge image */
	double	*b_in_H; /* Chose H segmentation rule */
	unsigned short m_in; /*Height of the input image*/
	unsigned short n_in; /*Width  of the input image*/
    unsigned short mp;  /*Height of the divided input image to 4x4 pixel windows*/
	unsigned short np; /*Width of the divided input image to 4x4 pixel windows*/
	unsigned int m_no; /*Total segmented line numbers */
	
	unsigned int *m_no1; /*Total segmented line numbers */
	
	unsigned int *yp_sx_ext;   /* Start Points X Coordinate of Segmented Lines */
	unsigned int *yp_sy_ext;   /* Start Points Y Coordinate of Segmented Lines */
	unsigned int *yp_ex_ext;   /* End Points X Coordinate of Segmented Lines */
	unsigned int *yp_ey_ext;   /* End Points X Coordinate of Segmented Lines */
	unsigned  int *yp_sx_ext0;
    unsigned  int *yp_sy_ext0;
    unsigned  int *yp_ex_ext0;
	unsigned  int *yp_ey_ext0;
	
	unsigned char *yp_ext; /* Recognized 4x4 line pieces will be extended  */
	unsigned  int  *dynamicData,*dynamicData0,*dynamicData1;
	unsigned  int *dynamicData_z,*dynamicData_s,*dynamicData_es1,*dynamicData_es;
	unsigned  int *dynamicData_x,*dynamicData_y;	
	unsigned char *b_in_ext; /* ImgEDGE will be extended  */
	
	unsigned int *yp_sx_ext1;   /* Start Points X Coordinate of Segmented Lines */
	unsigned int *yp_sy_ext1;   /* Start Points Y Coordinate of Segmented Lines */
	unsigned int *yp_ex_ext1;   /* End Points X Coordinate of Segmented Lines */
	unsigned int *yp_ey_ext1;   /* End Points X Coordinate of Segmented Lines */
	
	
	bool *b_in_seg;
	
    m_in = mxGetM(prhs[0]);  /* Get Height of the input image*/
    n_in = mxGetN(prhs[0]);   /*Get Width  of the input image*/  
	mp=(m_in-(m_in%4))/4; /*Calculate Height of the divided input image to 4x4 pixel windows*/
    np=(n_in-(n_in%4))/4; /*Calculate Width of the divided input image to 4x4 pixel windows*/ 
	
	
    S_OUT_CNT = mxCreateNumericMatrix( 1, 1, mxUINT32_CLASS,mxREAL); /*Create 1x1 matrix holds total segmented line numbers*/ 
	yp_cnt= mxGetPr(S_OUT_CNT);/* Give Recognized 4x4 line pieces' End points */	
	

	
	yp_cnt1= mxMalloc((1)  * sizeof(unsigned  int)); 	
	
   	yp_st =  mxGetPr(mxCreateNumericMatrix( mp, np, mxUINT8_CLASS,mxREAL)); /* Give Recognized 4x4 line pieces' Start points */
	yp_end = mxGetPr(mxCreateNumericMatrix( mp, np, mxUINT8_CLASS,mxREAL)); /* Give Recognized 4x4 line pieces' End points */
	yp_xy =  mxGetPr(mxCreateNumericMatrix( mp, np, mxUINT32_CLASS,mxREAL));/* Give Recognized 4x4 line pieces' Segmentation numbers */	
	yp = mxGetPr(mxCreateNumericMatrix( mp, np, mxUINT8_CLASS,mxREAL));   /*Give Recognized 4x4 line pieces' Slopes */
	yp_ext=  mxGetPr(mxCreateNumericMatrix( mp, np, mxUINT8_CLASS,mxREAL)); /* Give Recognized 4x4 line pieces will be extended*/
	b_in_ext=  mxGetPr(mxCreateNumericMatrix( m_in, n_in, mxUINT8_CLASS,mxREAL));  /* Give ImgEDGE will be extended*/
	
    b_in = mxGetPr(prhs[0]);   	 /* Get Input Binary Edge image */	
	b_in_H = mxGetPr(prhs[1]);   	 /* Chose H segmentation rule */
	
	b_in_seg = mxMalloc((65) * sizeof(bool));    /*Segmentation numbers */	
	
	if(( b_in_H[0]==0)||( b_in_H[0]==1)){
		b_in_seg = b_in_seg1;
	}
	else
	{
		b_in_seg = b_in_seg8;
	}
	
	
	
	dynamicData = mxMalloc((mp*np) * sizeof(unsigned  int));    /*Segmentation numbers */	
	dynamicData0 = mxMalloc((mp*np) * sizeof(unsigned  int));   /*X Coordinate Of Segmented 4x4 line pieces */	
	dynamicData1 = mxMalloc((mp*np) * sizeof(unsigned  int));   /*Y Coordinate Of Segmented 4x4 line pieces */	
	dynamicData_z = mxMalloc((mp*np) * sizeof(unsigned  int));  /*Corner Points */	
	dynamicData_s = mxMalloc((mp*np) * sizeof(unsigned  int));  /*Recognized 4x4 line pieces */	
	dynamicData_x = mxMalloc((mp*np) * sizeof(unsigned  int));  /*X Coordinate Of Recognized 4x4 line pieces */	
	dynamicData_y = mxMalloc((mp*np) * sizeof(unsigned  int));  /*Y Coordinate Of Recognized 4x4 line pieces */	
	dynamicData_es = mxMalloc((mp*np) * sizeof(unsigned  int)); /*Recognized 4x4 line pieces, the line Start&End Points */	
	dynamicData_es1 = mxMalloc((mp*np) * sizeof(unsigned  int)); /*Recognized 4x4 line pieces, the line Start&End Points */	
	  
	  
 	yp_sx_ext0= mxMalloc((mp*np) * sizeof(unsigned  int)); 
	yp_sy_ext0= mxMalloc((mp*np) * sizeof(unsigned  int)); 
	yp_ex_ext0= mxMalloc((mp*np) * sizeof(unsigned  int)); 
	yp_ey_ext0= mxMalloc((mp*np) * sizeof(unsigned  int)); 
	

		
    /* %%%%%%%%%%%%%%%% Segmentation 4x4 line pieces and Count the Lines  %%%%%%%%%%%%%%%% */	
	seg_and_count_lines(yp,yp_st,yp_end,yp_cnt,yp_cnt1,yp_xy, dynamicData,dynamicData0,dynamicData1,dynamicData_z,dynamicData_s,dynamicData_x,dynamicData_y,dynamicData_es1,dynamicData_es,yp_sx_ext0,yp_sy_ext0,yp_ex_ext0,yp_ey_ext0,yp_ext,mp,np, m_in,b_in,b_in_seg,b_in_H );
	
	m_no=yp_cnt[0]; /*Total segmented line numbers */
	m_no1=yp_cnt1[0]; /*Total segmented line numbers */
	
	yp_sx_ext =mxMalloc((m_no) * sizeof(unsigned  int));   /* Start Points X Coordinate of Segmented Lines */
	yp_sy_ext =mxMalloc((m_no) * sizeof(unsigned  int));    /* Start Points Y Coordinate of Segmented Lines */
	yp_ex_ext =mxMalloc((m_no) * sizeof(unsigned  int));   /* End Points X Coordinate of Segmented Lines */
	yp_ey_ext =mxMalloc((m_no) * sizeof(unsigned  int));   /* End Points X Coordinate of Segmented Lines */
	

	
	S_OUT_SX= mxCreateNumericMatrix( 1, m_no, mxUINT32_CLASS,mxREAL);  /* Start Points X Coordinate of Segmented Lines */
	S_OUT_SY= mxCreateNumericMatrix( 1, m_no, mxUINT32_CLASS,mxREAL);   /* Start Points Y Coordinate of Segmented Lines */
	S_OUT_EX= mxCreateNumericMatrix( 1, m_no, mxUINT32_CLASS,mxREAL);  /* End Points X Coordinate of Segmented Lines */
	S_OUT_EY= mxCreateNumericMatrix( 1, m_no, mxUINT32_CLASS,mxREAL);  /* End Points X Coordinate of Segmented Lines */
 
  
	
	
	yp_sx_ext1 = mxGetPr(S_OUT_SX);     /*  Get	Start Points X Coordinate of Segmented Lines */
	yp_sy_ext1 = mxGetPr(S_OUT_SY);      /* Get	 Start Points Y Coordinate of Segmented Lines */
	yp_ex_ext1 = mxGetPr(S_OUT_EX);     /*  Get	End Points X Coordinate of Segmented Lines */
	yp_ey_ext1 = mxGetPr(S_OUT_EY);     /*  Get	End Points X Coordinate of Segmented Lines */
	

	
	
	/* %%%%%%%%%%%%%%%% Total segmented line numbers with Start and End Points  %%%%%%%%%%%%%%%% */	
	segment_lines(yp_sx_ext,yp_sy_ext,yp_ex_ext,yp_ey_ext,dynamicData,dynamicData0,dynamicData1,yp_sx_ext0,yp_sy_ext0,yp_ex_ext0,yp_ey_ext0,m_no,m_no1,b_in_H);
 
 /* %%%%%%%%%%%%%%%% EXTENDED Total segmented line numbers with Start and End Points  %%%%%%%%%%%%%%%% */	
	/*extend_segment_lines(yp_sx_ext,yp_sy_ext,yp_ex_ext,yp_ey_ext,yp_sx_ext1,yp_sy_ext1,yp_ex_ext1,yp_ey_ext1,b_in_ext,yp_ext,b_in,mp, m_in,m_no);*/
	extend_segment_lines(yp_sx_ext,yp_sy_ext,yp_ex_ext,yp_ey_ext,yp_sx_ext1,yp_sy_ext1,yp_ex_ext1,yp_ey_ext1,b_in_ext,yp_ext,b_in,mp, m_in,m_no);
	
	mxFree(dynamicData);
	mxFree(dynamicData0);
	mxFree(dynamicData1);
	mxFree(dynamicData_s);
	mxFree(dynamicData_z);
	mxFree(dynamicData_x);
	mxFree(dynamicData_y);
	mxFree(dynamicData_es);
	mxFree(dynamicData_es1);
	mxFree(yp_sx_ext0);
	mxFree(yp_sy_ext0);
	mxFree(yp_ex_ext0);
	mxFree(yp_ey_ext0);
	mxFree(yp_sx_ext);
	mxFree(yp_sy_ext);
	mxFree(yp_ex_ext);
	mxFree(yp_ey_ext);
	mxFree(yp_cnt1);


    return;    
}
	
	

   
 
   

   
   
	
	
	