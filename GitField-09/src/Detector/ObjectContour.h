#pragma once

#include <vector>
#include "../CvPlus/cv/cv_def.h"

typedef struct ObjectContour
{
	int label;
	float confidence;
	int neural_count;
	float area_percent;
	float length_percent;
	cv::Rect neural_rect;
	std::vector<cv::Point> contour;
	cv::Rect rect;
	cv::Point2f center;
	void draw(cv::Mat &src, std::vector<std::string> &names, cv::Scalar color, int thick);
};
