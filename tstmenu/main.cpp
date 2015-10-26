
#include "duilib.h"
#include "output_debug.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

    Cbtvmgui *pFrame = new Cbtvmgui(_T("tstmenu.xml"));
    //pFrame->Create(NULL, _T("BTVMGUI"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->Create(NULL, _T("tstmenu"), 0,0);
    pFrame->ShowModal();
    DEBUG_INFO("\n");
    delete pFrame;
    DEBUG_INFO("\n");
    ::CoUninitialize();
    return 0;
}


