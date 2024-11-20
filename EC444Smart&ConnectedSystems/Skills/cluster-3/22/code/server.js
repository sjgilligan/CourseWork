// Required module
var dgram = require('dgram');

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.117';

// Create socket
var server = dgram.createSocket('udp4');

// Create server that listens on a port
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);

    // Send Ok acknowledgement
    //can send information back to client 
    let blinkTime = 100;
    console.log(blinkTime); 
    server.send(blinkTime.toString(),remote.port,remote.address,function(error){
      if(error){
        console.log('MEH!');
      }
      else{
        console.log('Sent: Ok!');
      }
    });

});

// Bind server to port and IP
server.bind(PORT, HOST);