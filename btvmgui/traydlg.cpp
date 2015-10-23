
#include "traydlg.h"

Ctraydlg::Ctraydlg( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{
	memset(&(this->m_NotifyIconData),0,sizeof(this->m_NotifyIconData));
	this->m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	this->m_NotifyIconData.hWnd = 0;
	this->m_NotifyIconData.uID = 1;
	this->m_NotifyIconData.uCallbackMesage = WM_TRAY_ICON_NOTIFY_MESSAGE;
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
	if (m_NotifyIconData.hWnd && m_NotifyIconData.uID > 0 && TrayIsVisible())
	{
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
	}
	return 0;
}

void Ctraydlg::TraySetIcon(HICON hIcon)
{
	this->m_NotifyIconData.hIcon = hICon;
	this->m_NotifyIconData.uFlags |= NIF_ICON;
	return;	
}

BOOL Ctraydlg::TrayHide()
{

}