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
constexpr UINT ID_AUTOSTART = 4;
constexpr DWORD TAP_MAX_MS = 500;
constexpr wchar_t REG_KEY[] = L"Software\\KeySwitcher";
constexpr wchar_t RUN_KEY[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
constexpr wchar_t APPROVED_KEY[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run";
constexpr wchar_t APP_NAME[] = L"KeySwitcher";

HWND g_wnd;
HICON g_icon;
UINT g_taskbarCreated;
DWORD g_mode;
DWORD g_pending;
DWORD g_pendingTime;
bool g_lDown, g_rDown;

struct Strings {
    WORD lang;
    const wchar_t* cycle;
    const wchar_t* fixed;
    const wchar_t* exit;
    const wchar_t* autostart;
};

const Strings STRINGS[] = {
    {LANG_ENGLISH, L"Left Ctrl — next language", L"Left Ctrl — primary language, Right Ctrl — secondary", L"Exit", L"Start with Windows"},
    {LANG_RUSSIAN, L"Левый Ctrl — следующий язык", L"Левый Ctrl — основной язык, правый Ctrl — дополнительный", L"Выход", L"Запускать вместе с Windows"},
    {LANG_UKRAINIAN, L"Лівий Ctrl — наступна мова", L"Лівий Ctrl — основна мова, правий Ctrl — додаткова", L"Вихід", L"Запускати разом із Windows"},
    {LANG_GERMAN, L"Linke Strg-Taste — nächste Sprache", L"Linke Strg-Taste — Hauptsprache, rechte Strg-Taste — Zweitsprache", L"Beenden", L"Mit Windows starten"},
    {LANG_FRENCH, L"Ctrl gauche — langue suivante", L"Ctrl gauche — langue principale, Ctrl droit — langue secondaire", L"Quitter", L"Lancer au démarrage de Windows"},
    {LANG_SPANISH, L"Ctrl izquierdo — siguiente idioma", L"Ctrl izquierdo — idioma principal, Ctrl derecho — idioma secundario", L"Salir", L"Iniciar con Windows"},
    {LANG_ITALIAN, L"Ctrl sinistro — lingua successiva", L"Ctrl sinistro — lingua principale, Ctrl destro — lingua secondaria", L"Esci", L"Avvia con Windows"},
    {LANG_PORTUGUESE, L"Ctrl esquerdo — próximo idioma", L"Ctrl esquerdo — idioma principal, Ctrl direito — idioma secundário", L"Sair", L"Iniciar com o Windows"},
    {LANG_POLISH, L"Lewy Ctrl — następny język", L"Lewy Ctrl — język główny, prawy Ctrl — język dodatkowy", L"Zakończ", L"Uruchamiaj z systemem Windows"},
    {LANG_TURKISH, L"Sol Ctrl — sonraki dil", L"Sol Ctrl — ana dil, sağ Ctrl — ikinci dil", L"Çıkış", L"Windows ile başlat"},
    {LANG_JAPANESE, L"左 Ctrl — 次の言語", L"左 Ctrl — メイン言語、右 Ctrl — サブ言語", L"終了", L"Windows の起動時に実行"},
    {LANG_KOREAN, L"왼쪽 Ctrl — 다음 언어", L"왼쪽 Ctrl — 기본 언어, 오른쪽 Ctrl — 보조 언어", L"종료", L"Windows 시작 시 실행"},
    {LANG_CHINESE_SIMPLIFIED, L"左 Ctrl — 下一种语言", L"左 Ctrl — 主要语言，右 Ctrl — 次要语言", L"退出", L"随 Windows 启动"},
};

const Strings TRADITIONAL_CHINESE = {LANG_CHINESE_TRADITIONAL, L"左 Ctrl — 下一種語言", L"左 Ctrl — 主要語言，右 Ctrl — 次要語言", L"結束", L"隨 Windows 啟動"};

const Strings& UiStrings()
{
    const LANGID ui = GetUserDefaultUILanguage();
    const WORD primary = PRIMARYLANGID(ui);
    if (primary == LANG_CHINESE) {
        const WORD sub = SUBLANGID(ui);
        if (sub == SUBLANG_CHINESE_TRADITIONAL || sub == SUBLANG_CHINESE_HONGKONG || sub == SUBLANG_CHINESE_MACAU)
            return TRADITIONAL_CHINESE;
        return STRINGS[ARRAYSIZE(STRINGS) - 1];
    }
    for (const Strings& s : STRINGS)
        if (s.lang == primary)
            return s;
    return STRINGS[0];
}

bool AutostartRegistered()
{
    return RegGetValueW(HKEY_CURRENT_USER, RUN_KEY, APP_NAME, RRF_RT_REG_SZ, nullptr, nullptr, nullptr) == ERROR_SUCCESS;
}

bool AutostartEnabled()
{
    if (!AutostartRegistered())
        return false;
    BYTE state[12];
    DWORD size = sizeof(state);
    if (RegGetValueW(HKEY_CURRENT_USER, APPROVED_KEY, APP_NAME, RRF_RT_REG_BINARY, nullptr, state, &size) == ERROR_SUCCESS)
        return !(state[0] & 1);
    return true;
}

void SetAutostart(bool enable)
{
    RegDeleteKeyValueW(HKEY_CURRENT_USER, APPROVED_KEY, APP_NAME);
    if (!enable) {
        RegDeleteKeyValueW(HKEY_CURRENT_USER, RUN_KEY, APP_NAME);
        return;
    }
    wchar_t cmd[MAX_PATH + 2];
    cmd[0] = L'"';
    const DWORD len = GetModuleFileNameW(nullptr, cmd + 1, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
        return;
    cmd[len + 1] = L'"';
    cmd[len + 2] = 0;
    RegSetKeyValueW(HKEY_CURRENT_USER, RUN_KEY, APP_NAME, REG_SZ, cmd, (len + 3) * sizeof(wchar_t));
}

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

WORD PreloadLanguage(const wchar_t* slot)
{
    wchar_t buf[16];
    DWORD size = sizeof(buf);
    if (RegGetValueW(HKEY_CURRENT_USER, L"Keyboard Layout\\Preload", slot, RRF_RT_REG_SZ, nullptr, buf, &size) != ERROR_SUCCESS)
        return 0;
    DWORD value = 0;
    for (const wchar_t* p = buf; *p; ++p) {
        const wchar_t c = *p;
        DWORD digit;
        if (c >= L'0' && c <= L'9') digit = c - L'0';
        else if (c >= L'a' && c <= L'f') digit = c - L'a' + 10;
        else if (c >= L'A' && c <= L'F') digit = c - L'A' + 10;
        else return 0;
        value = (value << 4) | digit;
    }
    return LOWORD(value);
}

void SwitchToSlot(int slot)
{
    DWORD tid;
    HWND target = TargetWindow(&tid);
    if (!target)
        return;
    HKL list[32];
    const int n = GetKeyboardLayoutList(32, list);
    if (n <= slot)
        return;
    const WORD lang = PreloadLanguage(slot == 0 ? L"1" : L"2");
    for (int i = 0; lang && i < n; ++i)
        if (LOWORD(reinterpret_cast<UINT_PTR>(list[i])) == lang) {
            RequestLayout(target, list[i]);
            return;
        }
    RequestLayout(target, list[slot]);
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

    HFONT font = CreateFontW(-(s * 7 / 8), 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
                             ANTIALIASED_QUALITY, 0, L"Segoe UI");
    HGDIOBJ oldFont = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255));
    RECT rc{0, 0, s, s};
    DrawTextW(dc, L"Ա", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
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
    const Strings& text = UiStrings();
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, ID_MODE_CYCLE, text.cycle);
    AppendMenuW(menu, MF_STRING, ID_MODE_FIXED, text.fixed);
    CheckMenuRadioItem(menu, ID_MODE_CYCLE, ID_MODE_FIXED, g_mode == 0 ? ID_MODE_CYCLE : ID_MODE_FIXED, MF_BYCOMMAND);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING | (AutostartEnabled() ? MF_CHECKED : MF_UNCHECKED), ID_AUTOSTART, text.autostart);
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, ID_EXIT, text.exit);

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(g_wnd);
    const UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, 0, g_wnd, nullptr);
    PostMessageW(g_wnd, WM_NULL, 0, 0);
    DestroyMenu(menu);

    if (cmd == ID_MODE_CYCLE || cmd == ID_MODE_FIXED) {
        g_mode = cmd == ID_MODE_FIXED ? 1 : 0;
        SaveMode();
    } else if (cmd == ID_AUTOSTART) {
        SetAutostart(!AutostartEnabled());
    } else if (cmd == ID_EXIT) {
        DestroyWindow(g_wnd);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_SWITCH:
        if (wp == VK_LCONTROL)
            g_mode == 0 ? SwitchNext() : SwitchToSlot(0);
        else if (wp == VK_RCONTROL && g_mode == 1)
            SwitchToSlot(1);
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
    if (AutostartEnabled())
        SetAutostart(true);

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
