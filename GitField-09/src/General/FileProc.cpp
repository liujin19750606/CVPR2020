#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include "FileProc.h"
#include "MyString.h"

#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"user32.lib")

//���ļ����뵽һ������buffer����
int File2Buffer(char *fname,char *buffer)
{ FILE *fp=fopen(fname,"rb");
  if(fp)
  { int read_n;
    char *pOffset=buffer;
	while(read_n=fread(pOffset,1,BUFFER_SIZE,fp))
    { pOffset+=read_n;
    }
	int file_size=pOffset-buffer;
	fclose(fp);
	return file_size;
  } else return 0;
}
//�ѻ���buffer[0..file_size-1]��������ݱ��浽�ļ�
int Buffer2File(char *buffer,int size,char *fname)
{ FILE *fp=fopen(fname,"wb");
  if(fp)
  { int write_n;
	char *pOffset=buffer;
	while(1)
	{ int left_size = size-(pOffset-buffer);
	  int write_ct = BUFFER_SIZE < left_size ? BUFFER_SIZE : left_size;//MIN(size-(pOffset-buffer),BUFFER_SIZE);
	  write_n=fwrite(pOffset,1,write_ct,fp);
	  pOffset+=write_n;
	  if(pOffset>=buffer+size)break;
	}
	int write_size = pOffset-buffer;
	assert(write_size==size);
	fclose(fp);
	return write_size;
  } else return 0;
}
//��ȡ�ļ���С
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;  
    FILE *fp;  
    fp = fopen(path, "r");  
    if(fp == NULL)  
        return filesize;  
    fseek(fp, 0L, SEEK_END);  
    filesize = ftell(fp);  
    fclose(fp);  
    return filesize;  
}


//�������������ռ��ظ���ɴ������⽨����һ������
BOOL GetOpenFileName2(LPSTR szName,LPSTR filter,LPSTR dir,char *title)
{ return GetOpenFileName(szName,filter,dir,title);
}

//���ļ��Ի���
BOOL GetOpenFileName(LPSTR szName,LPSTR filter,LPSTR dir,char *title)
{	OPENFILENAMEA ofn;
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = NULL;//ghApp;
	ofn.lpstrFilter       = NULL;
	//ofn.lpstrFilter       = "Video files (*.mpg; *.mpeg; *.mp4)\0*.mpg; *.mpeg; *.mp4\0\0";
	ofn.lpstrFilter=filter;// "Chess files (*.cnd)\0*.cnd\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	*szName = 0;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrInitialDir=dir;//   = NULL;
	ofn.lpstrTitle        = title;
	ofn.lpstrFileTitle    = NULL;
	//ofn.lpstrDefExt       = "MPG";
	ofn.lpstrDefExt       = "CND";
	//ofn.Flags             = OFN_FILEMUSTEXIST|OFN_READONLY|OFN_PATHMUSTEXIST;
	ofn.Flags             = OFN_PATHMUSTEXIST;
//bn(GetOpenFileName((LPOPENFILENAME)&ofn));
	return ::GetOpenFileName((LPOPENFILENAME)&ofn);

} // GetClipFileName

