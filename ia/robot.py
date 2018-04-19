#			0x090 0x27	/*P8.7, button 6*/
#			0x094 0x27	/*P8.8, button 7 /
#			0x044 0x27	/*P8.32 button 5*/
#			0x0dc 0x27	/*P9.23 button 3*/

import Adafruit_BBIO.GPIO as GPIO
import os
import time
import shlex

from os.path import exists

red_button = "P8_7"
col_button = "P8_8"
but1_button = "P8_32"
but2_button = "P9_23"
color = "none"
angle_base=0.001
robot_base=50

#inverse car RX/TX blablabla
asserv_in_file = "/tmp/robot_com_out"
asserv_out_file = "/tmp/robot_com_in"


def robot_com(arg):
    with open(asserv_out_file, "w") as out_file:
        out_file.write("a "+arg+"\n")
    print("envoi de :")
    print("a "+arg+"\0")

    with open(asserv_in_file, "r") as in_file:
        buff=in_file.read()
    print "reponse:"+buff
    return buff

def turn(angle, ref):
    if ref==0:
        robot_com("-G -r -x0 -y0 -m1 -t"+str(angle))
    else:
        reponse=robot_com("-G -P"+str(angle))
        buff=shlex.split(reponse)        
        robot_com("-G -r -x0 -y0 -m1 -t"+str(angle-reponse[2]))
    


	
	
