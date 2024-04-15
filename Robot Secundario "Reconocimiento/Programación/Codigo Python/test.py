import serial
import serial.tools.list_ports

def send_signal(serial, bin):
    serial.write(bin)

port_list = []  # creo una lista para guardar la lista de los nombres de cada conexion
ports = serial.tools.list_ports.comports()  # utilizo la libreria de serial para buscar los puertos
for port, desc, hwid in sorted(ports):  # checo entre los puertos
    port_list.append(str(port))  # guardo en la lista el puerto seleccionado

print(port_list)

ser = serial.Serial(port_list[2])

while (True):
    send_signal(ser, b'2')
    send_signal(ser, b'3')
