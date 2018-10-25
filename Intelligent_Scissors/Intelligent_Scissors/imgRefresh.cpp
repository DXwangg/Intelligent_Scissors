#include "imgRefresh.h"

extern Mat Source_image;
extern Mat gray_image;

extern list<CvPoint>path;
list<list<CvPoint>>Previous;
list<CvPoint>seed_points;

void refresh() {
	//���·��
	Mat tmp;
	Source_image.copyTo(tmp);

	for (auto a : Previous)
		for (auto i : a)
			circle(tmp, i, 1, Scalar(255, 0, 0));

	for (auto n : path)
		circle(tmp, n, 1, Scalar(0, 255, 0));

	for (auto s : seed_points)
		circle(tmp, s, 2, Scalar(0, 0, 255));
	//������ʾˢ��ͼƬ
	imshow(imagePath, tmp);
}

//����seed�����㣨��Ҫ���ģ�
void setSeedPoint(int x, int y) {
	CvPoint seed = cvPoint(x, y);
	//���Ѿ���seed��ʵʱ��ʾ��ǰ�����·��
	if (!seed_points.empty())
		getPath(seed_points.back(), seed);
	//seed����seed_points�㼯
	seed_points.push_back(seed);
	//seed֮������·������Previous·����
	Previous.push_back(path);

	double t = (double)getTickCount();
	// ���ظô�����ִ�����ĵ�ʱ�䣬��λΪ�룬��Ҫ�㷨����
	searchpath(seed_points.back());
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << t << "s" << endl;
}

//�����Ӧ
void on_mouse(int event, int x, int y, int flags, void * ustc)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		if (!seed_points.empty()) {
			getPath(seed_points.back(), cvPoint(x, y));
		}
	}
	else if (event == CV_EVENT_LBUTTONDOWN) {
		//�����������seed point
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
	//ÿ������¼��������»���
	refresh();
}
