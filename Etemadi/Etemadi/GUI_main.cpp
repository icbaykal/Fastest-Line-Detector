/*
  ETEMADI LINE DETECTOR'S TEST GUI
  Written by Dr. Ibrahim Cem Baykal
*/

#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <tchar.h>
#include <chrono>
#include "Basic_image.h"
#include "chainpix.h"
#include "FEX.h"
#include "Liste.h"
using namespace std;
using namespace Gdiplus;

extern Message msg;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define IDC_BTN1           1011
#define IDC_BTN2           1012
#define IDC_BTN3           1013
#define IDC_BTN4           1014
#define IDC_BTN5           1015
#define IDC_BTN6           1016
#define IDC_BTN7           1017
#define IDC_BTN8           1018
#define IDC_BTN9           1019
#define IDC_BTN10          1020
/*  Make the class name into a global variable  */
char szClassName[] = "WindowsApp";
HWND hwnd;
HWND HEdit1, HEdit2, HEdit3, HEdit4, mainbmp, secondbmp;
HWND HButton1, HButton2, HButton3, HButton4, HButton5, HButton6;
HWND HButton7, HButton8, HButton9, HButton10;
HINSTANCE hInst;   // current instance
ULONG_PTR           gdiplusToken;

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nFunsterStil)

{
	/* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default color as the background of the window */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		"ETEMADI TEST",       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		1300,                 /* The programs width */
		740,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);
	GdiplusStartupInput gdiplusStartupInput;


	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	/* Make the window visible on the screen */
	ShowWindow(hwnd, nFunsterStil);
	hInst = hThisInstance;

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	/* The program return-value is 0 - The value that PostQuitMessage() gave */
	return messages.wParam;
}


