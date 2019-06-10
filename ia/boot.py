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
    robot.com("-B -m0")
    time.sleep(0.1)
    print("motor 1 back")
    robot.com("-B -x0.75 -m1")
    time.sleep(0.1)
    print("motor 2 back")
    robot.com("-B -x0.75 -m2")
    time.sleep(0.1)
    time.sleep(2);
    reponse=""
    reponse=robot.com("-V")
    print("speed response:", reponse)
    speed=shlex.split(reponse)
    
    while (abs(float(speed[0]))>0.01 or abs(float(speed[1]))>0.01) :
        time.sleep(0.1);
        reponse=robot.com("-V")
        speed=shlex.split(reponse)
        print("speed response:", reponse)
        print(speed[0])
        print(speed[1])
        
    if cote=="gauche":
        print("bump cote gauche....")
        print("-Y -x-"+str(-1500+robot.robot_base)+" -t"+str(3.14159265359+robot.angle_base))
        reponse=robot.com("-Y -x"+str(0+robot.robot_base)+" -t"+str(3.14159265359+robot.angle_base)) 
    elif cote=="droite":
        reponse=robot.com("-Y -x"+str(2000-robot.robot_base)+" -t"+str(0+robot.angle_base))  
    elif cote=="devant":
        reponse=robot.com("-Y -y"+str(3000-robot.robot_base)+" -t"+str(1.57079632679+robot.angle_base)) 
    elif cote=="derriere":
        reponse=robot.com("-Y -y"+str(0+robot.robot_base)+" -t"+str(4.71238898038+robot.angle_base))
    else:
        print("bump rien....")

    robot.com("-B -m-1")
    #on avance a nouveau
    if cote=="gauche":
        reponse=robot.com("-G -r -x0 -y-220 -t0 -m0")
    else:
        reponse=robot.com("-G -r -x0 -y-150 -t0 -m0")
    print("on attent l'arrivee du robot...")
    while (robot.com("-E")!="yes"):
        time.sleep(0.1) 

black_button_pressed=False
def black_button_callback(channel):
    global black_button_pressed
    time.sleep(0.1)
    if GPIO.input(robot.but1_button)==0:
        print("black button pressed")
        black_button_pressed=True;
    
def fast_conf():
        #config vitesse rotation
        robot.com("-X -x5 -y12 -m1")

        #config acceleration lineaire
        robot.com("-X -x500 -y500 -m0")

        #config pid
        robot.com("-D -x0.0001 -y0.0004 -t-0.000 -m0")
        robot.com("-D -x0.5 -y-0.2 -t-0.000 -m1")
        
        


def init_robot():
    global black_button_pressed
    try:
        GPIO.setup(robot.trig_button,GPIO.IN)
        GPIO.setup(robot.col_button,GPIO.IN)
        GPIO.setup(robot.but1_button,GPIO.IN)
        GPIO.setup(robot.but2_button,GPIO.IN)
        GPIO.add_event_detect(robot.but2_button, GPIO.FALLING, callback=black_button_callback, bouncetime=300)
    except:
        pass
    boot_ok=False
    while(boot_ok==False):
        black_button_pressed=False
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
#        fast_conf()

        print("stop asserv")
        robot.com("-B -m0")
        print("motor 1 back")
        robot.com("-B -x-0 -m1")
        print("motor 2 back")
        robot.com("-B -x-0 -m2")


        print("les choses serieuses")
        robot.my_lcd.write("deamon connected...")
        # reponse=robot.com("-E")
        # print(reponse)

        print("changer 3 fois la couleur pour selectionner la couleur")
        robot.my_lcd.write("chg 3 times col")
        GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
        print("changer 2 fois la couleur pour selectionner la couleur")
        robot.my_lcd.write("chg 2 times col")
        GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
        print("changer 1 fois la couleur pour selectionner la couleur")
        robot.my_lcd.write("chg 1 times col")
        GPIO.wait_for_edge(robot.col_button, GPIO.RISING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
        
        robot.my_lcd.write("color : ")
        robot.my_lcd.write("press blue then")
        while GPIO.input(robot.but1_button)==1:
            if GPIO.input(robot.col_button)==1:
                robot.color="jaune"
                robot.my_lcd.lcd_display_string("jaune ",1,8)
            else:
                robot.color="violet"
                robot.my_lcd.lcd_display_string("violet",1,8)
            if(black_button_pressed==True):
                print("black button pressed, it should continue")
                continue;
            time.sleep(0.1)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
        if GPIO.input(robot.trig_button)==0:
            robot.my_lcd.write("remove trigger")
        GPIO.wait_for_edge(robot.trig_button, GPIO.RISING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
        robot.my_lcd.write("insert trigger")
        GPIO.wait_for_edge(robot.trig_button, GPIO.FALLING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
        robot.my_lcd.write("press blue again")
        GPIO.wait_for_edge(robot.but1_button, GPIO.FALLING)
        if(black_button_pressed):
            continue;
        robot.my_lcd.write("red button and blue again")
        GPIO.wait_for_edge(robot.but1_button, GPIO.FALLING)
        if(black_button_pressed):
            continue;
 
        print ("couleur robot", robot.color)
        if(robot.color=="jaune"):
            reponse=robot.com("-Y -x500 -y 500 -t1.57079632679")

            robot.my_lcd.write("righ !")
            fthewall("derriere")
            robot.turn(-1.57079632679, 0)
            while (robot.com("-E")!="yes"):
                time.sleep(0.1) 
        else:
            reponse=robot.com("-Y -x500 -y 2500 -t4.71238898038")
            robot.my_lcd.write("left !")
            fthewall("devant")
            robot.turn(1.57079632679, 0)
            while (robot.com("-E")!="yes"):
                time.sleep(0.1)

        print("F*** The Wall!")
        robot.my_lcd.write("f the wall : ")
        time.sleep(1)
        robot.my_lcd.write("go! ")
        fthewall("gauche")


        print("placement en position initiale")
        if(robot.color=="jaune"):
            print("pour les JAUNE")
#            reponse=robot.com("-G -x470 -y250 -t3.14159265359 -m0")
            robot.go_direct(470,250,3.14159265359,-1,False)
        else:
            print("pour le VIOLET")
#            reponse=robot.com("-G -x470 -y2750 -t3.14159265359 -m0")
            robot.go_direct(470,2750,3.14159265359,-1,False)
        print(reponse)

        truc=""
        while (robot.com("-E")!="yes"):
            time.sleep(0.1) 

        print("wait for trigger") 
        GPIO.wait_for_edge(robot.trig_button, GPIO.RISING)
        if(black_button_pressed==True):
            print("black button pressed, it should continue")
            continue;
            time.sleep(0.1)
        boot_ok=True
        
