
#include "duilib_log.h"
#include "StdAfx.h"

static int st_duilib_log_level = DUILIB_ERROR_LEVEL;
static FILE* st_duilib_log_fp = NULL;


int Duilib_init_log(void)
{
    char* pval=NULL;

    Duilib_fini_log();

    pval = getenv("DUILIB_LOG_LEVEL");
    if  (pval != NULL) {
        st_duilib_log_level = atoi(pval);
    }

    pval = getenv("DUILIB_LOG_FILE");
    if (pval != NULL) {
        st_duilib_log_fp = fopen(pval,"w+b");
    }

    return 0;
}

void Duilib_fini_log(void)
{
    st_duilib_log_level = DUILIB_ERROR_LEVEL;
    if (st_duilib_log_fp != NULL) {
        fclose(st_duilib_log_fp);
    }
    st_duilib_log_fp = NULL;
}

int _Duilib_get_level(void)
{
    static int st_inited = 0;
    if (st_inited == 0) {
        Duilib_init_log();
        st_inited = 1;
    }
    return st_duilib_log_level;
}

extern "C" void Duilib_InnerDebug(char* pFmtStr)
{
#ifdef UNICODE
    LPWSTR pWide=NULL;
    int len;
    BOOL bret;
    len = (int) strlen(pFmtStr);
    pWide = new wchar_t[len*2];
    bret = MultiByteToWideChar(CP_ACP,NULL,pFmtStr,-1,pWide,len*2);
    if(bret) {
        OutputDebugString(pWide);
    } else {
        OutputDebugString(L"can not change fmt string");
    }
    delete [] pWide;
#else
    OutputDebugString(pFmtStr);
#endif

    if (st_duilib_log_fp != NULL) {
        fprintf(st_duilib_log_fp,"%s",pFmtStr);
        fflush(st_duilib_log_fp);
    }
    return ;
}

extern "C" void Duilib_DebugOutString(int level,const char* file,int lineno,const char* fmt,...)
{
    char* pFmt=NULL;
    char* pLine=NULL;
    char* pWhole=NULL;
    va_list ap;

    if (level > _Duilib_get_level()) {
        return;
    }

    pFmt = new char[2000];
    pLine = new char[2000];
    pWhole = new char[4000];

    _snprintf_s(pLine,2000,1999,"%s:%d:time(0x%08x)\t",file,lineno,GetTickCount());
    va_start(ap,fmt);
    _vsnprintf_s(pFmt,2000,1999,fmt,ap);
    strcpy_s(pWhole,4000,pLine);
    strcat_s(pWhole,4000,pFmt);
    strcat_s(pWhole,4000,"\n");

    Duilib_InnerDebug(pWhole);
    delete [] pFmt;
    delete [] pLine;
    delete [] pWhole;

    return ;
}


extern "C" void Duilib_DebugBufferFmt(int level,const char* file,int lineno,unsigned char* pBuffer,int buflen,const char* fmt,...)
{
    int fmtlen=2000;
    char*pLine=NULL,*pCur;
    int formedlen;
    int ret;
    int i;

    if (level > _Duilib_get_level()) {
        return;
    }

    pLine = new char[fmtlen];
    pCur = pLine;
    formedlen = 0;

    ret = _snprintf_s(pCur,fmtlen-formedlen,fmtlen-formedlen-1,"[%s:%d:time(0x%08x)]\tbuffer %p (%d)",file,lineno,GetTickCount(),pBuffer,buflen);
    pCur += ret;
    formedlen += ret;

    if(fmt) {
        va_list ap;
        va_start(ap,fmt);
        ret = _vsnprintf_s(pCur,fmtlen-formedlen,formedlen-formedlen - 1,fmt,ap);
        pCur += ret;
        formedlen += ret;
    }

    for(i=0; i<buflen; i++) {
        if((formedlen +100)>fmtlen) {
            Duilib_InnerDebug(pLine);
            pCur = pLine;
            formedlen = 0;
        }
        if((i%16)==0) {
            ret = _snprintf_s(pCur,fmtlen-formedlen,fmtlen-formedlen-1,"\n");
            Duilib_InnerDebug(pLine);
            pCur = pLine;
            formedlen = 0;
            ret = _snprintf_s(pCur,fmtlen-formedlen,fmtlen-formedlen-1,"[0x%08x]\t",i);
            pCur += ret;
            formedlen += ret;
        }

        ret = _snprintf_s(pCur,fmtlen-formedlen,fmtlen-formedlen-1,"0x%02x ",pBuffer[i]);
        pCur += ret;
        formedlen += ret;
    }
    ret = _snprintf_s(pCur,fmtlen-formedlen,fmtlen-formedlen-1,"\n");
    pCur += ret;
    formedlen += ret;

    if(formedlen > 0) {
        Duilib_InnerDebug(pLine);
        pCur = pLine;
        formedlen = 0;
    }

    delete [] pLine;
    pLine = NULL;
    return ;
}


