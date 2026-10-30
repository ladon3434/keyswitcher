# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · **Türkçe** · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Sistem tepsisi simgesi](../icon-preview.png)

Klavye düzenini Ctrl'ye tek bir kısa basışla değiştiren, Windows 11 için küçücük bir araç. Sistem tepsisinde çalışır, yaklaşık 17 KB boyutundadır ve C çalışma zamanı ya da bağımlılık olmadan saf WinAPI ile yazılmıştır.

## Modlar

Bir mod seçmek için sistem tepsisindeki simgeye (Ermeni harfi Ա) tıklayın:

- **Sol Ctrl — sonraki dil**: Sol Ctrl, yüklü giriş dilleri arasında sırayla geçiş yapar.
- **Sol Ctrl — ana dil, sağ Ctrl — ikinci dil**: Sol Ctrl, Windows dil listenizdeki ilk dili, sağ Ctrl ise ikinci dili seçer. Sırayı Ayarlar → Zaman ve dil → Dil ve bölge bölümünden değiştirebilirsiniz.

Menüde ayrıca **Windows ile başlat** seçeneği vardır. Bu seçenek KeySwitcher'ı kullanıcınızın başlangıç programlarına ekler veya oradan kaldırır.

Düzeni yalnızca tek başına, kısa bir Ctrl basışı (0,5 saniyeden kısa, arada başka tuş veya fare düğmesi olmadan) değiştirir. Bu yüzden Ctrl+C, Ctrl+tıklama, Ctrl+tekerlek ve AltGr her zamanki gibi çalışır. Seçilen mod `HKCU\Software\KeySwitcher\Mode` içinde saklanır.

## Diller

Menü, Windows görüntüleme dilini izler: İngilizce, Rusça, Ukraynaca, Almanca, Fransızca, İspanyolca, İtalyanca, Portekizce, Lehçe, Türkçe, Japonca, Korece, Çince (Basitleştirilmiş ve Geleneksel). Diğer dillerde İngilizce kullanılır.

## Kurulum

`keyswitcher.exe` dosyasını [Releases](https://github.com/ladon3434/keyswitcher/releases) sayfasından indirip çalıştırın.

Windows ile birlikte başlaması için menüde **Windows ile başlat** seçeneğini işaretleyin.

## Derleme

**C++ ile masaüstü geliştirme** iş yükü yüklü Visual Studio 2022 veya daha yeni bir sürüm gerekir.

```
build.cmd
```

Sonuç `build\keyswitcher.exe` dosyasıdır. `res\keyswitcher.ico` simgesi `tools\make-icon.ps1` ile oluşturulur.

## Sınırlamalar

Windows, yönetici olarak çalışan pencerelerdeki tuş basışlarını yönetici olmayan bir programa iletmez. Bu pencerelerde de düzen değiştirmek için KeySwitcher'ı yönetici olarak çalıştırın, örneğin Görev Zamanlayıcı'da «En yüksek ayrıcalıklarla çalıştır» seçeneğiyle.

## Lisans

[MIT](../../LICENSE)
