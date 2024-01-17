#!/bin/sh

command=curl
#command=curl.exe
idpass=admin:io1234
#host=192.168.10.101
host=192.168.0.28
index=3

move() {
  # pos_x pos_y sleep
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=update&PT_preset4_x=$1&PT_preset4_y=$2"
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=list&group=PT_preset4"   
  $command --anyauth "http://$idpass@$host/camera-cgi/com/ptzconfig.cgi?movepresetpoint=4"
  sleep $3
}

movehome() {
  $command --anyauth "http://$idpass@$host/camera-cgi/com/ptz.cgi?move=home"
  sleep 5
}

list() {
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=StorageMusicInfo"
  exit
}

play() {
  $command --anyauth "http://$idpass@$host/camera-cgi/admin/param.cgi?action=MusicCtrl&MusicPlay=$1"
}

#list

move   0 -8 0 # 前下
move   0  0 0 # 前
movehome

sleep 1

move -4  0 0.3 # 左
move 4  0 0.6 # 右
move -4  0 0.6 # 左
move 4  0 0.3 # 右
movehome

sleep 3

play $index

move -8  0 0.8 # 左
move 8  0 1.2 # 右
move -8  0 1.2 # 左
move 8  0 0.8 # 右
movehome

exit
