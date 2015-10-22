
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
    CenterWindow();
}
