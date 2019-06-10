#            0x090 0x27    /*P8.7, button 6*/
#            0x094 0x27    /*P8.8, button 7 /
#            0x044 0x27    /*P8.32 button 5*/
#            0x0dc 0x27    /*P9.23 button 3*/



import time
import boot
import robot
import opponent






boot.fast_conf()

while(True):
    i=0
    for i in range(0,4):
        robot.go_direct(0,300,1.57,1,True)
    for i in range(0,2):
        robot.go_arc(300,300,3.14,1,True)

