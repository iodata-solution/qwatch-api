# PPM（Pulse Position Modulation : パルス位置変調)
# PPM（Pulse Position Modulation : パルス位置変調)
# 一般的にサーボモータに与えるPWM波の
# 振幅 4.8v(～5V) 
# 周期 10～20ms     20ms(50hz)      0.02sec間隔
# 範囲 1～2ms程度のパルス幅 中央1.5ms前後で、0.5msの範囲で動く  0.0015ms
# 50hz duty0.5   0.02周期　パルス幅0.01
# 50hz duty0.2   0.02周期　パルス幅0.004
# 50hz duty0.15  0.02周期　パルス幅0.003
# 50hz duty0.05  0.02周期　パルス幅0.0010   目標パルス最小値    0.0010/0.02=0.05
# 50hz duty0.075 0.02周期　パルス幅0.0015   目標パルス中央値　　0.0015/0.02=0.075
# 50hz duty0.01  0.02周期　パルス幅0.0020   目標パルス最大値    0.0020/0.02=0.1



'''
'''
import numpy as np
from scipy import signal
import scipy.io.wavfile as wavfile
#from scipy.io.wavfile import write
import matplotlib.pyplot as plt
import math, wave, array
import os
import sys



'''
print('getcwd:      ', os.getcwd()) # getcwd:       C:\work
print('__file__:    ', __file__)    # __file__:     c:\work\other\sound\pyServoAudio\ServoAudio01 .py
print('basename:    ', os.path.basename(__file__))  # basename:     ServoAudio01 .py
print('dirname:     ', os.path.dirname(__file__))   # dirname:      c:\work\other\sound\pyServoAudio
'''

def test_mono():
  print("  ### test_mono()")
  rate = 44100
  time = 10 # 4    # end
  fname = 'mono_l2.wav'  #'mono.wav'
  t = np.linspace(0, time, time * rate)
  #data = np.int16(1.0 * np.sin(2 * np.pi * 440 * t) * 32767)   # dtype: int16 上下に32000
  data = np.int16(1.0 * np.sin(2 * np.pi * 220 * t) * 32767)   # dtype: int16 上下に32000
  fname = os.path.join(os.path.dirname(__file__), fname)  # スクリプトの
  print('output:',fname)
  wavfile.write(fname, rate, data)

#test_mono()
#sys.exit()    # 終了

def test_stereo():
  print("  ### test_stereo()")
  rate = 44100
  time = 4    # end
  fname = 'stereo.wav'
  t = np.linspace(0, time, time * rate)
  #L = np.sin(2 * np.pi * 440 * t)
  #R = np.sin(2 * np.pi * 880 * t)
  L = np.int16(1.0 * np.sin(2 * np.pi * 440 * t) * 32767)   # dtype: int16 上下に32000
  R = np.int16(1.0 * np.sin(2 * np.pi * 880 * t) * 32767)   # 
  data = np.array([L,R]).T    # 「.T」で二次元配列（行列）の転置行列を得る
  fname = os.path.join(os.path.dirname(__file__), fname)  # スクリプトの
  print('output:',fname)
  wavfile.write(fname, rate, data)  # 64bit,   before int 16:16bit ok
  #wavfile.write(fname, rate, data.astype(np.int16)) # 16bit無音,   before int 16:16bit OK

  #filename = os.path.join(os.path.dirname(__file__), 'stereo.wav')  # スクリプトの
  #wavfile.write(filename, rate, data)

#test_stereo()
#sys.exit()    # 終了


def test_(): 
  print("  ### test_()")
  rate = 48000
  time = 4    # end
  fname = 'test.wav'
  t = np.linspace(0, time, time * rate)
  L = np.int16(1.0 * np.sin(2 * np.pi * 440 * t) * 32767)   # dtype: int16 上下に32000
  R = np.int16(1.0 * np.sin(2 * np.pi * 880 * t) * 32767)   # 
  data = L
  #data = np.array([L,R]).T    # 「.T」で二次元配列（行列）の転置行列を得る
  fname = os.path.join(os.path.dirname(__file__), fname)  # スクリプトの
  print('output:',fname)
  wavfile.write(fname, rate, data)
 
  data1=L
  #data1=signal
  #data1=v1
  data2=data
  #data2=s01
  maxplot=4400

  if 'data2' in locals() and 'data1' in locals():
    plt.subplot(2, 1, 1) #縦に並べる　1,2,?だと横に
    plt.plot(t[:maxplot], data1[:maxplot]) # plt.plot(t, data1)
    plt.subplot(2, 1, 2) #縦に並べる　1,2,?だと横に
    plt.plot(t[:maxplot], data2[:maxplot]) # plt.plot(t, data2)
  elif 'data1' in locals():
    plt.plot(t[:maxplot], data1[:maxplot]) #plt.plot(t, data1)
  plt.xlabel('seconds')
  plt.ylabel('voltage')
  plt.show()


#test_()
#sys.exit()    # 終了


def test_2():
  #rate = 44100 # サンプルレートを設定します
  rate = 48000 # サンプルレートを設定します
  freq = 50 # 50hz(20ms) PWM信号の周波数を設定します
  #freq = 5 # 5hz テスト
  test = 2 # 2Hz PWM信号のデューティ比生成用 
  #time = 1 #1 # 1秒間のデータ
  time = 5 #1 # 5秒間のデータ
  # freqA4 = 440.000	# Sin波周波数 A3 ラ
  # freqA2 = 220.000	# Sin波周波数 A2 ラ

  t = np.linspace(0, time, rate*time)   # 開始,終了,要素数

  ##### サーボ波形 #####
  #fname = 'test_square.wav'
  fname = 'test_servo.wav'
  amp = np.iinfo(np.int16).max
  print('amp',amp)
  #si = (np.sin(2*np.pi*test*t)/50+0.075)   # 
  si2 = (np.sin(2*np.pi*test*t)/50+0.075)   # 
  si1 = (np.sin(2*np.pi*1*t)/50+0.075)   # 
  #v1 = np.int16( signal.square(2 * np.pi * freq * t, duty=si) * 32767  ) # dtype: int16
  v1 = np.int16( signal.square(2 * np.pi * freq * t, duty=si2) * amp  ) # dtype: int16
  #v2 = np.int16( signal.square(2 * np.pi * freq * t, duty=si) * amp  ) # dtype: int16
  v2 = np.int16( signal.square(2 * np.pi * freq * t, duty=si1) * amp  ) # dtype: int16
  data=v2  # モノラル化
  #data = np.array([v1,v2]).T    # ステレオ化「.T」で二次元配列（行列）の転置行列を得る
  fname = os.path.join(os.path.dirname(__file__), fname)  # スクリプトの
  print('output:',fname)
  wavfile.write(fname, rate, data)



  #data1=L
  #data2=R
  data1=v1
  data2=v2
  #data1=si
  #data1=si2
  #data2=si1
  maxplot=48000

  if 'data2' in locals() and 'data1' in locals():
    plt.subplot(2, 1, 1) #縦に並べる　1,2,?だと横に
    plt.plot(t[:maxplot], data1[:maxplot]) # plt.plot(t, data1)
    plt.subplot(2, 1, 2) #縦に並べる　1,2,?だと横に
    plt.plot(t[:maxplot], data2[:maxplot]) # plt.plot(t, data2)
  elif 'data1' in locals():
    plt.plot(t[:maxplot], data1[:maxplot]) #plt.plot(t, data1)
  plt.xlabel('seconds')
  plt.ylabel('voltage')
  plt.show()


test_2()
sys.exit()    # 終了

