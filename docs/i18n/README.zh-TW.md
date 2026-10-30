# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · **繁體中文**

![系統匣圖示](../icon-preview.png)

一個適用於 Windows 11 的小工具：輕按一下 Ctrl 即可切換鍵盤配置。常駐系統匣，大小約 17 KB，以純 WinAPI 撰寫，不依賴 C 執行階段，也沒有任何第三方相依性。

## 模式

按一下系統匣圖示（亞美尼亞字母 Ա）選擇模式：

- **左 Ctrl — 下一種語言**：左 Ctrl 在已安裝的輸入語言之間循環切換。
- **左 Ctrl — 主要語言，右 Ctrl — 次要語言**：左 Ctrl 切換到 Windows 語言清單中的第一種語言，右 Ctrl 切換到第二種。順序可在「設定 → 時間與語言 → 語言與地區」中調整。

選單中還有 **隨 Windows 啟動**，用來將 KeySwitcher 加入或移出目前使用者的啟動程式。

只有單獨、短暫地按一下 Ctrl（少於 0.5 秒，中間沒有按其他按鍵或滑鼠按鈕）才會切換配置，因此 Ctrl+C、Ctrl+按一下、Ctrl+滾輪和 AltGr 都照常可用。所選模式儲存在 `HKCU\Software\KeySwitcher\Mode`。

## 語言

選單跟隨 Windows 顯示語言：英文、俄文、烏克蘭文、德文、法文、西班牙文、義大利文、葡萄牙文、波蘭文、土耳其文、日文、韓文、中文（簡體與繁體）。其他語言顯示英文。

## 安裝

從 [Releases](https://github.com/ladon3434/keyswitcher/releases) 下載 `keyswitcher.exe` 並執行。

若要隨 Windows 啟動，請在系統匣選單中勾選 **隨 Windows 啟動**。

## 建置

需要 Visual Studio 2022 或更新版本，並安裝 **使用 C++ 的桌面開發** 工作負載。

```
build.cmd
```

產出為 `build\keyswitcher.exe`。圖示 `res\keyswitcher.ico` 由 `tools\make-icon.ps1` 產生。

## 限制

Windows 不會把以系統管理員身分執行的視窗中的按鍵傳遞給沒有系統管理員權限的程式。若要在這類視窗中也能切換，請以系統管理員身分執行 KeySwitcher，例如在工作排程器中啟用「以最高權限執行」。

## 授權

[MIT](../../LICENSE)
