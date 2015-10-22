#pragma once
#include "duilib.h"

class Cbtvmgui: public CXMLWnd
{
public:
    explicit Cbtvmgui(LPCTSTR pszXMLPath);

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
};