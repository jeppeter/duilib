
#ifndef __UI_MENU_H__
#define __UI_MENU_H__

#include "duilib.h"

namespace DuiLib
{

class CListUI;

class CMenuUI : public CListUI
{

};

class CMenuElementUI : public CListContainerElementUI
{

};

class CMenuWnd : public CXMLWnd
{
public:
    explicit CMenuWnd(LPCTSTR pszXMLPath);
    virtual ~CMenuWnd();

    void SetHwnd(HWND hwnd);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(HWND hWnd);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hParent;
	POINT m_pt;
	CPaintManagerUI m_pm;
	CMenuElementUI* m_pOwner;
	CMenuUI* m_pLayout;
};

};


#endif /*__UI_MENU_H__*/

