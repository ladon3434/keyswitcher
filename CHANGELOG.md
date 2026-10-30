# Changelog

All notable changes to this project are documented here. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project uses [Semantic Versioning](https://semver.org/).

## [1.1.1] - 2026-10-30

### Added

- The Ա icon and version information are embedded in the executable, so it shows its icon, name, version and copyright in Explorer and Task Manager.

### Changed

- The tray icon is loaded from the executable instead of being drawn at startup.

## [1.1.0] - 2026-10-30

### Added

- «Start with Windows» checkbox in the tray menu, translated into all supported languages. The startup entry follows the executable if it is moved.

## [1.0.0] - 2026-10-30

### Added

- Tray icon (the Armenian letter Ա) with a mode menu and Exit.
- Mode «Left Ctrl — next language».
- Mode «Left Ctrl — primary language, Right Ctrl — secondary», following the order of the Windows language list.
- Menu in the Windows display language: English, Russian, Ukrainian, German, French, Spanish, Italian, Portuguese, Polish, Turkish, Japanese, Korean, Chinese (Simplified and Traditional).
- Lone-tap detection that leaves Ctrl shortcuts, Ctrl+mouse and AltGr intact.
- Single instance; the tray icon comes back after Explorer restarts.

[1.1.1]: https://github.com/ladon3434/keyswitcher/releases/tag/v1.1.1
[1.1.0]: https://github.com/ladon3434/keyswitcher/releases/tag/v1.1.0
[1.0.0]: https://github.com/ladon3434/keyswitcher/releases/tag/v1.0.0
