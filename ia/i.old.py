#			0x090 0x27	/*P8.7, button 6*/
#			0x094 0x27	/*P8.8, button 7 /
#			0x044 0x27	/*P8.32 button 5*/
#			0x0dc 0x27	/*P9.23 button 3*/


import os
import time
import boot
import robot
import opponent
import timer
import shlex
from math import *
import Adafruit_BBIO.GPIO as GPIO


# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)

GPIO.setup(robot.trig_button,GPIO.IN)
GPIO.setup(robot.col_button,GPIO.IN)
GPIO.setup(robot.but1_button,GPIO.IN)
GPIO.setup(robot.but2_button,GPIO.IN)

robot.my_lcd.write("let's begin")

robot.init_pipe()
robot.robot_com("-B -m0",1)

robot.robot_com("-B -x-0.8 -m1",1)
time.sleep(0.1)
robot.robot_com("-B -x0.8 -m1",1)
time.sleep(0.1)
robot.robot_com("-B -x0 -m1",1)


robot.my_lcd.write("init timer")
robot.my_lcd.write("init pipe")
robot.my_lcd.write("let's begin")
robot.my_lcd.write("now boot")


robot.my_lcd.write("")
robot.my_lcd.write("")
while GPIO.input(robot.but1_button)==1:
    if GPIO.input(robot.col_button)==1:
        robot.color="orange"
        print("orange")
        robot.my_lcd.lcd_display_string("orange ",1,8)
    else:
        print("green")
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
while GPIO.input(robot.trig_button)==0:
    time.sleep(0.1)
    
time.sleep(30)
robot_timer=timer.timer()
if(robot.color=="green"):
    robot.robot_com("-B -x-0.85 -m1",1)
else:
    robot.robot_com("-B -x0.85 -m1",1)

time_start=time.time()
#while (time_start-time.time()<20) and (opponent.check_danger()==False):
while (time.time()-time_start<4):
    pos=shlex.split(robot.robot_com("-P",3))
    print("pos ", float(pos[0]))
    if(robot.color=="green" and pos[0]<-1300):
        print("break from distance")
        break
#    if robot.color=="orange" and (float(pos[0])>1800 or opponent.check_danger())==True:
    if robot.color=="orange" and (float(pos[0])>1300 )==True:
        print("break from distance ", float(pos[0]), opponent.check_danger())
        break    
    time.sleep(0.01)

if(robot.color=="green"):
    robot.robot_com("-B -x0.85 -m1",1)
else:
    robot.robot_com("-B -x-0.85 -m1",1)
time.sleep(0.5)
robot.robot_com("-B -x0.85 -m1",1)
#print("danger:",opponent.check_danger())
print(time_start-time.time())
robot.robot_com("-B -x0 -m1",1)
    

print("fin du match")
