#			0x090 0x27	/*P8.7, button 6*/
#			0x094 0x27	/*P8.8, button 7 /
#			0x044 0x27	/*P8.32 button 5*/
#			0x0dc 0x27	/*P9.23 button 3*/


import os
import time
import boot
import robot
import opponent
import timer

# from signal import signal, SIGPIPE, SIG_DFL
# signal(SIGPIPE, SIG_DFL)

def go_domo():
    if(robot.color=="green"):
        robot.go_direct(200,300,pi,1)
        robot.turn(pi/6,0)
    else:
        robot.go_direct(-200,300,0,1)
        robot.turn(-pi/6,0)
    wait_arrived_no_opponent()
    robot.turn(pi/2,1)
    if(robot.color=="green"):
        robot.turn(0,1)
    else:
        robot.turn(pi/2,1)

def go_bee():
    if(robot.color=="green"):
        robot.go_direct(1000,1800,pi,1)
        robot.robot_com("-S -m3 -t1")
        time.sleep(1)
        robot.robot_com("-S -m3 -t0")
    else:
        robot.robot_com("-S -m4 -t1")
        time.sleep(1)
        robot.robot_com("-S -m4 -t0")

def go_ball(distrib):
    robot.robot_com("-S -m2 -t1")
    if(robot.color=="green"):
        if(distrib==1):
            go_arc(1200,1000,pi)
            go_arc_direct(1250,1000,pi,-1)
            robot.robot_com("-S -m2 -t0")
            time.sleep(1)
            robot.robot_com("-B -x0.72 -m3")
            time.sleep(10)
            robot.robot_com("-B -x0 -m3")
            go_arc(1200,1000,pi)
        if(distrib==2):
            go_arc(-800,1800,3*pi/2)
            go_arc_direct(-800,1850,3*pi/2,-1)
            robot.robot_com("-S -m2 -t0")
            time.sleep(1)
            robot.robot_com("-B -x0.72 -m3")
            time.sleep(10)
            robot.robot_com("-B -x0 -m3")
            go_arc(-800,1800,3*pi/2)
    if(robot.color=="orange"):
        if(distrib==1):
            go_arc(-1200,1000,0)
            go_arc_direct(-1250,1000,0,-1)
            robot.robot_com("-S -m2 -t0")
            time.sleep(1)
            robot.robot_com("-B -x0.72 -m3")
            time.sleep(10)
            robot.robot_com("-B -x0 -m3")
            go_arc(-1200,1000,0)
        if(distrib==2):
            go_arc(800,1800,3*pi/2)
            go_arc_direct(800,1850,3*pi/2,-1)
            robot.robot_com("-S -m2 -t0")
            time.sleep(1)
            robot.robot_com("-B -x0.72 -m3")
            time.sleep(10)
            robot.robot_com("-B -x0 -m3")
            go_arc(800,1800,3*pi/2)



if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    robot.my_lcd.write("not root")
    quit()

robot.my_lcd.write("let's begin")


timer.start_robot_daemon()
robot_timer=timer.timer()

robot.my_lcd.write("let's begin")
robot.my_lcd.write("now boot")
time.sleep(1)
boot.init_robot()
print("robot initialised, wait fot the match")
robot_timer.start()

while(True):
    time.sleep(0.1)
    go_domo()
    go_ball(1)
    go_bee()
    go_ball(2)

print("fin du match")
