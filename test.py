from pickle import TRUE
import serial
import time

class test():
    """Class to control light sources= 488 nm, 561 nm and white light trough arduino
        board. This code relies on the arduino code to programm the board.
    """

    def __init__(self, port='COM6'):
        self.arduino = serial.Serial(port)
    
    def open(self, minutes = None, duration = None): #, 
            # delay_start=None, 
            # train_interval=None, 
            # num_pulse=None, 
            # duty_cycle=None):
        """Blue laser on and blinks high-power blue LED
        """
        self.arduino.write(str.encode('9'))
        print('Sending start signal.')

        self.textfile = open('ArduinoData.txt', 'w')
        
        minutes = 0.25
        duration = minutes * 60

        if self.arduino.readline() != None:
            self.start_time = time.time()

        while TRUE:
            self.current_time = time.time()
            self.elapsed_time = self.current_time - self.start_time

            self.arduinodata = self.arduino.readline().decode('ascii')
            print(self.arduinodata)
            self.textfile.write(self.arduinodata)

            # print(self.elapsed_time)
            if self.elapsed_time > duration:
                break
        
        self.textfile.close()
        self.arduino.write(str.encode('0'))

    def close(self):
        self.arduino.write(str.encode('0'))
        self.programVar = 0
        print('Protocol stopped.')