import socket
import time

def get_open_ports():
    open_ports = []
    for port in range(1, 1025):  # Scan common ports (1-1024)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(1)
        result = sock.connect_ex(("localhost", port))
        if result == 0:
            open_ports.append(port)
        sock.close()
    return open_ports

baseline_open_ports = set(get_open_ports())

while True:
    current_open_ports = set(get_open_ports())
    new_open_ports = current_open_ports - baseline_open_ports
    
    if new_open_ports:
        print("Possible port scan detected. New open ports:", new_open_ports)
        # You can add more actions like sending an alert here
    
    baseline_open_ports = current_open_ports
    time.sleep(600)  # Adjust the interval as needed
