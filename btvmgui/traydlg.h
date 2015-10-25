#pragma once

#include "duilib.h"

class Ctraydlg: public CXMLWnd
{
public:
    explicit Ctraydlg(LPCTSTR pszXMLPath);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void TraySetIcon(HICON hIcon);
    void TraySetToolTip(LPCTSTR tooltip);
    BOOL TrayHide();
    BOOL TrayShow();
    BOOL TrayIsVisible();
private:
    void __show_tray();
    void __hide_tray();
    int m_trayshow;
    NOTIFYICONDATA m_NotifyIconData;
};