#            0x090 0x27    /*P8.7, button 6*/
#            0x094 0x27    /*P8.8, button 7 /
#            0x044 0x27    /*P8.32 button 5*/
#            0x0dc 0x27    /*P9.23 button 3*/



import time
import boot
import robot
import opponent






boot.fast_conf()

robot.com("-S -m0 -t0")
robot.com("-S -m1 -t1.5")
robot.com("-S -m3 -t0")
while(True):
    i=0
    while(opponent.check_danger()==False):
        if(i==2):
            robot.my_lcd.write("  .")
            i=0
        if(i==1):
            robot.my_lcd.write(" . ")
            i=2
        if(i==0):
            robot.my_lcd.write(".  ")
            i=1

    robot.my_lcd.write("detection!")
    robot.com("-S -m0 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t0")
    time.sleep(0.3)

    robot.com("-S -m3 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m3 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m3 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m3 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m3 -t0")
    time.sleep(0.3)

    robot.go_direct(0,100,0,0,True);
    robot.com("-S -m1 -t-1.5")
    time.sleep(5)
    
    robot.com("-S -m0 -t0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t-0.5")
    time.sleep(0.3)
    robot.com("-S -m0 -t0")
    time.sleep(1)    
    robot.com("-S -m1 -t1.5")
    robot.go_direct(0,-100,0,0,True);
    time.sleep(1)
