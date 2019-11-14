// MyHello.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <Windows.h>
#include <conio.h>

#include "../src/General/FileProc.h"

#include "../src/CvPlus/cv/cv_def.h"
#include "../src/CvPlus/CpImageProc.h"
#include "../src/Detector/CpObject.h"
#include "../src/Detector/ObjectContour.h"
#include "../dll/main_dll.h"
#include "../src/CvPlus/CPoly.h"
#include "../src/list/option_list.h"

#pragma comment(lib,"dll.lib")

#define  MAX_COLOR_NUMBER  16
cv::Scalar target_colors[MAX_COLOR_NUMBER] = { CV_RGB(220, 0, 70), CV_RGB(0, 180, 180),
CV_RGB(255, 255, 0),//bicyle
CV_RGB(110, 220, 0), CV_RGB(200, 120, 0), CV_RGB(210, 0, 0), CV_RGB(0, 240, 0),
CV_RGB(0, 255, 0),//car
CV_RGB(90, 120, 20), CV_RGB(20, 200, 10), CV_RGB(90, 190, 0), CV_RGB(200, 200, 100), CV_RGB(250, 0, 210), CV_RGB(240, 210, 0),
CV_RGB(255, 0, 255),//"motorbike"
CV_RGB(255, 0, 0),//"person"
};

typedef enum {
	CONVOLUTIONAL,
	DECONVOLUTIONAL,
	CONNECTED,
	MAXPOOL,
	MAXAVGPOOL,
	SOFTMAX,
	DETECTION,
	DROPOUT,
	CROP,
	ROUTE,
	COST,
	NORMALIZATION,
	AVGPOOL,
	LOCAL,
	SHORTCUT,
	ACTIVE,
	RNN,
	GRU,
	LSTM,
	CRNN,
	BATCHNORM,
	NETWORK,
	XNOR,
	REGION,
	REGION_ELLIPSE,
	YOLO,
	REORG,
	REORG3D,
	UPSAMPLE,
	LOGXENT,
	L2NORM,
	ARRAY_DEPTH,
	ARRAY_SOFTMAX,
	FIELD_VECTOR,
	PTS_FIELD,
	DELTA_BOX,
	ARRAY_PAF,
	ARRAY_FIELD,
	ARRAY_FIELDMC,
	COORDPOOL,
	CORNERPOOL,
	ROIPOOLING,
	RPNLOSS,
	FIELD2RPN,
	BLANK
} LAYER_TYPE;

