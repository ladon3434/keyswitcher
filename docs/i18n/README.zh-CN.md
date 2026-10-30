# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · **简体中文** · [繁體中文](README.zh-TW.md)

![托盘图标](../icon-preview.png)

一个适用于 Windows 11 的小工具：轻按一下 Ctrl 即可切换键盘布局。常驻系统托盘，大小约 17 KB，使用纯 WinAPI 编写，不依赖 C 运行时，也没有任何第三方依赖。

## 模式

点击托盘图标（亚美尼亚字母 Ա）选择模式：

- **左 Ctrl — 下一种语言**：左 Ctrl 在已安装的输入语言之间循环切换。
- **左 Ctrl — 主要语言，右 Ctrl — 次要语言**：左 Ctrl 切换到 Windows 语言列表中的第一种语言，右 Ctrl 切换到第二种。顺序可在“设置 → 时间和语言 → 语言和区域”中调整。

菜单中还有 **随 Windows 启动**，用于将 KeySwitcher 加入或移出当前用户的启动项。

只有单独、短促地按一下 Ctrl（少于 0.5 秒，中间没有按其他键或鼠标按钮）才会切换布局，因此 Ctrl+C、Ctrl+单击、Ctrl+滚轮和 AltGr 照常可用。所选模式保存在 `HKCU\Software\KeySwitcher\Mode`。

## 语言

菜单跟随 Windows 显示语言：英语、俄语、乌克兰语、德语、法语、西班牙语、意大利语、葡萄牙语、波兰语、土耳其语、日语、韩语、中文（简体和繁体）。其他语言显示英文。

## 安装

从 [Releases](https://github.com/ladon3434/keyswitcher/releases) 下载 `keyswitcher.exe` 并运行。

如需随 Windows 启动，请在托盘菜单中勾选 **随 Windows 启动**。

## 构建

需要 Visual Studio 2022 或更高版本，并安装 **使用 C++ 的桌面开发** 工作负载。

```
build.cmd
```

生成结果为 `build\keyswitcher.exe`。图标 `res\keyswitcher.ico` 由 `tools\make-icon.ps1` 生成。

## 限制

Windows 不会把以管理员身份运行的窗口中的按键传递给没有管理员权限的程序。若要在这类窗口中也能切换，请以管理员身份运行 KeySwitcher，例如在任务计划程序中启用“使用最高权限运行”。

## 许可证

[MIT](../../LICENSE)
