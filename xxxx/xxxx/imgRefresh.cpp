#include "imgRefresh.h"

extern Mat Source_image;
extern Mat gray_image;

extern list<CvPoint>path;
list<list<CvPoint>>Previous;
list<CvPoint>seed_points;

void refresh() {
	//填充路径
	Mat tmp;
	Source_image.copyTo(tmp);

	for (auto a : Previous)
		for (auto i : a)
			circle(tmp, i, 1, Scalar(255, 0, 0));

	for (auto n : path)
		circle(tmp, n, 1, Scalar(0, 255, 0));

	for (auto s : seed_points)
		circle(tmp, s, 2, Scalar(0, 0, 255));
	//重新显示刷新图片
	imshow(imagePath, tmp);
}

//设置seed并计算（主要消耗）
void setSeedPoint(int x, int y) {
	CvPoint seed = cvPoint(x, y);
	//若已经有seed则实时显示当前的最短路径
	if (!seed_points.empty())
		getPath(seed_points.back(), seed);
	//seed放入seed_points点集
	seed_points.push_back(seed);
	//seed之间的最短路径放入Previous路径集
	Previous.push_back(path);

	double t = (double)getTickCount();
	// 返回该处代码执行所耗的时间，单位为秒，主要算法消耗
	searchpath(seed_points.back());
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << t << "s" << endl;
}

//鼠标响应
void on_mouse(int event, int x, int y, int flags, void * ustc)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		if (!seed_points.empty()) {
			getPath(seed_points.back(), cvPoint(x, y));
		}
	}
	else if (event == CV_EVENT_LBUTTONDOWN) {
		//左键单击设置seed point
		setSeedPoint(x, y);
	}
	else if (event == CV_EVENT_RBUTTONDOWN) {
		if (!seed_points.empty()) {
			seed_points.pop_back();
			Previous.pop_back();
			path.clear();
			if (!seed_points.empty()) {
				searchpath(seed_points.back());
			}
		}
	}
	//每次鼠标事件都将更新画面
	refresh();
}
