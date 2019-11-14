#ifndef _CP_CPoly_H_
#define _CP_CPoly_H_

#include <vector>
#include "cv/cv_def.h"
#ifndef  DLL_TEST
#include "CpRotRect.h"
#endif
//using namespace std;

class CPoly
{
public:
   std::vector<cv::Point2f> poly;
   std::vector<cv::Point2f> poly_temp;
   CPoly();
   ~CPoly();
   int load(char *fname);
   int save(char *fname);
   void finish();
   void clear();
   void clear_temp();
   void draw(cv::Mat &dst,cv::Scalar color,int thick);
   int roi_control(char ch,CvSize video_size,int scale_step);
   void move(float dx,float dy);
   cv::Rect get_rect();
#ifndef  DLL_TEST
   CpRotRect rot_rect;
   void get_rot_rect();
   void trans(cv::Mat src, cv::Mat &dst);
#endif
};

#endif
