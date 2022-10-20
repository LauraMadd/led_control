import serial
import time

# This does not work, hogs the serial port, other classes can no longer make use of the port
# Classes that do not initialise the connection to the serial give following error: "AttributeError: 'Pump_control' object has no attribute 'arduino'"
# class __init__():
#     """Class to initialise the arduino connection
#     """
#     def __init__(self, port='COM6'):
#         self.arduino = serial.Serial(port)

# class Lights_control():
class Test():
    """Class to control light sources= 488 nm, 561 nm and white light trough arduino
        board. This code relies on the arduino code to programm the board.
    """

    def __init__(self, port='COM6'):
        self.arduino = serial.Serial(port)

    # def test(self):
    #     for i in range(10):
    #         self.arduino.write(str.encode('1'))
    #         self.play_obj = self.wave_obj.play()
    #         self.play_obj.wait_done()  # Wait until sound has finished playing
    #         time.sleep(1)
    #         self.arduino.write(str.encode('0'))
    #         self.play_obj = self.wave_obj.play()
    #         self.play_obj.wait_done()  # Wait until sound has finished playing
    #         time.sleep(1)

    def blue(self):
        """Blue laser on
        """
        self.arduino.write(str.encode('1'))
        print("Laser @ 488 nm On ")

    def green(self):
        """Green laser on
        """
        self.arduino.write(str.encode('2'))
        print("Laser @ 561 nm On ")

    
    def blue_green(self):
        """Blue and green laser on
        """
        self.arduino.write(str.encode('8'))
       
        print("Laser @ 488 and 561 nm On ")

    def dark(self):
        """Both lasers off
        """
        self.arduino.write(str.encode('0'))


    def brightfield_0(self):
        """White light off
        """
        self.arduino.write(str.encode('3'))

    def brightfield_on_25(self):
        """White light on 25%
        """
        self.arduino.write(str.encode('4'))

    def brightfield_on_50(self):
        """White light on 50%
        """
        self.arduino.write(str.encode('5'))

    def brightfield_on_75(self):
        """White light on 75%
        """
        self.arduino.write(str.encode('6'))

    def brightfield_on_100(self):
        """White light on 100%
        """
        self.arduino.write(str.encode('7'))
    
    # def high_power_widefield_opto(self, 
                                #   delay_start=None, 
                                #   train_interval=None, 
                                #   num_pulse=None, 
                                #   duty_cycle=None):
    def test(self, 
            delay_start=None, 
            train_interval=None, 
            num_pulse=None, 
            duty_cycle=None):

        """Blue laser on and blinks high-power blue LED
        """
        
        # self.arduino.write(str.encode('a' + str(delay_start))) 
        # time.sleep(1.1) # wait for Serial.parseInt() timer to run out before sending next serial, maybe make 1.1
        # self.arduino.write(str.encode('b' + str(train_interval))) 
        # time.sleep(1.1)
        # self.arduino.write(str.encode('c' + str(num_pulse))) 
        # time.sleep(1.1)
        # self.arduino.write(str.encode('d' + str(duty_cycle))) 
        # time.sleep(1.1)

        self.arduino.write(str.encode('a' + str(10000))) 
        time.sleep(1.5) # wait for Serial.parseInt() timer to run out before sending next serial, maybe make 1.1
        self.arduino.write(str.encode('b' + str(5000))) 
        time.sleep(1.5)
        self.arduino.write(str.encode('c' + str(1))) 
        time.sleep(1.5)
        self.arduino.write(str.encode('d' + str(20))) 
        time.sleep(1.5)
        
        self.textfile = open('ArduinoData.txt', 'w')

        minutes = 0.5
        duration = minutes * 60

        self.arduino.write(str.encode('9'))

        if self.arduino.readline() != None:
            self.start_time = time.time()
            

        while True:
            self.current_time = time.time()
            self.elapsed_time = self.current_time - self.start_time
            self.elapsed_min = self.elapsed_time//60
            self.elapsed_sec = self.elapsed_time - (60 * self.elapsed_min)
            self.arduinodata = self.arduino.readline().decode('ascii')
            self.textfile.write(self.arduinodata)
            # print("Running time (min:sec): " 
            #     + str(int(self.elapsed_min)) 
            #     + ":" 
            #     + str(round(self.elapsed_sec,2)))
            print(self.arduinodata)
            if self.elapsed_time > duration:
                break

        self.textfile.close()
        self.arduino.write(str.encode('0'))


    def pump_on(self):
        """Activates pump for the flow chamber
        """
        self.arduino.write(str.encode('e'))

    def pump_off(self):
        """Shuts off pump for the flow chamber
        """
        self.arduino.write(str.encode('f'))
    
    def close(self):
        """Close communication with arduino
        """
        self.arduino.close()
