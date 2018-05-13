import sys
from threading import Thread
import time
import os

def start_robot_daemon():
    print("start robot daemon")
 #   os.system("(cd /home/debian/robot;     /home/debian/robot/robot&)")
 #   os.system("(cd /home/debian/robot;sudo /home/debian/robot/robot&)")
#    os.system(" systemctl start robot_boot")
    time.sleep(0.3)
    print("quit")
    

def stop_robot_daemon():
    print("stop robot daemon")
    os.system(" systemctl stop robot_boot")
    sys.exit(0) 


class timer(Thread):
    def __init__(self):
        Thread.__init__(self)
    def run(self):
        print("95seconds left")
        time.sleep(95)
        print("95seconds done")
        stop_robot_daemon()
