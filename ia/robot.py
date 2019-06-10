#            0x090 0x27    /*P8.7, button 6*/
#            0x094 0x27    /*P8.8, button 7 /
#            0x044 0x27    /*P8.32 button 5*/
#            0x0dc 0x27    /*P9.23 button 3*/

import Adafruit_BBIO.GPIO as GPIO
import os
import time
import shlex
from lcddriver import lcd
from threading import Thread, Lock
from math import *
import opponent
import select
import fcntl
import timer
import servo


from os.path import exists

my_lcd = lcd()

trig_button = "P8_7"
col_button = "P8_8"
but1_button = "P8_32"
but2_button = "P9_23"
color = "none"
angle_base=0.001
robot_base=175

read = Lock()
write = Lock()
new_write = Lock()

#inverse car RX/TX blablabla
asserv_in_file = "/tmp/robot_com_out"
asserv_out_file = "/tmp/robot_com_in"

#file_lock = RLock()


out_file=open("/dev/null","r")
in_file=open("/dev/null","r")
out_file.close()
in_file.close()

response=""

#make a File lock to not send two commands at the same time
class FileLock:
    """Implements a file-based lock using flock(2).
    The lock file is saved in directory dir with name lock_name.
    dir is the current directory by default.
    """

    def __init__(self, lock_name, dir="."):
        self.lock_file = open(os.path.join(dir, lock_name), "w")

    def acquire(self, blocking=True):
        """Acquire the lock.
        If the lock is not already acquired, return None.  If the lock is
        acquired and blocking is True, block until the lock is released.  If
        the lock is acquired and blocking is False, raise an IOError.
        """
        ops = fcntl.LOCK_EX
        if not blocking:
            ops |= fcntl.LOCK_NB
        fcntl.flock(self.lock_file, ops)

    def release(self):
        """Release the lock. Return None even if lock not currently acquired"""
        fcntl.flock(self.lock_file, fcntl.LOCK_UN)

com_lock = FileLock("com_lock", dir="/var/lock")   

#com
#send a string command to asserv, and return the answer
def com(arg):
    global out_file
    global in_file
    global read
    global write


    com_lock.acquire(True)
    poller = select.epoll()
    in_file=os.open(asserv_in_file,os.O_RDONLY | os.O_NONBLOCK)
    poller.register(in_file, select.EPOLLET)

    buff=""

    out_file= open(asserv_out_file, "w")
    out_file.write("a "+arg+"\n")
    out_file.close()    
    print ("sent : "+"a "+arg+"\n")
    p = poller.poll()
    response=os.read(in_file,100)
    os.close(in_file)
    buff=response
    print("response:"+response)
    com_lock.release()
   
    return buff

#turn
#turn to the selected angle
def turn(angle, relative):
    #relative
    if relative==False:
        com("-G -r -x0 -y0 -m1 -t"+str(angle))
    #absolute
    else:
        reponse=com("-P")
        reponse=com("-P")
        print(reponse)
        buff=shlex.split(reponse)
        angle=angle-float(buff[2])
        com("-G -r -x0 -y0 -m1 -t"+str(angle))
    wait_arrived_no_opponent()

# go_direct
# first aim for the target spot. The go there and at last turn to the right angle
# direct : -1 to go backward, 1 to go forward, and 0 to go the fastest
def go_direct(x2,y2,t2,direct,relative):
    text=""
    while(len(text.split())!=6):
        try:
            file  = open("/tmp/pos", "r")
            text = file.readline()
            file.close()
        except:
            success=False
    (x1, y1, t1, move, dir, mode) = [t(s) for t,s in zip((float,float,float, int, float, int),text.split())]
    if(relative==False):
        x=x2-x1
        y=y2-y1
    else:
        x=x2
        y=y2
        
    print("NEW MOVE")
    print(x,y,x2,x1,y2,y1)
    if x==0:
        if y<0 :
            angle=pi/2
        else:
            angle=-pi/2
    else:
        angle=atan2(y,x)
    print ("ANGLE")
    print(angle)
    if(relative==True):
       angle=angle+ t1 - pi/2

    while t1>pi:
        t1=t1-pi
    while t1<-pi:
        t1=t1+pi

    if(y<0):
        anlgle=angle+pi
    if(direct<0):
        angle=angle+pi
    while(angle>pi):
        angle=angle-2*pi
    while(angle<-pi):
        angle=angle+2*pi
    if(direct==0):
        if (angle-t1)<-pi/2:
            angle=angle+pi
        elif (angle-t1)>pi/2:
            angle=angle-pi
    print(angle)
    time.sleep(1)
    turn(angle,1)
    while (com("-E")!="yes"):
        time.sleep(0.1) 

    if(relative==True):
        com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -r -t"+str(t2))
    else:
        com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -t"+str(t2))
    wait_arrived()
    #wait_arrived_no_opponent()
# go_arc
# directlty go to the target through a curved path
# direct : -1 to go backward, 1 to go forward, and 0 to go the fastest

def go_arc(x2,y2,t2,direct,relative):
    if(relative==True):
        com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -r -t"+str(t2))
    else:
        com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -t"+str(t2))
    wait_arrived()
    
#wait_arrived_no_opponent
#just wait the to arrive with no specific check
def wait_arrived_no_opponent():
    while (com("-E")!="yes"):
        time.sleep(0.1) 

#wait_arrived
#wait to arrive but also check if some robot is seen on the way, and then stop the robot
def wait_arrived():
    time_now=time.time();
    print("Debut de l'attente")
    while (com("-E")!="yes"):        
        time.sleep(0.1)
        if(time.time()-time_now>5):
            servo.servo(0,0)
    print("fin de l'attente")
            
#update_from_gyro_angle
#update asserv angle coordinate from its gyro angle
def update_from_gyro_angle():
    value = com("-H -m1")
    com("-Y -t"+value) 

#set_gyro_angle
#set the value of angle to the gyro
def set_gyro_angle(value):
    value = com("-H -m0 -t"+str(value))

#get_gyro_angle
#get angle from gyro
def get_gyro_angle():
    value = com("-H -m1")
    
def set_lidar():
    value = com("-l -m1")

def unset_lidar():
    value = com("-l -m0")
    

