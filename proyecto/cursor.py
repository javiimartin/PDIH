import cv2
import mediapipe as mp
import pyautogui

pyautogui.FAILSAFE = False

# Inicializa MediaPipe Hands.
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(static_image_mode=False,
                       max_num_hands=1,
                       min_detection_confidence=0.5)

# Inicializa la cámara.
cap = cv2.VideoCapture(0)

# Factor de escala para ajustar la velocidad del cursor del ratón.
scale_factor = 4

# Variable para rastrear el estado de la mano.
hand_closed = False

while cap.isOpened():
    success, image = cap.read()
    if not success:
        continue

    # Flip de la imagen y conversión a RGB.
    image = cv2.flip(image, 1)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    
    # Procesamiento de la imagen.
    results = hands.process(image_rgb)
    
    # Verifica si se detecta una mano.
    if results.multi_hand_landmarks:
        # Para cada mano detectada.
        for hand_landmarks in results.multi_hand_landmarks:
            # Calcula las coordenadas del rectángulo alrededor de la mano.
            x_min, y_min = float('inf'), float('inf')
            x_max, y_max = float('-inf'), float('-inf')
            for landmark in hand_landmarks.landmark:
                x, y = int(landmark.x * image.shape[1]), int(landmark.y * image.shape[0])
                x_min = min(x_min, x)
                y_min = min(y_min, y)
                x_max = max(x_max, x)
                y_max = max(y_max, y)
            
            # Dibuja el rectángulo alrededor de la mano.
            cv2.rectangle(image, (x_min, y_min), (x_max, y_max), (0, 255, 0), 2)
            
            # Muestra el estado de la mano (abierto/cerrado).
            hand_status = "Abierta" if not hand_closed else "Cerrada"
            cv2.putText(image, hand_status, (x_min, y_min - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            
            # Verifica si la mano está cerrada (comparando la posición del pulgar y el dedo índice).
            thumb = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_finger = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            hand_closed = thumb.x < index_finger.x
        
        # Realiza un clic si la mano está cerrada.
        if hand_closed:
            pyautogui.click()
        else:
            # Mueve el cursor del ratón a la posición de la mano (si está abierta).
            pyautogui.moveTo(x_min * scale_factor, y_min * scale_factor)
    
    # Muestra la imagen en una ventana.
    cv2.imshow('Controlador de Ratón', image)
    if cv2.waitKey(5) & 0xFF == 27:
        break

# Limpieza
cap.release()
cv2.destroyAllWindows()
