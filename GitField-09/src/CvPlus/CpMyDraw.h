#ifndef _CP_MY_DRAW_H__
#define _CP_MY_DRAW_H__

#include "cv/cv_def.h"

//绘制箭头
void cpDrawArrow(cv::Mat img,cv::Point Point1,cv::Point Point2,float R,CvScalar color,int thickness);
//绘制箭头
void cpDrawArrow(cv::Mat img, cv::Point Point1, float *dxdy, float L, float R, CvScalar color, int thickness);

void cvDrawCross(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness);
void cvDrawCrossX(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness);
void cvDrawCrossV(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness);

void draw_contour(cv::Mat &img, std::vector<cv::Point> &contour, cv::Scalar color, int thickness);
void draw_contour(cv::Mat &img, cv::Point2f *contour, int n, cv::Scalar color, int thickness);

//在图像img上绘制点集seq，半径R,粗细thick
void draw_points(cv::Mat &src, std::vector <cv::Point2f> &points, int R, int thick);

//https ://blog.csdn.net/guduruyu/article/details/70069426/ 
void draw_rot_rect(cv::Mat &src, cv::RotatedRect rotate_rect);

#endif
