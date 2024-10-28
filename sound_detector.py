import pyaudio
import numpy as np
import serial
import time

# Serial setup for Arduino
arduino = serial.Serial('/dev/cu.usbmodem11301', 9600)
time.sleep(2)

# PyAudio setup
CHUNK = 2048  # Increased buffer size
RATE = 22050  # Lowered sample rate

p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK)

print("Listening for audio...")

# Define the maximum volume and a threshold (adjust based on observed levels)
MAX_VOLUME = 40000  # Adjust based on actual volume range
THRESHOLD = MAX_VOLUME * 0.5  # Adjust threshold based on observed volume levels

while True:
    try:
        # Read audio data from the microphone
        data = np.frombuffer(stream.read(CHUNK, exception_on_overflow=False), dtype=np.int16)
        volume = int(np.linalg.norm(data))

        print(f"Volume: {volume}")

        # Check if volume is above the threshold to move the servo
        if volume > THRESHOLD:
            print("Speaking detected, moving the servo.")
            arduino.write(b"1\n")  # Send '1' to Arduino to move the servo
        else:
            print("Quiet detected, stopping the servo.")
            arduino.write(b"0\n")  # Send '0' to Arduino to stop the servo

        time.sleep(0.05)

    except IOError as e:
        print(f"Input overflowed: {e}")
        continue  # Skip this frame and continue with the next iteration
