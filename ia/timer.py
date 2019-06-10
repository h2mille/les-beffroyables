import sys
from threading import Thread
import time
import os
import robot
import shlex

is_blocked=False
counter=0

#start_robot_daemon
#start the robot daemon
#disabled, must be done my hand
def start_robot_daemon():
    print("start robot daemon")
    while not os.path.exists(robot.asserv_out_file):
        time.sleep(1)
    print("quit")
    

#stop_robot_daemon
#stop the robot daemon
#disbale all powered device
def stop_robot_daemon():
    print("stop robot daemon")
    os.system("sudo kill -9 $(pgrep robot)")
    os.system("/usr/bin/rc_test_motors -d0&")
    time.sleep(1)
    os.system("sudo kill -9 $(pgrep rc_test_motor)")
    sys.exit(0) 

#check that the robot will stop after 95 seconds
class timer(Thread):
    def __init__(self):
        Thread.__init__(self)
    def run(self):
        print("95seconds left")
        time.sleep(90)
#        robot.com("-Z -m1")
        stop_robot_daemon()
        print("95seconds done")

class unbreak(Thread):
    def __init__(self):
        Thread.__init__(self)
    def run(self):
        global counter
        reponse=""
        reponse=robot.com("-V")
        print("speed response:", reponse)
        try:
            speed=shlex.split(reponse)
        except:
            speed="10 10"
        while (counter<10) :
            try:
                reponse=robot.com("-V")
                print("speed response:", reponse)
                speed=shlex.split(reponse)
                
                if(abs(float(speed[0]))<10 and abs(float(speed[1]))<10) :
                    print("blocked!")
                    counter=counter+1
                else :
                    print("not blocked!")
                    counter=0
            except:
                pass
            time.sleep(1)
        print("motor stop")

