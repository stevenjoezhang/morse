import socket
import struct
import time

UDP_PORT = 49000
UDP_IP = "127.0.0.1"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.sendto(b"RPOS\0", (UDP_IP, UDP_PORT))

while True:
  raw, addr = sock.recvfrom(1024)
  headerlen = 5
  header = raw[0:headerlen]
  data = raw[headerlen:]
  values = struct.unpack("<dddffffffffff", data)
  print(values)
