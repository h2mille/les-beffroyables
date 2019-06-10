import robot

def servo(id,pos):
    robot.com("-S -t"+str(pos)+" -m"+str(id))

def open_servo():
    servo(1,-1)
    servo(2,1)

def close_servo():
    servo(1,1.3)
    servo(2,-1.3)