#include "StdAfx.h"
#include "UICrack.h"
#include "UIMenu.h"
#include "output_debug.h"


namespace DuiLib
{

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuUI
const TCHAR* const kMenuUIClassName = _T("MenuUI");
const TCHAR* const kMenuUIInterfaceName = _T("Menu");

CMenuUI::CMenuUI()
{
    if (GetHeader() != NULL)
        GetHeader()->SetVisible(false);
    this->m_msg = 0;
    this->m_hTopWnd = NULL;
    this->m_pObserver = NULL;
}

CMenuUI::~CMenuUI()
{
    this->m_msg = 0;
    this->m_hTopWnd = NULL;
    this->m_pObserver = NULL;
}

void CMenuUI::SetTopWnd(HWND hwnd)
{
    this->m_hTopWnd = hwnd;
    return ;
}

void CMenuUI::SetMessage(UINT msg)
{
    this->m_msg = msg;
    return ;
}

void CMenuUI::SetObserver(ContextMenuObserver * pObserver)
{
    this->m_pObserver = pObserver;
    return;
}

LPCTSTR CMenuUI::GetClass() const
{
    return kMenuUIClassName;
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuUIInterfaceName) == 0 ) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

void CMenuUI::DoEvent(TEventUI& event)
{
    return __super::DoEvent(event);
}

bool CMenuUI::Add(CControlUI* pControl)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
    if (pMenuItem == NULL)
        return false;

    for (int i = 0; i < pMenuItem->GetCount(); ++i) {
        if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
            (static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
        }
    }
    return CListUI::Add(pControl);
}

bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
    if (pMenuItem == NULL)
        return false;

    for (int i = 0; i < pMenuItem->GetCount(); ++i) {
        if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
            (static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
        }
    }
    return CListUI::AddAt(pControl, iIndex);
}

int CMenuUI::GetItemIndex(CControlUI* pControl) const
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
    if (pMenuItem == NULL)
        return -1;

    return __super::GetItemIndex(pControl);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
    if (pMenuItem == NULL)
        return false;

    return __super::SetItemIndex(pControl, iIndex);
}

bool CMenuUI::Remove(CControlUI* pControl)
{
    CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
    if (pMenuItem == NULL)
        return false;

    return __super::Remove(pControl);
}

