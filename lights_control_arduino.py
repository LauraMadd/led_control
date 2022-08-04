import serial
import time
# import simpleaudio as sa


class Lights_control():
    """Class to control light sources= 488 nm, 561 nm and white light trough arduino
        board. This code relies on the arduino code to programm the board.
    """

    def __init__(self, port='COM6'):
        self.arduino = serial.Serial(port)

    def test(self):
        for i in range(10):
            self.arduino.write(str.encode('1'))
            self.play_obj = self.wave_obj.play()
            self.play_obj.wait_done()  # Wait until sound has finished playing
            time.sleep(1)
            self.arduino.write(str.encode('0'))
            self.play_obj = self.wave_obj.play()
            self.play_obj.wait_done()  # Wait until sound has finished playing
            time.sleep(1)

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
    
    def high_power_widefield_opto(self, 
                                  delay_start=None, 
                                  train_interval=None, 
                                  num_pulse=None, 
                                  duty_cycle=None):
        """Blue laser on and blinks high-power blue LED
        """
        #add letter to sort during capture of serial
        # self.arduino.write(str.encode('a' + str(delay_start))) #delayStart
        # time.sleep(1.1) # wait for Serial.parseInt() timer to run out before sending next serial, maybe make 1.1
        # self.arduino.write(str.encode('b' + str(train_interval))) #delayTime
        # time.sleep(1.1)
        # self.arduino.write(str.encode('c' + str(num_pulse))) #blinkFreq
        # time.sleep(1.1)
        # self.arduino.write(str.encode('d' + str(duty_cycle))) #dutyCycle
        # time.sleep(1.1)
        # self.arduino.write(str.encode('9'))

        #add letter to sort during capture of serial
        self.arduino.write(str.encode('a' + str(10000))) #delayStart
        time.sleep(1.1) # wait for Serial.parseInt() timer to run out before sending next serial, maybe make 1.1
        self.arduino.write(str.encode('b' + str(2000))) #delayTime
        time.sleep(1.1)
        self.arduino.write(str.encode('c' + str(1))) #blinkFreq
        time.sleep(1.1)
        self.arduino.write(str.encode('d' + str(10))) #dutyCycle
        time.sleep(1.1)
        self.arduino.write(str.encode('9'))
    
    def close(self):
        """Close communication with arduino
        """
        self.arduino.close()
