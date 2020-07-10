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
  static int cxClient;
  static int cyClient;
  static int vScrollPos = 0;
  static TEXTMETRIC tm;

  switch(msg)
  {
  case WM_CREATE:
  {
    HDC hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
    cyChar = tm.tmHeight + tm.tmExternalLeading;
    ReleaseDC(hwnd, hdc);
    SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
    SetScrollPos(hwnd, SB_VERT, vScrollPos, TRUE);
    return 0;
  }
    break;

  case WM_SIZE:
    cxClient = LOWORD(lparam);
    cyClient = HIWORD(lparam);
    break;

  case WM_VSCROLL:
    switch(LOWORD(wparam))
    {
      case SB_LINEUP:
        vScrollPos -= 1;
        break;

      case SB_LINEDOWN:
        vScrollPos += 1;
        break;

      case SB_PAGEUP:
        vScrollPos -= cyClient / cyChar;
        break;

      case SB_PAGEDOWN:
        vScrollPos += cyClient / cyChar;
        break;

      case SB_THUMBPOSITION:
        vScrollPos = HIWORD(wparam);
        break;

      default:
        break;
    }

    vScrollPos = max(0, min(vScrollPos, NUMLINES -1));
    if(vScrollPos != GetScrollPos(hwnd, SB_VERT))
    {
      SetScrollPos(hwnd, SB_VERT, vScrollPos, TRUE);
      InvalidateRect(hwnd, nullptr, TRUE);
    }

    return 0;
    break;

  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    int y;

    for(int i = 0; i < NUMLINES; ++i)
    {
      y = cyChar * (i - vScrollPos);

      TextOut(hdc, 0, y, sysmetrics[i].szLabel, wcslen(sysmetrics[i].szLabel));
      TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, wcslen(sysmetrics[i].szDesc));

      SetTextAlign(hdc, TA_RIGHT | TA_TOP);
      wchar_t buffer[10];
      int cb = wsprintf(buffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex));
      TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, buffer, cb);
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