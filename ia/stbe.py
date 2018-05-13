
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


robot.my_lcd.write("init timer")


robot.init_pipe()
robot.robot_com("-S -t0 -m3",1)
