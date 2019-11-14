#include "../General/MyString.h"
#include "../CvPlus/CpMyDraw.h"
#include "CpObject.h"


void CvAvgCompConf::draw_fast(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names, int *seg_colors, int draw_poly)
{
	//assert(rect.x >= 0 && rect.y >= 0);
	//
	int color_id = label % 20 + 1;
	cv::Scalar color_cls = seg_colors ? cv::Scalar((seg_colors[color_id] & 0xff), ((seg_colors[color_id] >> 8) & 0xff), (seg_colors[color_id] >> 16)) : color;
	if (!names.empty())
	{
		char target_label[64];
		sprintf(target_label, "%s %f", names[label].c_str(), confidence);
		putText(img, target_label, cv::Point(rect.x, rect.y), CV_FONT_HERSHEY_DUPLEX, 0.6f, color_cls);
	}
	switch (draw_poly)
	{
	case 0://rectangle
	{
			   cv::rectangle(img, rect, seg_colors ? color_cls : color, thick);
	}
		break;
	case 1://pts
	{
			   for (int i = 0; i < pts.size(); i++)
			   {
				   cv::circle(img, pts[i], 1, seg_colors ? color_cls : color, thick);
				   //cvDrawCross(img, pts[i], 5, GetBackColor(color), thick);
				   //cv::line(img, pts[i], pts[(i + 1) % pts.size()], seg_colors ? color_cls : color, thick);
			   }
	}
		break;
	case 2://ellipse
	{
			   cv::ellipse(img, cv::Point(cen[0], cen[1]), cv::Size(rect.width / 2, rect.height / 2), atan2(q[1], q[0]) * 180 / CV_PI, 0, 360, color, 1);
	}
		break;
	}
	if (draw_poly)
	for (int i = 0; i < 4; i++)
	{
		cv::line(img, cv::Point2f(poly[2 * i], poly[2 * i + 1]), cv::Point2f(poly[(2 * i + 2) % 8], poly[(2 * i + 3) % 8]), color_cls, thick);
	}

	//绘制主方向
	if (this->nq == 2)
	{
		cv::line(img, cv::Point2f(cen[0], cen[1]), cv::Point2f((poly[0] + poly[2]) / 2, (poly[1] + poly[3]) / 2), color_cls, thick);
		//cv::line(img, cv::Point2f(cen[0], cen[1]), cv::Point2f(cen[0] + q[2] * this->rect.width / 2, cen[1] + q[3] * this->rect.width / 2), CV_RGB(0, 255, 0), 2);
	}
}

