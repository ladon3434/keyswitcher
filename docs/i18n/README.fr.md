# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · **Français** · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![Icône de la zone de notification](../icon-preview.png)

Un minuscule utilitaire pour Windows 11 qui change la disposition du clavier d'une simple pression sur Ctrl. Il vit dans la zone de notification, pèse environ 17 Ko et est écrit en WinAPI pur, sans runtime C ni dépendances.

## Modes

Cliquez sur l'icône de la zone de notification (la lettre arménienne Ա) pour choisir un mode :

- **Ctrl gauche — langue suivante** : Ctrl gauche fait défiler les langues de saisie installées.
- **Ctrl gauche — langue principale, Ctrl droit — langue secondaire** : Ctrl gauche active la première langue de votre liste de langues Windows, Ctrl droit la deuxième. L'ordre se change dans Paramètres → Heure et langue → Langue et région.

Le menu propose aussi **Lancer au démarrage de Windows**, qui ajoute KeySwitcher aux programmes de démarrage de votre utilisateur ou l'en retire.

Seule une pression courte et isolée sur Ctrl (moins de 0,5 s, sans autre touche ni bouton de souris entre-temps) change la disposition. Ctrl+C, Ctrl+clic, Ctrl+molette et AltGr continuent donc de fonctionner. Le mode choisi est enregistré dans `HKCU\Software\KeySwitcher\Mode`.

## Langues

Le menu suit la langue d'affichage de Windows : anglais, russe, ukrainien, allemand, français, espagnol, italien, portugais, polonais, turc, japonais, coréen, chinois (simplifié et traditionnel). Les autres langues utilisent l'anglais.

## Installation

Téléchargez `keyswitcher.exe` depuis [Releases](https://github.com/ladon3434/keyswitcher/releases) et lancez-le.

Pour le lancer avec Windows, cochez **Lancer au démarrage de Windows** dans le menu.

## Compilation

Nécessite Visual Studio 2022 ou plus récent avec la charge de travail **Développement Desktop en C++**.

```
build.cmd
```

Le résultat est `build\keyswitcher.exe`. L'icône `res\keyswitcher.ico` est générée par `tools\make-icon.ps1`.

## Limites

Windows ne transmet pas les frappes des fenêtres lancées en administrateur à un programme sans ces droits. Pour changer de disposition aussi dans ces fenêtres, lancez KeySwitcher en administrateur, par exemple via le Planificateur de tâches avec « Exécuter avec les autorisations maximales ».

## Licence

[MIT](../../LICENSE)
