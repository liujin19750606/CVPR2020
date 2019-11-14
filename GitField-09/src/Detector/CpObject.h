#pragma once

#include <vector>
#include "../CvPlus/cv/cv_def.h"
#include <stdio.h>
//#include "../darknet/box.h"
//using namespace std;
///////////////////////////////////CvAvgCompConf/////////////////////////////////////////////
typedef struct CvAvgCompConf
{
    cv::Rect_<float> rect;
    int label;
    float confidence;
	float q[4];
	int nq;
	int link_id, frame_track_id;
	void draw_fast(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names, int *seg_colors, int draw_poly);
	//
	float poly[8];
	float cen[2],cen_poly[2];
	std::vector<cv::Point2f> pts;
	//
	float pos[3];//相对于相机的三维坐标
	//
	cv::Mat R, T, Tenu;
	double blh[3];
	float rotbox23d(float f, float cx, float cy, float W, float *t);
	void draw3dbox(cv::Mat &dst, float f, float cx, float cy, float *abc, cv::Scalar color, int thick);
#ifndef   DLL_TEST
	void trans(cv::Mat A);
	int pts2rbox();
	int box_transA(float *pA, cv::Size size0, cv::Size size1, float least_edge);
	void draw(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names, int pts_mode);
	void draw_pts(cv::Mat &img, cv::Scalar *colors, int thick);
	void draw_pts(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names);
#endif
} CvAvgCompConf;

#ifndef   DLL_TEST
void get_rect(const float *d,cv::Size size,cv::Rect *pRect,CvAvgCompConf *pObj);

void trans_blob(std::vector<CvAvgCompConf> &blobs, cv::Mat A);
void trans_blob(std::vector<CvAvgCompConf> &blobs, cv::Rect rect, int coords);

int LoadLabels(char *label_name, std::vector<CvAvgCompConf> &blobs, int nq);
int LoadPts(char *label_name, std::vector<CvAvgCompConf> &blobs);

int find_best_box(std::vector<CvAvgCompConf> &blobs, CvAvgCompConf *pB, cv::Size size, float *pIOU, float min_iou);
int find_best_box(std::vector<CvAvgCompConf> &blobs, std::vector<char> &recall_mask, CvAvgCompConf *pB, cv::Size size, float *pIOU, float min_iou);
#endif
