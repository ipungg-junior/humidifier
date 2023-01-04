import socket
import json, subprocess

comm = "\{'command': start,'service':gunicorn\}"

def client_program():
    host = "191.101.80.221"  # as both code is running on same pc
    port = 55255  # socket server port number

    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server
    data = client_socket.recv(1024).decode()  # receive respons
    print(data)



if __name__ == '__main__':
    client_program()