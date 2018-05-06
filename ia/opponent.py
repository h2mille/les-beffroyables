import time
import daemon
from threading import Thread
import serial
import robot
import os
import fcntl

start_time=-1
in_danger=False
last_danger=0
def check_opponent():
    success = False
    text=""
#    with robot.file_lock:
    while(len(text.split())!=6):
        try:
            file  = open("/tmp/pos", "r")
            text = file.readline()
            file.close()
            success = True
        except:
            success=False

    (x, y, t, move, dir, mode) = [t(s) for t,s in zip((float,float,float, int, float, int),text.split())]
    with serial.Serial('/dev/ttyS2', 115200 , timeout=1) as ser:
        ser.write(str(x)+" "+str(y)+" "+str(t))
        line = ser.readline()
    return line


with open("/tmp/pos", "r") as f:
    fd = f.fileno()
    flag = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, flag | os.O_NONBLOCK)
    flag = fcntl.fcntl(fd, fcntl.F_GETFL)
    if flag & os.O_NONBLOCK:
        print "O_NONBLOCK!!"


def check_danger():
    success = False
    text=""
#    with robot.file_lock:
    
    while(len(text.split())!=6):
        try:
            file  = open("/tmp/pos", "r")
            text = file.readline()
            file.close()
            success = True
        except:
            success=False

    (x, y, t, move, dir, mode) = [t(s) for t,s in zip((float,float,float, int, float, int),text.split())]

    if( (mode==0) or (mode==1)):
        return False
    line=""
    time_now=time.time();
    timeout=0.1
    while((len(line.split())!=4) and (time.time()-time_now<timeout)):
        with serial.Serial('/dev/ttyS2', 115200 , timeout=0.1) as ser:
            ser.write(str(x)+" "+str(y)+" "+str(t))
            line = ser.readline()
        print(line)
    if(time.time()-time_now>=timeout):
        return False
    (back_left,back_right,front, sum) = [t(s) for t,s in zip((int, int, int, int),line.split())]

    if ((dir>0) and (front==1)):
        return True
    if ((dir<0) and ((back_left==1) or (back_right==1))):
        return True
    return False

def is_arrived():
    success = False
#    with robot.file_lock:
    while(len(text.split())!=6):
        try:
            file  = open("/tmp/pos", "r")
            text = file.readline()
            file.close()
            success = True
        except:
            success=False
        
    (x, y, t, move, dir, mode) = [t(s) for t,s in zip((float,float,float, int, float, int),text.split())]
    if(move==1):
        return True
    else:
        return False

    # while True:
	# test=check_oppoent()
	# i=0
        # i=i+1
	# print i
	# print test
#	time.sleep(0.05)
def start_chrono():
    start_time=time.time()

class TimeAndOpponent (Thread):
    # def __init__(self):
        # daemon.start_robot()
    def __init__(self):
        Thread.__init__(self)
#        daemon.start_robot()
        
    def run(self):
        global danger
        global in_danger
        global last_danger
        while(start_time==-1 or time.time()-start_time>95):
            danger=check_danger()
            if(danger==True):
                in_danger=True
                last_danger=time.time()
            elif(time.time()-last_danger<1):
                in_danger=True
            else:
                in_danger=False
            time.sleep(0.01)
 #       daemon.stop_robot()