//����������Buf[iLo,..,iHi]��������,ԭ����delphi���濴����һ�����򷽷�
void QuickSort2(std::vector <FileVector> &files, int iLo, int iHi, int order)
{ int Lo, Hi;
  Lo = iLo;Hi = iHi;
  FileVector Midval = files[(Lo + Hi)/2];
  if(order==0)//˳��
  { do
	{ while(strcmp(files[Lo].name,Midval.name)<0)Lo++;//��������˳���moves[Lo].Value�Ͳ���,ֱ���ҳ�һ����λ
      while(strcmp(files[Hi].name,Midval.name)>0)Hi--;//��������˳���moves[Hi].Value�Ͳ���,ֱ���ҳ�һ����λ
      if(Lo<= Hi)//���ָߵ�һ�Բ���������˳��ľ�
	  { if(Lo< Hi)//��������˳��
	    { FileVector SwapColor=files[Lo];//< bug<
          files[Lo]=files[Hi];//< bug<
          files[Hi]=SwapColor;//< bug<
	    }
        Lo++;Hi--;
	  }
	} while(Lo<=Hi);
  } else//����
  {
    do
	{ while(strcmp(files[Lo].name,Midval.name)>0)Lo++;//��������˳���moves[Lo].Value�Ͳ���,ֱ���ҳ�һ����λ
      while(strcmp(files[Hi].name,Midval.name)<0)Hi--;//��������˳���moves[Hi].Value�Ͳ���,ֱ���ҳ�һ����λ
      if(Lo<= Hi)//���ָߵ�һ�Բ���������˳��ľ�
	  { if(Lo< Hi)//��������˳��
	    { FileVector SwapColor=files[Lo];//< bug<
          files[Lo]=files[Hi];//< bug<
          files[Hi]=SwapColor;//< bug<
	    }
        Lo++;Hi--;
	  }
	} while(Lo<=Hi);
  }
  //����Ϊֹ,[iLo~Hi]>[Lo~iHi]
  if(Hi>iLo)QuickSort2(files,iLo,Hi,order);//��[iLo~Hi]        ����ݹ�����
  if(Lo<iHi)QuickSort2(files,Lo,iHi,order);//��        [Lo~iHi]����ݹ�����
}
//���ļ��Ի���
BOOL GetOpenFileNameMult(std::vector <FileVector> &files, LPSTR filter, LPSTR init_dir, char *out_path, int order, char *title)
{	OPENFILENAMEA ofn;
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = NULL;//ghApp;
	ofn.lpstrFilter       = NULL;
	//ofn.lpstrFilter       = "Video files (*.mpg; *.mpeg; *.mp4)\0*.mpg; *.mpeg; *.mp4\0\0";
	ofn.lpstrFilter=filter;// "Chess files (*.cnd)\0*.cnd\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	char szName[256*MAX_PATH];
	*szName = 0;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = sizeof(szName);//MAX_PATH;
	ofn.lpstrInitialDir=init_dir;//   = NULL;
	ofn.lpstrTitle        = title;
	ofn.lpstrFileTitle    = NULL;
	//ofn.lpstrDefExt       = "MPG";
	ofn.lpstrDefExt       = "CND";
	//ofn.Flags             = OFN_FILEMUSTEXIST|OFN_READONLY|OFN_PATHMUSTEXIST;
	ofn.Flags             = OFN_EXPLORER | OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST;
	ofn.lpstrFile[0] = '\0';
	int res =  ::GetOpenFileNameA((LPOPENFILENAMEA)&ofn);
	//
	char *p = szName + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�
	while( *p )
	{ FileVector a;
	  strcpy(a.name,p);
	  files.push_back(a);
      //lstrcat(szFileName, szPath);  //���ļ�������·��  
      //lstrcat(szFileName, p);    //�����ļ���  
      //lstrcat(szFileName, TEXT("\n")); //����   
      p += lstrlenA(p) +1;     //������һ���ļ�
	}
	//
	if(files.size()>0)
	if(order>=0)//-1�ǲ�����,0��˳��,1������
	{ QuickSort2(files,0,files.size()-1,order);
#ifdef _DEBUG
	  int k;
	  for(k=0;k<(int)files.size()-1;k++)
	  { if(order==0)//˳��
		 assert(strcmp(files[k].name,files[k+1].name)<=0);//˳����֤
	    else//����
		 assert(strcmp(files[k].name,files[k+1].name)>=0);//������֤
	  }
	  for(k=0;k<(int)files.size();k++)
	  { printf("%s\n",files[k].name);
	  }
#endif
	}
	//
	if(out_path)//���·��out_path
	{ if(files.size()>1)
	  { 
	    strcpy(out_path,szName);//
	    strcat(out_path,"\\");
	  } else
	  { ExtractFileName(szName,out_path,NULL);
	  }
	}
	//
	return res;

} // GetClipFileName

