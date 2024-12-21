#ChatGPT:
import keyboard
import socket

# Configuration
SERVER_IP = "192.168.1.132"  # Replace with the actual server IP if needed
SERVER_PORT = 5555      # Replace with the server port if needed

def udp_client():
    # Create a UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    print(f"UDP client running. Sending controls to {SERVER_IP}:{SERVER_PORT}")
    print("Press W, A, S, D, X to send controls. Press 'Q' to quit.")
    
    try:
        while True:
            if keyboard.is_pressed('w'):
                client_socket.sendto(b'w', (SERVER_IP, SERVER_PORT))
                print("Sent: 'w'")
                while keyboard.is_pressed('w'):  # Wait for key release
                    pass
            elif keyboard.is_pressed('a'):
                client_socket.sendto(b'a', (SERVER_IP, SERVER_PORT))
                print("Sent: 'a'")
                while keyboard.is_pressed('a'):  # Wait for key release
                    pass
            elif keyboard.is_pressed('s'):
                client_socket.sendto(b's', (SERVER_IP, SERVER_PORT))
                print("Sent: 's'")
                while keyboard.is_pressed('s'):  # Wait for key release
                    pass
            elif keyboard.is_pressed('d'):
                client_socket.sendto(b'd', (SERVER_IP, SERVER_PORT))
                print("Sent: 'd'")
                while keyboard.is_pressed('d'):  # Wait for key release
                    pass
            elif keyboard.is_pressed('x'):
                client_socket.sendto(b'x', (SERVER_IP, SERVER_PORT))
                print("Sent: 'x'")
                while keyboard.is_pressed('x'):  # Wait for key release
                    pass
            elif keyboard.is_pressed('q'):  # Quit condition
                print("Quit (Q) pressed")
                break
    except KeyboardInterrupt:
        print("\nProgram interrupted.")
    finally:
        client_socket.close()
        print("Client socket closed.")

if __name__ == "__main__":
    udp_client()

