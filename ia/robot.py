#            0x090 0x27    /*P8.7, button 6*/
#            0x094 0x27    /*P8.8, button 7 /
#            0x044 0x27    /*P8.32 button 5*/
#            0x0dc 0x27    /*P9.23 button 3*/

import Adafruit_BBIO.GPIO as GPIO
import os
import time
import shlex
from lcddriver import lcd
from threading import Thread, RLock
from math import *
import opponent

from os.path import exists

my_lcd = lcd()

trig_button = "P8_7"
col_button = "P8_8"
but1_button = "P8_32"
but2_button = "P9_23"
color = "none"
angle_base=0.001
robot_base=50

read = False
#inverse car RX/TX blablabla
asserv_in_file = "/tmp/robot_com_out"
asserv_out_file = "/tmp/robot_com_in"

#file_lock = RLock()


out_file=open("/dev/null","r")
in_file=open("/dev/null","r")
out_file.close()
in_file.close()

response=""


def init_pipe():
    fifo_tread=FIFO_OUT()
    fifo_tread.start()
    

def robot_com(arg, number=1):
    global out_file
    global in_file
    global read
    success = True
    buff=""
    global response
    # while(len(response.split())<=number):
    success = True
    buff=""
    try:
        out_file= open(asserv_out_file, "w")
    except:
        success = False
        print("broken pipe ^^")
        return False

    out_file.write("a "+arg+"\n")
    print ("sent : ","a "+arg+"\n")
    out_file.close()
    while(response==''):
        print("response from thread ",response)
        time.sleep(0.01)
    print(response)
    buff=response
    read=True
    print(response)

    time.sleep(0.1)    
    return buff

def turn(angle, ref):
    #relatif
    if ref==0:
        robot_com("-G -r -x0 -y0 -m1 -t"+str(angle),1)
    #absolu
    else:
        reponse=robot_com("-P",3)
        buff=shlex.split(reponse)        
        angle=angle-float(buff[2])
        robot_com("-G -r -x0 -y0 -m1 -t"+str(angle),1)
    wait_arrived_no_opponent()

def go_direct(x2,y2,t2,direct):
    text=""
    while(len(text.split())!=6):
        try:
            file  = open("/tmp/pos", "r")
            text = file.readline()
            file.close()
        except:
            success=False
    (x1, y1, t1, move, dir, mode) = [t(s) for t,s in zip((float,float,float, int, float, int),text.split())]
    x=x2-x1
    y=y2-y1
    angle=atan2(y,x)
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
    turn(angle,1)
    while (robot_com("-E",1)!="yes"):
        time.sleep(0.1) 
    robot_com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -t"+str(t2),1)
    wait_arrived()

def go_arc(x2,y2,t2,direct):
    text=""
    robot_com("-G -x"+str(x2)+" -y"+str(y2)+" -m0 -t"+str(t2),1)
    wait_arrived()
    
def wait_arrived_no_opponent():
    while (robot_com("-E",1)!="yes"):
        time.sleep(0.1) 

def wait_arrived():
    time_now=0;
    danger_happened=False
    print("Debut de l'attente")
    while (robot_com("-E",1)!="yes"):
        time.sleep(0.1)
        danger=opponent.check_danger()
        print("danger:",danger)
        if(danger== True):
             print("danger")             
             danger_happened=True
             while(True):
                 robot_com("-Z -m1",1)
                 time.sleep(0.1)
        else:
             if(danger_happened==True):
                 danger_happened=False
                 robot_com("-Z -m0",1)
    print("fin de l'attente")
            
def update_from_gyro_angle():
    value = robot_com("-H -m1",1)
    robot_com("-Y -t"+value,1) 

def set_gyro_angle(value):
    value = robot_com("-H -m0 -t"+str(value),1)
def get_gyro_angle():
    value = robot_com("-H -m1",1)
    
    
class FIFO_OUT(Thread):
    def __init__(self):
        Thread.__init__(self)
        
    def run(self):
        global out_file
        global in_file
        global read
        global response
        while(True):
            in_file=open(asserv_in_file,"r")
            response=in_file.read()
            print("response from thread ",response)
            while(read==False):
                print("response from thread ",response)
                time.sleep(0.1)
            response=''
            read=False