//�����ļ��Ի���
/*BOOL GetSaveFileName(LPSTR szName,LPSTR filter,LPSTR dir)
{	OPENFILENAME ofn;
    ZeroMemory(&ofn,sizeof(SAVEFILENAME));
	ofn.lStructSize       = sizeof(SAVEFILENAME);
	ofn.hwndOwner         = NULL;//ghApp;
	ofn.lpstrFilter       = NULL;
	//ofn.lpstrFilter       = "Video files (*.mpg; *.mpeg; *.mp4)\0*.mpg; *.mpeg; *.mp4\0\0";
	ofn.lpstrFilter=filter;// "Chess files (*.cnd)\0*.cnd\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	*szName = 0;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrInitialDir=dir;//   = NULL;
	ofn.lpstrTitle        = NULL;
	ofn.lpstrFileTitle    = NULL;
	//ofn.lpstrDefExt       = "MPG";
	ofn.lpstrDefExt       = "CND";
	//ofn.Flags             = OFN_FILEMUSTEXIST|OFN_READONLY|OFN_PATHMUSTEXIST;
	ofn.Flags             = OFN_PATHMUSTEXIST;
//bn(GetOpenFileName((LPOPENFILENAME)&ofn));
	return GetOpenFileName((LPOPENFILENAME)&ofn);
} // GetClipFileName*/
BOOL GetSaveFileName2(LPSTR szName,LPSTR filter,LPSTR dir)
{	OPENFILENAMEA ofn;
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = NULL;//ghApp;
	ofn.lpstrFilter       = NULL;
	//ofn.lpstrFilter       = "Video files (*.mpg; *.mpeg; *.mp4)\0*.mpg; *.mpeg; *.mp4\0\0";
	ofn.lpstrFilter=filter;// "Chess files (*.cnd)\0*.cnd\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	*szName = 0;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrInitialDir=dir;//   = NULL;
	ofn.lpstrTitle        = NULL;
	ofn.lpstrFileTitle    = NULL;
	//ofn.lpstrDefExt       = "MPG";
	ofn.lpstrDefExt       = "CND";
	//ofn.Flags             = OFN_FILEMUSTEXIST|OFN_READONLY|OFN_PATHMUSTEXIST;
	ofn.Flags             = OFN_PATHMUSTEXIST;
//bn(GetOpenFileName((LPOPENFILENAME)&ofn));
	return ::GetSaveFileName((LPOPENFILENAME)&ofn);
} 
//�쿴�ļ��Ƿ���ڲ��õ��ļ���ϢFindFileData
int FileExist(LPSTR szName,WIN32_FIND_DATAA &FindFileData)
{ //WIN32_FIND_DATAA FindFileData;
  HANDLE hFind;
  hFind = FindFirstFileA(szName,&FindFileData);
  return (hFind != INVALID_HANDLE_VALUE);
}
int FileExist(LPSTR szName,WIN32_FIND_DATAA *FindFileData)
{ char fname[512];
  strcpy(fname,szName);
  int len = strlen(szName);
  while (szName[len - 1] == '\\' || szName[len - 1] == '/')len--;
  fname[len]='\0';
  
  WIN32_FIND_DATAA _FindFileData;
  HANDLE hFind;
  hFind = FindFirstFileA(fname,FindFileData ? FindFileData : &_FindFileData);
  return (hFind != INVALID_HANDLE_VALUE);
}
int FileExist_slash(LPSTR szName,WIN32_FIND_DATAA *FindFileData)
{
	WIN32_FIND_DATAA _FindFileData;
	HANDLE hFind;
	char szNameCut[MAX_PATH];
	strcpy(szNameCut,szName);
	szNameCut[strlen(szName)-1] = '\0';
	hFind = FindFirstFileA(szNameCut,FindFileData ? FindFileData : &_FindFileData);
	return (hFind != INVALID_HANDLE_VALUE);
}
///////////////////
//���ļ�fp��ɨ��һ�м�¼��Line����
char* ScanALine(FILE *fp,char *Line)
{ int count=0;
  char c;
  while(1)
  { c=fgetc(fp);
    if(c=='\n')
	{ Line[count]='\0';
	  break;
	}
    Line[count]=c;
	count++;
  }
  return Line;
}
//ɨ��һ���ı��ļ���ÿһ���ַ�����¼����
int ScanAFile(char *file_name,char Lines[][MAX_FILE_LINE_LEN])
{ FILE *fp=fopen(file_name,"r");
  if(fp)
  { int count=0;//,read_num;
    char *p;
    do
	{ //read_num=fscanf(fp,"%s",Lines[count]);
	  p=fgets(Lines[count],MAX_FILE_LINE_LEN,fp);
	  count++;
	} while(p);
	//
	fclose(fp);
	return count;
  } return -1;
}
//�����ǻ�õ�ǰ·����API�������÷���(ע��Ҫ#include<windows.h>)
//char CurDirectory[MAX_STRING_LEN];
//GetCurrentDirectory(MAX_STRING_LEN,CurDirectory);

