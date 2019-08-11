/*
HELPER FUNCTIONS FOR THE GUI
Written by Dr. Ibrahim Cem Baykal
*/
#pragma once
#include <windows.h>


//Basic image class. Holds the information about the image and the pixel data.
class BIMG
{
public:
	int width, height, bits_per_pixel; 
	unsigned char *buffer;//this one holds the raw data
	int bufsize;
	HBITMAP HBitmap; //bitmap handle
	BIMG() { buffer = NULL; }
	~BIMG() { DeleteObject(HBitmap); }
};

int CreateImage(BIMG &img, int width, int height, int bits);
int LoadImage(wchar_t *wcpath, BIMG &img);
int DLine(BIMG &i, int xstart, int ystart, int xend, int yend, int color);

//------------MESSAGE ----------------
#define MESG_BUF_SIZE	2048
class Message
{
	char messagebuf[MESG_BUF_SIZE];
	wchar_t wchbuf[MESG_BUF_SIZE];
	int result;
	int fprecision;
	HWND Editwin;
public:
	virtual ~Message() {}
	Message();
	void SetWindow(HWND c);
	int Post(char *ch);
	int Post(wchar_t *wch);
	int Post(int i);
	int Post(const char *ch, int i);
	int Post(const char *ch, int i, int j);
	int Post(const char *ch, int i, int j, int k);
	int Post(float f);
	int Post(const char *ch, float f);
	int Post(const char *ch, float f, float g);
	int Post(double d);
	int Post(const char *ch, double d);
	int Post(const char *ch, double d, double e);
	int Post(const char *ch, char *d);
	void Clear();
};