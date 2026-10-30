# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · **Español** · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Icono de la bandeja](../icon-preview.png)

Una pequeña utilidad para Windows 11 que cambia la distribución del teclado con una sola pulsación de Ctrl. Vive en la bandeja del sistema, ocupa unos 17 KB y está escrita en WinAPI puro, sin runtime de C ni dependencias.

## Modos

Haz clic en el icono de la bandeja (la letra armenia Ա) para elegir un modo:

- **Ctrl izquierdo — siguiente idioma**: Ctrl izquierdo recorre en ciclo los idiomas de entrada instalados.
- **Ctrl izquierdo — idioma principal, Ctrl derecho — idioma secundario**: Ctrl izquierdo activa el primer idioma de tu lista de idiomas de Windows y Ctrl derecho el segundo. El orden se cambia en Configuración → Hora e idioma → Idioma y región.

El menú también incluye **Iniciar con Windows**, que añade KeySwitcher a los programas de inicio de tu usuario o lo quita.

Solo una pulsación corta y aislada de Ctrl (menos de 0,5 s, sin otra tecla ni botón del ratón entre medias) cambia la distribución, así que Ctrl+C, Ctrl+clic, Ctrl+rueda y AltGr siguen funcionando. El modo elegido se guarda en `HKCU\Software\KeySwitcher\Mode`.

## Idiomas

El menú sigue el idioma de la interfaz de Windows: inglés, ruso, ucraniano, alemán, francés, español, italiano, portugués, polaco, turco, japonés, coreano, chino (simplificado y tradicional). Para otros idiomas se usa el inglés.

## Instalación

Descarga `keyswitcher.exe` desde [Releases](https://github.com/ladon3434/keyswitcher/releases) y ejecútalo.

Para que arranque con Windows, marca **Iniciar con Windows** en el menú de la bandeja.

## Compilación

Requiere Visual Studio 2022 o posterior con la carga de trabajo **Desarrollo para el escritorio con C++**.

```
build.cmd
```

El resultado es `build\keyswitcher.exe`. El icono `res\keyswitcher.ico` lo genera `tools\make-icon.ps1`.

## Limitaciones

Windows no entrega las pulsaciones de las ventanas ejecutadas como administrador a un programa sin esos permisos. Para cambiar de distribución también en esas ventanas, ejecuta KeySwitcher como administrador, por ejemplo con el Programador de tareas y la opción «Ejecutar con los privilegios más altos».

## Licencia

[MIT](../../LICENSE)
