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
  static int iMaxWidth;
  //static int vScrollPos = 0;
  static int iVertPos;
  static int iHorzPos;
  static TEXTMETRIC tm;
  static SCROLLINFO si;

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
    
    iMaxWidth = 40 * cxChar + 22 * cxCaps;    
    return 0;
  }
    break;

  case WM_SIZE:
    cxClient = LOWORD(lparam);
    cyClient = HIWORD(lparam);
    
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = NUMLINES - 1;
    si.nPage = cyClient / cyChar;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = 2 + iMaxWidth / cxChar;
    si.nPage = cxClient / cxChar;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
    return 0;

    break;

  case WM_VSCROLL:

    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hwnd, SB_VERT, &si);

    iVertPos = si.nPos;

    switch(LOWORD(wparam))
    {
      case SB_TOP:
        si.nPos = si.nMin;
        break;

      case SB_BOTTOM:
        si.nPos = si.nMax;
        break;

      case SB_LINEUP:
        si.nPos -= 1;
        break;

      case SB_LINEDOWN:
        si.nPos += 1;
        break;

      case SB_PAGEUP:
        si.nPos -= si.nPage;
        break;

      case SB_PAGEDOWN:
        si.nPos += si.nPage;
        break;

      case SB_THUMBPOSITION:
        si.nPos = si.nTrackPos;
        break;

      default:
        break;
    }
    
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
    GetScrollInfo(hwnd, SB_VERT, &si);

    if(si.nPos != iVertPos)
    {
      ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), nullptr, nullptr);
      UpdateWindow(hwnd);
    }

    return 0;
    break;

  case WM_HSCROLL:

    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hwnd, SB_HORZ, &si);

    iHorzPos = si.nPos;

    switch(LOWORD(wparam))
    {
      case SB_LINELEFT:
        si.nPos -= 1;
        break;

      case SB_LINERIGHT:
        si.nPos += 1;
        break;

      case SB_PAGELEFT:
        si.nPos -= si.nPage;
        break;

      case SB_PAGERIGHT:
        si.nPos += si.nPage;
        break;

      case SB_THUMBPOSITION:
        si.nPos = si.nTrackPos;
        break;

      default:
        break;
    }
    
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
    GetScrollInfo(hwnd, SB_HORZ, &si);

    if(si.nPos != iHorzPos)
    {
      ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, nullptr, nullptr);
    }

    return 0;
    break;

  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    GetScrollInfo(hwnd, SB_VERT, &si);
    iVertPos = si.nPos;
    GetScrollInfo(hwnd, SB_HORZ, &si);
    iHorzPos = si.nPos;

    int iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
    int iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

    int y;
    int x;

    for(int i = iPaintBeg; i <= iPaintEnd; ++i)
    {
      x = cxChar * (1 - iHorzPos);
      y = cyChar * (i - iVertPos);

      TextOut(hdc, x, y, sysmetrics[i].szLabel, wcslen(sysmetrics[i].szLabel));
      TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, wcslen(sysmetrics[i].szDesc));

      SetTextAlign(hdc, TA_RIGHT | TA_TOP);
      wchar_t buffer[10];
      int cb = wsprintf(buffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex));
      TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, buffer, cb);
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