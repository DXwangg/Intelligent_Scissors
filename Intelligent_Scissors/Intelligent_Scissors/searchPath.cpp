#include "searchPath.h"

extern Mat Source_image;
extern double maxG;
extern Mat GX_gradient;
extern Mat GY_gradient;

Mat gray_image;
Mat gradient_magnitude_image;
Mat canny_image;
imgNode **graph;
list<CvPoint>path;

//Ȩ��
double Wz = 0.43;
double Wd = 0.43;
double Wg = 0.13  ;

//canny�����ߵ���ֵ
#define HIGH 50
#define LOW 20

Mat GX_gradient;
Mat GY_gradient;
double maxG;

void SobleFilter(Mat &source, Mat &dst) {
	dst.copyTo(GX_gradient);
	dst.copyTo(GY_gradient);

	maxG = 0;
	Vec3i GX_3V, GY_3V;
	double GX, GY, GXY;
	for (int y = 1; y < source.rows - 1; y++)
		for (int x = 1; x < source.cols - 1; x++) {
			/*	GX = {   -1 0 1
						 -2 0 2
						 -1 0 1 };

				GY = {   -1-2-1
						  0 0 0
						  1 2 1 };*/
			GX_3V = (Vec3i)source.at<Vec3b>(y + 1, x + 1) - (Vec3i)source.at<Vec3b>(y + 1, x - 1)
				+ 2 * (Vec3i)source.at<Vec3b>(y, x + 1) - 2 * (Vec3i)source.at<Vec3b>(y, x - 1)
				+ (Vec3i)source.at<Vec3b>(y - 1, x + 1) - (Vec3i)source.at<Vec3b>(y - 1, x - 1);

			GY_3V = (Vec3i)source.at<Vec3b>(y + 1, x + 1) - (Vec3i)source.at<Vec3b>(y - 1, x + 1)
				+ 2 * (Vec3i)source.at<Vec3b>(y + 1, x) - 2 * (Vec3i)source.at<Vec3b>(y - 1, x)
				+ (Vec3i)source.at<Vec3b>(y + 1, x - 1) - (Vec3i)source.at<Vec3b>(y - 1, x - 1);

			GX = max_V3(GX_3V);
			GY = max_V3(GY_3V);
			GXY = sqrt(pow(GX, 2) + pow(GY, 2));
			maxG = max(maxG, GXY);

			dst.at<uchar>(y, x) = GXY;
			GX_gradient.at<uchar>(y, x) = GX;
			GY_gradient.at<uchar>(y, x) = GY;
		}

	namedWindow("gradient_magnitude_image", CV_WINDOW_AUTOSIZE);
	imshow("gradient_magnitude_image", dst);
}

void Canny_Edge_detection(Mat &source, Mat &dst) {

	//��˹ƽ������
	GaussianBlur(source, dst, Size(5, 5), 0, 0);
	
	Canny(dst, dst, LOW, HIGH, 3);
	imshow("canny_image", dst);
}

int max_V3(Vec3i t)
{
	int tempmax = 0;
	for (int i = 0; i < 3; i++)
		tempmax = max(tempmax, abs(t[i]));

	return tempmax;
}

void makeimg() {

	cvtColor(Source_image, gray_image, CV_BGR2GRAY);

	//ȷ����ͬ��ͼƬ����ͨ����
	gray_image.copyTo(gradient_magnitude_image);
	gray_image.copyTo(canny_image);

	//�����ݶȺͱ�Ե
	SobleFilter(Source_image, gradient_magnitude_image);
	Canny_Edge_detection(gray_image, canny_image);

	namedWindow(imagePath, CV_WINDOW_AUTOSIZE);
	imshow(imagePath, Source_image);

	//������ͼ����ͬ��С��imgNode���ݽṹ
	graph = new imgNode*[gray_image.rows];
	for (int i = 0; i < gray_image.rows; i++) {
		graph[i] = new imgNode[gray_image.cols];
	}

	//��ʼ������
	for (int i = 0; i < gray_image.rows; i++)
		for (int j = 0; j < gray_image.cols; j++) {
			graph[i][j].y = i;
			graph[i][j].x = j;
		}

	cvSetMouseCallback(imagePath, on_mouse, 0);
}


