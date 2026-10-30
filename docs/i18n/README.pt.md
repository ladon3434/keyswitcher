# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · **Português** · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Ícone da bandeja](../icon-preview.png)

Um pequeno utilitário para Windows 11 que troca o layout do teclado com um único toque em Ctrl. Fica na bandeja do sistema, tem cerca de 17 KB e é escrito em WinAPI puro, sem runtime C nem dependências.

## Modos

Clique no ícone da bandeja (a letra armênia Ա) para escolher um modo:

- **Ctrl esquerdo — próximo idioma**: o Ctrl esquerdo alterna entre os idiomas de entrada instalados.
- **Ctrl esquerdo — idioma principal, Ctrl direito — idioma secundário**: o Ctrl esquerdo ativa o primeiro idioma da sua lista de idiomas do Windows e o Ctrl direito, o segundo. A ordem é alterada em Configurações → Hora e idioma → Idioma e região.

O menu também tem **Iniciar com o Windows**, que adiciona o KeySwitcher aos programas de inicialização do seu usuário ou o remove.

Só um toque curto e isolado em Ctrl (menos de 0,5 s, sem outra tecla ou botão do mouse no meio) troca o layout, então Ctrl+C, Ctrl+clique, Ctrl+roda do mouse e AltGr continuam funcionando. O modo escolhido fica salvo em `HKCU\Software\KeySwitcher\Mode`.

## Idiomas

O menu segue o idioma de exibição do Windows: inglês, russo, ucraniano, alemão, francês, espanhol, italiano, português, polonês, turco, japonês, coreano, chinês (simplificado e tradicional). Para outros idiomas, é usado o inglês.

## Instalação

Baixe o `keyswitcher.exe` em [Releases](https://github.com/ladon3434/keyswitcher/releases) e execute-o.

Para iniciá-lo com o Windows, marque **Iniciar com o Windows** no menu da bandeja.

## Compilação

É necessário o Visual Studio 2022 ou mais recente com a carga de trabalho **Desenvolvimento para desktop com C++**.

```
build.cmd
```

O resultado é `build\keyswitcher.exe`. O ícone `res\keyswitcher.ico` é gerado por `tools\make-icon.ps1`.

## Limitações

O Windows não entrega as teclas pressionadas em janelas executadas como administrador a um programa sem esses privilégios. Para trocar o layout também nessas janelas, execute o KeySwitcher como administrador, por exemplo pelo Agendador de Tarefas com «Executar com privilégios mais altos».

## Licença

[MIT](../../LICENSE)
