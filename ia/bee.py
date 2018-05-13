
import os
import time
import boot
import robot
import timer
from math import *
import stop0

# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)

def bee():
    robot.my_lcd.write("Bee ...")


    robot.init_pipe()

    robot.robot_com("-S -t1.2 -m1",1)
    #robot.robot_com("-S -t-1.2-m2",1)

    time.sleep(2)
    #robot.robot_com("-S -t1 -m1",1)
    #robot.robot_com("-S -t-1 -m2",1)

    time.sleep(5)

    robot.robot_com("-S -t0 -m1",1)
    #robot.robot_com("-S -t0 -m2",1)



    #robot_timer=timer.timer()
    print("GO GO GO")
    #robot.robot_com("-S -r -m0 -x0.3 -t0.5")

    robot.robot_com("-S -t0 -m3",1)


    robot.my_lcd.write("+50 bien !")

