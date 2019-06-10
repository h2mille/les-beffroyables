import time
import boot
import robot
import opponent




robot.unset_lidar()
#config vitesse rotation
robot.com("-X -x5 -y12 -m1")
time.sleep(0.1)

#config acceleration lineaire
robot.com("-X -x700 -y800 -m0")
time.sleep(0.1)

#config pid
robot.com("-D -x0.002 -y0.0001 -t-0.000 -m0")
#boot.fast_conf()

robot.com("-X -x5 -y12 -m1")
time.sleep(0.1)

#config acceleration lineaire, vitesse puis acceleration
robot.com("-X -x500 -y00 -m0")
time.sleep(0.1)

#config pid

robot.com("-X -x300 -y100 -m0")

robot.com("-D -x0.0001 -y0.0004 -t-0.000 -m0")
robot.com("-D -x0.5 -y-0.2 -t-0.000 -m1")
#boot.fast_conf()

reponse=robot.com("-Y -x200 -y500 -t0") 

robot.set_lidar()
while(True):
    time.sleep(0.1)
   robot.go_direct(1200,500,0,0,False)
   time.sleep(0.3)
   robot.go_direct(200,500,0,0,False)
   time.sleep(0.3)
