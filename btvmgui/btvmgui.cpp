
#include "btvmgui.h"

Cbtvmgui::Cbtvmgui( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{

}


void Cbtvmgui::Notify( TNotifyUI& msg )
{
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
		::MessageBox(this->GetHWND(),_T("Set Icon"),_T("Notice"),MB_OK);
		::PostMessage(this->GetHWND(), (UINT)WM_SETICON, ICON_BIG, (LPARAM)hicon);
	}else{
		::MessageBox(this->GetHWND(),_T("Not Set Icon"),_T("Notice"),MB_OK);
	}

    CenterWindow();
}
