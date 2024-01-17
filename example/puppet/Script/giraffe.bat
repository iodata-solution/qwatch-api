@rem @echo off
set command=curl.exe
set idpass=admin:io1234
@rem set host=192.168.10.101
set host=192.168.0.28
set index=3

@rem call :sleep_chk
@rem call :list

@rem  前下,前,Home
call :move  0 -8 0
call :move  0  0 0 
call :movehome

@rem timeout 1
call :sleep_ms 500

@rem  左,右,左,右,Home
call :move -4  0 0
call :move  4  0 100
call :move -4  0 100
call :move  4  0 0
call :movehome

@rem timeout 3
call :sleep_ms 2500

call :play %index%

@rem  左,右,左,右,Home
call :move -8  0 300
call :move  8  0 700
call :move -8  0 700
call :move  8  0 300
call :movehome

@exit

:move 
  @rem argument:pos_x,pos_y,sleep
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=update&PT_preset4_x=%1&PT_preset4_y=%2"
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=list&group=PT_preset4"   
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/com/ptzconfig.cgi?movepresetpoint=4"
  @rem timeout %3
  call :sleep_ms %3
  exit /b

:movehome 
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/com/ptz.cgi?move=home"
  @rem timeout 5
  call :sleep_ms 4500
  exit /b

:list
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=StorageMusicInfo"
  exit /b

:play
  @rem playlist_number
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=MusicCtrl&MusicPlay=%1"
  exit /b

:sleep_ms
  @rem 注意：コマンド処理に時間がかかる
  if 0 equ %1 ( exit /b )
  powershell -Command "sleep -m %1"
  exit /b

:sleep_chk
  set "stp=%time: =0%"
  powershell -Command "sleep -m 0"
  set "etp=%time: =0%"
  set /a stp=(1%stp:~0,2%-100)*3600000 + (1%stp:~3,2%-100)*60000 + (1%stp:~6,2%-100)*1000 + (1%stp:~9,2%-100)*10
  set /a etp=(1%etp:~0,2%-100)*3600000 + (1%etp:~3,2%-100)*60000 + (1%etp:~6,2%-100)*1000 + (1%etp:~9,2%-100)*10
  set /a duration=%etp%-%stp% 
  echo duration=%duration%
  @rem テストPCでは余分に約500msかかる
  exit
