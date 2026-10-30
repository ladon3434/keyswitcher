# KeySwitcher

[English](../../README.md) · [Русский](README.ru.md) · [Українська](README.uk.md) · [Deutsch](README.de.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [Português](README.pt.md) · [Polski](README.pl.md) · [Türkçe](README.tr.md) · **日本語** · [한국어](README.ko.md) · [简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md)

![トレイアイコン](../icon-preview.png)

Ctrl を 1 回短く押すだけでキーボード レイアウトを切り替える、Windows 11 用の小さなユーティリティです。タスク トレイに常駐し、サイズは約 17 KB。C ランタイムも依存関係も使わない純粋な WinAPI で書かれています。

## モード

トレイ アイコン (アルメニア文字 Ա) をクリックしてモードを選びます。

- **左 Ctrl — 次の言語**: 左 Ctrl で、インストールされている入力言語を順番に切り替えます。
- **左 Ctrl — メイン言語、右 Ctrl — サブ言語**: 左 Ctrl で Windows の言語一覧の 1 番目の言語、右 Ctrl で 2 番目の言語に切り替えます。順序は「設定 → 時刻と言語 → 言語と地域」で変更できます。

メニューには **Windows の起動時に実行** もあり、KeySwitcher をユーザーのスタートアップに追加したり削除したりできます。

レイアウトが切り替わるのは、Ctrl を単独で短く押したとき (0.5 秒未満、途中でほかのキーやマウス ボタンを押さない場合) だけです。そのため Ctrl+C、Ctrl+クリック、Ctrl+ホイール、AltGr はこれまでどおり使えます。選んだモードは `HKCU\Software\KeySwitcher\Mode` に保存されます。

## 言語

メニューは Windows の表示言語に従います: 英語、ロシア語、ウクライナ語、ドイツ語、フランス語、スペイン語、イタリア語、ポルトガル語、ポーランド語、トルコ語、日本語、韓国語、中国語 (簡体字・繁体字)。その他の言語では英語で表示されます。

## インストール

[Releases](https://github.com/ladon3434/keyswitcher/releases) から `keyswitcher.exe` をダウンロードして実行します。

Windows と一緒に起動するには、トレイ メニューで **Windows の起動時に実行** にチェックを入れます。

## ビルド

**C++ によるデスクトップ開発** ワークロードを含む Visual Studio 2022 以降が必要です。

```
build.cmd
```

出力は `build\keyswitcher.exe` です。アイコン `res\keyswitcher.ico` は `tools\make-icon.ps1` で生成されます。

## 制限事項

Windows は、管理者として実行されているウィンドウのキー入力を、管理者権限のないプログラムには渡しません。そのようなウィンドウでも切り替えたい場合は、KeySwitcher を管理者として実行してください。たとえばタスク スケジューラで「最上位の特権で実行する」を有効にします。

## ライセンス

[MIT](../../LICENSE)
