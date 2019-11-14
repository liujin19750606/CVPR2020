#include "cv/cv_def.h"

//绘制箭头
void cpDrawArrow(cv::Mat img,cv::Point Point1,cv::Point Point2,float R,CvScalar color,int thickness)
{ float Cos20=0.93969f,Sin20=0.34202f;
  double dx=(float)Point2.x-Point1.x,dy=(float)Point2.y-Point1.y;
  double l=sqrt(dx*dx+dy*dy);
  dx/=l;dy/=l;
  //绘制起止点线段
  cv::line(img,cv::Point(Point1.x,Point1.y),cv::Point(Point2.x,Point2.y),color,thickness);
  //DrawLine(P0,P1,rgb);
  //绘制箭头
  if(fabs(l)>1.0f)
  {
    float x1=(float)Point2.x,y1=(float)Point2.y;
    cv::line(img,cv::Point(cvRound(x1),cvRound(y1)),
	             cv::Point(cvRound(x1-R*(Cos20*dx-Sin20*dy)),cvRound(y1-R*(Cos20*dy+Sin20*dx))),
		     color,thickness);
    cv::line(img,cv::Point(cvRound(x1),cvRound(y1)),
	             cv::Point(cvRound(x1-R*(Cos20*dx+Sin20*dy)),cvRound(y1-R*(Cos20*dy-Sin20*dx))),
		     color,thickness);
  }
}
//绘制箭头
void cpDrawArrow(cv::Mat img, cv::Point Point1, float *dxdy, float L, float R, CvScalar color, int thickness)
{
	float Cos20 = 0.93969f, Sin20 = 0.34202f;
	float dx = dxdy[0];
	float dy = dxdy[1];
	cv::Point Point2(Point1.x + L*dx, Point1.y + L*dy);// = Point1 + L*cv::Point2f(dx, dy);
	//绘制起止点线段
	cv::line(img, cv::Point(Point1.x, Point1.y), cv::Point(Point2.x, Point2.y), color, thickness);
	//DrawLine(P0,P1,rgb);
	//绘制箭头
	if (fabs(L)>1.0f)
	{
		float x1 = (float)Point2.x, y1 = (float)Point2.y;
		cv::line(img, cv::Point(cvRound(x1), cvRound(y1)),
			cv::Point(cvRound(x1 - R*(Cos20*dx - Sin20*dy)), cvRound(y1 - R*(Cos20*dy + Sin20*dx))),
			color, thickness);
		cv::line(img, cv::Point(cvRound(x1), cvRound(y1)),
			cv::Point(cvRound(x1 - R*(Cos20*dx + Sin20*dy)), cvRound(y1 - R*(Cos20*dy - Sin20*dx))),
			color, thickness);
	}
}
//???Point??????????
#define MAX2(a,b)   ((a)>(b) ? (a) : (b))
#define MIN2(a,b)   ((a)<(b) ? (a) : (b))
void cvDrawCross(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness)
{ cv::line(img,cv::Point(MAX2(Point.x-R,0),Point.y),cv::Point(MIN2(Point.x+R,img.cols-1),Point.y),color,thickness);
  cv::line(img, cv::Point(Point.x, MAX2(Point.y - R, 0)), cv::Point(Point.x, MIN2(Point.y + R,img.rows-1)), color, thickness);
}
//???Point???????????
void cvDrawCrossX(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness)
{ cv::line(img,cv::Point(Point.x-R,Point.y-R),cv::Point(Point.x+R,Point.y+R),color,thickness);
  cv::line(img,cv::Point(Point.x+R,Point.y-R),cv::Point(Point.x-R,Point.y+R),color,thickness);
}
//???Point?????????
void cvDrawCrossV(cv::Mat &img,cv::Point Point,int R,cv::Scalar color,int thickness)
{ cv::line(img,cv::Point(Point.x-R,Point.y),cv::Point(Point.x+R,Point.y),color,thickness);
  cv::line(img,cv::Point(Point.x,Point.y),cv::Point(Point.x,Point.y-R-R),color,thickness);
}

void draw_contour(cv::Mat &img, std::vector<cv::Point> &contour, cv::Scalar color, int thickness)
{
	for (int i = 0; i < contour.size()-1; i++)
	{
		cv::line(img, contour[i], contour[i+1], color, thickness);
	}
	cv::line(img, contour[contour.size() - 1], contour[0], color, thickness);
}
void draw_contour(cv::Mat &img, cv::Point2f *contour,int n, cv::Scalar color, int thickness)
{
	for (int i = 0; i < n - 1; i++)
	{
		cv::line(img, contour[i], contour[i + 1], color, thickness);
	}
	cv::line(img, contour[n - 1], contour[0], color, thickness);
}

//在图像img上绘制点集seq，半径R,粗细thick
void draw_points(cv::Mat &src, std::vector <cv::Point2f> &points, int R, int thick)
{
	int i;
	for (i = 0; i<points.size(); i++)
	{
		cv::Point2f *p = &points[i];
		cv::circle(src, *p, R, CV_RGB(255, 0, 0), thick);
	}
}

//https ://blog.csdn.net/guduruyu/article/details/70069426/ 
void draw_rot_rect(cv::Mat &src, cv::RotatedRect rotate_rect)
{
	//获取旋转矩形的四个顶点
	cv::Point2f vertices[4];
	rotate_rect.points(vertices);

	//逐条边绘制
	for (int j = 0; j < 4; j++)
	{
		cv::line(src, vertices[j], vertices[(j + 1) % 4], cv::Scalar(0, 255, 0));
	}
}