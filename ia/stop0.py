import time
import opponent
from threading import Thread
import os

class STOP (Thread):
    def __init__(self):
        Thread.__init__(self)
        
    def run(self):
        if(opponent.check_danger()==True):
            os.system("init 0");
        time.sleep(0.1)
            