#ifndef __DUILIB_LOG_H_5404E5EF7E1C7AD54D6E0511DABBFC8D__
#define __DUILIB_LOG_H_5404E5EF7E1C7AD54D6E0511DABBFC8D__


#include <Windows.h>

#define  DUILIB_FATAL_LEVEL              0
#define  DUILIB_ERROR_LEVEL              10
#define  DUILIB_WARN_LEVEL               20
#define  DUILIB_INFO_LEVEL               30
#define  DUILIB_DEBUG_LEVEL              40
#define  DUILIB_TRACE_LEVEL              50

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus*/

void Duilib_DebugOutString(int level,const char* file,int lineno,const char* fmt,...);
void Duilib_DebugBufferFmt(int level,const char* file,int lineno,unsigned char* pBuffer,int buflen,const char* fmt,...);

int Duilib_TcharToAnsi(TCHAR *ptchar,char** ppChar,int*pCharSize);
int Duilib_AnsiToTchar(const char *pChar,TCHAR **pptchar,int *ptcharsize);
int Duilib_UnicodeToAnsi(wchar_t* pWideChar, char** ppChar, int*pCharSize);
int Duilib_AnsiToUnicode(char* pChar, wchar_t **ppWideChar, int*pWideCharSize);
int Duilib_init_log(void);
void Duilib_fini_log(void);


#ifdef __cplusplus
};
#endif /* __cplusplus*/

#define  DUILIB_DEBUG(fmt,...) Duilib_DebugOutString(DUILIB_DEBUG_LEVEL,__FILE__,__LINE__,fmt,__VA_ARGS__)
#define  DUILIB_ERROR(fmt,...) Duilib_DebugOutString(DUILIB_ERROR_LEVEL,__FILE__,__LINE__,fmt,__VA_ARGS__)

#define  DUILIB_BUFFER(ptr,blen) Duilib_DebugBufferFmt(DUILIB_DEBUG_LEVEL,__FILE__,__LINE__,(unsigned char*)ptr,blen,NULL)
#define  DUILIB_BUFFER_FMT(ptr,blen,...) Duilib_DebugBufferFmt(DUILIB_DEBUG_LEVEL,__FILE__,__LINE__,(unsigned char*)ptr,blen,__VA_ARGS__)

#define DUILIB_GETERRNO(ret) do{ ret = GetLastError(); if (ret == 0) {ret = -1;} else {ret = -ret;}}while(0)
#define DUILIB_SETERRNO(ret) do{if (ret > 0) {SetLastError(ret);} else {SetLastError(-ret);}} while(0)


#endif /* __DUILIB_LOG_H_5404E5EF7E1C7AD54D6E0511DABBFC8D__ */
