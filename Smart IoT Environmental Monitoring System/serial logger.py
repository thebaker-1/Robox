import serial
import csv

# Configuration
SERIAL_PORT = 'COM3'
BAUD_RATE = 9600
CSV_FILE = 'c:/Users/learn/Desktop/MAIN/DEV/Pr/_Robox/Smart IoT Environmental Monitoring System/robot_log.csv'

try:
    # Initialize serial connection
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, open(CSV_FILE, mode='w', newline='') as file:
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        writer = csv.writer(file)
        writer.writerow(["Timestamp", "Distance (cm)", "State"])

        while True:
            try:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    print(f"Received: {line}")
                    data = line.split(',')
                    if len(data) == 3:
                        writer.writerow(data)
                        file.flush()
            except KeyboardInterrupt:
                print("Logging stopped.")
                break
except serial.SerialException as e:
    print(f"Error: {e}")