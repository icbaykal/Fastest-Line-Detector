/*
HELPER FUNCTIONS FOR THE GUI
Written by Dr. Ibrahim Cem Baykal
*/
#include <windows.h>
#include <gdiplus.h>
#include <cstdio>
#include "Basic_image.h"
using namespace Gdiplus;
#define _CRT_NON_CONFORMING_SWPRINTFS
Message msg;

int FillBMPInfo(BITMAPINFO *info, int x, int y,int bits)
{
	info->bmiHeader.biBitCount = bits;
	info->bmiHeader.biClrImportant = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biHeight = y;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biSize = sizeof(BITMAPINFO);
	info->bmiHeader.biSizeImage = x*y * bits/8;
	info->bmiHeader.biWidth = x;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	return(0);
}

int CreateImage(BIMG &img, int width, int height, int bits)
{
	img.bufsize = (bits*width*height) / 8 ;
	img.buffer = (unsigned char*)calloc(img.bufsize, 1);
	img.width = width;	
	img.height = height;
	img.bits_per_pixel = bits;
	BITMAPINFO binfo;
	FillBMPInfo(&binfo, img.width, img.height,img.bits_per_pixel);
	img.HBitmap = CreateDIBSection(NULL, &binfo, DIB_RGB_COLORS, (void **)&img.buffer, NULL, NULL);
	return(0);
}

//this method always brings 32 bit images
int LoadImage(wchar_t *wcpath, BIMG &img)
{
	BITMAP bm;
	HBITMAP HBitmap;
	Bitmap Image1(wcpath);//loading the bitmap file while creating Bitmap object
	Image1.GetHBITMAP(0, &HBitmap);//extracting handle
	GetObject(HBitmap, sizeof(bm), &bm);	//extracting BITMAP structure
	CreateImage(img, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);	//creating our own bitmap using size and bits per pixel
	memcpy(img.buffer, bm.bmBits, img.bufsize); //copying image data to our own bitmap
	DeleteObject(HBitmap);
	return(0);
}

int DLine(BIMG &i, int xstart, int ystart, int xend, int yend, int color)
{
	unsigned int *RGB = (unsigned int *)i.buffer;
	double slope, dy;
	int x, y, temp;
	if (xstart < 0)xstart=0;if (xstart >= i.width)xstart= i.width-1;
	if (xend < 0)xend=0;if (xend >= i.width) xend = i.width-1;
	if (ystart < 0)ystart = 0;if (ystart >= i.height)ystart = i.height-1;
	if (yend<0)yend=0;if (yend >= i.height)yend = i.height - 1;
	if (abs(xend - xstart) > abs(yend - ystart))
	{		
		slope = (double)(yend - ystart);
		slope = slope / (double)(xend - xstart);
		dy = 0.0;
		
		if (xend < xstart)
		{
			temp = yend;
			yend = ystart;
			ystart = temp;
			temp = xend;
			xend = xstart;
			xstart = temp;
		}
		y = ystart;
		for (x = xstart;x <= xend;x++)
		{
			RGB[y*i.width + x] = color;			
			y = ystart + ((double)(x - xstart)*slope + 0.5);
		}
	}
	else
	{
		slope = (double)(xend - xstart) / (double)(yend - ystart);
		if (yend < ystart)
		{
			temp = yend;
			yend = ystart;
			ystart = temp;
			temp = xend;
			xend = xstart;
			xstart = temp;
		}
		x = xstart;
		for (y = ystart;y <= yend;y++)
		{
			
			RGB[y*i.width + x] = color;
			x = xstart + ((double)(y - ystart)*slope + 0.5);
		}
	}
	return(0);
}

//--------------------MESSAGE ----------------------
void Message::SetWindow(HWND c) { Editwin = c; return; }
Message::Message() { messagebuf[0] = 0; fprecision = 3; }
void Message::Clear() {
	memset(messagebuf, 0, 2048); SendMessage(Editwin, EM_SETSEL, (WPARAM)0,
		(LPARAM)-1); SendMessage(Editwin, EM_REPLACESEL, 0, (LPARAM)messagebuf);
	UpdateWindow(Editwin);
}//SetWindowText(Editwin,NULL);}
int Message::Post(char *ch)
{
	int textlen = GetWindowTextLength(Editwin);
	int l, sz = (int)strlen(ch) + 1;
	if (sz>MESG_BUF_SIZE)sz = MESG_BUF_SIZE;
	l = 0;
	for (int x = 0; x<sz; x++)
	{
		if (ch[x] != 0xa) wchbuf[l] = ch[x];
		else
		{
			wchbuf[l] = 0xd;
			l++;
			wchbuf[l] = 0xa;
		}
		l++;
	}
	SendMessageW(Editwin, EM_SETSEL, (WPARAM)textlen, (LPARAM)textlen);
	SendMessageW(Editwin, EM_REPLACESEL, 0, (LPARAM)wchbuf);
	messagebuf[0] = 0; wchbuf[0] = 0;
	return(1);
}
int Message::Post(wchar_t *wch)
{
	int textlen = GetWindowTextLength(Editwin);
	SendMessage(Editwin, EM_SETSEL, (WPARAM)textlen, (LPARAM)textlen);
	SendMessage(Editwin, EM_REPLACESEL, 0, (LPARAM)wch);
	messagebuf[0] = 0;
	return(1);
}
// ------------- INT  ----------------
int Message::Post(int i)
{
	sprintf(messagebuf, "%d", i); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, int i)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, i); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, int i, int j)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, i, j); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, int i, int j, int k)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, i, j, k); Post(messagebuf);
	return(1);
}
//----------------- FLOAT -----------------
int Message::Post(float f)
{
	sprintf(messagebuf, "%f", f); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, float f)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, f); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, float f, float g)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, f, g); Post(messagebuf);
	return(1);
}
//--------------------DOUBLE -----------------
int Message::Post(double d)
{
	sprintf(messagebuf, "%e", d); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, double d)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, d); Post(messagebuf);
	return(1);
}
int Message::Post(const char *ch, double d, double e)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, d, e); Post(messagebuf);
	return(1);
}
//-----------------String --------------------
int Message::Post(const char *ch, char *d)
{
	messagebuf[0] = 0; sprintf(messagebuf, ch, d); Post(messagebuf);
	return(1);
}