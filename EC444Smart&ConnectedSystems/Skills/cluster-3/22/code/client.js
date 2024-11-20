// Import the dgram module
const dgram = require('dgram');

// Create a UDP client socket
const client = dgram.createSocket('udp4');

// Message to send to the server
const message = Buffer.from('1');

// Server details
const serverPort = 3333;  // Port number of the UDP server
const serverAddress = '192.168.1.142';  // IP address of the server


// SEND MESSAGE
client.send(message, 0, message.length, serverPort, serverAddress, (err) => {
  if (err) {
    console.error('Error sending message:', err);
    client.close();
  } else {
    console.log('Message sent to UDP server');
  }
});

// Handle message received from the server
//RECEIVING MESSAGE 
client.on('message', (msg, rinfo) => {
  console.log(`Received message: ${msg} from ${rinfo.address}:${rinfo.port}`);
  client.close();  // Close the client socket after receiving the response
});

// Handle errors
client.on('error', (err) => {
  console.log(`Client error: ${err.stack}`);
  client.close();
});