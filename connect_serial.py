import serial
import time

ser = serial.Serial('COM3', 9600)
time.sleep(2)
data = 'a'
ser.write(bytes(data, 'UTF-8'))
read_char =ser.read(12)
print(f'serial_text : {read_char}')
ser.close()