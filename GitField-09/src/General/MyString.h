#ifndef __MYSTRING2__
#define __MYSTRING2__

#define MAX_STRING_LEN 256
extern char strbuf[MAX_STRING_LEN];

#ifdef __cplusplus  
extern "C" {  
#endif  
//��str�����ȡ�ַ���res_str
int str_find(char *str,char *sub_str,char *end_str,char *res_str);
//��str�����ȡsub_strǰ����ַ���res_str
//int str_find(char *str,char *sub_str,char *res_str);

//��str1��ȡ�ӵ�i���ַ���ʼ����Ϊn�����ִ�str2
void strnicpy(char* str1,char* str2,int i,int n);
//�滻�ַ���str+nΪsub_str
void ni_replace(char *str,int i,int n,char *sub_str);
 void n_replace(char *str,int n,char *sub_str);
//�滻str����ִ�sub_strΪnew_str
char *str_replace(char *str,char *sub_str,char *new_str);
//��str����ִ��������new_str
//int str_append(char *str,char *new_str,int mode=0);

//���ַ��������д�д�ַ�ת����Сд
void str_no_cap(char *str);

//��ȡ·�����ļ���
int ExtractFileName(char* filename,char* dir,char* fname);
//��ȡ���һ��·����
int ExtractLastPath(char* filename,char* dir,char* lastpath);
int ExtractLastPath2(char *filename,char* dir,char* lastpath);
//�滻�ļ���,����·��
int replace_file_name(char* filename,char* replace_name,char* new_name);

//��ȡ�ļ�������չ��
int ExtractFileExt(char* filename,char *name,char *ext);
int ExtractPathFileExt(char* path_name,char *path,char *name,char *ext);
//���ļ���filename������Ϻ�׺Append,������һ���µ���չ��NewExt
int AppendFileName(char* filename,char *Append,char *NewExt,char *NewName);
//���ļ���filename������Ϻ�׺Append
//int AppendFileName(char* filename,char *Append,char *NewName);
//��·��path�������ļ���filename���������չ��NewExt,�õ�NewName
int AppendFileName2(char *path,char *filename,char *NewExt,char *NewName);

/*Replaces a file's extension, which is assumed to be everything after the
last dot ('.') character.
@param file the name of a file
@param extn a new extension for \a file; should not include a dot (i.e.
	\c "jpg", not \c ".jpg") unless the new file extension should contain
	two dots.
@return Returns a new string formed as described above.  If \a file does
	not have an extension, this function simply adds one.*/
extern char* replace_extension( const char* file, const char* extn );
//void replace_extension( const char* file, const char* extn, char *new_file );
//Ѱ��ȫ·���ַ����ļ�����ĩβ�ļ���ָ��
char *search_last_fname(char *fullname);

/*A function that removes the path from a filename.  Similar to the Unix
basename command.
@param pathname a (full) path name
@return Returns the basename of \a pathname.*/
extern char* basename2( char* pathname );

//���ַ���str�ָ��new_str,stack[0..return-1]
int SplitString(char *str,char *needle,
				char *new_str,char **stack);
int SplitString2(char *str,char needle);
int remove_needle(char *new_str,char **stack,int n);
int SplitStringA(char *str, char *needle, char *new_str, char **stack);

//�ж��ַ���������
int is_num_char(char c);
//�ж��ַ�������ĸ
int is_cap_char(char c);

//�ߵ��ַ���
void str_reverse(char *str,int n);

//Bufferĩβ����\0��������ַ���
void copy2(char *buf,int n1,char *str);

//�ַ����ֶγ��ַ�����
int create_split_strings(char *str,char *sp_strs[64]);
//�ͷ��ַ�����
void release_split_strings(char *sp_strs[64],int n);

//��ids[0..ni-1]+str1,sp,str2,sp,str3�����buffer[]
int compress_strings(int *ids,int ni,
				     char *str1,char *str2,char *str3,
				     char *sp,
				     char *buffer);

int extract_strings(char *buffer,
					int *ids,int ni,
					char *str1,char *str2,char *str3,char *sp);

//���ַ����Ի��н�β�ĳ���\0��β
void end_str(char *str);

void get_path(char *rel_path, char *work_path, char *full_path);

//�����޼��ַ���str�ո��str2
void trim(char *str,char *str2);

#ifdef __cplusplus  
}
#endif  

#endif
