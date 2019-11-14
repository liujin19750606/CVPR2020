#include "cv/cv_def.h"
#include "CpImageProc.h"
//
float cpAreaResize(int area,cv::Mat src,cv::Mat &dst)
{
    int w = src.cols;
    int h = src.rows;
    float khw = (float)h/w;
    //h/w = hd/wd = khw;
    //wd*hd=area
    //so wd^2*khw = area
    //wd = sqrt(area/khw)
    //hd = wd * khw
	float wd = sqrt(area / khw);
	float hd = wd * khw;
	cv::resize(src, dst, cv::Size(cvRound(wd), cvRound(hd)));
	return (float)wd / (float)w;
}

cv::Mat fit_image(cv::Mat image, cv::Size new_size)
{
	cv::Mat dst(new_size, CV_8UC3);
	float rate_src = (float)image.rows / image.cols;
	float rate_dst = (float)new_size.height / new_size.width;
	if (rate_dst > rate_src)
	{
		cv::Size size_ok = cv::Size(new_size.width, new_size.width*rate_src);
		cv::Mat roi = dst(cv::Rect(0, 0, size_ok.width, size_ok.height));
		cv::resize(image, roi, roi.size());
	} else
	{
		cv::Size size_ok = cv::Size(new_size.height / rate_src, new_size.height);
		cv::Mat roi = dst(cv::Rect(0, 0, size_ok.width, size_ok.height));
		cv::resize(image, roi, roi.size());
	}
	return dst;
}
int AdjustRect(cv::Rect_<float> *pRect, cv::Size size)
{
	int flag = 0;
	if (pRect->x < 0)
	{
		pRect->width += pRect->x;
		pRect->x = 0;
		flag = 1;
	}
	if (pRect->x + pRect->width >= size.width)
	{
		pRect->width = size.width - 1 - pRect->x;
		flag = 1;
	}

	if (pRect->y < 0)
	{
		pRect->height += pRect->y;
		pRect->y = 0;
		flag = 1;
	}
	if (pRect->y + pRect->height >= size.height)
	{
		pRect->height = size.height - 1 - pRect->y;
		flag = 1;
	}

	return flag;
}
cv::Rect AdjustRect(cv::Rect rect, cv::Size size)
{
	cv::Rect rect2 = rect;
	int x2 = rect.x + rect.width, y2 = rect.y + rect.height;
	if (rect2.x < 0)
		rect2.x = 0;
	if (x2 >size.width - 1)
	{
		x2 = size.width - 1;
		rect2.width = x2 - rect2.x;
	}
	if (rect2.y < 0)
		rect2.y = 0;
	if (y2 >size.height - 1)
	{
		y2 = size.height - 1;
		rect2.height = y2 - rect2.y;
	}

	return rect2;
}