//�������ж��ļ��Ƿ���ڵ�API�����÷�
//char szName[129];
//if(GetOpenFileName(szName,//����ļ���
//                   "Chess files (*.cnd)\0*.cnd\0\0",//�ļ�������
//				   NULL))//�ļ�Ŀ¼
//{ WIN32_FIND_DATAA FindFileData;
//  if(!FileExist(szName,FindFileData))SaveBoard(szName);//�ļ������ڲű���
//   else
//   if(bn("�ļ��Ѿ������Ƿ񸲸�?","����!"))SaveBoard(szName);
//}

//�����õ�·��path_name��,��չ��ΪfilterExt�������ļ�files[0..nCount-1],�����ļ�����nCount
//�����ÿؼ�,��ҪMFC��һ��,����͵����µ���://"D:\\*.txt"
int BrowPathFiles_old(char *path_name,char *filterExt,char files[][MAX_PATH])
{ char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,filterExt);
  WIN32_FIND_DATAA FileData; 
  HANDLE hSearch;
  int nCount=0;
  BOOL fFinished = FALSE;
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
	while (FindNextFileA(hSearch, &FileData))
	{ strcpy(files[nCount],FileData.cFileName);nCount++;}
    if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL,"Couldn't find next file.","��ã�",MB_OK);}
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{ MessageBoxA(NULL,"Couldn't close search handle.","��ã�",MB_OK); }
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  { MessageBoxA(NULL,"No file found.","��ã�",MB_OK); }

