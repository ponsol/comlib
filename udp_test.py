import socket
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
#client.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
client.bind(("", 8444))
while True:
    data, addr = client.recvfrom(1024)
    print("received message: %s"%data)
