#			0x090 0x27	/*P8.7, button 6*/
#			0x094 0x27	/*P8.8, button 7 /
#			0x044 0x27	/*P8.32 button 5*/
#			0x0dc 0x27	/*P9.23 button 3*/

import Adafruit_BBIO.GPIO as GPIO
import os
import time
import shlex
import robot

from os.path import exists


    
#recule 
def fthewall(cote):
    print("stop asserv")
    robot.robot_com("-B -m0",1)
    print("motor 1 back")
    robot.robot_com("-B -x-0.8 -m1",1)
    print("motor 2 back")
    robot.robot_com("-B -x-0.8 -m2",1)
    time.sleep(2);
    reponse=""
    reponse=robot.robot_com("-V",2)
    speed=shlex.split(reponse)
    while (abs(float(speed[0]))>0.01 or abs(float(speed[1]))>0.01) :
        time.sleep(0.1);
        reponse=robot.robot_com("-V",2)
        speed=shlex.split(reponse)
        print(speed[0])
        print(speed[1])
        
    if cote=="gauche":
        print("bump cote gauche....")
        print("-Y -x-"+str(-1500+robot.robot_base)+" -t"+str(0+robot.angle_base))
        reponse=robot.robot_com("-Y -x"+str(-1500+robot.robot_base)+" -t"+str(0+robot.angle_base),1) 
    elif cote=="droite":
        reponse=robot.robot_com("-Y -x"+str(1500-robot.robot_base)+" -t"+str(3.14159265359+robot.angle_base),1 )  
    elif cote=="devant":
        reponse=robot.robot_com("-Y -y"+str(2000-robot.robot_base)+" -t"+str(4.71238898038+robot.angle_base),1) 
    elif cote=="derriere":
        reponse=robot.robot_com("-Y -y"+str(0+robot.robot_base)+" -t"+str(1.57079632679+robot.angle_base),1)
    else:
        print("bump rien....")

    robot.robot_com("-B -m-1",1)
    #on avance a nouveau
    reponse=robot.robot_com("-G -r -x0 -y500 -t0 -m0",1)
    print("on attent l'arrivee du robot...")
    while (robot.robot_com("-E",1)!="yes"):
        time.sleep(0.1) 

def init_robot():

    GPIO.setup(robot.trig_button,GPIO.IN)
    GPIO.setup(robot.col_button,GPIO.IN)
    GPIO.setup(robot.but1_button,GPIO.IN)
    GPIO.setup(robot.but2_button,GPIO.IN)
    print("BOOT ok.")
    if exists(robot.asserv_out_file)==False:
        print("start first robot deamon")
        robot.my_lcd.write("no deamon...")
        quit()
    if exists(robot.asserv_in_file)==False:
        print("start first robot deamon")
        robot.my_lcd.write("no deamon...")
        quit()

    print("ca va passer?")
    #config angulaire
    robot.robot_com("-X -x5 -y12 -m1",1)

    #config lineaire
    robot.robot_com("-X -x700 -y800 -m0",1)

    #config pid
    robot.robot_com("-D -x0.001 -y0.0001 -t-0.000 -m0",1)

    print("stop asserv")
    robot.robot_com("-B -m0",1)
    print("motor 1 back")
    robot.robot_com("-B -x-0 -m1",1)
    print("motor 2 back")
    robot.robot_com("-B -x-0 -m2",1)


    print("les choses serieuses")
    robot.my_lcd.write("deamon connected...")
    # reponse=robot.robot_com("-E",1)
    # print(reponse)

    print("changer 3 fois la couleur pour selectionner la couleur")
    robot.my_lcd.write("chg 3 times col")
    GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
    print("changer 2 fois la couleur pour selectionner la couleur")
    robot.my_lcd.write("chg 2 times col")
    GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
    print("changer 1 fois la couleur pour selectionner la couleur")
    robot.my_lcd.write("chg 1 times col")
    GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
    
    robot.my_lcd.write("color : ")
    robot.my_lcd.write("press blue then")
    while GPIO.input(robot.but1_button)==1:
        if GPIO.input(robot.col_button)==1:
            robot.color="green"
            robot.my_lcd.lcd_display_string("green ",1,8)
        else:
            robot.color="orange"
            robot.my_lcd.lcd_display_string("orange",1,8)
        time.sleep(0.1)
    if GPIO.input(robot.trig_button)==0:
        robot.my_lcd.write("remove trigger")
        while GPIO.input(robot.trig_button)==0:
            1
    robot.my_lcd.write("insert trigger")
    while GPIO.input(robot.trig_button)==1:
        1
    robot.my_lcd.write("press blue again")
    while GPIO.input(robot.but1_button)==1:
        1
    robot.my_lcd.write("red button and blue again")
    while GPIO.input(robot.but1_button)==1:
        1

    print("F*** The Wall!")
    robot.my_lcd.write("f the wall : ")
    time.sleep(2)
    robot.my_lcd.write("go! ")
    fthewall("derriere")
    print ("couleur robot", robot.color)
    if(robot.color=="green"):
        robot.turn(1.57079632679, 0)
        while (robot.robot_com("-E",1)!="yes"):
            time.sleep(0.1) 

        fthewall("droite")
    else:
        robot.turn(-1.57079632679, 0)
        while (robot.robot_com("-E",1)!="yes"):
            time.sleep(0.1)
        print("F*** the wall gauche")
        fthewall("gauche")

    print("placement en position initiale")
    if(robot.color=="green"):
        reponse=robot.robot_com("-G -x1300 -y200 -t3.14159265359 -m0",1)
    else:
        reponse=robot.robot_com("-G -x-1300 -y200 -t0 -m0",1)
    print(reponse)

    while (robot.robot_com("-E",1)!="yes",1):
        time.sleep(0.1) 

    print("wait for trigger")        
    while GPIO.input(robot.trig_button)==1:
        1
