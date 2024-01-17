#!/bin/sh

command=curl
#command=curl.exe
idpass=admin:abcd1234
#host=192.168.10.101
host=192.168.0.28

moveshot() {
  # pos_x pos_y filename sleep
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=update&PT_preset4_x=$1&PT_preset4_y=$2"
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=list&group=PT_preset4"   
  $command --anyauth "http://$idpass@$host/camera-cgi/com/ptzconfig.cgi?movepresetpoint=4"
  sleep $4
  $command --anyauth "http://$idpass@$host/snapshot.jpg" -o $3
}
#exit
movehome() {
  $command --anyauth "http://$idpass@$host/camera-cgi/com/ptz.cgi?move=home"
  sleep 5
}

# 簡易キャリブレーション
movehome
moveshot -32 -8 01.jpg 6 # 左下
moveshot  32 20 01.jpg 11 # 右上

# 撮影開始
moveshot   1 20 01.jpg 7 # 真上

moveshot  17 10 03.jpg 5 # 右上
moveshot   1 10 02.jpg 5 # 前上
moveshot -15 10 05.jpg 5 # 左上
moveshot -32 10 04.jpg 5 # 後上

moveshot -32  0 08.jpg 4 # 後
moveshot -15  0 09.jpg 5 # 左
moveshot   1  0 06.jpg 5 # 前
moveshot  17  0 07.jpg 5 # 右

moveshot  17 -8 11.jpg 4 # 右下
moveshot   1 -8 10.jpg 5 # 前下
moveshot -15 -8 13.jpg 5 # 左下
moveshot -32 -8 12.jpg 5 # 後下

movehome

exit