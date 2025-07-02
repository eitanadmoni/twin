import socket
import argparse
import struct
import time


def client_connection(ip, port):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((ip, port))
            print(f"Connected to server at {ip} in port {port}")
            message = "PING".encode()
            n = len(message)
            s.send(struct.pack('=I', n))
            time.sleep(0.1)
            s.send(message)
            data = s.recv(1024)
            print(data.decode())

    except Exception as e:
        print(f"An error occurred: {e}")
        s.close


def get_arguments():
    parser = argparse.ArgumentParser(description="Client of domain")
    parser.add_argument("ip", type=str, help="ip of server to connect to")
    parser.add_argument("port", type=int, help="port of server to connect to")
    return parser.parse_args()


def main():
    args = get_arguments()
    client_connection(args.ip, args.port)

    
if __name__ == "__main__":
    main()