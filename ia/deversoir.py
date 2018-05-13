
import os
import time
import boot
import robot
import timer
from math import *
import stop0
import Adafruit_BBIO.GPIO as GPIO

# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)


#Moteur continue
def deversoir():
    mot="P8_44"
    GPIO.setup(mot,GPIO.OUT)
    GPIO.output(mot,GPIO.HIGH)

    robot.my_lcd.write("init timer")


    robot.init_pipe()
    robot.robot_com("-S -t0 -m3",1)
    time.sleep(0.5)
    robot.robot_com("-S -t0.9 -m3",1)
    #time.sleep(0.5)


    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #2
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #3
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #4
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #5
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #6
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    #7
    if robot.color=="green":
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)
    else:
        robot.robot_com("-S -t0.3 -m3",1)
        time.sleep(0.5)

        robot.robot_com("-S -t0.7 -m3",1)
        time.sleep(0.5)


    time.sleep(0.5)


    GPIO.output(mot,GPIO.LOW)


    #robot_timer=timer.timer()
    #robot.my_lcd.write("init pipe")
    #robot.init_pipe()
    #robot.my_lcd.write("let's begin")
    #robot.my_lcd.write("now boot")
    print("GO GO GO")
    #robot.robot_com("-S -r -m0 -x0.3 -t0.5")

    robot.robot_com("-S -t0 -m3",1)


    print("fin du match")