BIMG img1, img2;

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmEvent;
	switch (message)                  /* handle the messages */
	{
	case WM_DESTROY:
		GdiplusShutdown(gdiplusToken);
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	case WM_CREATE:
	{
		
		HEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 910, 400, 365, 265, hwnd, NULL, hInst, NULL);
		
		mainbmp = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 440, 440, hwnd, NULL, hInst, NULL);
		

		HButton1 = CreateWindow("BUTTON", "OPEN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 2, 0, 80, 22, hwnd, (HMENU)IDC_BTN1, hInst, NULL);
		HButton2 = CreateWindow("BUTTON", "ETEMADI ORIGINAL", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 82, 0, 150, 22, hwnd, (HMENU)IDC_BTN2, hInst, NULL);
		HButton3 = CreateWindow("BUTTON", "ETEMADI MIN 7 PIXEL LIMIT", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 234, 0, 210, 22, hwnd, (HMENU)IDC_BTN3, hInst, NULL);
		
		msg.SetWindow(HEdit3);

	}
	break;
	case WM_COMMAND:
		wmEvent = HIWORD(wParam);
		switch (LOWORD(wParam))
		{
		case IDC_BTN1://OPEN
		{
			wchar_t  wc[256];
			OPENFILENAMEW ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = wc;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wc);
			ofn.lpstrFilter =NULL;//this part needs further impr
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileNameW(&ofn);
			LoadImage(wc, img1);
			SendMessage(mainbmp, STM_SETIMAGE, 0, (LPARAM)img1.HBitmap);
			msg.Post("%dx %d, ", img1.width, img1.height);
			msg.Post("%d bits per pixel\n", img1.bits_per_pixel);

		}
		break;
		case IDC_BTN2://8 bit
		{
			double duration;
			chrono::duration<double> elapsed;
			int imgwidth, imgheight;
			Liste LineList;
			msg.Post("Etemadi benchmark started...\n");
			auto difstart = chrono::high_resolution_clock::now();
			for (int repeat = 0; repeat < 200; repeat++)
			{
				convert_chainpix(img1);
				Liste StringListID[5000];
				for (int i = 0; i < 5000; i++) StringListID[i] = CreatList();
				ORTPoint *StringList;
				int segno = link_and_save(0, StringListID);
				//msg.Post("Number of segments: %d\n", segno);
				imgwidth = img1.width; imgheight = img1.height;


				FEX(LineList, segno, StringListID, imgwidth, imgheight);
				for (int i = 0; i < 5000; i++) DestList(&StringListID[i]);
			}
			auto difend = chrono::high_resolution_clock::now();
			msg.Post("finished.\n");
			elapsed = difend - difstart;
			duration = elapsed.count();
			msg.Post("%f sec. elapsed for 200 iterations\n", duration);
			int LineCounter = (int)SizeList(LineList);
			msg.Post("%d lines detected\n", LineCounter);
			//msg.Post("noof lines : %d\n", LineCounter);
			//long Buffer = FirstElmList(*LineList);
			CreateImage(img2, img1.width, img1.height, 32);
			ORTLine *Line = (ORTLine *)FirstElmList(LineList);
			DLine(img2, Line->Start.Col + imgwidth / 2 - 1, Line->Start.Row + imgheight / 2 - 1, Line->End.Col + imgwidth / 2 - 1, Line->End.Row + imgheight / 2 - 1, 0x00ffffff);
			for (int i = 2; i <= LineCounter; i++)
			{
				Line = (ORTLine *)NextElmList(LineList);
				//if (((int)Line->End.Col + imgwidth / 2 - 1) < 3)msg.Post("###  %d\n", ((int)Line->End.Col + imgwidth / 2 - 1));
				DLine(img2, (int)Line->Start.Col + imgwidth / 2 - 1, (int)Line->Start.Row + imgheight / 2 - 1, (int)Line->End.Col + imgwidth / 2 - 1, (int)Line->End.Row + imgheight / 2 - 1, 0x00ffffff);
			}
			DestList(&LineList);
			SendMessage(mainbmp, STM_SETIMAGE, 0, (LPARAM)img2.HBitmap);
			
		}
			break;
		case IDC_BTN3://MIN 7 PIXEL LIMIT
		{
			double duration;
			chrono::duration<double> elapsed;
			int imgwidth, imgheight;
			Liste LineList;
			msg.Post("Etemadi 7 pixel limit benchmark started...\n");
			auto difstart = chrono::high_resolution_clock::now();
			for (int repeat = 0; repeat < 200; repeat++)
			{
				convert_chainpix(img1);
				Liste StringListID[5000];
				for (int i = 0; i < 5000; i++) StringListID[i] = CreatList();
				ORTPoint *StringList;
				int segno = link_and_save(0, StringListID);
				//msg.Post("Number of segments: %d\n", segno);
				imgwidth = img1.width; imgheight = img1.height;
				FEX_Modified(LineList, segno, StringListID, imgwidth, imgheight);
				for (int i = 0; i < 5000; i++) DestList(&StringListID[i]);
			}
			auto difend = chrono::high_resolution_clock::now();
			msg.Post("finished.\n");
			elapsed = difend - difstart;
			duration = elapsed.count();
			msg.Post("%f sec. elapsed for 200 iterations\n", duration);
			int LineCounter = (int)SizeList(LineList);
			msg.Post("%d lines detected\n", LineCounter);
			//msg.Post("noof lines : %d\n", LineCounter);
			//long Buffer = FirstElmList(*LineList);
			CreateImage(img2, img1.width, img1.height, 32);
			ORTLine *Line = (ORTLine *)FirstElmList(LineList);
			DLine(img2, Line->Start.Col + imgwidth / 2 - 1, Line->Start.Row + imgheight / 2 - 1, Line->End.Col + imgwidth / 2 - 1, Line->End.Row + imgheight / 2 - 1, 0x00ffffff);
			for (int i = 2; i <= LineCounter; i++)
			{
				Line = (ORTLine *)NextElmList(LineList);
				//if (((int)Line->End.Col + imgwidth / 2 - 1) < 3)msg.Post("###  %d\n", ((int)Line->End.Col + imgwidth / 2 - 1));
				DLine(img2, (int)Line->Start.Col + imgwidth / 2 - 1, (int)Line->Start.Row + imgheight / 2 - 1, (int)Line->End.Col + imgwidth / 2 - 1, (int)Line->End.Row + imgheight / 2 - 1, 0x00ffffff);
			}
			DestList(&LineList);
			SendMessage(mainbmp, STM_SETIMAGE, 0, (LPARAM)img2.HBitmap);
		}
		break;
		case IDC_BTN4://Load JPG
		{

		}

		}
		break;
		case IDC_BTN5:
		{

		}
		break;
		case IDC_BTN6://Convert Gray 8
		{


		}
		break;
		case IDC_BTN7:
		{

		}
		break;
		case IDC_BTN8:
		{

		}
		break;
		case IDC_BTN9:
		{


		}
		case IDC_BTN10:
		{

		}
		break;
		
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}


