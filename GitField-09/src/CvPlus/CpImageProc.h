#ifndef _CP_IMAGE_PROC_H__
#define _CP_IMAGE_PROC_H__

#include "cv/cv_def.h"

//�����������Ȳ�������
float cpAreaResize(int area, cv::Mat src, cv::Mat &dst);

cv::Mat fit_image(cv::Mat image, cv::Size new_size);

int AdjustRect(cv::Rect_<float> *pRect, cv::Size size);

cv::Rect AdjustRect(cv::Rect rect, cv::Size size);

#endif