//  char buf[20]="";
//  sprintf(buf,"��%d��TXT�ļ�",nCount);
//  MessageBoxA(NULL,buf,"��ã�",MB_OK);
  return nCount;
}
int BrowPathFiles(char *path_name, char *filterExt, std::vector<std::string> &files)
{
	char path_ext_name[MAX_PATH];
	//��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
	strcpy(path_ext_name, path_name); strcat(path_ext_name, "*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount = 0;
	BOOL fFinished = FALSE;
	char fname[512], ext_name[512];
	str_no_cap(filterExt);
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
		while (FindNextFileA(hSearch, &FileData))
		if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30)
		{
			ExtractFileExt(FileData.cFileName, fname, ext_name);
			str_no_cap(ext_name);
			if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
			{
				files.push_back(FileData.cFileName);
			}
		}
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{ //MessageBoxA(NULL,"Couldn't find next file.","��ã�",MB_OK);
		}
		// Close the search handle. 
		if (!FindClose(hSearch))
		{
			//	MessageBoxA(NULL,"Couldn't close search handle.","��ã�",MB_OK);
		}
	}
	else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
	{ //MessageBoxA(NULL,"No file found.","��ã�",MB_OK);
	}

	return files.size();
}
int BrowPathFiles(char *path_name, char *filterExt, std::vector<FileVector> &files)
{ 
  char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  int nCount=0;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(filterExt);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
	while (FindNextFileA(hSearch, &FileData))
	if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30)
	{ FileVector afile;
	  ExtractFileExt(FileData.cFileName,fname,ext_name);
	  str_no_cap(ext_name);
	  if (strcmp(ext_name, ".") && (strcmp(filterExt, ext_name) == 0 || strcmp(filterExt, "*.*") == 0))
	  { strcpy(afile.name,FileData.cFileName);
	    files.push_back(afile);
	  }
	}
    if (GetLastError() != ERROR_NO_MORE_FILES)
	{ //MessageBoxA(NULL,"Couldn't find next file.","��ã�",MB_OK);
	}
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{ 
	//	MessageBoxA(NULL,"Couldn't close search handle.","��ã�",MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  { //MessageBoxA(NULL,"No file found.","��ã�",MB_OK);
  }

  return files.size();
}
int BrowPathFiles(char *path_name,char *filterExt,char files[][MAX_PATH])
{ char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  int nCount=0;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(filterExt);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
	while (FindNextFileA(hSearch, &FileData))
	if((FileData.dwFileAttributes&0xff)!=FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30) 
	{ ExtractFileExt(FileData.cFileName,fname,ext_name);
	  str_no_cap(ext_name);
	  if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
	  { strcpy(files[nCount],FileData.cFileName);nCount++;
	  }
	}
	if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{
		MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  {
	  MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
  }

//  char buf[20]="";
//  sprintf(buf,"��%d��TXT�ļ�",nCount);
//  MessageBoxA(NULL,buf,"��ã�",MB_OK);
  return nCount;
}

std::vector<FileVector> BrowPathFiles(char *path_name, char *filterExt)
{
	std::vector<FileVector> files;
  char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  int nCount=0;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(filterExt);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
	while (FindNextFileA(hSearch, &FileData))
	if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30)
	{ FileVector afile;
	  ExtractFileExt(FileData.cFileName,fname,ext_name);
	  str_no_cap(ext_name);
	  if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
	  { strcpy(afile.name,FileData.cFileName);
	    files.push_back(afile);
	  }
	}
	if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{
		MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  {
	  MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
  }

  return files;
}
//typedef DWORD (CALLBACK *FILE_BROW_DEAL_PROC) (void *pParams);
int BrowPathFiles(char *path_name,char *filterExt,FILE_BROW_DEAL_PROC one_loop_fun)
{ int count=0;
  char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(filterExt);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	while (FindNextFileA(hSearch, &FileData))
	if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30)
	{ FileVector afile;
	  ExtractFileExt(FileData.cFileName,fname,ext_name);
	  str_no_cap(ext_name);
	  if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
	  { strcpy(afile.name,FileData.cFileName);
	    //files.push_back(afile);
	    if(one_loop_fun)
		{ one_loop_fun(afile.name);
		  count++;
		}
	  }
	}
	if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{
		MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  {
	  MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
  }

  return count;
}
//typedef DWORD (CALLBACK *FILE_BROW_DEAL_PROC2) (FileVector *filev,void *pParams);
int BrowPathFilesFun(char *path_name,char *filterExt,FILE_BROW_DEAL_PROC2 one_loop_fun,void *pParam)
{ int count=0;
  char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(filterExt);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	while (FindNextFileA(hSearch, &FileData))
	if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY && (FileData.dwFileAttributes & 0xff) != 0x30)
	{ FileVector afile;
	  ExtractFileExt(FileData.cFileName,fname,ext_name);
	  str_no_cap(ext_name);
	  if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
	  { strcpy(afile.name,FileData.cFileName);
	    //files.push_back(afile);
	    if(one_loop_fun)
		{ one_loop_fun(&afile,pParam);
		  count++;
		}
	  }
	}
	if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{
		MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  ;//{ MessageBoxA(NULL,"No file found.","��ã�",MB_OK); }

  return count;
}
//Ѱ���ļ���·���µ����ļ��в�����std::vector<FileVector>
int BrowPathDirectory(char *path_name, std::vector<FileVector> &files)
{ 
/*	std::vector<FileVector> files;*/
	char path_ext_name[MAX_PATH];
	//��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
	strcpy(path_ext_name,path_name);
	strcat(path_ext_name,"*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount=0;
	BOOL fFinished = FALSE;
	char fname[512],ext_name[512];
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
		while (FindNextFileA(hSearch, &FileData))
		if ((FileData.dwFileAttributes & 0xff) == FILE_ATTRIBUTE_DIRECTORY || (FileData.dwFileAttributes & 0xff) == 0x30)
			{
				FileVector afile;
				ExtractFileExt(FileData.cFileName,fname,ext_name);
				str_no_cap(ext_name);
				if(strcmp(ext_name,".")&&strcmp(ext_name,".."))
				{ 
					strcpy(afile.name,FileData.cFileName);
					files.push_back(afile);
				}
			}
			if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
			// Close the search handle. 
			if (!FindClose(hSearch)) 
			{ 
				MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
			}
	} 
	else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
	{ 
		MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
	}	
	return files.size();
}
int BrowTreePathFiles(char *path_name, char *filterExt, std::vector<std::string> &files)
{
	std::vector<std::string> files_local;
	char ext[8];
	strcpy(ext, filterExt);
	BrowPathFiles(path_name, ext, files_local);
	for (int i = 0; i < files_local.size(); i++)
	{
		files.push_back(std::string(path_name) + "/" + files_local[i]);
	}
	std::vector<FileVector> paths;
	BrowPathDirectory(path_name, paths);
	char sub_path[512];
	for (int i = 0; i < paths.size(); i++)
	{
		sprintf(sub_path, "%s/%s/", path_name, paths[i].name);
		BrowTreePathFiles(sub_path, filterExt, files);
	}
	return files.size();
}
//Ѱ���ļ����¼������ļ����µ����д�postfix��׺�����ļ�
int AllFilesRec(char *path_name, char *postfix, std::vector <FileVector> &files)
{
	char path_ext_name[MAX_PATH];
  //��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
  strcpy(path_ext_name,path_name);
  strcat(path_ext_name,"*.*");
  WIN32_FIND_DATAA FileData;
  HANDLE hSearch;
  int nCount=0;
  BOOL fFinished = FALSE;
  char fname[512],ext_name[512];
  str_no_cap(postfix);
  // Start searching for path_ext_name files in the current directory. 
  hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
  if (hSearch != INVALID_HANDLE_VALUE)
  { //
	//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
	while (FindNextFileA(hSearch, &FileData))
	if ((FileData.dwFileAttributes & 0xff) == FILE_ATTRIBUTE_DIRECTORY || (FileData.dwFileAttributes & 0xff) == 0x30) //�õ��ļ���
	{ 
		//FileVector file;
		ExtractFileExt(FileData.cFileName,fname,ext_name);
		str_no_cap(ext_name);
		if (strcmp(ext_name, ".") && strcmp(ext_name, "..") && strstr(postfix, ext_name))
		{
			int file_num=0;
			char son_path_ext_name[MAX_PATH];//�������ļ���·����
			strcpy(son_path_ext_name,path_name);
			strcat(son_path_ext_name,FileData.cFileName);
			strcat(son_path_ext_name,"\\");
			file_num = AllFilesRec(son_path_ext_name,postfix,files);
		}
	}
	else//�õ��ļ�
	{ 
		FileVector afile;
		ExtractFileExt(FileData.cFileName,fname,ext_name);
		str_no_cap(ext_name);
		if (strcmp(ext_name, ".") && strstr(postfix, ext_name) && strcmp(ext_name, ""))
		{ 
			strcpy(afile.name,path_name);
			strcat(afile.name,FileData.cFileName);
			files.push_back(afile);
		}
	}
    if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL,"Couldn't find next file.","��ã�",MB_OK);}
    // Close the search handle. 
    if (!FindClose(hSearch)) 
	{
		MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
	}
  } else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
  {
	  MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
  }
  return files.size();
}


