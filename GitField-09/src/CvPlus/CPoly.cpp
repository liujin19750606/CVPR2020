#include <stdio.h>
#include "CPoly.h"

CPoly::CPoly()
{
    ;
}
CPoly::~CPoly()
{

}
int CPoly::load(char *fname)
{
    FILE *fp = fopen(fname,"r");
    if(fp)
    {
        poly.clear();
        while(1)
        {
            cv::Point2f pt;
            int rn = fscanf(fp,"%f %f\n", &pt.x, &pt.y);
            if(rn==2)
            {
                poly.push_back(pt);
            } else
                break;
        }
        fclose(fp);
        return 0;
    } else
        return 1;
}
int CPoly::save(char *fname)
{
    if(poly.size()>2)
    {
      FILE *fp = fopen(fname,"w");
      if(fp)
      {
        for(int i=0;i<(int)poly.size();i++)
        {
            fprintf(fp,"%f %f\n", poly[i].x, poly[i].y);
        }
        fclose(fp);
        return 0;
      } else
        return 1;
    } else
    {
        remove(fname);
        return 2;
    }
}
void CPoly::finish()
{
    poly = poly_temp;
    poly_temp.clear();
}
void CPoly::clear()
{
    poly.clear();
}
void CPoly::clear_temp()
{
    poly_temp.clear();
}
void CPoly::draw(cv::Mat &dst,cv::Scalar color,int thick)
{
    if(poly.size()>0)
    {
       for(int i=0;i<(int)poly.size()-1;i++)
       {
         cv::line(dst,poly[i],poly[i+1],color,thick);
       }
       cv::line(dst,poly[poly.size()-1],poly[0],color,thick);
    }
    if(poly_temp.size()>0)
    {
        if(poly_temp.size()>1)
        {
          for(int i=0;i<(int)poly_temp.size()-1;i++)
          {
            cv::line(dst,poly_temp[i],poly_temp[i+1],color,thick);
          }
        } else
          cv::circle(dst,poly_temp[0],dst.cols/100,color,thick);
    }
}

void CPoly::move(float dx,float dy)
{
    for(int i=0;i<(int)poly.size();i++)
    {
        poly[i].x+=dx;
        poly[i].y+=dy;
    }
}
cv::Rect CPoly::get_rect()
{
    int left = 100000, right = -left;
    int top = 100000, bottom = -top;
    for(int i=0;i<(int)poly.size();i++)
    {
        if(poly[i].x<left)
            left = poly[i].x;
        if(poly[i].x>right)
            right = poly[i].x;
        if(poly[i].y<top)
            top = poly[i].y;
        if(poly[i].y>bottom)
            bottom = poly[i].y;
    }
    return cv::Rect(left,top,right-left,bottom-top);
}

int CPoly::roi_control(char ch,CvSize video_size,int scale_step)
{
    int res = 0;
    switch(ch)
    {
      case 'a'://left
        {
           move(-scale_step,0);
           res = 1;
        }
        break;
      case 'd'://right
        {
           move(scale_step,0);
           res = 1;
        }
        break;
     case 'w'://up
       {
          move(0,-scale_step);
          res = 1;
       }
       break;
     case 's'://down
       {
         move(0,scale_step);
         res = 1;
       }
      break;
    }
    return res;
}

#ifndef  DLL_TEST
void CPoly::get_rot_rect()
{
	rot_rect.rot_rect = cv::minAreaRect(poly);
	rot_rect.getAff(2);
}
void CPoly::trans(cv::Mat src, cv::Mat &dst)
{
	cv::warpAffine(src, dst, rot_rect.A, rot_rect.rot_rect.size);
}
#endif