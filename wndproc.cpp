#include "wndproc.h"

LRESULT CALLBACK WindowProc(
  HWND   hwnd,
  UINT   msg,
  WPARAM wparam,
  LPARAM lparam)
{
  switch(msg)
  {
  case WM_CREATE:
    break;

  case WM_COMMAND:
    break;

  case WM_CTLCOLORSTATIC:
    break;

  case WM_PAINT:
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }

  return 0;
}