#include <windows.h>
#include <atlbase.h>
#include <string>
#include "NucAnalyzerLib_i.h"  // IDL’den üretilen header

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hEditInput, hBtnAnalyze, hEditResult;
    switch (msg) {
    case WM_CREATE:
        hEditInput = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL,
            10, 10, 400, 100, hWnd, (HMENU)1001, GetModuleHandle(NULL), NULL);

        hBtnAnalyze = CreateWindow(L"BUTTON", L"Analiz Et", WS_CHILD | WS_VISIBLE,
            10, 120, 100, 30, hWnd, (HMENU)1002, GetModuleHandle(NULL), NULL);

        hEditResult = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
            10, 160, 400, 100, hWnd, (HMENU)1003, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1002) {
            int len = GetWindowTextLength(hEditInput);
            std::wstring seq(len, L'\0');
            GetWindowText(hEditInput, &seq[0], len + 1);

            CComPtr<INucleotideAnalyzer> spAnalyzer;
            HRESULT hr = spAnalyzer.CoCreateInstance(CLSID_NucleotideAnalyzer);
            if (SUCCEEDED(hr)) {
                CComBSTR result;
                hr = spAnalyzer->Analyze(CComBSTR(seq.c_str()), &result);
                if (SUCCEEDED(hr)) {
                    // Sonucu edit kontrolüne yaz
                    SetWindowText(hEditResult, CW2CT(result));
                }
                else {
                    MessageBox(hWnd, L"Analyze çaðrýsý baþarýsýz oldu.", L"Hata", MB_ICONERROR);
                }
            }
            else {
                MessageBox(hWnd, L"COM nesnesi oluþturulamadý.", L"Hata", MB_ICONERROR);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow) {
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    const wchar_t CLASS_NAME[] = L"NucAnalyzerWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"NucAnalyzer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 350,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return (int)msg.wParam;
}