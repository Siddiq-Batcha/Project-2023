import cv2
import machine

# Define the GPIO pin number
pin_num = 12

# Load the pre-trained model for human detection
model = cv2.CascadeClassifier('haarcascade_frontalface_alt.xml')

# Connect to the ESP32-CAM video stream
cap = cv2.VideoCapture('http://192.168.29.8:81/stream')

# Set the GPIO pin to output mode
pin = machine.Pin(pin_num, machine.Pin.OUT)


# Check if the video stream is open
if not cap.isOpened():
    print("Error: Could not open video stream")
    exit()

# Loop through the video frames
while True:
    # Read a frame from the video stream
    ret, frame = cap.read()

    # Check if the frame is empty
    if not ret:
        print("Error: Could not read frame")
        break

    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect humans in the frame
    humans = model.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

    
    # Draw rectangles around the detected humans
    for (x, y, w, h) in humans:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
        # Set the GPIO pin to high
        pin.value(1)
    

    # Display the resulting frame
    cv2.imshow('Human Detection', frame)

    # Press 'q' to quit
    if cv2.waitKey(1) == ord('q'):
        break

# Release the resources
cap.release()
cv2.destroyAllWindows()
