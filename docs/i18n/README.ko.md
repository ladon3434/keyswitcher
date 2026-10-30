# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · [日本語](README.ja.md) · **한국어** · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![트레이 아이콘](../icon-preview.png)

Ctrl을 한 번 짧게 눌러 키보드 레이아웃을 전환하는 Windows 11용 초소형 유틸리티입니다. 시스템 트레이에 상주하며 크기는 약 17KB이고, C 런타임이나 종속성 없이 순수 WinAPI로 작성되었습니다.

## 모드

트레이 아이콘(아르메니아 문자 Ա)을 클릭해 모드를 선택합니다.

- **왼쪽 Ctrl — 다음 언어**: 왼쪽 Ctrl로 설치된 입력 언어를 차례로 전환합니다.
- **왼쪽 Ctrl — 기본 언어, 오른쪽 Ctrl — 보조 언어**: 왼쪽 Ctrl은 Windows 언어 목록의 첫 번째 언어를, 오른쪽 Ctrl은 두 번째 언어를 선택합니다. 순서는 설정 → 시간 및 언어 → 언어 및 지역에서 바꿀 수 있습니다.

메뉴에는 **Windows 시작 시 실행**도 있어서 KeySwitcher를 사용자 시작 프로그램에 추가하거나 제거할 수 있습니다.

레이아웃은 Ctrl을 단독으로 짧게 눌렀을 때만(0.5초 미만, 그사이에 다른 키나 마우스 버튼을 누르지 않은 경우) 전환됩니다. 따라서 Ctrl+C, Ctrl+클릭, Ctrl+휠, AltGr은 평소처럼 동작합니다. 선택한 모드는 `HKCU\Software\KeySwitcher\Mode`에 저장됩니다.

## 언어

메뉴는 Windows 표시 언어를 따릅니다: 영어, 러시아어, 우크라이나어, 독일어, 프랑스어, 스페인어, 이탈리아어, 포르투갈어, 폴란드어, 터키어, 일본어, 한국어, 중국어(간체 및 번체). 그 밖의 언어에서는 영어로 표시됩니다.

## 설치

[Releases](https://github.com/ladon3434/keyswitcher/releases)에서 `keyswitcher.exe`를 내려받아 실행합니다.

Windows와 함께 시작하려면 트레이 메뉴에서 **Windows 시작 시 실행**을 체크합니다.

## 빌드

**C++를 사용한 데스크톱 개발** 워크로드가 포함된 Visual Studio 2022 이상이 필요합니다.

```
build.cmd
```

결과물은 `build\keyswitcher.exe`입니다. 아이콘 `res\keyswitcher.ico`는 `tools\make-icon.ps1`로 생성됩니다.

## 제한 사항

Windows는 관리자 권한으로 실행된 창의 키 입력을 관리자 권한이 없는 프로그램에 전달하지 않습니다. 이런 창에서도 전환하려면 KeySwitcher를 관리자 권한으로 실행하세요. 예를 들어 작업 스케줄러에서 "가장 높은 수준의 권한으로 실행"을 사용할 수 있습니다.

## 라이선스

[MIT](../../LICENSE)
