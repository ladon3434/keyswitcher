# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · **Italiano** · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Icona nell'area di notifica](../icon-preview.png)

Una piccolissima utility per Windows 11 che cambia il layout della tastiera con una sola pressione di Ctrl. Vive nell'area di notifica, pesa circa 17 KB ed è scritta in WinAPI puro, senza runtime C né dipendenze.

## Modalità

Fai clic sull'icona nell'area di notifica (la lettera armena Ա) per scegliere una modalità:

- **Ctrl sinistro — lingua successiva**: Ctrl sinistro scorre a rotazione le lingue di input installate.
- **Ctrl sinistro — lingua principale, Ctrl destro — lingua secondaria**: Ctrl sinistro attiva la prima lingua del tuo elenco di lingue di Windows, Ctrl destro la seconda. L'ordine si cambia in Impostazioni → Data/ora e lingua → Lingua e area geografica.

Il menu contiene anche **Avvia con Windows**, che aggiunge KeySwitcher ai programmi di avvio del tuo utente o lo rimuove.

Solo una pressione breve e isolata di Ctrl (meno di 0,5 s, senza altri tasti o pulsanti del mouse nel mezzo) cambia il layout, quindi Ctrl+C, Ctrl+clic, Ctrl+rotellina e AltGr continuano a funzionare. La modalità scelta è salvata in `HKCU\Software\KeySwitcher\Mode`.

## Lingue

Il menu segue la lingua di visualizzazione di Windows: inglese, russo, ucraino, tedesco, francese, spagnolo, italiano, portoghese, polacco, turco, giapponese, coreano, cinese (semplificato e tradizionale). Per le altre lingue si usa l'inglese.

## Installazione

Scarica `keyswitcher.exe` da [Releases](https://github.com/ladon3434/keyswitcher/releases) ed eseguilo.

Per avviarlo con Windows, spunta **Avvia con Windows** nel menu.

## Compilazione

Serve Visual Studio 2022 o successivo con il carico di lavoro **Sviluppo di applicazioni desktop con C++**.

```
build.cmd
```

Il risultato è `build\keyswitcher.exe`. L'icona `res\keyswitcher.ico` è generata da `tools\make-icon.ps1`.

## Limitazioni

Windows non inoltra i tasti premuti nelle finestre avviate come amministratore a un programma senza questi privilegi. Per cambiare layout anche in quelle finestre, avvia KeySwitcher come amministratore, ad esempio tramite l'Utilità di pianificazione con «Esegui con i privilegi più elevati».

## Licenza

[MIT](../../LICENSE)
