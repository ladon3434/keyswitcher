#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <intrin.h>

extern "C" {
#pragma function(memset)
void* __cdecl memset(void* dst, int value, size_t count)
{
    __stosb(static_cast<unsigned char*>(dst), static_cast<unsigned char>(value), count);
    return dst;
}
}

namespace {

constexpr UINT WM_TRAY = WM_APP + 1;
constexpr UINT WM_SWITCH = WM_APP + 2;
constexpr UINT ID_MODE_CYCLE = 1;
constexpr UINT ID_MODE_FIXED = 2;
constexpr UINT ID_EXIT = 3;
constexpr DWORD TAP_MAX_MS = 500;
constexpr wchar_t REG_KEY[] = L"Software\\KeySwitcher";

HWND g_wnd;
HICON g_icon;
UINT g_taskbarCreated;
DWORD g_mode;
DWORD g_pending;
DWORD g_pendingTime;
bool g_lDown, g_rDown;

void LoadMode()
{
    DWORD value = 0, size = sizeof(value);
    if (RegGetValueW(HKEY_CURRENT_USER, REG_KEY, L"Mode", RRF_RT_REG_DWORD, nullptr, &value, &size) == ERROR_SUCCESS)
        g_mode = value == 1 ? 1 : 0;
}

void SaveMode()
{
    HKEY key;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, REG_KEY, 0, nullptr, 0, KEY_SET_VALUE, nullptr, &key, nullptr) == ERROR_SUCCESS) {
        RegSetValueExW(key, L"Mode", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&g_mode), sizeof(g_mode));
        RegCloseKey(key);
    }
}

HWND TargetWindow(DWORD* threadId)
{
    HWND fg = GetForegroundWindow();
    *threadId = fg ? GetWindowThreadProcessId(fg, nullptr) : 0;
    GUITHREADINFO gi{};
    gi.cbSize = sizeof(gi);
    if (fg && GetGUIThreadInfo(*threadId, &gi) && gi.hwndFocus)
        return gi.hwndFocus;
    return fg;
}

void RequestLayout(HWND target, HKL layout)
{
    PostMessageW(target, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(layout));
}

void SwitchNext()
{
    DWORD tid;
    HWND target = TargetWindow(&tid);
    if (!target)
        return;
    HKL list[32];
    int n = GetKeyboardLayoutList(32, list);
    if (n < 2)
        return;
    HKL current = GetKeyboardLayout(tid);
    int next = 0;
    for (int i = 0; i < n; ++i)
        if (list[i] == current) { next = (i + 1) % n; break; }
    RequestLayout(target, list[next]);
}

void SwitchToLanguage(WORD primaryLang)
{
    DWORD tid;
    HWND target = TargetWindow(&tid);
    if (!target)
        return;
    HKL list[32];
    int n = GetKeyboardLayoutList(32, list);
    for (int i = 0; i < n; ++i)
        if (PRIMARYLANGID(LOWORD(reinterpret_cast<UINT_PTR>(list[i]))) == primaryLang) {
            RequestLayout(target, list[i]);
            return;
        }
}

bool ModifierHeld()
{
    return (GetAsyncKeyState(VK_SHIFT) | GetAsyncKeyState(VK_MENU) |
            GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wp, LPARAM lp)
{
    if (code == HC_ACTION) {
        const auto* k = reinterpret_cast<const KBDLLHOOKSTRUCT*>(lp);
        if (!(k->flags & LLKHF_INJECTED)) {
            const bool down = wp == WM_KEYDOWN || wp == WM_SYSKEYDOWN;
            const DWORD vk = k->vkCode;
            if (vk == VK_LCONTROL || vk == VK_RCONTROL) {
                bool& held = vk == VK_LCONTROL ? g_lDown : g_rDown;
                if (down) {
                    if (!held) {
                        held = true;
                        const bool both = g_lDown && g_rDown;
                        g_pending = (both || ModifierHeld()) ? 0 : vk;
                        g_pendingTime = k->time;
                    }
                } else {
                    held = false;
                    if (g_pending == vk && k->time - g_pendingTime <= TAP_MAX_MS)
                        PostMessageW(g_wnd, WM_SWITCH, vk, 0);
                    g_pending = 0;
                }
            } else if (down) {
                g_pending = 0;
            }
        }
    }
    return CallNextHookEx(nullptr, code, wp, lp);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wp, LPARAM lp)
{
    if (code == HC_ACTION && wp != WM_MOUSEMOVE)
        g_pending = 0;
    return CallNextHookEx(nullptr, code, wp, lp);
}

HICON MakeIcon()
{
    const int s = GetSystemMetrics(SM_CXSMICON);
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = s;
    bi.bmiHeader.biHeight = -s;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    DWORD* bits = nullptr;
    HDC screen = GetDC(nullptr);
    HBITMAP color = CreateDIBSection(screen, &bi, DIB_RGB_COLORS, reinterpret_cast<void**>(&bits), nullptr, 0);
    HDC dc = CreateCompatibleDC(screen);
    HGDIOBJ oldBmp = SelectObject(dc, color);

    HBRUSH bg = CreateSolidBrush(RGB(0x2B, 0x5C, 0xD8));
    HGDIOBJ oldBrush = SelectObject(dc, bg);
    HGDIOBJ oldPen = SelectObject(dc, GetStockObject(NULL_PEN));
    RoundRect(dc, 0, 0, s + 1, s + 1, s / 3, s / 3);

    HFONT font = CreateFontW(-(s * 3 / 4), 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
                             ANTIALIASED_QUALITY, 0, L"Segoe UI");
    HGDIOBJ oldFont = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255));
    RECT rc{0, 0, s, s};
    DrawTextW(dc, L"Ая", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    GdiFlush();

    for (int i = 0; i < s * s; ++i)
        if (bits[i]) bits[i] |= 0xFF000000;

    SelectObject(dc, oldFont);
    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
    SelectObject(dc, oldBmp);
    DeleteObject(font);
    DeleteObject(bg);
    DeleteDC(dc);
    ReleaseDC(nullptr, screen);

    HBITMAP mask = CreateBitmap(s, s, 1, 1, nullptr);
    ICONINFO ii{};
    ii.fIcon = TRUE;
    ii.hbmColor = color;
    ii.hbmMask = mask;
    HICON icon = CreateIconIndirect(&ii);
    DeleteObject(mask);
    DeleteObject(color);
    return icon;
}

