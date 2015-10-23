#include "btvmgui.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);

    Cbtvmgui *pFrame = new Cbtvmgui(_T("btvmgui.xml"));
    //pFrame->Create(NULL, _T("BTVMGUI"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->Create(NULL, _T("BTVMGUI"), 0,0);
    pFrame->ShowModal();

    delete pFrame;
    ::CoUninitialize();
    return 0;
}

