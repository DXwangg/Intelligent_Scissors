#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "imgRefresh.h"
#include "imgData.h"
#include "searchPath.h"

using namespace cv;
using namespace std;

Mat Source_image;
const char* imagePath = "lena.jpg";
int main()
{
	Source_image = imread(imagePath, CV_LOAD_IMAGE_COLOR);
	//开始处理图像
	makeimg();
	waitKey(0);
	return 0;

}