extern "C" int Duilib_UnicodeToAnsi(wchar_t* pWideChar, char** ppChar, int*pCharSize)
{
    char* pRetChar = *ppChar;
    int retcharsize = *pCharSize;
    int ret, wlen, needlen;

    if (pWideChar == NULL) {
        if (*ppChar) {
            delete [] pRetChar;
        }
        *ppChar = NULL;
        *pCharSize = 0;
        return 0;
    }
    wlen = (int)wcslen(pWideChar);
    needlen = WideCharToMultiByte(CP_ACP, 0, pWideChar, wlen, NULL, 0, NULL, NULL);
    if (retcharsize <= needlen) {
        retcharsize = (needlen + 1);
        pRetChar = new char[needlen + 1];
        assert(pRetChar);
    }

    ret = WideCharToMultiByte(CP_ACP, 0, pWideChar, wlen, pRetChar, retcharsize, NULL, NULL);
    if (ret != needlen) {
        ret = ERROR_INVALID_BLOCK;
        goto fail;
    }
    pRetChar[needlen] = '\0';
    needlen += 1;

    if ((*ppChar) && (*ppChar) != pRetChar) {
        char* pTmpChar = *ppChar;
        delete [] pTmpChar;
    }
    *ppChar = pRetChar;
    *pCharSize = retcharsize;

    return needlen;
fail:
    if (pRetChar && pRetChar != (*ppChar)) {
        delete [] pRetChar;
    }
    pRetChar = NULL;
    retcharsize = 0;
    SetLastError(ret);
    return -ret;
}


extern "C" int Duilib_AnsiToUnicode(char* pChar, wchar_t **ppWideChar, int*pWideCharSize)
{
    wchar_t *pRetWideChar = *ppWideChar;
    int retwidecharsize = *pWideCharSize;
    int ret, len, needlen;

    if (pChar == NULL) {
        if (*ppWideChar) {
            delete [] pRetWideChar;
        }
        *ppWideChar = NULL;
        *pWideCharSize = 0;
        return 0;
    }

    len = (int) strlen(pChar);
    needlen = MultiByteToWideChar(CP_ACP, 0, pChar, len, NULL, 0);
    if (retwidecharsize <= needlen) {
        retwidecharsize = needlen + 1;
        pRetWideChar = new wchar_t[retwidecharsize];
        assert(pRetWideChar);
    }

    ret = MultiByteToWideChar(CP_ACP, 0, pChar, len, pRetWideChar, retwidecharsize);
    if (ret != needlen) {
        ret = ERROR_INVALID_BLOCK;
        goto fail;
    }
    pRetWideChar[needlen] = '\0';

    if ( (*ppWideChar) && (*ppWideChar) != pRetWideChar) {
        wchar_t *pTmpWideChar = *ppWideChar;
        delete [] pTmpWideChar;
    }
    *ppWideChar = pRetWideChar;
    *pWideCharSize = retwidecharsize;
    return ret;
fail:
    if (pRetWideChar && pRetWideChar != (*ppWideChar)) {
        delete [] pRetWideChar;
    }
    pRetWideChar = NULL;
    retwidecharsize = 0;
    SetLastError(ret);
    return -ret;
}

#ifndef _UNICODE
int Duilib_chartoansi(const char *ptchar, char** ppChar, int*pCharSize)
{
    int needlen=0;
    int needsize=*pCharSize;
    int ret;
    char* pRetChar = *ppChar;

    if (ptchar == NULL){
        /*if null, we just free memory*/
        if (pRetChar){
            free(pRetChar);
        }
        *ppChar = NULL;
        *pCharSize = 0;
        return 0;
    }

    needlen = (int)strlen(ptchar);
    needlen += 1;
    if (pRetChar == NULL || *pCharSize < needlen){
        pRetChar =(char*) malloc(needlen);
        if (pRetChar == NULL){
            ret = -GetLastError();
            if (ret >= 0) {
                ret = -1;
            }
            goto fail;
        }
        needsize = needlen;
    }

    strncpy(pRetChar,ptchar,needlen);
    if (pRetChar != *ppChar && *ppChar != NULL){
        free(*ppChar);
    }

    *ppChar = pRetChar;
    *pCharSize = needsize;

    return needlen;
fail:
    if (pRetChar != *ppChar && pRetChar != NULL){
        free(pRetChar);
    }
    pRetChar = NULL;
    return ret;
}

#endif


extern "C" int Duilib_TcharToAnsi(TCHAR *ptchar, char** ppChar, int*pCharSize)
{
    int ret;
#ifdef _UNICODE
    ret = Duilib_UnicodeToAnsi(ptchar,ppChar,pCharSize);
#else
    ret = Duilib_chartoansi(ptchar,ppChar,pCharSize);
#endif
    return ret;
}

extern "C" int Duilib_AnsiToTchar(const char *pChar,TCHAR **pptchar,int *ptcharsize)
{
    int ret;
#ifdef _UNICODE
    ret = Duilib_AnsiToUnicode((char*)pChar,pptchar,ptcharsize);
#else
    ret = Duilib_chartoansi(pChar,pptchar,ptcharsize);
#endif
    return ret;
}