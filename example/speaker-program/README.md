# SampleCode_SpeakerProgram-win
Qwatch搭載のスピーカーに音声を出力させるアプリケーションのサンプルソースです。 

## 概要  
- `Main.cpp` ： UI、メイン処理を行うソースファイル  
- `SendData_Proc.cpp` ：APIの生成、音声データの送信を行うソースファイル

## アプリケーションの利用方法
1. カメラのIPアドレスを入力します。  
2. カメラのパスワードを入力します。  
3. 音声ファイル名を入力します。  
※音声ファイルは`Music`フォルダ内に置いてください。

## 音声出力処理の説明
1. カメラのホストポートに対してソケット接続  
2. カメラAPI(transimit.cgi)を送信  
   ※送信するAPIは`Transmit関数`をご確認ください  
3. カメラから401 Unauthorized受信  
4. 一度セッションを切断して再接続した後にDigest認証情報を付与して再送  
5. 200 OK の応答を受信  
6. Socket 通信は維持したままコンテンツタイプに合致する音声データを送信  
7. 通話を終了後、Socket を切断

## 動作環境
Windows 10  
Windows 11

## 開発環境
Windows用C++ (VisualStudio2022, Windows10 build21H2)

## フレームワーク
Visual Studio 2022 C++ (v143)  
MFC

## 対応カメラ
TS-NS410W  
TS-NA230WP  

## 対応音声ファイル   
TS-NS410W : AAC-LC 16kHz   
TS-NA230WP : AAC-LC 16kHz

## ライセンス
ライセンスについては、[LICENSE](../../LICENSE)をご確認ください。