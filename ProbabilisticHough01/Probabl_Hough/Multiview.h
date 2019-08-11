#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int compute3DStructure(Mat &p, Mat &q, Mat &R, Mat &T, Mat &XP);
int Multiview(Mat &Khat, Mat &xn1, Mat &xn2, Mat &Reslt);