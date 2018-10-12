#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "imgData.h"
#include "imgRefresh.h"
#include <assert.h>
#include <ctime>  
#include <cmath>
#include <list>
#include <iostream>
using namespace std;
using namespace cv;
const double PI = acos(-1);


int max_V3(Vec3i t);//最大像素点 求Gmax

void Canny_Edge_detection(Mat &source, Mat &dst);//canny算子处理图像

void SobleFilter(Mat &source, Mat &dst);//sobel 算子

double local_cost(imgNode *p, imgNode *q);//计算成本

void searchpath(CvPoint start_point);//实现论文寻路算法

void getPath(CvPoint start_point, CvPoint end_point);//获取路径

void normalize(Vec2d &dst);//向量归一化处理

void makeimg();//图像处理

