import socket
import time

# Server IP and port
server_ip = "10.200.7.50"
server_port = 500
message = "Hello, UDP Server!"

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Set a timeout for receiving a response (optional)
sock.settimeout(2)  # Waits for 2 seconds for a response

try:
    # Record the time before sending the message
    send_time = time.time()
    
    # Send the message to the server
    print(f"Sending message to {server_ip}:{server_port}")
    sock.sendto(message.encode(), (server_ip, server_port))

    # Wait for a response from the server
    response, server_address = sock.recvfrom(1024)  # Buffer size of 1024 bytes

    # Record the time after receiving the response
    receive_time = time.time()

    # Calculate the round-trip time (RTT)
    rtt = (receive_time - send_time) * 1000  # Convert to milliseconds
    print(f"Received echo from server: {response.decode()}")
    print(f"Round-trip time (latency): {rtt:.2f} ms")

except socket.timeout:
    print("No response received from the server.")

finally:
    # Close the socket
    sock.close()
