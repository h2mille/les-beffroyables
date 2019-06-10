import timer
import boot
import robot
import os
from os.path import exists
import time
import timer
import servo


if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    robot.my_lcd.write("not root")
    quit()

#timer.start_robot_daemon()
robot.my_lcd.write("go 2019!")

timer.start_robot_daemon()
robot.my_lcd.write("daemon start")
servo.open_servo()
time.sleep(0.5)
servo.close_servo()
servo.servo(0,0)

robot.com("-X -x500 -y500 -m0")

robot.com("-D -x0.0001 -y0.0004 -t-0.000 -m0")
robot.com("-D -x0.5 -y-0.2 -t-0.000 -m1")

boot.init_robot()
thread_1 = timer.timer()
thread_1.start()
#thread_2 = timer.unbreak()
#thread_2.start()
#depart:450;250;pi
robot.set_lidar()
score = 0
if(robot.color=="jaune"):
    #sortie
    robot.go_arc(270,450,1.50,1,False)
    #direction accelerateur
    robot.go_direct(250,1600,1.50,1,False)
    servo.servo(0,1.3)
    time.sleep(0.5)
    robot.go_direct(250,1680,1.50,1,False)
    servo.servo(0,0)
    time.sleep(0.2)
    score+=20
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("20       ")    
    #direction goldenium
    robot.go_direct(265,2100,1.57,1,False)
    servo.servo(0,1.4)
    time.sleep(0.5)
    robot.go_direct(265,2220,1.57,1,False)
    servo.servo(0,0)
    time.sleep(0.2)
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("40       ")    
    #go milieu
    robot.go_direct(1200,1300,1,-1,False)
    #go home
    servo.open_servo()
    time.sleep(0.5)    
    robot.go_direct(600,500,1,-1,False) 
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("45       ")    
    
    
else:
    #sortie
    robot.go_arc(270,2550,4.60,1,False)
    #direction accelerateur
    robot.go_direct(260,1400,4.7,1,False)
    servo.servo(0,-1.3)
    time.sleep(0.2)
    robot.go_direct(260,1320,4.6,1,False)
    servo.servo(0,0)
    time.sleep(0.2)
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("20       ")    
    #direction goldenium
    robot.go_direct(295,900,4.71,1,False)
    servo.servo(0,-1.4)
    time.sleep(0.5)
    robot.go_direct(295,780,4.71,1,False)
    servo.servo(0,0)
    time.sleep(0.2)
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("40       ")    
    #go milieu
    robot.go_direct(1200,1700,5,-1,False)
    servo.open_servo()
    robot.go_direct(600,2500,5,-1,False)
    robot.my_lcd.write("SCORE:")
    robot.my_lcd.write("45       ")    
    
    
    
