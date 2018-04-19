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
    robot.robot_com("-m0 -B")
    robot.robot_com("-B -x-0.3 -m1")
    robot.robot_com("-B -x-0.3 -m2")
    time.sleep(1);
    reponse=robot.robot_com("-V")
    speed=shlex.split(reponse)
    while (abs(float(speed[0]))>0.01 or abs(float(speed[1]))>0.01) :
        time.sleep(0.1);
        reponse=robot.robot_com("-V")
        speed=shlex.split(reponse)
        print(speed[0])
        print(speed[1])
        
    if cote=="gauche":
        reponse=robot.robot_com("-Y -x-"+str(-1500+robot.robot_base)+" -t"+str(0+robot.angle_base)) 
    elif cote=="droite":
        reponse=robot.robot_com("-Y -x"+str(1500-robot.robot_base)+" -t"+str(3.14159265359+robot.angle_base) )  
    elif cote=="devant":
        reponse=robot.robot_com("-Y -y"+str(2000-robot.robot_base)+" -t"+str(4.71238898038+robot.angle_base)) 
    elif cote=="derriere":
        reponse=robot.robot_com("-Y -y"+str(0+robot.robot_base)+" -t"+str(1.57079632679+robot.angle_base))
    robot.robot_com("-B -m-1")
    #on avance a nouveau
    reponse=robot.robot_com("-G -r -x0 -y200 -t0 -m1")
    print("on attent l'arrivee du robot...")
    while (robot.robot_com("-E")!="yes"):
        time.sleep(0.1) 

def init_robot():
    GPIO.setup(robot.red_button,GPIO.IN)
    GPIO.setup(robot.col_button,GPIO.IN)
    GPIO.setup(robot.but1_button,GPIO.IN)
    GPIO.setup(robot.but2_button,GPIO.IN)
    print("BOOT ok.")
    if exists(robot.asserv_out_file)==False:
        print("start first robot deamon")
        quit()
    if exists(robot.asserv_in_file)==False:
        print("start first robot deamon")
        quit()

    print("les choses serieuses")
    reponse=robot.robot_com("-E")
    print(reponse)

    print("changer 3 fois la couleur pour selectionner la couleur")
    GPIO.wait_for_edge(col_button, GPIO.RISING)
    print("changer 2 fois la couleur pour selectionner la couleur")
    GPIO.wait_for_edge(col_button, GPIO.RISING)
    print("changer 1 fois la couleur pour selectionner la couleur")
    GPIO.wait_for_edge(col_button, GPIO.RISING)
    if GPIO.input(robot.col_button)==1:
        color="green"
    else:
        color="orange"
    print("appuyez sur le bouton1 pour continuer")
    GPIO.wait_for_edge(but1_button, GPIO.RISING)

    print("F*** The Wall!")
    fthewall("derriere")
    if(color=="green"):
        robot.turn(1.57079632679, 0)
        while (robot.robot_com("-E")!="yes"):
            time.sleep(0.1) 

        fthewall("droite")
    else:
        robot.turn(-1.57079632679, 0)
        while (robot.robot_com("-E")!="yes"):
            time.sleep(0.1) 
        fthewall("gauche")

    print("palcement en position initiale")
    if(color=="green"):
        reponse=robot.robot_com("-G -x1300 -y200 -t3.14159265359 -m1")
    else:
        reponse=robot.robot_com("-G -x-1300 -y200 -t0 -m1")
    print(reponse)
