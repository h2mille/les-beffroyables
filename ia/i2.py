#            0x090 0x27    /*P8.7, button 6*/
#            0x094 0x27    /*P8.8, button 7 /
#            0x044 0x27    /*P8.32 button 5*/
#            0x0dc 0x27    /*P9.23 button 3*/


import os
import time
import boot
import robot
import opponent
import timer
import shlex
from math import *
import Adafruit_BBIO.GPIO as GPIO
import boot


# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)

GPIO.setup(robot.trig_button,GPIO.IN)
GPIO.setup(robot.col_button,GPIO.IN)
GPIO.setup(robot.but1_button,GPIO.IN)
GPIO.setup(robot.but2_button,GPIO.IN)

robot.my_lcd.write("let's begin")
boot.fast_conf()

robot.robot_com("-Z -x-400 -y300 -m1",1)
time.sleep(0.3)
robot.robot_com("-Z -x400 -y-300 -m1",1)
time.sleep(0.3)
robot.robot_com("-Z -x0 -y0 -m1",1)
print("move done")

robot.my_lcd.write("init timer")
robot.my_lcd.write("init pipe")
robot.my_lcd.write("let's begin")
robot.my_lcd.write("now boot")


robot.robot_com("-S -t1.2 -m3",1)
robot.robot_com("-S -t-1.4 -m2",1)
robot.my_lcd.write("")
robot.my_lcd.write("")
while GPIO.input(robot.but1_button)==1:
    if GPIO.input(robot.col_button)==1:
        robot.color="orange"
        print("orange")
        robot.my_lcd.lcd_display_string("orange ",1,8)
    else:
        print("green ")
        robot.color="green"
        robot.my_lcd.lcd_display_string("green",1,8)
robot.my_lcd.write("mettre la tirette")
while GPIO.input(robot.trig_button)==1:
    time.sleep(0.1)
robot.my_lcd.write("bouton bleu")
while GPIO.input(robot.but1_button)==1:
    time.sleep(0.1)

robot.robot_com("-Y -x0 -y0 -t0")
robot.my_lcd.write("ready")
robot.robot_com("-S -t1.2 -m3",1)
robot.robot_com("-S -t-1.2 -m2",1)
###################################################################################################
while GPIO.input(robot.trig_button)==0:
    time.sleep(0.1)
robot_timer=timer.timer()
print("begin.....")
#while (time_start-time.time()<20) and (opponent.check_danger()==False):
if robot.color=="orange":
	robot.robot_com("-S -t1.2 -m2",1)
	time.sleep(1)
	robot.robot_com("-S -t0 -m3",1)
else:
	robot.robot_com("-S -t-1.2 -m3",1)
	time.sleep(1)
	robot.robot_com("-S -t0 -m2",1)
	
time.sleep(1)
#on avance doucement 120 cm
robot.robot_com("-Y -x0 -y0 -t0")
if(robot.color=="green"):
    robot.robot_com("-Z -x400 -y300 -m1",1)
else:
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
time_start=time.time()
while (time.time()-time_start<1):
    pos=shlex.split(robot.robot_com("-P",3))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])<-1200):
        break
    if robot.color=="orange" and float(pos[0])>1200 :
        break    
    time.sleep(0.01)

robot.robot_com("-Z -x0 -y0 -m1",1)
if(robot.color=="orange" ):
    robot.robot_com("-S -t0.9 -m3",1)
else:
    robot.robot_com("-S -t-0.9 -m2",1)

time.sleep(2)
if(robot.color=="green"):
    robot.robot_com("-Z -x400 -y300 -m1",1)
else:
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
time_start=time.time()
while (time.time()-time_start<3):
    pos=shlex.split(robot.robot_com("-P",3))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])<-1200):
        break
    if robot.color=="orange" and float(pos[0])>1200 :
        break    
    time.sleep(0.01)

robot.robot_com("-Z -x0 -y0 -m1",1)

if(robot.color=="orange" ):
    robot.robot_com("-S -t0.9 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.6 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t0.9 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.6 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t0.9 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.6 -m3",1)
    time.sleep(1)
else:
    robot.robot_com("-S -t-0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t0.7 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t-1.3 -m2",1)
    time.sleep(1)
    robot.robot_com("-S -t0.2 -m3",1)
    time.sleep(1)

#on recule de 40 cm
robot.robot_com("-Y -x0 -y0 -t0")
if(robot.color=="green"):
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
else:
    robot.robot_com("-Z -x400 -y300 -m1",1)
time_start=time.time()
while (time.time()-time_start<3.0):
    pos=shlex.split(robot.robot_com("-P",2))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])>600):
        break
    if robot.color=="orange" and float(pos[0])<-600 :
        break    
    time.sleep(0.01)

robot.robot_com("-Z -x0 -y0 -m1",1)
if(robot.color=="orange" ):
    robot.robot_com("-S -t-0.6 -m3",1)
time.sleep(5)

#on avance jusqu'au bout
robot.robot_com("-Y -x0 -y0 -t0")
if(robot.color=="green"):
    robot.robot_com("-Z -x400 -y300 -m1",1)
else:
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
time_start=time.time()
while (time.time()-time_start<5):
    pos=shlex.split(robot.robot_com("-P",3))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])<-1000):
        break
    if robot.color=="orange" and float(pos[0])>1000 :
        break    
    time.sleep(0.01)
    robot.robot_com("-Z -x0 -m1",1)
robot.robot_com("-B -x0 -y0 -m1",1)

if(robot.color=="orange" ):
    robot.robot_com("-S -t0.9 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t-0.6 -m3",1)
    time.sleep(1)
    robot.robot_com("-S -t-0 -m3",1)

time.sleep(5)

#on recule de 120 cm
robot.robot_com("-Y -x0 -y0 -t0")
if(robot.color=="green"):
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
else:
    robot.robot_com("-Z -x400 -y300 -m1",1)
time_start=time.time()
while (time.time()-time_start<4):
    pos=shlex.split(robot.robot_com("-P",2))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])>1200):
        break
    if robot.color=="orange" and float(pos[0])<-1200 :
        break    
    time.sleep(0.01)    
robot.robot_com("-Y -x0 -y0 -t0")
#on reavance pour relacher les balles
if(robot.color=="green"):
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
else:
    robot.robot_com("-Z -x400 -y300 -m1",1)
time_start=time.time()
while (time.time()-time_start<2):
    pos=shlex.split(robot.robot_com("-P",2))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])>1200):
        break
    if robot.color=="orange" and float(pos[0])<-1200 :
        break    
    time.sleep(0.01)    
robot.robot_com("-Z -x0 -y0 -m1",1)
time.sleep(1)
if(robot.color=="green"):
    robot.robot_com("-Z -x400 -y300 -m1",1)
else:
    robot.robot_com("-Z -x-400 -y-300 -m1",1)
time_start=time.time()
while (time.time()-time_start<2):
    pos=shlex.split(robot.robot_com("-P",2))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and float(pos[0])>1200):
        break
    if robot.color=="orange" and float(pos[0])<-1200 :
        break    
    time.sleep(0.01)    
robot.robot_com("-Z -x0 -y0 -m1",1)
    

print("fin du match")
