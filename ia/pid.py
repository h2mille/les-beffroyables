import time
import boot
import robot
import opponent
#config vitesse rotation
robot.com("-X -x5 -y12 -m1")
time.sleep(0.1)

#config acceleration lineaire
robot.com("-X -x1000 -y1000 -m0")
time.sleep(0.1)

#config pid
robot.com("-D -x0.001 -y0.0004 -t-0.000 -m0")
robot.com("-D -x0.5 -y-0.2 -t-0.000 -m1")
#boot.fast_conf()