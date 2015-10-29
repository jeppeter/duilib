
#include "instwiz.h"
using namespace DuiLib;

LPCTSTR CInstWiz::GetWindowClassName() const
{
    return _T("Install Window");
}


void  CInstWiz::SetSkinXMLPath( LPCTSTR path )
{
    _tcscpy( m_skinXMLPath, path );
}

CDuiString CInstWiz::GetSkinFile()
{
    CDuiString file(this->m_skinXMLPath);
    return file;
}

CDuiString CInstWiz::GetSkinFolder()
{
    return _T("");
}

void CInstWiz::SetTableIdx(LPCTSTR tabname, int idx)
{
    CTabLayoutUI* pTab = NULL;
    pTab = static_cast<CTabLayoutUI*>( this->m_PaintManager.FindControl(tabname) );
    if (pTab == NULL) {
        return ;
    }

    pTab->SelectItem(idx);
    return ;
}

