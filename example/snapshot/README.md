# SnapshotSample-win
このサンプルでは、QwatchのLiveView映像をお客様が用意したWebサイト上で表示することを想定し、html形式にて作成しています。  

## 概要
`index.html`     : カメラ1台のLive viewを表示するHTML  
`tile_video.html`: カメラ6台分のLive viewを同時に表示するHTML

## 利用方法
HTMLファイルにある`http://xxx.xxx.xxx.xxx:80/snapshot.cgi`のIPアドレスをお使いのカメラのものに変更してください。

## 動作環境
Google Chrome  
Microsoft Edge

## 対応カメラ  
※2023/03/07時点  
- TS-NA230WP
- TS-NA220W
- TS-NA220
- TS-NS410W
- TS-NA310W
- TS-WRLP

## 注意事項
- 本サンプルはローカル環境にあるカメラのLiveViewのみ表示可能です。  
- LiveView表示にはDigest認証が必要です。起動するとDigest認証のダイアログが表示されます。

## ライセンス
ライセンスについては、[LICENSE](../../LICENSE)をご確認ください。