int main(int argc, char* argv[])
{
	char work_path[512];
	GetModulePath(work_path);

	char cfg_path[512];
	sprintf(cfg_path, "%s/cfg/", work_path);
	//
	char data_path[512] = "E:/datas/remote_pts_new/";// "E:/datas/voc_seg_benchmark/"; //"D:/Projects/TianJing/dll_test/datas/farmland_video/"
	//
	char cfg_file[512];
	sprintf(cfg_file, "%s/options.txt", cfg_path);
	list *options = read_data_cfg(cfg_file);
	if (options)
	{
		char default_data_path[512];
		sprintf(default_data_path, "%s/../datas/remote_pts_new/", work_path);
		strcpy(data_path, option_find_str(options, "data_path", default_data_path));
		free_list(options);
	}
	printf("data_path=%s\n", data_path);

	DWORD64 pY = create_dhandle();
	int ret = load_path(pY, data_path);//  ../datas/coco/train2014/     E:/datas/VOC_Segment/
	if (ret == 0)
	{
		std::vector<std::string> names;
		get_names(pY, names);

		CPoly poly;
		char poly_path[512];
		sprintf(poly_path, "%s/poly.txt", work_path);
		poly.load(poly_path);
		cv::Rect roi = poly.get_rect();
		set_poly_mask(pY, &roi, poly.poly);

		LAYER_TYPE type = (LAYER_TYPE)get_output_type(pY);

		char brow_path[512];
		sprintf(brow_path, "%s/JpegImages/", data_path);
		std::vector<FileVector> files;
		BrowPathFiles(brow_path, ".jpg", files);

		switch (type)
		{
		case REGION:
		case REGION_ELLIPSE:
		case YOLO:
		  {
			  for (int i = 0; i < files.size(); i++)
			  {
			  	 char full_name[512];
			  	 sprintf(full_name, "%s/%s", brow_path, files[i].name);
			  	 cv::Mat frame = cv::imread(full_name);
			  	 cpAreaResize(800 * 600, frame, frame);
				 //
				 CvAvgCompConf *blobs = NULL;
				 cv::Rect frame_roi = poly.poly.size() ? AdjustRect(roi, frame.size()) : cv::Rect(0, 0, frame.cols, frame.rows);
				 int num = detect2(pY, frame(frame_roi), &blobs, 0.25f, 0.4f, 1);

				 if (blobs)
				 {
					 for (int j = 0; j < num; j++)
					 {
						 CvAvgCompConf *pO = &blobs[j];
						 pO->rect.x += frame_roi.x; pO->rect.y += frame_roi.y;
						 pO->draw_fast(frame, target_colors[pO->label % MAX_COLOR_NUMBER], 2, names, NULL, 0);
					 }
					 //delete []blobs;
					 free_objs(blobs);
					 //
					 if (0)
					 {
						 char result[4096];
						 int num = detect_string(pY, full_name, result, 0.25f, 0.4f, 1);
						 printf("%s\n", result);
					 }
				 }
				 poly.draw(frame, CV_RGB(255, 255, 255), 1);
				 //
				 cv::imshow("frame", frame);
				 char ch = cv::waitKey(0);
				 if (ch == 27)
					 break;
			  }
			  //
		  }
		  break;

		case ARRAY_SOFTMAX:
		  {
			 for (int i = 0; i < files.size(); i++)
			 {
			     char full_name[512];
			     sprintf(full_name, "%s/%s", brow_path, files[i].name);
			     cv::Mat frame = cv::imread(full_name);
			     cpAreaResize(800 * 600, frame, frame);
			     //
				 ObjectContour *blobs = NULL;
			     cv::Mat fd_segment;
			     int num = detect_segment(pY, frame, &blobs, 0.75f);
			     if (blobs)
			     {
			 	    for (int j = 0; j < num; j++)
			 	    {
			 	  	  ObjectContour *pO = &blobs[j];
			 	  	  pO->draw(frame, names, target_colors[pO->label % MAX_COLOR_NUMBER], 2);
			 	    }
			 	    //delete []blobs;
			 	    free_contours(blobs);
			 	    //
					if (0)
					{
						static char result[4096];
						int num = detect_seg_string(pY, full_name, result, 0.25f, 0.4f, 1);
						printf("%s\n", result);
					}
			     }
				 cv::imshow("frame", frame);
				 char ch = cv::waitKey(0);
				 if (ch == 27)
					 break;
			 }
		  }
		  break;

		case ARRAY_FIELD:
		case ARRAY_FIELDMC:
		  {
			  for (int i = 0; i < files.size(); i++)
			  {
			      char full_name[512];
			      sprintf(full_name, "%s/%s", brow_path, files[i].name);
			      cv::Mat frame = cv::imread(full_name);
			      cpAreaResize(800 * 600, frame, frame);
			      //
				  CvAvgCompConf *blobs = NULL;
				  cv::Rect frame_roi = poly.poly.size() ? AdjustRect(roi, frame.size()) : cv::Rect(0, 0, frame.cols, frame.rows);
				  int num = detect_field(pY, frame, &blobs, 1, 0.25f, 2.5f, 0.4f, 0.5f, 0.005f);

				  if (blobs)
				  {
					  for (int j = 0; j < num; j++)
					  {
						  CvAvgCompConf *pO = &blobs[j];
						  pO->rect.x += frame_roi.x; pO->rect.y += frame_roi.y;
						  pO->draw_fast(frame, target_colors[pO->label % MAX_COLOR_NUMBER], 2, names, NULL, 1);
					  }
					  //delete []blobs;
					  free_objs(blobs);
				  }
				  poly.draw(frame, CV_RGB(255, 255, 255), 1);
				  //
				  cv::imshow("frame", frame);
				  char ch = cv::waitKey(0);
				  if (ch == 27)
					  break;
			  }
			  char jpeg_path[512];
			  sprintf(jpeg_path, "%s/jpegImages/", data_path);
			  float mAP = field_mAP(pY, jpeg_path, 0.6f, 0.4f, 0.5f, NULL);
			  _getch();
		  }
		  break;
		}
	}
	free_dhandle(pY);

	return 0;
}

