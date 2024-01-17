@rem @echo off

set command=curl.exe
set idpass=admin:abcd1234
@rem set host=192.168.10.101
set host=192.168.0.28


@rem 簡易キャリブレーション 左下⇒右上
call :movehome
call :moveshot -32 -8 01.jpg 6
call :moveshot  32 20 01.jpg 11 



@rem 撮影開始
@rem 真上
call :moveshot  1 20 01.jpg 7
@rem 右上⇒前上⇒左上⇒後上
call :moveshot  17 10 03.jpg 5
call :moveshot   1 10 02.jpg 5
call :moveshot -15 10 05.jpg 5
call :moveshot -32 10 04.jpg 5
@rem 後⇒左⇒前⇒右
call :moveshot -32  0 08.jpg 4
call :moveshot -15  0 09.jpg 5
call :moveshot   1  0 06.jpg 5
call :moveshot  17  0 07.jpg 5
@rem 右下⇒前下⇒左下⇒後下
call :moveshot  17 -8 11.jpg 4
call :moveshot   1 -8 10.jpg 5
call :moveshot -15 -8 13.jpg 5
call :moveshot -32 -8 12.jpg 5

call :movehome

@exit

:moveshot 
  @rem argument:pos_x,pos_y,filename,sleep
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=update&PT_preset4_x=%1&PT_preset4_y=%2"
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/admin/param.cgi?action=list&group=PT_preset4"   
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/com/ptzconfig.cgi?movepresetpoint=4"
  timeout %4
  %command% --anyauth "http://%idpass%@%host%/snapshot.jpg" -o %3
  exit /b

:movehome 
  %command% --anyauth "http://%idpass%@%host%/camera-cgi/com/ptz.cgi?move=home"
  timeout 5
  exit /b
