#ifndef __FILEPROC__
#define __FILEPROC__

#include <windows.h>
#include <stdio.h>

#define MAX_FILE_LINE_LEN  512//һ���ַ����512�ֽ�

#define BUFFER_SIZE (0x2000)
#define MAX_FILE_SIZE (20*1024*1024)

#include "iostream"
#include <vector>
//using namespace std;
//typedef char FileVector[MAX_PATH];
typedef struct
{ char name[MAX_PATH];
} FileVector;

//���ļ����뵽һ������buffer����
int File2Buffer(char *fname,char *buffer);
//�ѻ���buffer[0..file_size-1]��������ݱ��浽�ļ�
int Buffer2File(char *buffer,int size,char *fname);
//��ȡ�ļ���С
unsigned long get_file_size(const char *path);

//�������������ռ��ظ���ɴ������⽨����һ������
BOOL GetOpenFileName2(LPSTR szName,LPSTR filter,LPSTR dir,char *title=NULL);
//���ļ��Ի���
BOOL GetOpenFileName(LPSTR szName,LPSTR filter,LPSTR dir,char *title=NULL);
//�����ļ��Ի���
BOOL GetSaveFileName2(LPSTR szName,LPSTR filter,LPSTR dir);

//�쿴�ļ��Ƿ���ڲ��õ��ļ���ϢFindFileData
int FileExist(LPSTR szName,WIN32_FIND_DATAA &FindFileData);
int FileExist(LPSTR szName,WIN32_FIND_DATAA *FindFileData=NULL);
int FileExist_slash(LPSTR szName,WIN32_FIND_DATAA *FindFileData=NULL);
//Ѱ���ļ����¼������ļ����µ����д�postfix��׺�����ļ�
int AllFilesRec(char *path_name, char *postfix, std::vector <FileVector> &files);
//���ļ�fp��ɨ��һ�м�¼��Line����
char* ScanALine(FILE *fp,char *Line);
//ɨ��һ���ı��ļ���ÿһ���ַ�����¼����
int ScanAFile(char *file_name,char Lines[][MAX_FILE_LINE_LEN]);

//�����ǻ�õ�ǰ·����API�������÷���(ע��Ҫ#include<windows.h>)
//char CurDirectory[MAX_STRING_LEN];
//GetCurrentDirectory(MAX_STRING_LEN,CurDirectory);

//�������ж��ļ��Ƿ���ڵ�API�����÷�
//char szName[129];
//if(GetOpenFileName(szName,//����ļ���
//                   "Chess files (*.cnd)\0*.cnd\0\0",//�ļ�������
//				   NULL))//�ļ�Ŀ¼
//{ WIN32_FIND_DATA FindFileData;
//  if(!FileExist(szName,FindFileData))SaveBoard(szName);//�ļ������ڲű���
//   else
//   if(bn("�ļ��Ѿ������Ƿ񸲸�?","����!"))SaveBoard(szName);
//}
//�����õ�·��path_name��,��չ��ΪfilterExt�������ļ�files[0..nCount-1],�����ļ�����nCount
//�����ÿؼ�,��ҪMFC��һ��,����͵����µ���://"D:\\*.txt"
int BrowPathFiles(char *path_name, char *filterExt, std::vector<std::string> &files);
int BrowPathFiles(char *path_name, char *filterExt, std::vector<FileVector> &files);
int BrowPathFiles_old(char *path_name,char *filterExt,char files[][MAX_PATH]);
int BrowPathFiles(char *path_name,char *filterExt,char files[][MAX_PATH]);
std::vector<FileVector> BrowPathFiles(char *path_name, char *filterExt);
typedef DWORD (CALLBACK *FILE_BROW_DEAL_PROC) (void *pParams);
int BrowPathFiles(char *path_name,char *filterExt,FILE_BROW_DEAL_PROC one_loop_fun);
typedef DWORD (CALLBACK *FILE_BROW_DEAL_PROC2) (FileVector *filev,void *pParams);
int BrowPathFilesFun(char *path_name,char *filterExt,FILE_BROW_DEAL_PROC2 one_loop_fun,void *pParam);
 //�÷�����:
 /*if(GetOpenFileName(szName,//����ļ���
                    "Chess files (*.cnd; *.fen)\0*.cnd; *.fen\0\0",//�ļ�������
				    NULL))//�ļ�Ŀ¼
 { char path_name[256],fname[150],files[200][MAX_PATH];
   ExtractFileName(szName,path_name,fname);//����·��path_name���ļ���fname
   int i,FileNumber=BrowPathFiles(path_name,"*.fen",files);
   //�õ��ļ����б��ַ���ListFiles
   char ListFiles[2048]="",buf[256];
   sprintf(buf,"��%d���ļ�\n",FileNumber);
   strcat(ListFiles,buf);
   for(i=0;i<FileNumber;i++)
   { sprintf(buf,"%d: %s\n",i,files[i]);
     strcat(ListFiles,buf);
   }
   bn(ListFiles,"");
 }*/
//Ѱ���ļ���·���µ����ļ��в�����std::vector<FileVector>
int BrowPathDirectory(char *path_name, std::vector<FileVector> &files);

//ѡ��·���Ի���
int SelectDirDlg(char Dir[]);

//���ļ���·������ȡ�ļ���������ļ�·������ȡ�ļ���
void PicDocNameFromDocRoot(char *root_name,char *doc_name);

void GetModulePath(char *module_path,int back_debug=0);

//��ȡ�ļ�fp��һ�е�str
int fread_line(FILE *fp,char *str);

//ɾ������Ŀ¼
int DeleteOneDirectory(char *full_path, int del_dir=1);
//ɾ��path������Ŀ¼,����!
void DelAllDirectorys(char *path);

//���ļ��Ի���
BOOL GetOpenFileNameMult(std::vector <FileVector> &files, LPSTR filter, LPSTR init_dir, char *out_path, int order, char *title = NULL);

//������·�����������·��
int update_path(char *path,char *work_path);

//�����༶Ŀ¼
int CreatMultDir(char *pszDir);

int GetLatestPathFiles(char *path_name, char *filterExt, char *latest_name, char *start_str);

///Ŀ¼�Ƿ���ڵļ�飺
int  CheckFolderExist(char *strPath);

void get_full_directory(char *fname, char *full_name);

int find_names(std::vector<std::string> &names, char *file_name);

int save_names(char *data_path, char *train_path);

int BrowTreePathFiles(char *path_name, char *filterExt, std::vector<std::string> &files);

int search_shortest_name(std::vector<FileVector> &files);
int search_shortest_name(std::vector<std::string> &files);

#endif