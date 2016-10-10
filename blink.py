###########################################################################
# Setup code goes below, this is called once at the start of the program: #
###########################################################################
import time
import machine
pin = machine.Pin(2, machine.Pin.OUT)
 
while True:
    ###################################################################
    # Loop code goes inside the loop here, this is called repeatedly: #
    ###################################################################
    pin.high()
    time.sleep(1.0)  # Delay for 1 second.
    pin.low()
    time.sleep(1.0)  # Delay for 1 second.