#ifndef   DLL_TEST
#include "../Detector/CpObjectMask.h"
void CvAvgCompConf::draw_pts(cv::Mat &img, cv::Scalar *colors, int thick)
{
	for (int i = 0; i < pts.size(); i++)
	{
		//cv::circle(img, pts[i], 5, GetBackColor(color), thick);
		cvDrawCross(img, pts[i], 5, colors[i% pts.size()], thick);
	}
}
void CvAvgCompConf::draw(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names, int pts_mode)
{
	if (nq == 2 && fabs(q[0] * q[0] + q[1] * q[1] - 1) < 1e-5)
	{
		float r = std::max(rect.width, rect.height) / 2;
		//cpDrawArrow( img,cv::Point Point1,cv::Point Point2,float R,CvScalar color,int thickness);
		cpDrawArrow(img,
			cv::Point2f(cen[0], cen[1]),
			cv::Point2f(cen[0] + r*q[0], cen[1] + r*q[1]),
			r / 8,
			CV_RGB(255, 0, 0), 2);
		//image trans to box
		//[x']=[ dx  dy] [x-bx] + [bw/2]
		//[y']  [-dy  dx] [y-by]    [bh/2]
		//so
		//[x] = [dx  -dy] [x'-bw/2] + [bx]
		//[y]    [dy   dx] [y'-bh/2] + [by]
		//equals to
		//[x] = [dx  -dy] [x'] + [bx-dx*bw/2+dy*bh/2]
		//[y]    [dy   dx] [y'] + [by-dy*bw/2-dx*bh/2]
		float x1 = cen[0] - q[0] * rect.width / 2 + q[1] * rect.height / 2;
		float y1 = cen[1] - q[1] * rect.width / 2 - q[0] * rect.height / 2;
		cv::Point2f pts[4];
		pts[0] = cv::Point2f(0, 0);
		pts[1] = cv::Point2f(rect.width, 0);
		pts[2] = cv::Point2f(rect.width, rect.height);
		pts[3] = cv::Point2f(0, rect.height);
		cv::Point2f pts_trans[4];
		for (int i = 0; i<4; i++)
		{
			pts_trans[i].x = q[0] * pts[i].x - q[1] * pts[i].y + x1;
			pts_trans[i].y = q[1] * pts[i].x + q[0] * pts[i].y + y1;
		}
		cv::line(img, pts_trans[0], pts_trans[1], color, thick);
		cv::line(img, pts_trans[1], pts_trans[2], color, thick);
		cv::line(img, pts_trans[2], pts_trans[3], color, thick);
		cv::line(img, pts_trans[3], pts_trans[0], color, thick);
		//draw label
		if (!names.empty())
		{
			float font_scale = 0.007f*std::max(rect.width, rect.height);
			int baseline;
			cv::Size text_size = cv::getTextSize(names[label].c_str(), 1, font_scale, 1, &baseline);
			cv::putText(img, names[label].c_str(), cv::Point(cen[0] - text_size.width / 2, cen[1] + text_size.height / 2), 1, font_scale, color);
		}
	}
	else
	{
		assert(rect.x >= 0 && rect.y >= 0);
		if (pts_mode == 0)
		{
			cv::rectangle(img, rect, color, thick);
			//
			if (!names.empty())
			{
				cv::Mat rect_obj = img(rect);
				int label_height = 24;
				if (2 * label_height <= rect.height)
				{
					cv::Mat rect_label = rect_obj(cv::Rect(0, 0, rect.width, 2 * label_height));
					rect_label.setTo(color);
					cv::Scalar bkcolor = GetBackColor(color);
					char target_label[64];
					sprintf(target_label, "%s %f", names[label].c_str(), confidence);
					putText(rect_label, target_label, cv::Point(1, label_height - label_height / 2 + 2), CV_FONT_HERSHEY_DUPLEX, 0.6f, bkcolor);
					if (pos[2] > 0)
					{
						sprintf(target_label, "[%.4f %.4f %.4f]", pos[0], pos[1], pos[2]);
						putText(rect_label, target_label, cv::Point(1, label_height * 2 - label_height / 2 + 2), CV_FONT_HERSHEY_DUPLEX, 0.4f, bkcolor);
					}
				}
			}
			for (int i = 0; i < pts.size(); i++)
			{
				//cv::circle(img, pts[i], 5, GetBackColor(color), thick);
				cvDrawCross(img, pts[i], 5, GetBackColor(color), thick);
			}
		}
		else
		{
			//绘制一个闭合多边形
			for (int i = 0; i < pts.size() - 1; i++)
			{
				cv::line(img, pts[i], pts[i + 1], color, thick);
			}
			cv::line(img, pts[pts.size() - 1], pts[0], color, thick);
			//
			char target_label[64];
			sprintf(target_label, "%s %f", names[label].c_str(), confidence);
			putText(img, target_label, ((cv::Point2f *)(this->cen))[0], CV_FONT_HERSHEY_DUPLEX, 0.6f, color);
		}
	}
}
void CvAvgCompConf::draw_pts(cv::Mat &img, cv::Scalar color, int thick, std::vector<std::string> &names)
{
	for (int i = 0; i < 4; i++)
		line(img, pts[i], pts[(i + 1) % 4], color, thick);
	//
	//draw label
	float font_scale = 0.007f*std::max(rect.width, rect.height);
	int baseline;
	cv::Size text_size = cv::getTextSize(names[label].c_str(), 1, font_scale, 1, &baseline);
	if (!names.empty())
		cv::putText(img, names[label].c_str(), cv::Point(cen[0] - text_size.width / 2, cen[1] + text_size.height / 2), 1, font_scale, color);
}
void CvAvgCompConf::trans(cv::Mat A)
{
	float *pA = A.ptr<float>();
	pts[0].x = pA[0] * rect.x + pA[1] * rect.y + pA[2];
	pts[0].y = pA[3] * rect.x + pA[4] * rect.y + pA[5];
	pts[1].x = pA[0] * (rect.x + rect.width) + pA[1] * rect.y + pA[2];
	pts[1].y = pA[3] * (rect.x + rect.width) + pA[4] * rect.y + pA[5];
	pts[2].x = pA[0] * (rect.x + rect.width) + pA[1] * (rect.y + rect.height) + pA[2];
	pts[2].y = pA[3] * (rect.x + rect.width) + pA[4] * (rect.y + rect.height) + pA[5];
	pts[3].x = pA[0] * rect.x + pA[1] * (rect.y + rect.height) + pA[2];
	pts[3].y = pA[3] * rect.x + pA[4] * (rect.y + rect.height) + pA[5];
	//
	cen[0] = pA[0] * (rect.x + rect.width / 2) + pA[1] * (rect.y + rect.height / 2) + pA[2];
	cen[1] = pA[3] * (rect.x + rect.width / 2) + pA[4] * (rect.y + rect.height / 2) + pA[5];
}
int CvAvgCompConf::pts2rbox()
{
	assert(pts.size() == 4);
	//中心点
	cv::Point2f cen = (pts[0] + pts[1] + pts[2] + pts[3]) *0.25f;
	//方向
	cv::Point2f cenf = (pts[0] + pts[1]) *0.5f;
	cv::Point2f cenb = (pts[2] + pts[3]) *0.5f;
	float dx = cenf.x - cenb.x, dy = cenf.y - cenb.y;
	float L = sqrt(dx*dx + dy*dy);
	float cos_t = dx / L, sin_t = dy / L;
	float r[4] = { cos_t, sin_t, -sin_t, cos_t };
	//转换
	cv::Point2f tp[4];
	for (int i = 0; i < 4; i++)
	{
		float px = pts[i].x - cen.x;
		float py = pts[i].y - cen.y;
		tp[i] = cv::Point2f(cos_t*px + sin_t*py, -sin_t*px + cos_t*py);
	}
	//a,b
	float lx = (tp[2].x + tp[3].x) / 2, rx = (tp[0].x + tp[1].x) / 2;
	float a = (rx - lx) / 2;
	//assert(a > 0);
	float ty = (tp[0].y + tp[3].y) / 2, by = (tp[1].y + tp[2].y) / 2;
	float b = (by - ty) / 2;
	//assert(b > 0);
	//
	if (a > 0.001f && b > 0.001f)
	{
		this->cen[0] = cen.x;
		this->cen[1] = cen.y;
		this->rect.width = 2 * a;
		this->rect.height = 2 * b;
		float ecorners[8] = { a, -b, a, b, -a, b, -a, -b };
		for (int i = 0; i < 4; i++)
		{
			poly[2 * i] = this->cen[0] + cos_t*ecorners[2 * i] - sin_t*ecorners[2 * i + 1];
			poly[2 * i + 1] = this->cen[1] + sin_t*ecorners[2 * i] + cos_t*ecorners[2 * i + 1];
		}
		q[0] = cos_t; q[1] = sin_t;
		q[2] = atan2(sin_t, cos_t);
		return 0;
	} else
		return 1;
}
float CvAvgCompConf::rotbox23d(float f,float cx,float cy,float W,float *t)
{
	//assert(pts.size() == 4);
	float uc = this->cen[0], vc = this->cen[1];
	float lmd = W / rect.width;
	float cos_t = q[0], sin_t = q[1];
	R.create(3, 3, CV_32FC1);
	float *pR = R.ptr<float>();
	pR[0] = cos_t; pR[1] =-sin_t; pR[2] = 0;
	pR[3] = sin_t; pR[4] = cos_t; pR[5] = 0;
	pR[6] =     0; pR[7] = 0;     pR[8] = 1;
	//
	float dX = uc - cx, dY = vc - cy;
	float t3[3] = { lmd*dX, lmd*dY, lmd*f};
	if (t)
	{
		t[0] = t3[0]; t[1] = t3[1]; t[2] = t3[2];
	}
	cv::Mat mt(3, 1, CV_32FC1, t3);
	T=-R.t()*mt;

	return rect.height*lmd;
}
void CvAvgCompConf::draw3dbox(cv::Mat &dst, float f, float cx, float cy, float *abc, cv::Scalar color, int thick)
{
	float W = abc[0], H = abc[1], Z = abc[2];
	std::vector<cv::Point3f> box_pts(8);
	box_pts[0] = cv::Point3f(-W / 2, -H/2,   -Z/2);
	box_pts[1] = cv::Point3f( W / 2, -H/2,   -Z/2);
	box_pts[2] = cv::Point3f( W / 2,  H / 2, -Z/2);
	box_pts[3] = cv::Point3f(-W / 2,  H / 2, -Z/2);
	box_pts[4] = cv::Point3f(-W / 2, -H / 2, Z / 2);
	box_pts[5] = cv::Point3f(W / 2, -H / 2, Z / 2);
	box_pts[6] = cv::Point3f(W / 2, H / 2, Z / 2);
	box_pts[7] = cv::Point3f(-W / 2, H / 2, Z / 2);
	//
	cv::Mat R64;
	R.convertTo(R64, CV_64FC1);
	cv::Mat T64;
	T.convertTo(T64, CV_64FC1);
	cv::Mat t64 = -R64*T64;
	std::vector<cv::Point2f> pts_proj;
	float dK[9] = { f, 0, cx, 0, f, cy, 0, 0, 1 };
	cv::Mat K(3, 3, CV_32FC1, dK);
	cv::projectPoints(box_pts, R64, t64, K, cv::Mat(), pts_proj);
	assert(pts_proj.size() == 8);
	//cv::line(dst, cv::Point(proj_coords[0], proj_coords[1]), cv::Point(proj_coords[2], proj_coords[3]), color, thick);
	cv::line(dst, pts_proj[1], pts_proj[2], color, thick);
	cv::line(dst, pts_proj[2], pts_proj[3], color, thick);
	//cv::line(dst, cv::Point(proj_coords[6], proj_coords[7]), cv::Point(proj_coords[0], proj_coords[1]), color, thick);
	//
	cv::line(dst, pts_proj[4], pts_proj[5], color, thick);
	cv::line(dst, pts_proj[5], pts_proj[6], color, thick);
	cv::line(dst, pts_proj[6], pts_proj[7], color, thick);
	cv::line(dst, pts_proj[7], pts_proj[4], color, thick);
	//
	//cv::line(dst, cv::Point(proj_coords[0], proj_coords[1]), cv::Point(proj_coords[8], proj_coords[9]), color, thick);
	cv::line(dst, pts_proj[1], pts_proj[5], color, thick);
	cv::line(dst, pts_proj[2], pts_proj[6], color, thick);
	cv::line(dst, pts_proj[3], pts_proj[7], color, thick);
	//
	cpDrawArrow(dst, pts_proj[0], pts_proj[1], 15.4f, CV_RGB(255, 0, 0), thick);//x轴cpDrawArrow
	cpDrawArrow(dst, pts_proj[0], pts_proj[3], 15.4f, CV_RGB(0, 255, 0), thick);//y轴
	cpDrawArrow(dst, pts_proj[0], pts_proj[4], 15.4f, CV_RGB(0, 0, 255), thick);//z轴
}

