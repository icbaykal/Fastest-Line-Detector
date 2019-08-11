#include <windows.h>  
#include <commctrl.h>
#include <chrono>
#include "message.h"
#include "Multiview.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>

using namespace cv;
extern Message msg;  //the message object

#define IDI_ICON1 101
#define IDC_OPEN				3000
#define IDC_PRO_HOUGH			4000
#define IDC_STD_HOUGH			4001


HINSTANCE hInst;   // current instance
HWND     hWnd;    //parent window
HWND mainbmp, linesbmp, edgebmp, mesgbox;
HWND realparent, edgeparent;
HWND ProbHoughstatic, ProbHoughEd1, ProbHoughEd2, ProbHoughEd3, ProbHoughEd4;
HWND StdHoughstatic, StdHoughEd1, StdHoughEd2, StdHoughEd3, StdHoughEd4;
HDC hDC;
HGLRC hRC;

LPCTSTR lpszAppName = "Probabilistic Hough";
LPCTSTR lpszTitle = "PROBABILISTIC HOUGH";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

char mes[64], tmes[32];
Mat edges;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      mesg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu, TransMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	TransMenu = CreatePopupMenu();

	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");
	AppendMenu(TransMenu, MF_STRING, IDC_PRO_HOUGH, "Prob. Hough Tr.");
	AppendMenu(TransMenu, MF_STRING, IDC_STD_HOUGH, "Stand. Hough Tr.");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)TransMenu, "TRANSFORM");



	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		80, 0,
		1400, 1080,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);
	//Initialize Common Controls  




	if (!hWnd)
		return(FALSE);
	//---------------------------------
	mainbmp = CreateWindow(WC_STATIC, "", WS_CHILD | WS_VISIBLE | SS_BITMAP,
		10, 0, 640, 520, hWnd, NULL, hInst, NULL);
	cv::namedWindow("My Window", 1);//,CV_WINDOW_AUTOSIZE);
	HWND hWnd2 = (HWND)cvGetWindowHandle("My Window");
	realparent = GetParent(hWnd2);
	::SetParent(hWnd2, mainbmp);

	/*edgebmp = CreateWindow(WC_STATIC, "", WS_CHILD | WS_VISIBLE | SS_BITMAP,
		10, 515, 640, 520, hWnd, NULL, hInst, NULL);
	cv::namedWindow("Edge Window", 1);//,CV_WINDOW_AUTOSIZE);
	HWND hWnd3 = (HWND)cvGetWindowHandle("Edge Window");
	edgeparent = GetParent(hWnd3);
	::SetParent(hWnd3, edgebmp);*/
	//-----------------------------

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&mesg, NULL, 0, 0))
	{
		TranslateMessage(&mesg);
		DispatchMessage(&mesg);
	}

	return(mesg.wParam);
}


int FillBMPInf(BITMAPINFO *info, int x, int y, int bits)
{
	info->bmiHeader.biBitCount = bits;
	info->bmiHeader.biClrImportant = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biHeight = -y;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biSize = sizeof(BITMAPINFO);
	info->bmiHeader.biSizeImage = (x*y*bits) / 8;
	if (info->bmiHeader.biSizeImage < 0)
		info->bmiHeader.biSizeImage = -info->bmiHeader.biSizeImage;
	info->bmiHeader.biWidth = x;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	return(0);
}
class STMImage
{
public:
	HBITMAP HBMRGB;
	BITMAPINFO info;
	unsigned char *buf;
	unsigned char *RGB;
	unsigned char *pieces;
	int *seg;
	int width;
	int height;
	unsigned char *piecetable;
	int noofsegments;
	int validsegments;
	STMImage() { buf = NULL; RGB = NULL; pieces = NULL;piecetable = NULL; }
	void freebuffers() { free(buf); free(RGB);free(pieces);free(piecetable);free(seg); }
	~STMImage() { freebuffers(); }
};

