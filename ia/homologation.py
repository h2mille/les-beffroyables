import timer
import boot
import robot
import os
from os.path import exists
import time
import timer
import Adafruit_BBIO.GPIO as GPIO

def servo(id,pos):
    robot.com("-S -t"+str(pos)+" -m"+str(id))

def open_servo():
    servo(1,-1)
    servo(2,1)

def close_servo():
    servo(1,1.3)
    servo(2,-1.3)

if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    robot.my_lcd.write("not root")
    quit()

#timer.start_robot_daemon()
robot.my_lcd.write("go 2019!")

timer.start_robot_daemon()
robot.my_lcd.write("daemon start")
open_servo()
time.sleep(0.5)
close_servo()
servo(0,0)
GPIO.setup(robot.trig_button,GPIO.IN)

robot.com("-X -x500 -y500 -m0")

robot.com("-D -x0.0001 -y0.0004 -t-0.000 -m0")
robot.com("-D -x0.5 -y-0.2 -t-0.000 -m1")
GPIO.wait_for_edge(robot.trig_button, GPIO.RISING)

thread_1 = timer.timer()
thread_1.start()
robot.set_lidar()
robot.com("-Y -x1000 -y250 -t1.57")
robot.go_direct(1000,1250,1.57,1,False)
open_servo()
robot.go_direct(1000,350,1.57,-1,False)
#    time.sleep(0.5)
#    robot.go_direct(250,2200,1.50,1,False)
#    servo(0,1.5)
#    time.sleep(0.5)
#    robot.go_direct(250,2300,1.50,1,False)
#    servo(0,0)
#    time.sleep(0.5)
#    robot.go_direct(1500,1300,1.50,-1,False)
#    open_servo()
#    robot.go_arc(600,375,1.50,1,False)
#    robot.go_direct(0,-300,1.50,-1,True)
    
    
    
