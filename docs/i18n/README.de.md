# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · **Deutsch** · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Taskleistensymbol](../icon-preview.png)

Ein winziges Tool für Windows 11, das das Tastaturlayout mit einem kurzen Druck auf Strg umschaltet. Läuft im Infobereich der Taskleiste, ist etwa 17 KB groß und in reinem WinAPI geschrieben, ohne C-Runtime und ohne Abhängigkeiten.

## Modi

Klicken Sie auf das Symbol im Infobereich (der armenische Buchstabe Ա), um einen Modus zu wählen:

- **Linke Strg-Taste — nächste Sprache**: Die linke Strg-Taste wechselt reihum durch die installierten Eingabesprachen.
- **Linke Strg-Taste — Hauptsprache, rechte Strg-Taste — Zweitsprache**: Die linke Strg-Taste wählt die erste Sprache aus Ihrer Windows-Sprachliste, die rechte die zweite. Die Reihenfolge ändern Sie unter Einstellungen → Zeit und Sprache → Sprache und Region.

Im Menü gibt es außerdem **Mit Windows starten**: Damit wird KeySwitcher zum Autostart Ihres Benutzers hinzugefügt oder daraus entfernt.

Nur ein einzelner, kurzer Druck auf Strg (unter 0,5 s, ohne andere Taste oder Maustaste dazwischen) schaltet das Layout um. Strg+C, Strg+Klick, Strg+Mausrad und AltGr funktionieren also weiterhin. Der gewählte Modus wird in `HKCU\Software\KeySwitcher\Mode` gespeichert.

## Sprachen

Das Menü folgt der Anzeigesprache von Windows: Englisch, Russisch, Ukrainisch, Deutsch, Französisch, Spanisch, Italienisch, Portugiesisch, Polnisch, Türkisch, Japanisch, Koreanisch, Chinesisch (vereinfacht und traditionell). Für andere Sprachen wird Englisch verwendet.

## Installation

Laden Sie `keyswitcher.exe` unter [Releases](https://github.com/ladon3434/keyswitcher/releases) herunter und starten Sie die Datei.

Damit das Tool mit Windows startet, setzen Sie im Menü den Haken bei **Mit Windows starten**.

## Erstellen

Benötigt wird Visual Studio 2022 oder neuer mit der Workload **Desktopentwicklung mit C++**.

```
build.cmd
```

Das Ergebnis ist `build\keyswitcher.exe`. Das Symbol `res\keyswitcher.ico` wird von `tools\make-icon.ps1` erzeugt.

## Einschränkungen

Windows leitet Tastendrücke aus Fenstern mit Administratorrechten nicht an ein Programm ohne diese Rechte weiter. Damit das Umschalten auch dort funktioniert, starten Sie KeySwitcher mit Administratorrechten, zum Beispiel über die Aufgabenplanung mit «Mit höchsten Privilegien ausführen».

## Lizenz

[MIT](../../LICENSE)