void TrayIcon(DWORD action)
{
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof(nid);
    nid.hWnd = g_wnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = g_icon;
    lstrcpynW(nid.szTip, L"KeySwitcher", ARRAYSIZE(nid.szTip));
    Shell_NotifyIconW(action, &nid);
}

void ShowMenu()
{
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, ID_MODE_CYCLE, L"Левый Ctrl — переключить язык");
    AppendMenuW(menu, MF_STRING, ID_MODE_FIXED, L"Левый Ctrl — русский, правый Ctrl — английский");
    CheckMenuRadioItem(menu, ID_MODE_CYCLE, ID_MODE_FIXED, g_mode == 0 ? ID_MODE_CYCLE : ID_MODE_FIXED, MF_BYCOMMAND);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, ID_EXIT, L"Выход");

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(g_wnd);
    const UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, 0, g_wnd, nullptr);
    PostMessageW(g_wnd, WM_NULL, 0, 0);
    DestroyMenu(menu);

    if (cmd == ID_MODE_CYCLE || cmd == ID_MODE_FIXED) {
        g_mode = cmd == ID_MODE_FIXED ? 1 : 0;
        SaveMode();
    } else if (cmd == ID_EXIT) {
        DestroyWindow(g_wnd);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_SWITCH:
        if (wp == VK_LCONTROL)
            g_mode == 0 ? SwitchNext() : SwitchToLanguage(LANG_RUSSIAN);
        else if (wp == VK_RCONTROL && g_mode == 1)
            SwitchToLanguage(LANG_ENGLISH);
        return 0;
    case WM_TRAY:
        if (LOWORD(lp) == WM_RBUTTONUP || LOWORD(lp) == WM_LBUTTONUP)
            ShowMenu();
        return 0;
    case WM_DESTROY:
        TrayIcon(NIM_DELETE);
        PostQuitMessage(0);
        return 0;
    }
    if (msg == g_taskbarCreated && msg) {
        TrayIcon(NIM_ADD);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int Run()
{
    HANDLE mutex = CreateMutexW(nullptr, TRUE, L"KeySwitcher.SingleInstance");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    LoadMode();

    HINSTANCE inst = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = inst;
    wc.lpszClassName = L"KeySwitcherWnd";
    RegisterClassW(&wc);
    g_wnd = CreateWindowExW(0, wc.lpszClassName, L"KeySwitcher", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, inst, nullptr);
    if (!g_wnd)
        return 1;

    g_taskbarCreated = RegisterWindowMessageW(L"TaskbarCreated");
    g_icon = MakeIcon();
    TrayIcon(NIM_ADD);

    HHOOK kb = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, inst, 0);
    HHOOK ms = SetWindowsHookExW(WH_MOUSE_LL, MouseProc, inst, 0);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (ms) UnhookWindowsHookEx(ms);
    if (kb) UnhookWindowsHookEx(kb);
    DestroyIcon(g_icon);
    CloseHandle(mutex);
    return 0;
}

}

extern "C" void __stdcall Entry()
{
    ExitProcess(static_cast<UINT>(Run()));
}
