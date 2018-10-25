#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "imgData.h"
#include "searchPath.h"
#include <list>
#include <iostream>

using namespace std;
using namespace cv;
extern const char* imagePath;

void refresh();

void on_mouse(int event, int x, int y, int flags, void* ustc);

void setSeedPoint(int x, int y);