SIZE CMenuUI::EstimateSize(SIZE szAvailable)
{
    int cxFixed = 0;
    int cyFixed = 0;
    for( int it = 0; it < GetCount(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
        if( !pControl->IsVisible() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        cyFixed += sz.cy;
        if( cxFixed < sz.cx )
            cxFixed = sz.cx;
    }
    return CDuiSize(cxFixed, cyFixed);
}

void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    CListUI::SetAttribute(pstrName, pstrValue);
}

/////////////////////////////////////////////////////////////////////////////////////
//
class CMenuBuilderCallback: public IDialogBuilderCallback
{

    CControlUI* CreateControl(LPCTSTR pstrClass)
    {
        if (_tcsicmp(pstrClass, kMenuUIInterfaceName) == 0) {
            CMenuUI* pMenu=new CMenuUI();
            pMenu->SetTopWnd(this->m_hTopWnd);
            pMenu->SetMessage(this->m_msg);
            pMenu->SetObserver(this->m_pObserver);
            return pMenu;
        } else if (_tcsicmp(pstrClass, kMenuElementUIInterfaceName) == 0) {
            CMenuElementUI* pElem=new CMenuElementUI();
            pElem->SetTopWnd(this->m_hTopWnd);
            pElem->SetMessage(this->m_msg);
            pElem->SetObserver(this->m_pObserver);
            return pElem;
        }
        return NULL;
    }
public:
    void SetTopWnd(HWND hwnd)
    {
        this->m_hTopWnd = hwnd;
        return ;
    }
    void SetMessage(UINT msg)
    {
        this->m_msg = msg;
        return ;
    }

    void SetObserver(ContextMenuObserver* pObserver)
    {
        this->m_pObserver = pObserver;
        return ;
    }
private:
    HWND m_hTopWnd;
    UINT m_msg;
    ContextMenuObserver* m_pObserver;
};

CMenuWnd::CMenuWnd(LPCTSTR pszXMLPath,HWND hParent ): CXMLWnd(pszXMLPath),
    m_hParent(hParent),
    m_pOwner(NULL),
    m_pLayout(),
    m_xml(pszXMLPath) , m_msg(0),m_hTopWnd(NULL)
{
    this->m_pObserver = NULL;
    this->m_pCreateObserver = NULL;
}

void CMenuWnd::SetTopWnd(HWND hwnd)
{
    this->m_hTopWnd = hwnd;
    return ;
}

void CMenuWnd::SetMessage(UINT msg)
{
    this->m_msg = msg;
    return ;
}

void CMenuWnd::SetObserver(ContextMenuObserver * pObserver)
{
    this->m_pObserver = pObserver;
    return ;
}

BOOL CMenuWnd::Receive(ContextMenuParam param)
{
    switch (param.wParam) {
    case 1:
        Close();
        break;
    case 2: {
        HWND hParent = GetParent(m_hWnd);
        while (hParent != NULL) {
            if (hParent == param.hWnd) {
                Close();
                break;
            }
            hParent = GetParent(hParent);
        }
    }
    break;
    default:
        break;
    }

    return TRUE;
}

void CMenuWnd::Init(CMenuElementUI* pOwner,  LPCTSTR pSkinType, POINT point)
{
    m_BasedPoint = point;
    m_pOwner = pOwner;
    m_pLayout = NULL;

    if (pSkinType != NULL)
        m_sType = pSkinType;

    //m_xml = this->GetSkinFile();
    this->m_pCreateObserver = new ContextMenuObserver();
    this->m_pCreateObserver->AddReceiver(this);
    this->m_pObserver = this->m_pCreateObserver;

    this->Create((m_pOwner == NULL) ? m_hParent : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif
}

LPCTSTR CMenuWnd::GetWindowClassName() const
{
    return _T("MenuWnd");
}

CMenuWnd::~CMenuWnd()
{
    if (this->m_pObserver) {
        this->m_pObserver = NULL;
    }
    if (this->m_pCreateObserver) {
        delete this->m_pCreateObserver ;
    }
    this->m_pCreateObserver =NULL;
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
    this->RemoveObserver();
    if( m_pOwner != NULL ) {
        for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
            if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)) != NULL ) {
                (static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
                (static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
                (static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
            }
        }
        m_pOwner->m_pWindow = NULL;
        m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
        m_pOwner->Invalidate();
    }
    delete this;
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( uMsg == WM_CREATE ) {
        if( m_pOwner != NULL) {
            LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
            styleValue &= ~WS_CAPTION;
            ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
            RECT rcClient;
            ::GetClientRect(*this, &rcClient);
            ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
                           rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

            m_pm.Init(m_hWnd);
            // The trick is to add the items to the new container. Their owner gets
            // reassigned by this operation - which is why it is important to reassign
            // the items back to the righfull owner/manager when the window closes.
            m_pLayout = new CMenuUI();
            m_pLayout->SetTopWnd(this->m_hTopWnd);
            m_pLayout->SetMessage(this->m_msg);
            m_pLayout->SetObserver(this->m_pObserver);
            m_pLayout->SetManager(&m_pm, NULL, true);
            LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(kMenuUIInterfaceName);
            if( pDefaultAttributes ) {
                m_pLayout->ApplyAttributeList(pDefaultAttributes);
            }
            m_pLayout->SetBkColor(0xFFFFFFFF);
            m_pLayout->SetBorderColor(0xFF85E4FF);
            m_pLayout->SetBorderSize(0);
            m_pLayout->SetAutoDestroy(false);
            m_pLayout->EnableScrollBar();
            for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
                if(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ) {
                    (static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
                    m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
                }
            }
            m_pm.AttachDialog(m_pLayout);

            // Position the popup window in absolute space
            RECT rcOwner = m_pOwner->GetPos();
            RECT rc = rcOwner;

            int cxFixed = 0;
            int cyFixed = 0;

#if defined(WIN32) && !defined(UNDER_CE)
            MONITORINFO oMonitor = {};
            oMonitor.cbSize = sizeof(oMonitor);
            ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
            CDuiRect rcWork = oMonitor.rcWork;
#else
            CDuiRect rcWork;
            GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
            SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

            for( int it = 0; it < m_pOwner->GetCount(); it++ ) {
                if(m_pOwner->GetItemAt(it)->GetInterface(kMenuElementUIInterfaceName) != NULL ) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->GetItemAt(it));
                    SIZE sz = pControl->EstimateSize(szAvailable);
                    cyFixed += sz.cy;

                    if( cxFixed < sz.cx )
                        cxFixed = sz.cx;
                }
            }
            cyFixed += 4;
            cxFixed += 4;

            RECT rcWindow;
            GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

            rc.top = rcOwner.top;
            rc.bottom = rc.top + cyFixed;
            ::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
            rc.left = rcWindow.right;
            rc.right = rc.left + cxFixed;
            rc.right += 2;

            bool bReachBottom = false;
            bool bReachRight = false;
            LONG chRightAlgin = 0;
            LONG chBottomAlgin = 0;

            RECT rcPreWindow = {0};
            ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(*(this->m_pObserver));
            ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
            while( pReceiver != NULL ) {
                CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
                if( pContextMenu != NULL ) {
                    GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

                    bReachRight = rcPreWindow.left >= rcWindow.right;
                    bReachBottom = rcPreWindow.top >= rcWindow.bottom;
                    if( pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow()
                            ||  bReachBottom || bReachRight )
                        break;
                }
                pReceiver = iterator.next();
            }

            if (bReachBottom) {
                rc.bottom = rcWindow.top;
                rc.top = rc.bottom - cyFixed;
            }

            if (bReachRight) {
                rc.right = rcWindow.left;
                rc.left = rc.right - cxFixed;
            }

            if( rc.bottom > rcWork.bottom ) {
                rc.bottom = rc.top;
                rc.top = rc.bottom - cyFixed;
            }

            if (rc.right > rcWork.right) {
                rc.right = rcWindow.left;
                rc.left = rc.right - cxFixed;

                //rc.top = rcWindow.bottom;
                //rc.bottom = rc.top + cyFixed;
            }

            if( rc.top < rcWork.top ) {
                rc.top = rcOwner.top;
                rc.bottom = rc.top + cyFixed;
            }

            if (rc.left < rcWork.left) {
                rc.left = rcWindow.right;
                rc.right = rc.left + cxFixed;
            }

            //DEBUG_INFO("window rect (left:top | right:bottom) (%d:%d | %d:%d)\n",
            //           rcWindow.left,rcWindow.top,rcWindow.right,rcWindow.bottom);
            //DEBUG_INFO("rc.left:rc.top rc.right:rc.bottom %d:%d %d:%d\n",
            //           rc.left,rc.top,rc.right,rc.bottom);

            MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
        } else {
            m_pm.Init(m_hWnd);

            CDialogBuilder builder;
            CMenuBuilderCallback menuCallback;
            menuCallback.SetTopWnd(this->m_hTopWnd);
            menuCallback.SetMessage(this->m_msg);
            menuCallback.SetObserver(this->m_pObserver);
            CControlUI* pRoot = builder.Create(m_xml, m_sType.GetData(), &menuCallback, &m_pm);
            m_pm.AttachDialog(pRoot);

#if defined(WIN32) && !defined(UNDER_CE)
            MONITORINFO oMonitor = {};
            oMonitor.cbSize = sizeof(oMonitor);
            ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
            CDuiRect rcWork = oMonitor.rcWork;
#else
            CDuiRect rcWork;
            GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
            SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
            szAvailable = pRoot->EstimateSize(szAvailable);
            m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

            DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;

            SIZE szInit = m_pm.GetInitSize();
            CDuiRect rc;
            CDuiPoint point = m_BasedPoint;
            rc.left = point.x;
            rc.top = point.y;
            rc.right = rc.left + szInit.cx;
            rc.bottom = rc.top + szInit.cy;

            int nWidth = rc.GetWidth();
            int nHeight = rc.GetHeight();

            if (dwAlignment & eMenuAlignment_Right) {
                rc.right = point.x;
                rc.left = rc.right - nWidth;
            }

            if (dwAlignment & eMenuAlignment_Bottom) {
                rc.bottom = point.y;
                rc.top = rc.bottom - nHeight;
            }

            /*now we adjust the menu display in the monitor*/
            if (rc.bottom >= rcWork.bottom) {
                rc.bottom = this->m_BasedPoint.y;
                rc.top = rc.bottom - nHeight;
            }

            if (rc.left >= rcWork.right) {
                rc.right = this->m_BasedPoint.x;
                rc.left = rc.right - nWidth;
            }

            //DEBUG_INFO("rc.left %d rc.top %d rc.right %d rc.bottom %d\n",
            //           rc.left,rc.top,rc.left + rc.GetWidth(),rc.top + rc.GetHeight());
            //DEBUG_INFO("rcwork.left %d rcwork.top %d rcwork.right %d rcwork.bottom %d\n",
            //           rcWork.left,rcWork.top,rcWork.left + rcWork.GetWidth(),rcWork.top + rcWork.GetHeight());
            SetForegroundWindow(m_hWnd);
            MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
            SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_SHOWWINDOW);
        }

        return 0;
    } else if( uMsg == WM_CLOSE ) {
        if( m_pOwner != NULL ) {
            m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
            m_pOwner->SetPos(m_pOwner->GetPos(), false);
            m_pOwner->SetFocus();
        }
    } else if( uMsg == WM_RBUTTONDOWN || uMsg == WM_CONTEXTMENU || uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDBLCLK ) {
        return 0L;
    } else if( uMsg == WM_KILLFOCUS ) {
        HWND hFocusWnd = (HWND)wParam;

        BOOL bInMenuWindowList = FALSE;
        ContextMenuParam param;
        param.hWnd = GetHWND();

        ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(*(this->m_pObserver));
        ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
        while( pReceiver != NULL ) {
            CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
            if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
                bInMenuWindowList = TRUE;
                break;
            }
            pReceiver = iterator.next();
        }

        if( !bInMenuWindowList ) {
            param.wParam = 1;
            this->m_pObserver->RBroadcast(param);

            return 0;
        }
    } else if( uMsg == WM_KEYDOWN) {
        if( wParam == VK_ESCAPE) {
            Close();
        }
    }

    LRESULT lRes = 0;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
