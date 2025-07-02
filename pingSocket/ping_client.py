import socket
import argparse
import struct
import time

MESSAGE_LENGTH = 1024
SLEEP_BETWEEN_MESSAGES = 0.1

def client_connection(ip, port):
    """
    Function that opens a client, sends PING to the server and prints its answer
    ip: string of the server's ip
    port: int of the server's port
    """
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((ip, port))
            print(f"Connected to server at {ip} in port {port}")
            message = "PING".encode()
            n = len(message)
            s.send(struct.pack('=I', n))
            time.sleep(SLEEP_BETWEEN_MESSAGES)
            s.send(message)
            data = s.recv(MESSAGE_LENGTH)
            print(data.decode())

    except Exception as e:
        print(f"An error occurred: {e}")
        s.close


def get_arguments():
    """
    Function to parse command line arguments (ip and port)
    """
    parser = argparse.ArgumentParser(description="Client of domain")
    parser.add_argument("ip", type=str, help="ip of server to connect to")
    parser.add_argument("port", type=int, help="port of server to connect to")
    return parser.parse_args()


def main():
    args = get_arguments()
    client_connection(args.ip, args.port)

    
if __name__ == "__main__":
    main()