#include<shlobj.h>
#pragma comment(lib,"shell32.lib")
//�� �� lgwzxm (���ۿ�) �Ĵ������ᵽ: �� 
//: ��vc�д��ļ��Ի��������CFileDialog���DoModul()ʵ�֣�����֪��ѡ��· 
//: ���Ի������ʵ�֣����λ�����˰��æ�����ڼ�һ�ݸ�ԭ���ߺ���������л 
//: л�ˣ� 
//ѡ��·���Ի���
int SelectDirDlg(char Dir[]) 
{ 
	BROWSEINFOA bi;  
	ITEMIDLIST *pidl;  
	
	bi.hwndOwner = NULL;  
	bi.pidlRoot = NULL;  
	bi.pszDisplayName = Dir;  
	bi.lpszTitle = "ѡ��һ��Ŀ¼";  
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  
	bi.lpfn = NULL;  
	bi.lParam = 0;  
	bi.iImage = 0;  
    
	pidl = SHBrowseForFolderA( &bi ); 
	//Display "Select Folder" dialog box,
	//Get the folder name and convert it into a ITEMLIST   
	//data structure.
	
	if ( pidl)  
	{ 
      if (SHGetPathFromIDListA( pidl, Dir ))// Retrieve folder name from ITEMLIST structure.  
	  { return 1;
	  } else
	  { Dir[0] = 0;
	    return 0;
	  }
	} else
	{ Dir[0] = 0;
	  return 0;
	}
}

