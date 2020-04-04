macro InsertDebugInfo()
{
    hwnd = GetCurrentWnd()
    if (hwnd == 0)
        stop
    sel = GetWndSel(hwnd)
    hbuf = GetWndBuf(hwnd)
    
    //szline==printf("\r\n[%16s: %4d][%s] \r\n", __FILE__, __LINE__, __func__);
    szline = "printf(\"\\r\\n[%16s: %4d][%s] \\r\\n\", __FILE__, __LINE__, __func__);"
    
    SetBufSelText(hbuf, szline)
}