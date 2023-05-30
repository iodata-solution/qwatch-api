# SampleCode_discovery-win
このサンプルでは、同一ネットワーク上にあるカメラ製品の検索方法を想定し、WS-Discoveryを使い開発しています。  

## 概要
- `SampleCode_discoveryDlg.cpp`　：UIの処理を行うソースファイル  
- `discovery_proc.cpp`　　　　　  ：検索処理を行うソースファイル

## アプリケーションの利用方法
1.  検索パケットを送信するインターフェイスのIPアドレスを設定します。  
2.  デバイス検索ボタンを押します。  
3.  数秒後リストにカメラのIPアドレス、デバイス名、UUIDが表示されます。  
4.  リストに表示されたカメラを選択し、OKボタンを押すとカメラの設定画面が表示されます。

## 動作環境
Windows10  
Windows11

## 開発環境
Windows用C++ (VisualStudio2022, Windows10 build21H2にて作成)

## 対応カメラ
※2023/03/07時点  
- TS-NS110W  
- TS-NS210   : ONVIF S  
- TS-NA220   : ONVIF S  
- TS-NA220W  : ONVIF S  
- TS-NS310W  : ONVIF S,T  
- TS-NS410W  : ONVIF S  
- TS-NA230WP : ONVIF S  

## 注意事項
- 他OSで動作させるためには、WindowsAPI使用箇所の変更が必要です。  
- 本プログラムは、弊社のカメラ製品のみ検索が可能です。

## ライセンス
ライセンスについては、[LICENSE](../../LICENSE)をご確認ください。