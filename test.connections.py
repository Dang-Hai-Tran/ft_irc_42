import socket
import threading


def create_connection(i):
    try:
        # replace with your server's IP and port
        PORT = 5555
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', PORT))
        print(f"Client {i} connected successfully.")
        s.close()
    except socket.error as e:
        print(f"Client {i} failed to connect: {e}")


threads = []
for i in range(500):
    t = threading.Thread(target=create_connection, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()