//���ļ���·������ȡ�ļ���������ļ�·������ȡ�ļ���
void PicDocNameFromDocRoot(char *root_name,char *doc_name)
{
	std::string strname = root_name;
	std::string::iterator iter = strname.end();
	
	while(iter!=strname.begin() && (*iter)!='\\')
	{
		--iter;
	}
	strname.erase(strname.begin(),iter+1);
	
	strcpy(doc_name,strname.c_str());
	doc_name[strlen(doc_name)]='\0';
}

void GetModulePath(char *module_path,int back_debug)
{
  char module_file[512],exe_name[256];
  GetModuleFileNameA( NULL, module_file, 512 );
  ExtractFileName(module_file,module_path,exe_name);
  str_no_cap(module_path);
  if(strstr(module_path,"debug")||strstr(module_path,"release"))
  {  char back_dir[512],last[256];
	 ExtractLastPath2(module_path,back_dir,last);
	 if (strstr(back_dir, "x64"))
		 ExtractLastPath2(back_dir, module_path, last);//strcat(module_path, "../");
	 else
		 strcpy(module_path, back_dir);
  }
}
//��ȡ�ļ�fp��һ�е�str
int fread_line(FILE *fp,char *str)
{ fgets(str,512,fp);
  int l=strlen(str);
  str[l-1]='\0';
  return l;
}
//ɾ������Ŀ¼
int DeleteOneDirectory(char *full_path,int del_dir)
{ std::vector <FileVector> files;
  BrowPathFiles(full_path,"*.*",files);
  int j;
  for(j=0;j<(int)files.size();j++)
  { char full_file[512];
    sprintf(full_file,"%s\\%s",full_path,files[j].name);
    DeleteFileA(full_file);
  }
  if (del_dir)
    RemoveDirectoryA(full_path);
  printf("path %s is deleted!\n",full_path);
  return files.size();
}
//ɾ��path������Ŀ¼,����!
void DelAllDirectorys(char *path)
{ std::vector <FileVector> paths;
  BrowPathDirectory(path,paths);
  int i;
  for(i=0;i<(int)paths.size();i++)
  { char full_path[512];
    sprintf(full_path,"%s\\%s\\",path,paths[i].name);
	DeleteOneDirectory(full_path);
  }
}

