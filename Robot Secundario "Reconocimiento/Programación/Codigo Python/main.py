# Librerias
import cv2 as cv
import mediapipe as mp
import serial
import serial.tools.list_ports
from math import sqrt

def distance_formula(p_1, p_2) -> float:
    distance = sqrt((p_2[0] - p_1[0]) ** 2 + (p_2[1] - p_1[1]) ** 2)
    return distance

def get_coordinate(coordinate_point, height, width):
    position = []
    position.append(coordinate_point.x * width)
    position.append(coordinate_point.y * height)
    return position

def send_signal(serial, bin):
    serial.write(bin)

try:
    port_list = []
    ports = serial.tools.list_ports.comports()
    for port, desc, hwid in sorted(ports):
        port_list.append(str(port))

    ser = serial.Serial(port_list[2])
    print("connected")

    cap = cv.VideoCapture(0)
    drawing_module = mp.solutions.drawing_utils
    mp_hands = mp.solutions.hands

    # Estados iniciales (todos desactivados)
    prev_index_state = False
    prev_middle_state = False
    prev_ring_state = False
    prev_pinky_state = False

    with mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.7, min_tracking_confidence=0.7,
                        max_num_hands=1) as hands:

        while True:
            _, frame = cap.read()
            image_height, image_width, _ = frame.shape
            results = hands.process(cv.cvtColor(frame, cv.COLOR_BGR2RGB))

            if results.multi_hand_landmarks:
                for handLandmark in results.multi_hand_landmarks:
                    drawing_module.draw_landmarks(frame, handLandmark, mp_hands.HAND_CONNECTIONS)

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

                    # Control de dedos basado en distancia con la muñeca
                    if (distance_formula(index_fingertip_coordinate, wrist_coordinate) < 120):
                        if not prev_index_state:
                            print("turn on finger 1")
                            send_signal(ser, b'1')
                            prev_index_state = True
                    else:
                        if prev_index_state:
                            # print("turn off finger 1")
                            send_signal(ser, b'2')
                            prev_index_state = False

                    if (distance_formula(middle_fingertip_coordinate, wrist_coordinate) < 120):
                        if not prev_middle_state:
                            print("turn on finger 2")
                            send_signal(ser, b'3')
                            prev_middle_state = True
                    else:
                        if prev_middle_state:
                            print("turn off finger 2")
                            send_signal(ser, b'4')
                            prev_middle_state = False

                    if (distance_formula(ring_fingertip_coordinate, wrist_coordinate) < 120):
                        if not prev_ring_state:
                            print("turn on finger 3")
                            send_signal(ser, b'5')
                            prev_ring_state = True
                    else:
                        if prev_ring_state:
                            print("turn off finger 3")
                            send_signal(ser, b'6')
                            prev_ring_state = False

                    if (distance_formula(pinky_coordinate, wrist_coordinate) < 120):
                        if not prev_pinky_state:
                            print("turn on finger 4")
                            send_signal(ser, b'7')
                            prev_pinky_state = True
                    else:
                        if prev_pinky_state:
                            print("turn off finger 4")
                            send_signal(ser, b'8')
                            prev_pinky_state = False

            cv.imshow('frame', cv.flip(frame, 1))
            if cv.waitKey(1) == ord('q'):
                break

    cap.release()
    cv.destroyAllWindows()
    ser.close()

except Exception as e:
    print(e)
    if type(e) == IndexError:
        print("No Arduino was recognized in the system")
