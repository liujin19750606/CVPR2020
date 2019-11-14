#include "../CvPlus/CpMyDraw.h"
#include "ObjectContour.h"

void ObjectContour::draw(cv::Mat &src, std::vector<std::string> &names, cv::Scalar color, int thick)
{
	assert(label>=0);
	draw_contour(src, contour, color, 2);
	cv::Scalar color_bk(255 - color.val[0], 255 - color.val[1], 255 - color.val[2]);
	cv::putText(src, names[label].c_str(), center, 1, 1.0f, color_bk, thick);
}
