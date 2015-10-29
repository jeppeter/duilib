
#include "instwiz.h"
#include "output_debug.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);
    MSG msg = { 0 };
    CInstWiz *pFrame = new CInstWiz();
    pFrame->SetSkinXMLPath(_T("UninstallPackages_english.xml") );
    pFrame->Create(NULL, _T("BTVMGUI"), 0,0);
    pFrame->CenterWindow();
    pFrame->SetTableIdx(_T("WizardTab"),2);
    pFrame->ShowModal();
    DEBUG_INFO("\n");
    delete pFrame;
    DEBUG_INFO("\n");
    ::CoUninitialize();
    return 0;
}

