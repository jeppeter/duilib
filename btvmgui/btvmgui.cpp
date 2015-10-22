
#include "btvmgui.h"

Cbtvmgui::Cbtvmgui( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{

}


void CFrameWnd::Notify( TNotifyUI& msg )
{
    __super::Notify(msg);
}


CControlUI* CFrameWnd::CreateControl( LPCTSTR pstrClassName )
{
    return NULL;
}

void CFrameWnd::InitWindow()
{
    CenterWindow();
}
