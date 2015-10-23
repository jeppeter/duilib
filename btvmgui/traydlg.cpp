
#include "traydlg.h"
#include "output_debug.h"

#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER+0x10)

Ctraydlg::Ctraydlg( LPCTSTR pszXMLPath )
    : CXMLWnd(pszXMLPath)
{
    memset(&(this->m_NotifyIconData),0,sizeof(this->m_NotifyIconData));
    this->m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    this->m_NotifyIconData.hWnd = 0;
    this->m_NotifyIconData.uID = 1;
    this->m_NotifyIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;
    this->m_NotifyIconData.hIcon = NULL;
    this->m_NotifyIconData.uFlags = NIF_MESSAGE;
}

BOOL Ctraydlg::TrayIsVisible()
{
    return this->m_trayshow;
}

LRESULT Ctraydlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CXMLWnd::OnDestroy(uMsg,wParam,lParam,bHandled);
    if (m_NotifyIconData.hWnd && m_NotifyIconData.uID > 0 && TrayIsVisible()) {
        Shell_NotifyIcon(NIM_DELETE, &(this->m_NotifyIconData));
    }
    return 0;
}

LRESULT Ctraydlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LRESULT res;

    res = CXMLWnd::OnCreate(uMsg,wParam,lParam,bHandled);
    /*now we should set the param*/
    this->m_NotifyIconData.hWnd = this->GetHWND();
    this->m_NotifyIconData.uID = 1;
    return res;
}

void Ctraydlg::TraySetIcon(HICON hIcon)
{
    this->m_NotifyIconData.hIcon = hIcon;
    this->m_NotifyIconData.uFlags |= NIF_ICON;
    return;
}

BOOL Ctraydlg::TrayHide()
{
    BOOL bret=TRUE;
    if (this->m_trayshow) {
        bret = Shell_NotifyIcon(NIM_DELETE,&(this->m_NotifyIconData));
        if (bret) {
            this->m_trayshow = 0;
        }
    }
    return bret;
}

BOOL Ctraydlg::TrayShow()
{
    BOOL bret= TRUE;
    if (this->m_trayshow == 0) {
        bret = Shell_NotifyIcon(NIM_ADD,&(this->m_NotifyIconData));
        if (bret) {
            this->m_trayshow = 1;
        }
    }
    return bret;
}


LRESULT Ctraydlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled)
{
    if (uMsg == WM_LBUTTONDBLCLK) {
        DEBUG_INFO("double click on wparam %d lparam %d\n",wParam,lParam);
    }
    return CXMLWnd::MessageHandler(uMsg,wParam,lParam,bHandled);
}