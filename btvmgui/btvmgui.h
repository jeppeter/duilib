#pragma once
#include "traydlg.h"

class Cbtvmgui: public Ctraydlg
{
public:
    explicit Cbtvmgui(LPCTSTR pszXMLPath);
    virtual ~Cbtvmgui();

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
    virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    int m_timeout;
    TCHAR m_hyperlink[256];
};