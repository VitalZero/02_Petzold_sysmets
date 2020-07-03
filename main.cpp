// ===========================================================
// File: main.cpp
// Date: 03/July/2020
// Description: List system metrics as in Petzold's Programming in
//   Windows 5th edition. SYSMETS1.C
// Copyright: VZ
// ===========================================================

#include "includes.h"
#include "wndproc.h"

ATOM RegisterWC(const wchar_t* className);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(
  HINSTANCE instance,
  HINSTANCE,
  LPSTR cmd,
  int show)
{
  const wchar_t* CLASS_NAME = L"VZ Window";
  const wchar_t* WINDOW_NAME = L"Petzold's list system metrics";

  if(!RegisterWC(CLASS_NAME))
  {
    MessageBox(nullptr, L"Error", L"No se pudo registrar la ventana",
	       MB_ICONINFORMATION);
    return FALSE;
  }
  
  HWND mainWindow = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    CLASS_NAME,
    WINDOW_NAME,
    WS_OVERLAPPEDWINDOW | WS_VSCROLL,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    instance,
    nullptr);

  if(!mainWindow)
  {
    MessageBox(0, L"Error", L"No se pudo crear la ventana principal",
	       MB_ICONINFORMATION);
    return FALSE;
  }

  ShowWindow(mainWindow, show);
  UpdateWindow(mainWindow);

  MSG msg = {0};
  BOOL retMsg;
  while((retMsg = GetMessage(&msg, 0, 0, 0)) != 0)
  {
    if(retMsg == -1)
    {
    }
    else
    {
      if(!IsDialogMessage(mainWindow, &msg))
      {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
      }
    }
  }
  
  return msg.wParam;
}

ATOM RegisterWC(const wchar_t* className)
{
  WNDCLASS wc = {0};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.hIcon = LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);
  wc.hCursor = LoadCursor(GetModuleHandle(nullptr), IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszClassName = className;
  wc.style = CS_VREDRAW | CS_HREDRAW;

  return RegisterClass(&wc);
}