int Line(STMImage &i, int xstart, int ystart, int xend, int yend, int color)
{
	unsigned int *RGB = (unsigned int *)i.RGB;
	double slope, dy;
	int x, y, temp;
	if (xstart < 4)return(-1);if (xstart >= i.width - 4)return(-1);
	if (xend < 4)return(-1);if (xend >= i.width - 4)return(-1);
	if (ystart < 4)return(-1);if (ystart >= i.height - 4)return(-1);
	if (yend < 4)return(-1);if (yend >= i.height - 4)return(-1);
	if (abs(xend - xstart) > abs(yend - ystart))
	{

		slope = (double)(yend - ystart);
		slope = slope / (double)(xend - xstart);
		dy = 0.0;
		y = ystart;
		for (x = xstart;x <= xend;x++)
		{
			//y = ystart +  ((double)x*slope + 0.5);
			RGB[y*i.width + x] = color;
			//RGB[(y + 1)*i.width + x] = color;
			//RGB[(y + 2)*i.width + x] = color;
			dy += ((double)x*slope + 0.5);
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
		for (y = ystart;y <= yend;y++)
		{
			x = xstart + ((double)(y - ystart)*slope + 0.5);
			RGB[y*i.width + x] = color;
			//RGB[y*i.width + x + 1] = color;
			//RGB[y*i.width + x + 2] = color;
		}
	}
	return(0);
}

STMImage i;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
	{

		linesbmp = CreateWindow(WC_STATIC, "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME,
			660, 350, 640, 480, hWnd, NULL, hInst, NULL);
		mesgbox = CreateWindow(WC_EDIT, "", WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | ES_MULTILINE,
			660, 53, 450, 140, hWnd, NULL, hInst, NULL);

		ProbHoughstatic = CreateWindow(WC_STATIC, "Pr. Probabilistic H. Transform Parameters:", WS_CHILD | WS_VISIBLE,
			660, 3, 300, 20, hWnd, NULL, hInst, NULL);
		ProbHoughEd1 = CreateWindow(WC_EDIT, "180.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			660, 33, 55, 20, hWnd, NULL, hInst, NULL);
		ProbHoughEd2 = CreateWindow(WC_EDIT, "20", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			720, 33, 55, 20, hWnd, NULL, hInst, NULL);
		ProbHoughEd3 = CreateWindow(WC_EDIT, "10.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			780, 33, 55, 20, hWnd, NULL, hInst, NULL);
		ProbHoughEd4 = CreateWindow(WC_EDIT, "10.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			840, 33, 55, 20, hWnd, NULL, hInst, NULL);

		/*StdHoughstatic = CreateWindow(WC_STATIC, "Standard H. Transform Parameters:", WS_CHILD | WS_VISIBLE,
			660, 83, 300, 20, hWnd, NULL, hInst, NULL);
		StdHoughEd1 = CreateWindow(WC_EDIT, "180.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			660, 113, 55, 20, hWnd, NULL, hInst, NULL);
		StdHoughEd2 = CreateWindow(WC_EDIT, "20", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			720, 113, 55, 20, hWnd, NULL, hInst, NULL);
		StdHoughEd3 = CreateWindow(WC_EDIT, "10.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			780, 113, 55, 20, hWnd, NULL, hInst, NULL);
		StdHoughEd4 = CreateWindow(WC_EDIT, "10.0", WS_CHILD | WS_VISIBLE | WS_DLGFRAME,
			840, 113, 55, 20, hWnd, NULL, hInst, NULL);*/


		msg.SetWindow(mesgbox);
		msg.Post("!!! This program processes only edge detected BINARY images!!!\n");
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OPEN:
		{
			OPENFILENAME ofn;
			msg.Post("!!! This program processes only edge detected BINARY images!!!\n");
			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);

			Mat image;
			Mat del(512, 640, CV_8U);
			del = 128;
			image = cv::imread(szFile, 0);//if 0 grayscale
			if (image.rows < 10) break;
			sprintf_s(mes, 64, "image resolution:%d x %d ", image.rows, image.cols);
			SetWindowText(mesgbox, mes);

			cv::imshow("My Window", del);
			//cv::imshow("Edge Window", del);
			//ShowWindow(realparent, SW_HIDE);
			UpdateWindow(hWnd);
			cv::imshow("My Window", image);
			ShowWindow(realparent, SW_HIDE);

			//GaussianBlur(image, image, Size(7, 7), 5.0, 5.0);
			//Canny(image, edges, 0, 50, 3);
			edges = image;
			//cv::imshow("Edge Window", edges);
			//ShowWindow(edgeparent, SW_HIDE);

			//-------------------------------------------------------------
			HDC lineDC = GetDC(linesbmp);
			i.width = image.cols;
			i.height = image.rows;
			FillBMPInf(&i.info, i.width, i.height, 32);
			i.HBMRGB = CreateDIBSection(NULL, &i.info, DIB_RGB_COLORS, (void **)&i.RGB, 0, 0);
			//--------------------------------

		}
		break;
		case IDC_PRO_HOUGH:
		{
			double duration;
			chrono::duration<double> elapsed;
			char ct[10];
			//,ProbHoughEd1, ProbHoughEd2, ProbHoughEd3, ProbHoughEd4;
			GetWindowTextA(ProbHoughEd1, ct, 10);
			double theta = atof(ct);
			GetWindowTextA(ProbHoughEd2, ct, 10);
			int thresh = atoi(ct);
			GetWindowTextA(ProbHoughEd3, ct, 10);
			double minLinLen = atof(ct);
			GetWindowTextA(ProbHoughEd4, ct, 10);
			double maxLinLen = atof(ct);

			vector<Vec4i> lines;

			
			auto difstart = chrono::high_resolution_clock::now();
			for (int repeat = 0; repeat < 200; repeat++)
			{
				HoughLinesP(edges, lines, 1.0, CV_PI / theta, thresh, minLinLen, maxLinLen);
			}
			auto difend = chrono::high_resolution_clock::now();
			
			memset(i.RGB, 0, i.width * i.height * 4);
			for (size_t j = 0; j < lines.size(); j++)
			{
				Vec4i l = lines[j];
				//line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
				//msg.Post("[%d %d] -", l[0], l[1]);
				// msg.Post("[%d %d] \N", l[2], l[3]);
				Line(i, l[0], l[1], l[2], l[3], 0xffffffff);
			}
			
			msg.Post("Number of Lines: %d\n", (int)lines.size());
			
			SendMessage(linesbmp, STM_SETIMAGE, 0, (LPARAM)i.HBMRGB);
			elapsed = difend - difstart;
			duration = elapsed.count();
			msg.Post("%f sec. elapsed for 200 iterations\n", duration);
		}
		break;
		case IDC_STD_HOUGH:
		{
			Mat cdst;
			vector <Vec2f> lines;			
			HoughLines(edges,lines, 1.0, CV_PI / 180, 80,0,0);
			cvtColor(edges, cdst, CV_GRAY2BGR);
			memset(i.RGB, 0, i.width * i.height * 4);
			msg.Post("noof lines: %d\n", (int)lines.size());
			
			for (size_t ii = 0; ii < lines.size(); ii++)
			{
				float rho = lines[ii][0], theta = lines[ii][1];
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				pt1.x = cvRound(x0 + 1000 * (-b));
				pt1.y = cvRound(y0 + 1000 * (a));
				pt2.x = cvRound(x0 - 1000 * (-b));
				pt2.y = cvRound(y0 - 1000 * (a));				
				line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);				
			}
			itoa(lines.size(), tmes, 10);
			strcpy_s(mes, 64, "Number of Lines: ");
			strcat_s(mes, 64, tmes);
			SetWindowText(mesgbox, mes);
			cv::imshow("Edge Window", cdst);
		}

			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:

		i.~STMImage();
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}
