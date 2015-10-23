
#include "btvmgui.h"

Cbtvmgui::Cbtvmgui( LPCTSTR pszXMLPath )
: Ctraydlg(pszXMLPath)
{
}

void Cbtvmgui::__show_tray()
{
	if (this->m_trayshow){
		return;
	}

	this->m_trayshow = 1;
	return ;
}

void Cbtvmgui::__hide_tray()
{
	if (this->m_trayshow == 0){
		return;
	}

	this->m_trayshow = 0;
	return ;
}


void Cbtvmgui::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("click")){
		if (msg.pSender->GetName() == _T("ButtonExit")){
			Close();
			return ;
		} else if (msg.pSender->GetName() == _T("closebtn")){

		}
	}
    __super::Notify(msg);
}


CControlUI* Cbtvmgui::CreateControl( LPCTSTR pstrClassName )
{
    return NULL;
}

void Cbtvmgui::InitWindow()
{
	/*now to set icon*/
	HINSTANCE hinstance=NULL;
	hinstance = CPaintManagerUI::GetInstance();
	HANDLE hicon = NULL;
	hicon = ::LoadImage(hinstance,_T("logo.ico"), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

	if (hicon != NULL){
		::PostMessage(this->GetHWND(), (UINT)WM_SETICON, ICON_BIG, (LPARAM)hicon);
	}else{
		::MessageBox(this->GetHWND(),_T("Not Set Icon"),_T("Notice"),MB_OK);
	}

	CButtonUI* phyper= NULL;
	phyper = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hyperlink")));
	if (phyper){
		phyper->SetText(_T("{u}{a}http://www.sina.com.cn{/a}{/u}"));
	}

    CenterWindow();
}
