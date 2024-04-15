#Librerias
import cv2 as cv #Computer Vision, Camara y vista
import mediapipe as mp #Reconocimiento "Bolitas"
import serial #Mandar Datos
import serial.tools.list_ports #Leer puertos
from math import sqrt #Raiz Cuadrada


def distance_formula(p_1, p_2) -> float: #Distancia entre cooordenadas
    """
    it gets the distance between 2 positions given in parameters
    p_1: Tuple(float, float)
    p_2: Tuple(float, float)
    """

    distance = sqrt((p_2[0] - p_1[0])  ** 2 + (p_2[1] - p_1[1])  ** 2) #Formula de distancia entre puntos
    return distance


def get_coordinate(coordinate_point, height, width): #Coordenada de un punto
    """
    it returns a list with the coordinate x in the position [0] and position y in the [1] position
    """
    #Ajuste de coordenadas x,y
    position = []
    position.append(coordinate_point.x * width) #Coordenada x
    position.append(coordinate_point.y * height) #Coordenada y
    return position


def send_signal(serial, bin): #Enviar datos
    serial.write(bin)


try:

    port_list = []  # creo una lista para guardar la lista de los nombres de cada conexion
    ports = serial.tools.list_ports.comports()  # utilizo la libreria de serial para buscar los puertos
    for port, desc, hwid in sorted(ports):  # checo entre los puertos
        port_list.append(str(port))  # guardo en la lista el puerto seleccionado

    ser = serial.Serial(port_list[2])  # abre la primera serial de la lista
    print("connected")  # imprime "connected" para checar que si este conectado

    cap = cv.VideoCapture(0)  # abre la camara 0 con la libreria de cv2

    drawing_module = mp.solutions.drawing_utils  # guarda las funciones de drawing_utils en la variable
    mp_hands = mp.solutions.hands  # guarda el modulo de manos en la variable mp_hands

    with mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.7, min_tracking_confidence=0.7,
                        max_num_hands=1) as hands:

        # abre el atributo Hands del modulo
        while (True):
            _, frame = cap.read() #Prender la camara y guardar en frame lo que lea la camara
            image_height, image_width, _ = frame.shape #Alto y ancho del frame
            results = hands.process(cv.cvtColor(frame, cv.COLOR_BGR2RGB)) #crea un objeto para nodos de mano

            if results.multi_hand_landmarks != None:
                #Reconocimiento dedos y dibuja el mapa de la mano
                for handLandmark in results.multi_hand_landmarks:
                    drawing_module.draw_landmarks(frame, handLandmark, mp_hands.HAND_CONNECTIONS)
                    #Agarrar coordenada de muñeca con la función get coordinate
                    wrist_coordinate = get_coordinate(handLandmark.landmark[mp_hands.HandLandmark.WRIST], image_height,
                                                      image_width)

                    index_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP], image_height, image_width)

                    middle_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP], image_height, image_width)

                    ring_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.RING_FINGER_TIP], image_height, image_width)

                    pinky_coordinate = get_coordinate(handLandmark.landmark[mp_hands.HandLandmark.PINKY_TIP],
                                                      image_height, image_width)
                #Medir distancia menor a 120 pixeles, Si: Enviar señal, No: Señal opuesta
                if (distance_formula(index_fingertip_coordinate, wrist_coordinate) < 120):
                    print("turn on finger 1")
                    send_signal(ser, b'1')
                else:
                    # print("turn off finger 1")
                    send_signal(ser, b'2')

                if (distance_formula(middle_fingertip_coordinate, wrist_coordinate) < 120):
                    print("turn on finger 2")
                    send_signal(ser, b'3')
                else:
                    print("turn off finger 2")
                    send_signal(ser, b'4')

                if (distance_formula(ring_fingertip_coordinate, wrist_coordinate) < 120):
                    print("turn on finger 3")
                    send_signal(ser, b'5')
                else:
                    print("turn off finger 3")
                    send_signal(ser, b'6')

                if (distance_formula(pinky_coordinate, wrist_coordinate) < 120):
                    print("turn off finger 4")
                    send_signal(ser, b'7')
                else:
                    print("turn off finger 4")
                    send_signal(ser, b'8')

                #debuggeo para leer la coordenada
                #print(distance_formula(pinky_coordinate, wrist_coordinate))

                

            cv.imshow('frame', cv.flip(frame, 1)) #Mostrar camara en pantalla
            if (cv.waitKey(1) == ord('q')): #Salir de bucle al presionar q
                break

        #Suelta la camara y todos los recursos que utilice el sistema
        cap.release()
        cv.destroyAllWindows()
        ser.close()
except Exception as e: #Debuggeo de errores
    print(e)
    if (type(e) == IndexError):
        print("no Arduino was recognized in the system")
