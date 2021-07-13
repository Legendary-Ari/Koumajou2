#pragma once
#ifdef _AFX
const int WINCX = 1600;
const int WINCY = 800;
const int CLIENTCX = 1040;
const int CLIENTCY = 624;
#else
const int WINCX = 1040;
const int WINCY = 624;
const int CLIENTCX = 520;
const int CLIENTCY = 312;
#endif // _AFX

extern HWND g_hWND; 

