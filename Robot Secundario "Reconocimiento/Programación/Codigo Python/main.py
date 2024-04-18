# Librerias
import cv2 as cv #libreria para la visión computacional
import mediapipe as mp #mediapipe es para poder poner las marcas y poder sacar las coordenadas de los puntos de la mano
import serial #para poder mandar datos serial al arduino
import serial.tools.list_ports #para poder leer los puertos del arduino
from math import sqrt #sacar la raiz cuadrada de un numero

def distance_formula(p_1, p_2) -> float: 
    """
    distancia de formula entre p_1 y p_2
    """
    distance = sqrt((p_2[0] - p_1[0]) ** 2 + (p_2[1] - p_1[1]) ** 2)
    return distance

def get_coordinate(coordinate_point, height, width):
    """
    coordinate_point: es una lista con la posición x y posición y 
    height: es la altura de la pantalla
    width: es la anchura de la pantalla
    sacar las coordenadas de un punto y ajusta por la altura y anchura de la pantalla
    """
    position = []
    position.append(coordinate_point.x * width)
    position.append(coordinate_point.y * height)
    return position

def send_signal(serial, bin):
    """
    serial: es el puerto serial
    bin: es la información binaria
    para mandar una señal binaria a un serial
    """
    serial.write(bin)

try:
    port_list = [] # crea una lista de puertos
    ports = serial.tools.list_ports.comports()   # pone la lista de los puertos y su información en la variable ports
    for port, desc, hwid in sorted(ports): # desempaca la información de los puertos en el puerto, desc, y hwid
        port_list.append(str(port)) # pone el dato del puerto en la lista de port_list para poder ver los disponibles

    ser = serial.Serial(port_list[2]) # se pone el puerto ser de port_list en posición 2 ya que es el disponible en este caso
    print("connected") # pone "connected" en la terminal para simbolizar que se está corriendo correctamente el código

    cap = cv.VideoCapture(0) # selecciona la camara del sistema en la variable cap
    drawing_module = mp.solutions.drawing_utils # guarda el mapa de la librería de mano de mediapipe 
    mp_hands = mp.solutions.hands # guarda para poner el mapa de las manos y sus coordenadas

    # Estados iniciales (todos desactivados)
    prev_index_state = False
    prev_middle_state = False
    prev_ring_state = False
    prev_pinky_state = False

    with mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.7, min_tracking_confidence=0.7,
                        max_num_hands=1) as hands: # abre el media pipe y pone que con una confianza de 0.7 mara detectar máximo una mano y le pone el alias hands

        while True: # abre un bucle para el funcionamiento del programa
            _, frame = cap.read() # lee la camara y pone en frame el frame que saca
            image_height, image_width, _ = frame.shape # guardo en las variables image_height, image_width las dimenciones de la camara
            results = hands.process(cv.cvtColor(frame, cv.COLOR_BGR2RGB)) # cambia el valor de colores de BGR a RGB

            if results.multi_hand_landmarks: # checa si hay manos en la 
                for handLandmark in results.multi_hand_landmarks: # dibuja las manos y pone las coordenadas y checa las coordenadas
                    drawing_module.draw_landmarks(frame, handLandmark, mp_hands.HAND_CONNECTIONS) # hace el dibujo en la pantalla

                    wrist_coordinate = get_coordinate(handLandmark.landmark[mp_hands.HandLandmark.WRIST], image_height, 
                                                      image_width) # saca la coordenada de la muñeca
                    index_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP], image_height, image_width) # la coordenada de la punta del dedo indice
                    middle_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP], image_height, image_width) # la coordenada de la punta del dedo de en medio
                    ring_fingertip_coordinate = get_coordinate(
                        handLandmark.landmark[mp_hands.HandLandmark.RING_FINGER_TIP], image_height, image_width) # la coordenada de la punta del dedo anular
                    pinky_coordinate = get_coordinate(handLandmark.landmark[mp_hands.HandLandmark.PINKY_TIP],
                                                      image_height, image_width) # la coordenada de la punta del dedo meñique

                    # Control de dedos basado en distancia con la muñeca
                    if (distance_formula(index_fingertip_coordinate, wrist_coordinate) < 120): # checa si la distancia entre la muñeca y el dedo indice es menor a 120.
                        if not prev_index_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn on finger 1")
                            send_signal(ser, b'1') # manda una señal binaria 1
                            prev_index_state = True
                    else:
                        if prev_index_state: # checa el estado previa si esta levantado el dedo o no
                            # print("turn off finger 1")
                            send_signal(ser, b'2') # manda una señal binaria 2
                            prev_index_state = False

                    if (distance_formula(middle_fingertip_coordinate, wrist_coordinate) < 120): # checa si la distancia entre la muñeca y el dedo medio es menor a 120.
                        if not prev_middle_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn on finger 2")
                            send_signal(ser, b'3') # manda una señal binaria 3
                            prev_middle_state = True
                    else:
                        if prev_middle_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn off finger 2")
                            send_signal(ser, b'4') # manda una señal bianria 4
                            prev_middle_state = False

                    if (distance_formula(ring_fingertip_coordinate, wrist_coordinate) < 120): # checa si la distancia entre la muñeca y el dedo anular es menor a 120.
                        if not prev_ring_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn on finger 3")
                            send_signal(ser, b'5') # manda una señal bianria 5
                            prev_ring_state = True
                    else:
                        if prev_ring_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn off finger 3") 
                            send_signal(ser, b'6') # manda una señal bianria 6
                            prev_ring_state = False

                    if (distance_formula(pinky_coordinate, wrist_coordinate) < 120): # checa si la distancia entre la muñeca y el dedo meñique es menor a 120.
                        if not prev_pinky_state: # checa el estado previa si esta levantado el dedo o no
                            print("turn on finger 4")
                            send_signal(ser, b'7') # manda una señal bianria 7
                            prev_pinky_state = True
                    else:
                        if prev_pinky_state:  # checa el estado previa si esta levantado el dedo o no
                            print("turn off finger 4")
                            send_signal(ser, b'8') # manda una señal bianria 8
                            prev_pinky_state = False

            cv.imshow('frame', cv.flip(frame, 1)) # voltea la imagen horizontalmente
            if cv.waitKey(1) == ord('q'): # checa si se presiona la tecla "q"
                break # en caso de que se presione la tecla q se sale del bucle

    cap.release() # suelta la camara para liberar recursos
    cv.destroyAllWindows() # destruyen las ventanas de visualización
    ser.close() # cierra el serial

except Exception as e: # le las posibles excepciones
    print(e) # imprime la excepción
    if type(e) == IndexError: # en un caso de error de indice se podría asumir que es por la falta de lectura de puertos arduinos
        print("No Arduino was recognized in the system") # imprime la falta de ardui
