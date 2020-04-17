#include <iostream>
#include<cmath>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//定义计算两点距离的函数
double Distance(Point pt1, Point pt2)
{
	return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
}

int main()
{
	cv::Mat binMat;
	cv::Mat gryMat = cv::imread("D:\\Files\\rim.png", 0);
	cv::Mat srcMat = cv::imread("D:\\Files\\rim.png");//读取图像
	//检测图像是否读取成功
	if (srcMat.empty())
	{
		std::cout << "Can't open the image" << endl;
		return -1;
	}
	cv::Mat dstMat;
	srcMat.copyTo(dstMat);


	//反二值化
	cv::threshold(gryMat, binMat, 100, 255, THRESH_BINARY_INV);


	//寻找连通域
	vector<vector<Point>>contours;
	cv::findContours(binMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


	//循环画出检测结果
	for (int i = 0; i < contours.size(); i++)
	{
		//获得最小外接矩形
		cv::RotatedRect rbox = minAreaRect(contours[i]);
		//计算宽长比和面积
		cv::Point2f vtx[4];
		rbox.points(vtx);
		double bLR = Distance(vtx[0], vtx[1])/Distance(vtx[1],vtx[2]);
		double area = Distance(vtx[0], vtx[1]) * Distance(vtx[1], vtx[2]);
		//筛选
		if (bLR > 0.9&&bLR < 1.1)
		{
			if (area > 50)
			{
				cv::drawContours(dstMat, contours, i, Scalar(0, 255, 255), -1, 8);
			}
		}
	}
	//显示结果
	cv::imshow("src", srcMat);
	cv::imshow("dst", dstMat);
	waitKey(0);
}

