# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · **Polski** · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Ikona w zasobniku](../icon-preview.png)

Malutkie narzędzie dla Windows 11, które przełącza układ klawiatury jednym krótkim naciśnięciem Ctrl. Działa w zasobniku systemowym, waży około 17 KB i jest napisane w czystym WinAPI, bez środowiska uruchomieniowego C i bez zależności.

## Tryby

Kliknij ikonę w zasobniku (ormiańska litera Ա), aby wybrać tryb:

- **Lewy Ctrl — następny język**: lewy Ctrl przełącza po kolei zainstalowane języki wprowadzania.
- **Lewy Ctrl — język główny, prawy Ctrl — język dodatkowy**: lewy Ctrl włącza pierwszy język z listy języków Windows, prawy — drugi. Kolejność zmienisz w Ustawienia → Czas i język → Język i region.

W menu jest też opcja **Uruchamiaj z systemem Windows**, która dodaje KeySwitcher do programów startowych użytkownika lub go z nich usuwa.

Układ przełącza tylko pojedyncze, krótkie naciśnięcie Ctrl (poniżej 0,5 s, bez innego klawisza ani przycisku myszy pomiędzy), więc Ctrl+C, Ctrl+klik, Ctrl+kółko i AltGr działają jak zwykle. Wybrany tryb jest zapisywany w `HKCU\Software\KeySwitcher\Mode`.

## Języki

Menu jest wyświetlane w języku interfejsu Windows: angielski, rosyjski, ukraiński, niemiecki, francuski, hiszpański, włoski, portugalski, polski, turecki, japoński, koreański, chiński (uproszczony i tradycyjny). Dla pozostałych języków używany jest angielski.

## Instalacja

Pobierz `keyswitcher.exe` ze strony [Releases](https://github.com/ladon3434/keyswitcher/releases) i uruchom.

Aby uruchamiał się razem z Windows, zaznacz **Uruchamiaj z systemem Windows** w menu.

## Kompilacja

Wymagane jest Visual Studio 2022 lub nowsze z pakietem roboczym **Programowanie aplikacji klasycznych w języku C++**.

```
build.cmd
```

Wynik to `build\keyswitcher.exe`. Ikonę `res\keyswitcher.ico` generuje skrypt `tools\make-icon.ps1`.

## Ograniczenia

Windows nie przekazuje naciśnięć klawiszy z okien uruchomionych jako administrator do programu bez tych uprawnień. Aby przełączać układ także w takich oknach, uruchom KeySwitcher jako administrator, na przykład przez Harmonogram zadań z opcją «Uruchom z najwyższymi uprawnieniami».

## Licencja

[MIT](../../LICENSE)