//������·�����������·��
int update_path(char *path,char *work_path)
{ if(!FileExist(path) || !strstr(path,":"))
  { if(work_path && !strstr(path,":"))
    { char full_path[512];
      sprintf(full_path,"%s\\%s",work_path,path);
	  if(FileExist(full_path))
	  { strcpy(path,full_path);//�޸ĳ����·��
	    return 1;
	  }
    }
  }
  return 0;
}

//�����༶Ŀ¼
int CreatMultDir(char *pszDir)
{
	int i = 0;
	int iRet;
	int iLen = strlen(pszDir);

	//��ĩβ��/
	if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
	}

	// ����Ŀ¼
	for (i = 0;i <= iLen;i ++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{ 
			pszDir[i] = '\0';

			//���������,����
			iRet = ::FileExist(pszDir,0);
			if (iRet==0)
			{
				iRet = ::CreateDirectoryA(pszDir,NULL);
				if (iRet != 0)
				{
					//return -1;
				} 
			}
			//֧��linux,������\����/
			pszDir[i] = '/';
		} 
	}
	return 0;
}


int GetLatestPathFiles(char *path_name, char *filterExt, char *latest_name, char *start_str)
{
	int len = strlen(start_str);
	int max = -1;
	char _latest_name[512];
	std::vector<FileVector> files;
	int rn = BrowPathFiles(path_name, filterExt, files);
	for (int i = 0; i < (int)files.size(); i++)
	{
		char name[256],ext[12];
		ExtractFileExt(files[i].name, name, ext);
		char *p = strstr(name, start_str);
		if (p)
		{
			int n = atoi(p + len);
			if (n > max)
			{
				strcpy(_latest_name, files[i].name);
				max = n;
			}
			//printf("%s\n", p + len);
		}
	}
	sprintf(latest_name, "%s/%s", path_name,_latest_name);
	return max;
}

void get_full_directory(char *fname,char *full_name)
{
	if (!strstr(fname, ":"))
	{
		char module_path[512];
		GetModulePath(module_path, 1);
		if (full_name)
		 sprintf(full_name, "%s/%s", module_path, fname);
		else
		{
			char full_name[512];
			sprintf(full_name, "%s/%s", module_path, fname);
			strcpy(fname, full_name);
		}
	}
}

int find_names(std::vector<std::string> &names, char *file_name)
{
	int ret = -1;
	for (int i = 0; i < names.size(); i++)
	{
		if (strstr(file_name, names[i].c_str()))
		{
			ret = i;
			break;
		}
	}
	return ret;
}
int save_names(char *data_path, char *train_path)
{
	std::vector<FileVector> paths;
	BrowPathDirectory(data_path, paths);
	//
	char file_names[512];
	sprintf(file_names, "%s/names.txt", train_path);
	FILE *fp = fopen(file_names, "w");
	if (fp)
	{
		for (int i = 0; i < paths.size(); i++)
		{
			fprintf(fp, "%s\n", paths[i].name);
		}
		fclose(fp);
		return 0;
	}
	else
		return 1;
}


int search_shortest_name(std::vector<FileVector> &files)
{
	int short_len = 999999;
	int fid = -1;
	for (int i = 0; i < files.size(); i++)
	{
		int len = strlen(files[i].name);
		if (len < short_len)
		{
			fid = i;
			short_len = len;
		}
	}
	assert(files.size()==0 || fid>=0);

	return fid;
}
int search_shortest_name(std::vector<std::string> &files)
{
	int short_len = 999999;
	int fid = -1;
	for (int i = 0; i < files.size(); i++)
	{
		int len = strlen(files[i].c_str());
		if (len < short_len)
		{
			fid = i;
			short_len = len;
		}
	}
	assert(files.size() == 0 || fid >= 0);

	return fid;
}