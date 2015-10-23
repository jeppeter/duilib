#pragma once
#include "traydlg.h"

class Cbtvmgui: public Ctraydlg
{
public:
    explicit Cbtvmgui(LPCTSTR pszXMLPath);

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
    virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
private:
};