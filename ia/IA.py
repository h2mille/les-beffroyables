#			0x090 0x27	/*P8.7, button 6*/
#			0x094 0x27	/*P8.8, button 7 /
#			0x044 0x27	/*P8.32 button 5*/
#			0x0dc 0x27	/*P9.23 button 3*/

import Adafruit_BBIO.GPIO as GPIO
import os
import time
import shlex
import boot
import robot

from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE, SIG_DFL)


if os.getuid()!=0:
    print("il faut etre admin (sudo)")
    quit()
	
print("ca marche!")
boot.init_robot()

