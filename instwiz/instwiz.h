
#pragma once
#include "duilib.h"


class CInstWiz : public WindowImplBase
{
public:
    CInstWiz() {};
    ~CInstWiz() {};
    virtual LPCTSTR GetWindowClassName() const;
    void       SetSkinXMLPath( LPCTSTR path );
    void SetTableIdx(LPCTSTR tabname,int idx);
protected:
    virtual CDuiString GetSkinFolder();
    virtual CDuiString GetSkinFile();
private:
    TCHAR                               m_skinXMLPath[MAX_PATH];

};

