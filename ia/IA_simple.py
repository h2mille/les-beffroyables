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
from math import *
import Adafruit_BBIO.GPIO as GPIO


# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)


if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    robot.my_lcd.write("not root")
    quit()

GPIO.setup(robot.trig_button,GPIO.IN)
GPIO.setup(robot.col_button,GPIO.IN)
GPIO.setup(robot.but1_button,GPIO.IN)
GPIO.setup(robot.but2_button,GPIO.IN)

robot.my_lcd.write("let's begin")

mot_green="P9_13"
mot_orange="P9_12"
GPIO.setup(mot_green,GPIO.OUT)
GPIO.setup(mot_orange,GPIO.OUT)
GPIO.output(mot_orange,GPIO.LOW)
GPIO.output(mot_green,GPIO.HIGH)
time.sleep(0.2)
GPIO.output(mot_green,GPIO.LOW)
GPIO.output(mot_orange,GPIO.LOW)

robot.my_lcd.write("init timer")
robot_timer=timer.timer()
robot.my_lcd.write("init pipe")
robot.my_lcd.write("let's begin")
robot.my_lcd.write("now boot")


while GPIO.input(robot.but1_button)==1:
    if GPIO.input(robot.col_button)==1:
        robot.color="orange"
        print("orange")
        robot.my_lcd.lcd_display_string("orange ",1,8)
    else:
        print("green")
        robot.color="green"
        robot.my_lcd.lcd_display_string("green",1,8)
if GPIO.input(robot.trig_button)==1:
    robot.my_lcd.write("mettre la tirette")
while GPIO.input(robot.trig_button)==0:
    time.sleep(0.1)
    
if(robot.color=="green"):
    os.system("rc_test_motors -m1 -d0.8")
else:
    os.system("rc_test_motors -m1 -d-0.8")

time_start=time.time()
while (time_start-time.time()<20) and (opponent.check_danger()==False):
    time.sleep(0.1)
print("danger:",opponent.check_danger())
print(time_start-time.time())
    
os.system("rc_test_motors -m1 -d0")

print("fin du match")
