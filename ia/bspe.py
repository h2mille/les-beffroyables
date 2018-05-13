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

def go_domo():
    if(robot.color=="green"):
        robot.go_direct(470,1120,pi,1)
        robot.turn(pi/6,0)
    else:
        robot.go_direct(-470,1120,0,1)
        robot.turn(-pi/6,0)
    wait_arrived_no_opponent()
    robot.turn(pi/2,1)
    if(robot.color=="green"):
        robot.turn(0,1)
    else:
        robot.turn(pi/2,1)

if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    robot.my_lcd.write("not root")
    quit()

robot.my_lcd.write("let's begin")


GPIO.setup(robot.trig_button,GPIO.IN)
GPIO.setup(robot.col_button,GPIO.IN)
GPIO.setup(robot.but1_button,GPIO.IN)
GPIO.setup(robot.but2_button,GPIO.IN)

timer.start_robot_daemon()
robot.my_lcd.write("init timer")
robot_timer=timer.timer()
robot.my_lcd.write("init pipe")
robot.init_pipe()
robot.my_lcd.write("let's begin")
robot.my_lcd.write("now boot")
time.sleep(1)

boot.fast_conf()
robot.robot_com("-Y -x-1200 y200 -t0.785  ",1) 
GPIO.wait_for_edge(robot.trig_button, GPIO.RISING)
print("robot initialised, wait fot the match")
robot_timer.start()
robot.robot_com("-S -r -m0 -x0.3 -t0.5")

robot.robot_com("-G -r -m0 -x0.3 -t0.5")
while(True):
    robot.robot_com("-G -x0 -y1000 -m0 -t3.14 -r",1)
    robot.wait_arrived()

print("fin du match")
