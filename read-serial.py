from serial import Serial

# Get serial port and baud rate from user
print("Example (Windows): COM3, 9600")
print("Example (Linux): /dev/ttyUSB0, 9600")
port = input("Enter serial port: ")
baud = int(input("Enter baud rate: "))

with Serial(port, baud) as ser:
    while True:
        print(ser.readline().decode('utf-8'), end='')