void get_rect(const float *d, cv::Size size, cv::Rect *pRect, CvAvgCompConf *pObj)
{
	int rect_xmin = (int)(d[3] * size.width);
	int rect_ymin = (int)(d[4] * size.height);
	int rect_xmax = (int)(d[5] * size.width);
	int rect_ymax = (int)(d[6] * size.height);
	int left = pRect ? pRect->x : 0;
	int top = pRect ? pRect->y : 0;
	pObj->cen[0] = left + (0.5*(rect_xmin + rect_xmax));
	pObj->cen[1] = top + (0.5*(rect_ymin + rect_ymax));
	pObj->rect.x = left + rect_xmin;
	pObj->rect.y = top + rect_ymin;
	pObj->rect.width = (int)(rect_xmax - rect_xmin);
	pObj->rect.height = (int)(rect_ymax - rect_ymin);
}
void trans_blob(std::vector<CvAvgCompConf> &blobs, cv::Mat A)
{
	for (int i = 0; i < blobs.size(); i++)
	{
		blobs[i].trans(A);
	}
}
void trans_blob(std::vector<CvAvgCompConf> &blobs, cv::Rect rect, int coords)
{
	for (int i = 0; i < (int)blobs.size(); ++i)
	{
		CvAvgCompConf *pb = &blobs[i];
		//assert(pb->rect.x >= 0 || yolo.lo.coords == 6);
		if (coords == 4)
		{
			pb->rect.x += rect.x;
			pb->rect.y += rect.y;
		}
		pb->cen[0] += rect.x;
		pb->cen[1] += rect.y;
	}
}
#define MAX_BOXES_PER_GRID (1000)
int LoadLabels(char *label_name, std::vector<CvAvgCompConf> &blobs,int nq)
{
	FILE *fp = fopen(label_name, "r");
	if (fp)
	{
		char line[1024];
		int i;
		for (i = 0; i<MAX_BOXES_PER_GRID; i++)//polys.size()
		{
			CvAvgCompConf a;// *pBR = &polys[i];
			char *p = fgets(line, 1024, fp);
			char new_str[1024], *stack[128];
			int n = SplitStringA(line, " ", new_str, stack);
			if (p && (n == 1 + 4 || n == 1 + 4 + nq))
			{
				a.label = atoi(stack[0]);
				float cx = atof(stack[1]);
				float cy = atof(stack[2]);
				float w = atof(stack[3]);
				float h = atof(stack[4]);
				a.poly[0] = cx;
				a.poly[1] = cy;
				a.poly[2] = w;
				a.poly[3] = h;
				a.nq = nq;
				//5~5+2*maxpts
				if (n == 1 + 4 + nq)
				{
					for (int j = 0; j<nq; j++)
					{
						a.q[j] = atof(stack[1 + 4 + j]);
					}
					//a.R.create(3, 3, CV_32FC1);
					//mtQ2R(a.q, a.R.ptr<float>());
				}
				else
				{
					for (int j = 0; j < nq; j++)
					{
						a.q[j] = 0;

					}
				}
				a.confidence = 1.0f;
				blobs.push_back(a);
			} else
				break;
		}
		fclose(fp);
		return 0;
	}
	else
		return 1;
}
int LoadPts(char *label_name, std::vector<CvAvgCompConf> &blobs)
{
	FILE *fp = fopen(label_name, "r");
	if (fp)
	{
		char line[1024];
		int i;
		for (i = 0; i<blobs.size(); i++)//polys.size()
		{
			CvAvgCompConf *pO = &blobs[i];// *pBR = &polys[i];
			pO->q[0] = pO->q[1] = 0;
			char *p = fgets(line, 1024, fp);
			if (p)
			{
				if (strcmp(p, "-") && strcmp(p, "-\n"))
				{
					char new_str[1024], *stack[128];
					int n = SplitStringA(line, " ", new_str, stack);
					assert(n % 2 == 0);
					int npts = n / 2;
					pO->pts.clear();
					int offset = 0;
					for (int i = 0; i < npts; i++)
					{
						pO->pts.push_back(cv::Point2f(atof(stack[offset]), atof(stack[offset + 1])));
						offset += 2;
					}
					assert(pO->pts.size() == npts);
				}
			} else
				break;
		}
		fclose(fp);
		return 0;
	}
	else
		return 1;
}
#include "../darknet/box.h"
int find_best_box(std::vector<CvAvgCompConf> &blobs, CvAvgCompConf *pB, cv::Size size, float *pIOU, float min_iou)
{
	float _box[4] = { (float)(pB->rect.x + pB->rect.width / 2) / size.width, (float)(pB->rect.y + pB->rect.height / 2) / size.height, (float)pB->rect.width / size.width, (float)pB->rect.height / size.height };
	box b0 = float_to_box(_box, 0);
	int fid = -1;
	float max_iou = -1.0f;
	for (int i = 0; i < blobs.size(); i++)
	{
		CvAvgCompConf *pR = &blobs[i];
		box b1 = float_to_box(pR->poly, 0);
		float iou = box_iou(b0, b1);
		if (iou>min_iou && iou>max_iou)
		{
			fid = i;
			max_iou = iou;
		}
	}
	if (pIOU)
		*pIOU = max_iou;
	return fid;
}
int find_best_box(std::vector<CvAvgCompConf> &blobs, std::vector<char> &recall_mask, CvAvgCompConf *pB,cv::Size size, float *pIOU, float min_iou)
{
	float _box[4] = { (float)(pB->rect.x + pB->rect.width / 2) / size.width, (float)(pB->rect.y + pB->rect.height / 2) / size.height, (float)pB->rect.width / size.width, (float)pB->rect.height / size.height };
	assert(recall_mask.size() == blobs.size());
	box b0 = float_to_box(_box, 0);
	int fid = -1;
	float max_iou = -1.0f;
	for (int i = 0; i < blobs.size(); i++)
	if (recall_mask[i]==0)
	{
		CvAvgCompConf *pR = &blobs[i];
		box b1 = float_to_box(pR->poly, 0);
		float iou = box_iou(b0, b1);
		if (iou>min_iou && iou>max_iou)
		{
			fid = i;
			max_iou = iou;
		}
	}
	if (pIOU)
		*pIOU = max_iou;
	return fid;
}
#endif