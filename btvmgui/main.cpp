



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);

    CFrameWnd *pFrame = new CFrameWnd(_T("btvmgui.xml"));
    pFrame->Create(NULL, _T("BTVMGUI"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowModal();

    delete pFrame;
    ::CoUninitialize();
    return 0;
}

