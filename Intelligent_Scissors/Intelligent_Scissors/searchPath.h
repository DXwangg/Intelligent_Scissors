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


int max_V3(Vec3i t);//������ص� ��Gmax

void Canny_Edge_detection(Mat &source, Mat &dst);//canny���Ӵ���ͼ��

void SobleFilter(Mat &source, Mat &dst);//sobel ����

double local_cost(imgNode *p, imgNode *q);//����ɱ�

void searchpath(CvPoint start_point);//ʵ������Ѱ·�㷨

void getPath(CvPoint start_point, CvPoint end_point);//��ȡ·��

void normalize(Vec2d &dst);//������һ������

void makeimg();//ͼ����

