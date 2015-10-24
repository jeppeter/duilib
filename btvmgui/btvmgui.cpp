
#include "btvmgui.h"
#include "output_debug.h"
#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")

Cbtvmgui::Cbtvmgui(LPCTSTR pszXMLPath)
    : Ctraydlg(pszXMLPath)
{
    this->m_timeout = 3;
}


Cbtvmgui::~Cbtvmgui()
{
    DEBUG_INFO("\n");
}


void Cbtvmgui::Notify(TNotifyUI& msg)
{
    TCHAR numstr[32];
    int timeout;
    if(msg.sType == _T("click")) {
        if(msg.pSender->GetName() == _T("ButtonExit")) {
            Close();
            return ;
        } else if(msg.pSender->GetName() == _T("closebtn")) {
            this->TrayShow();
            this->ShowWindow(false,true);
            return ;
        } else if(msg.pSender->GetName() == _T("Buttonup")) {
            CLabelUI* pLabel=static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTimeout")));
            if(pLabel) {
                timeout = this->m_timeout;
                timeout ++;
                if(timeout <= 30) {
                    this->m_timeout = timeout;
                }
                _sntprintf(numstr,sizeof(numstr),_T("%d"),this->m_timeout);
                pLabel->SetText(numstr);
            }
            return ;
        } else if(msg.pSender->GetName() == _T("Buttondown")) {
            CLabelUI* pLabel=static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTimeout")));
            if(pLabel) {
                timeout = this->m_timeout;
                timeout --;
                if(timeout >= 1) {
                    this->m_timeout = timeout;
                }
                _sntprintf(numstr,sizeof(numstr),_T("%d"),this->m_timeout);
                pLabel->SetText(numstr);
            }
            return ;
        } else if(msg.pSender->GetName() == _T("hyperlink")) {
            ::ShellExecute(NULL, _T("open"),this->m_hyperlink, NULL, NULL, SW_SHOWNORMAL);
        }
    }
    __super::Notify(msg);
}


CControlUI* Cbtvmgui::CreateControl(LPCTSTR pstrClassName)
{
    return NULL;
}


void Cbtvmgui::InitWindow()
{
    /*now to set icon*/
    HINSTANCE hinstance=NULL;
    hinstance = CPaintManagerUI::GetInstance();
    HICON hicon = NULL;
    TCHAR numstr[32];
    TImageInfo* bitmap=NULL;
    CLabelUI* pLabel=static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTimeout")));
    if(pLabel) {
        _sntprintf(numstr,sizeof(numstr),_T("%d"),this->m_timeout);
        pLabel->SetText(numstr);
    }

    bitmap = CRenderEngine::LoadImage(_T("icon.png"),NULL,0);
    if(bitmap) {
        Gdiplus::Bitmap* pTmpBitmap=Gdiplus::Bitmap::FromHBITMAP(bitmap->hBitmap,NULL);
        if(pTmpBitmap == NULL) {
            goto failed;
        }
        hicon = NULL;
        pTmpBitmap->GetHICON(&hicon);


        if(hicon != NULL) {
            ::PostMessage(this->GetHWND(), (UINT)WM_SETICON, ICON_BIG  , (LPARAM)hicon);
            ::PostMessage(this->GetHWND(), (UINT)WM_SETICON, ICON_SMALL, (LPARAM)hicon);
            this->TraySetIcon((HICON)hicon);
        } else {
            ::MessageBox(this->GetHWND(),_T("Not Set Icon"),_T("Notice"),MB_OK);
        }
failed:
        if(pTmpBitmap) {
            delete pTmpBitmap;
        }
        pTmpBitmap = NULL;
        CRenderEngine::FreeImage(bitmap,true);
        bitmap = NULL;
    } else {
        ::MessageBox(this->GetHWND(),_T("Not Get Bitmap"),_T("Notice"),MB_OK);
    }

    CButtonUI* phyper= NULL;
    phyper = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hyperlink")));
    if(phyper) {
        phyper->SetText(_T("{u}{a}http://www.sina.com.cn{/a}{/u}"));
        _tcsncpy(this->m_hyperlink,_T("http://www.sina.com.cn"),sizeof(this->m_hyperlink)/sizeof(this->m_hyperlink[0]));
    }

    this->TraySetToolTip(_T("Bingte VMTool GUI"));

    CenterWindow();
}


LRESULT Cbtvmgui::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM  lParam, BOOL& bHandled)
{
    LRESULT res;
    DEBUG_INFO("\n");
    res = Ctraydlg::OnDestroy(uMsg, wParam, lParam,bHandled);
    DEBUG_INFO("\n");
    return res;
}

