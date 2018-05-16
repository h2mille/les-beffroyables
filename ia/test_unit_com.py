import robot
import time


time_start=time.time()
for i in range(0,1000):
    robot.robot_com("-P ")
    time.sleep(0.001)



print("used time: " +str((time.time()-time_start)))