#include "wndproc.h"
#include "sysmets.h"

LRESULT CALLBACK WindowProc(
  HWND   hwnd,
  UINT   msg,
  WPARAM wparam,
  LPARAM lparam)
{
  static int cxChar;
  static int cxCaps;
  static int cyChar;

  switch(msg)
  {
  case WM_CREATE:
    break;

  case WM_COMMAND:
    break;

  case WM_CTLCOLORSTATIC:
    break;

  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
    cyChar = tm.tmHeight + tm.tmExternalLeading;

    for(int i = 0; i < NUMLINES; ++i)
    {
      TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, wcslen(sysmetrics[i].szLabel));
      TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, wcslen(sysmetrics[i].szDesc));

      SetTextAlign(hdc, TA_RIGHT | TA_TOP);
      wchar_t buffer[10];
      int cb = wsprintf(buffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex));
      TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, buffer, cb);
      SetTextAlign(hdc, TA_LEFT | TA_TOP);
    }

    EndPaint(hwnd, &ps);
    return 0;
  }
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