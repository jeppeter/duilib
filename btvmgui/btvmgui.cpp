
#include "btvmgui.h"
#include "output_debug.h"
#include "resource.h"

#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER+0x1)

/************************************************
*   these two menu item is in xml file of command attribute
************************************************/
#define WM_MENU_OPEN_DIALOG                  120
#define WM_MENU_EXIT                                  121



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
    TCHAR numstr[32];
    HICON hicon=NULL;
    CLabelUI* pLabel=static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTimeout")));
    if(pLabel) {
        _sntprintf(numstr,sizeof(numstr),_T("%d"),this->m_timeout);
        pLabel->SetText(numstr);
    }

    this->SetIcon(IDR_MAINFRAME);


    CButtonUI* phyper= NULL;
    phyper = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hyperlink")));
    if(phyper) {
        phyper->SetText(_T("{u}{a}http://www.sina.com.cn{/a}{/u}"));
        _tcsncpy(this->m_hyperlink,_T("http://www.sina.com.cn"),sizeof(this->m_hyperlink)/sizeof(this->m_hyperlink[0]));
    }

    hicon = (HICON)::LoadImage(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    if (hicon) {
        this->TraySetIcon(hicon);
    }
    this->TraySetToolTip(_T("Bingte VMTool GUI"));
    this->TraySetMessage(WM_TRAY_ICON_NOTIFY_MESSAGE);
    this->TraySetId(1);

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

void Cbtvmgui::OnFinalMessage(HWND hWnd)
{
    /*we delete out here*/
    return ;
}

LRESULT Cbtvmgui::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TRAY_ICON_NOTIFY_MESSAGE ) {
        if (lParam == WM_LBUTTONDBLCLK) {
            this->ShowWindow(true,true);
            this->TrayHide();
            return 1;
        } else if (lParam == WM_RBUTTONDOWN) {
            /*now to show the menu*/
            CMenuWnd* pMenu = new CMenuWnd(_T("menutest.xml"),m_hWnd);
            POINT pt;
            GetCursorPos(&pt);
            CDuiPoint point(pt.x,pt.y);
            DEBUG_INFO("current 0x%x\n",this->GetHWND());
            pMenu->SetTopWnd(this->GetHWND());
            pMenu->Init(NULL, _T("xml"), point);
            return 1;
        }
    } else if (uMsg == MENU_ITEM_MESSAGE) {
        DEBUG_INFO("got menu item message wparam 0x%x lparam 0x%x\n",wParam,lParam);
        if (wParam == WM_MENU_OPEN_DIALOG) {
            this->ShowWindow(true,true);
            this->TrayHide();
        } else if (wParam == WM_MENU_EXIT) {
            Close();
        }

        return 1;
    }
    return __super::HandleMessage(uMsg,wParam,lParam);
}


