import socket
import json, subprocess, threading

tr = threading.Thread()

connections = []

def fun(com):
    process = subprocess.Popen(['systemctl', com, 'nginx'], 
    stdout=subprocess.PIPE,
    universal_newlines=True)
    print(f'{com} Done.')

host = "191.101.80.221"
port = 55255  # initiate port no above 1024
server_socket = socket.socket()  # get instance
server_socket.bind((host, port))  # bind host address and port together
# configure how many client the server can listen simultaneously
server_socket.listen()
conn, address = server_socket.accept()  # accept new connection
connections.append(conn)
while True:
    # receive data stream. it won't accept data packet greater than 1024 bytes
    data = conn.recv(1024).decode()
    if (data=="1"):
        t1 = threading.Thread(target=fun, args=("stop",))
        t1.start()
        t1.join()
        conn.send(("Done.").encode())

    if (data=="2"):
        t1 = threading.Thread(target=fun, args=("start",))
        t1.start()
        t1.join()
        conn.send(("Done.").encode())

