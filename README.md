# KeySwitcher

[![Build](https://github.com/ladon3434/keyswitcher/actions/workflows/build.yml/badge.svg)](https://github.com/ladon3434/keyswitcher/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

[Русский](README.ru.md)

A tiny Windows 11 tray utility that switches the keyboard layout with a single tap of Ctrl. About 9 KB, plain WinAPI, no C runtime, no dependencies.

## Modes

Click the «Ая» tray icon to choose a mode:

- **Left Ctrl — next language**: Left Ctrl cycles through the installed input languages.
- **Left Ctrl — Russian, Right Ctrl — English**: each Ctrl selects its own language.

Only a lone, short Ctrl tap (under 0.5 s, no other key or mouse button in between) switches the layout, so Ctrl+C, Ctrl+click, Ctrl+wheel and AltGr keep working. The chosen mode is stored in `HKCU\Software\KeySwitcher\Mode`.

## Install

Download `keyswitcher.exe` from [Releases](https://github.com/ladon3434/keyswitcher/releases) and run it.

To start it with Windows, press Win+R, type `shell:startup` and put a shortcut to `keyswitcher.exe` there.

## Build

Requires Visual Studio 2022 or newer with the **Desktop development with C++** workload.

```
build.cmd
```

The result is `build\keyswitcher.exe`.

## Limitations

Windows does not deliver keystrokes from elevated (administrator) windows to a non-elevated program. To switch layouts in such windows too, run KeySwitcher elevated, for example via Task Scheduler with «Run with highest privileges».

## License

[MIT](LICENSE)
