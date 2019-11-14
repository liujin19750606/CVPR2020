
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HELLODLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HELLODLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef HELLODLL_EXPORTS
#define HELLODLL_API __declspec(dllexport)
#else
#define HELLODLL_API __declspec(dllimport)
#endif

// This class is exported from the HelloDll.dll
class HELLODLL_API CHelloDll {
public:
	CHelloDll(void);
	// TODO: add your methods here.
};

extern HELLODLL_API int nHelloDll;

HELLODLL_API int fnHelloDll(void);

HELLODLL_API DWORD64 create_dhandle();
HELLODLL_API int free_dhandle(DWORD64 pYo);

HELLODLL_API int load_path(DWORD64 pYo, char *model_path);

HELLODLL_API int get_output_type(DWORD64 pYo);

HELLODLL_API void get_names(DWORD64 pYo, std::vector<std::string> &names);

HELLODLL_API void set_poly_mask(DWORD64 pYo, cv::Rect *pRoi, std::vector<cv::Point2f> &poly);

HELLODLL_API int detect2(DWORD64 pYo, cv::Mat frame, CvAvgCompConf **objs, float prob_thresh, float nms_merge_iou, int edge);
HELLODLL_API int detect_string(DWORD64 pYo, char *img_name, char *result, float prob_thresh, float nms_merge_iou, int edge);

HELLODLL_API void free_objs(CvAvgCompConf *objs);

HELLODLL_API int detect_segment(DWORD64 pYo, cv::Mat frame, ObjectContour **objs, float prob_thresh);
HELLODLL_API int detect_seg_string(DWORD64 pYo, char *img_name, char *result, float prob_thresh, float nms_merge_iou, int edge);

HELLODLL_API void free_contours(ObjectContour *objs);

HELLODLL_API int detect_field(DWORD64 pYo, cv::Mat frame, CvAvgCompConf **objs, float scale, float thresh, float nms_pts_dist, float nms_merge_iou, float max_e, float ab0);
HELLODLL_API float field_mAP(DWORD64 pYo, char *jpg_path, float prob_thresh, float nms_merge_iou, float iou_thod, float *pmHP);