void getPath(CvPoint start_point, CvPoint end_point) {
	path.clear();
	imgNode* free_point = &graph[end_point.y][end_point.x];
	imgNode* s = &graph[start_point.y][start_point.x];
	//����back_pointerָ·
	while ((free_point != s) && free_point->back_pointer != nullptr) {
		path.push_back(cvPoint(free_point->x, free_point->y));
		free_point = free_point->back_pointer;
	}
}

void searchpath(CvPoint start_point) {

	double gtmp;
	imgNode * p;
	PathList L;
	imgNode * s = &graph[start_point.y][start_point.x];
	//��ʼ��graph
	for (int i = 0; i < gray_image.rows; i++)
		for (int j = 0; j < gray_image.cols; j++) {
			graph[i][j].visit = false;
			graph[i][j].cost = 0;
			graph[i][j].inList = false;
			graph[i][j].next = nullptr;
			graph[i][j].back_pointer = nullptr;
		}
	L.insert(s);
	while (!L.isEmpty()) {
		//ʵ���˱�����������
		p = L.head;//����
		L.remove(p);//�Ƴ������������
		graph[p->y][p->x].visit = true;//����ѷ���
		//������Χ8���ھ�����
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++) {
				if (p->x + j < 0 || p->x + j > gray_image.cols - 1 ||
					p->y + i < 0 || p->y + i > gray_image.rows - 1)
					continue;//��ֹԽ��

				if (graph[p->y + i][p->x + j].visit)//�����Ѿ������
					continue;
				imgNode * r = &graph[p->y + i][p->x + j];

				//(abs(i)+abs(j) == 2?sqrt(2):1) �öԽ���Ԫ�س��ϸ���2
				gtmp = graph[p->y][p->x].cost + (abs(i) + abs(j) == 2 ? sqrt(2) : 1)*local_cost(p, r);
				if (gtmp < graph[p->y + i][p->x + j].cost
					&&r->inList) {
					L.remove(r);
					r->inList = false;
					r->next = nullptr;
				}
				if (r->inList == false) {//�ھӲ���L��
					graph[p->y + i][p->x + j].cost = gtmp;
					r->back_pointer = p;//ָ����������
					L.insert(r);
					r->inList = true;
				}
			}
	}

}

//�ݶ���ֵ 
double fG(imgNode *p) {
	return 1 - gradient_magnitude_image.at<uchar>(p->y, p->x) / maxG;
}

//������˹�����ֵ
double fZ(imgNode *p) {
	double result = 1 - canny_image.at<uchar>(p->y, p->x) / 255;
	return result;
}

//�ݶȷ���ֵ
double fD(imgNode *p, imgNode *q) {
	//xy�ݶ�
	uchar IX, IY;
	//p
	IX = GX_gradient.at<uchar>(p->y, p->x);
	IY = GY_gradient.at<uchar>(p->y, p->x);
	Vec2d Dp(IX, IY); Vec2d Dip(IY, (-1)*IX);
	normalize(Dp);
	normalize(Dip);
	//q
	IX = GX_gradient.at<uchar>(q->y, q->x);
	IY = GY_gradient.at<uchar>(q->y, q->x);
	Vec2d Dq(IX, IY); Vec2d Diq(IY, (-1)*IX);
	normalize(Dq);
	normalize(Diq);
	//L(p,q)=q-p  or  p-q 
	cv::Vec2d q_p(q->x - p->x, q->y - p->y);
	cv::Vec2d p_q(p->x - q->x, p->y - q->y);
	normalize(q_p);
	normalize(p_q);
	//dp=Dp*L(p,q)
	double dp = Dip.dot(q_p) >= 0 ? Dip.dot(q_p) : Dip.dot(p_q);
	double dq = Dip.dot(q_p) >= 0 ? q_p.dot(Diq) : p_q.dot(Diq);

	double result = (1.0 / (PI))*(acos(dp) + acos(dq));
	return result;
}

double local_cost(imgNode *p, imgNode *q) {
	return Wz * fZ(q) + Wd * fD(p, q) + Wg * fG(q);
}

void normalize(Vec2d &dst) {
	double sum = sqrt(pow(dst[0], 2) + pow(dst[1], 2));
	dst[0] = dst[0] / sum;
	dst[1] = dst[1] / sum;
}