const TCHAR* const kMenuElementUIClassName = _T("MenuElementUI");
const TCHAR* const kMenuElementUIInterfaceName = _T("MenuElement");

CMenuElementUI::CMenuElementUI():
    m_pWindow(NULL)
{
    m_cxyFixed.cy = 25;
    m_bMouseChildEnabled = true;
    m_command = 0;
    m_hTopWnd = NULL;
    m_msg = 0;

    SetMouseChildEnabled(false);
}

void CMenuElementUI::SetTopWnd(HWND hwnd)
{
    this->m_hTopWnd = hwnd;
    return;
}

void CMenuElementUI::SetMessage(UINT msg)
{
    this->m_msg = msg;
    return ;
}

void CMenuElementUI::SetObserver(ContextMenuObserver * pObserver)
{
    this->m_pObserver = pObserver;
    return;
}

CMenuElementUI::~CMenuElementUI()
{}

LPCTSTR CMenuElementUI::GetClass() const
{
    return kMenuElementUIClassName;
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuElementUIInterfaceName) == 0 ) return static_cast<CMenuElementUI*>(this);
    return CListContainerElementUI::GetInterface(pstrName);
}

void CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
    CMenuElementUI::DrawItemBk(hDC, m_rcItem);
    DrawItemText(hDC, m_rcItem);
    for (int i = 0; i < GetCount(); ++i) {
        if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) == NULL)
            GetItemAt(i)->DoPaint(hDC, rcPaint);
    }
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_sText.IsEmpty() ) return;

    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    if( pInfo->bShowHtml )
        CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
                                    NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
    else
        CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
                                pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}


SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
    SIZE cXY = {0};
    for( int it = 0; it < GetCount(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
        if( !pControl->IsVisible() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        cXY.cy += sz.cy;
        if( cXY.cx < sz.cx )
            cXY.cx = sz.cx;
    }
    if(cXY.cy == 0) {
        TListInfoUI* pInfo = m_pOwner->GetListInfo();

        DWORD iTextColor = pInfo->dwTextColor;
        if( (m_uButtonState & UISTATE_HOT) != 0 ) {
            iTextColor = pInfo->dwHotTextColor;
        }
        if( IsSelected() ) {
            iTextColor = pInfo->dwSelectedTextColor;
        }
        if( !IsEnabled() ) {
            iTextColor = pInfo->dwDisabledTextColor;
        }

        RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
        rcText.left += pInfo->rcTextPadding.left;
        rcText.right -= pInfo->rcTextPadding.right;
        if( pInfo->bShowHtml ) {
            int nLinks = 0;
            CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, DT_CALCRECT | pInfo->uTextStyle);
        } else {
            CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
        }
        cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + 20;
        cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
    }

    if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
    return cXY;
}

void CMenuElementUI::DoEvent(TEventUI& event)
{
    if( event.Type == UIEVENT_MOUSEENTER ) {
        CListContainerElementUI::DoEvent(event);
        if( m_pWindow ) return;
        bool hasSubMenu = false;
        for( int i = 0; i < GetCount(); ++i ) {
            if( GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ) {
                (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);

                hasSubMenu = true;
            }
        }
        if( hasSubMenu ) {
            m_pOwner->SelectItem(GetIndex(), true);
            CreateMenuWnd();
        } else {
            ContextMenuParam param;
            param.hWnd = m_pManager->GetPaintWindow();
            param.wParam = 2;
            this->m_pObserver->RBroadcast(param);
            m_pOwner->SelectItem(GetIndex(), true);
        }
        return;
    }

    if( event.Type == UIEVENT_BUTTONDOWN ) {
        if( IsEnabled() ) {
            CListContainerElementUI::DoEvent(event);
            DEBUG_INFO("buttondown command %d hwnd 0x%x\n",this->m_command,this->m_hTopWnd);
            ::PostMessage(this->m_hTopWnd,this->m_msg,this->m_command,0);
            if( m_pWindow ) return;

            bool hasSubMenu = false;
            for( int i = 0; i < GetCount(); ++i ) {
                if( GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ) {
                    (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                    (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);

                    hasSubMenu = true;
                }
            }
            if( hasSubMenu ) {
                CreateMenuWnd();
            } else {
                ContextMenuParam param;
                param.hWnd = m_pManager->GetPaintWindow();
                param.wParam = 1;
                this->m_pObserver->RBroadcast(param);
            }
        }
        return;
    }

    CListContainerElementUI::DoEvent(event);
}

bool CMenuElementUI::Activate()
{
    if (CListContainerElementUI::Activate() && m_bSelected) {
        if( m_pWindow ) return true;
        bool hasSubMenu = false;
        for (int i = 0; i < GetCount(); ++i) {
            if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                (static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);

                hasSubMenu = true;
            }
        }
        if (hasSubMenu) {
            CreateMenuWnd();
        } else {
            ContextMenuParam param;
            param.hWnd = m_pManager->GetPaintWindow();
            param.wParam = 1;
            this->m_pObserver->RBroadcast(param);
        }

        return true;
    }
    return false;
}

CMenuWnd* CMenuElementUI::GetMenuWnd()
{
    return m_pWindow;
}

void CMenuElementUI::CreateMenuWnd()
{
    if( m_pWindow ) return;

    m_pWindow = new CMenuWnd(_T(""),m_pManager->GetPaintWindow());
    ASSERT(m_pWindow);
    m_pWindow->SetTopWnd(this->m_hTopWnd);
    m_pWindow->SetMessage(this->m_msg);
    m_pWindow->SetObserver(this->m_pObserver);

    ContextMenuParam param;
    param.hWnd = m_pManager->GetPaintWindow();
    param.wParam = 2;
    this->m_pObserver->RBroadcast(param);

    m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), CDuiPoint());
}

void CMenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if (_tcscmp(pstrName, _T("command")) == 0) {
        LPTSTR pstr=NULL;
        this->m_command = _tcstol(pstrValue, &pstr, 10);
        return ;
    }
    return __super::SetAttribute(pstrName,pstrValue);
}


} // namespace DuiLib