const express = require('express');
const http = require('http');

var dgram = require('dgram');

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.115'; // pi ip address EDIT

// Create socket
var server = dgram.createSocket('udp4');

const app = express();
const http_server = http.createServer(app);
const io = require('socket.io')(server);


app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

<<<<<<< HEAD

// Listen for incoming WebSocket connections on the localhost port
io.on('connection', (socket) => {
    console.log('A user connected');

    // Forward data from the serial port to the socket (address:port)
    parser.on('data', (data) => {
        //console.log(`${data}\n`);
        socket.emit('data', data);
        console.log(`${data}`);
    });

    // Handle disconnections
    socket.on('disconnect', () => {
        console.log('A user disconnected');
    });
});

// Start the server
server.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
<<<<<<< Updated upstream
});
=======
// Create server that listens on a port
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

let ActivityArr = [0, 0, 0, 0];
let leader = -1;
let leader_changed = false;
let wait = 0;

function leaderCheck() 
{
    let ActivityMax = Math.max(...ActivityArr);
    let ActivityMax_Index = ActivityArr.indexOf(ActivityMax);

    if(leader != ActivityMax_Index)
    {
        leader = ActivityMax_Index;
        leader_changed = true;
    }
}

//setInterval(leaderCheck, 600000);

// On connection, print out received message
server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
    
    let data = message.toString().split(' ');

    switch(remote.address)
    {
        case '192.168.1.110':
            ActivityArr[0] = data[0] + data[1];
            break;
        case '192.168.1.132':
            ActivityArr[1] = data[0] + data[1];
            break;
        case '192.168.1.118':
            ActivityArr[2] = data[0] + data[1];
            break;
        case '192.168.1.115':
            ActivityArr[3] = data[0] + data[1];
            break;
    }

    if(leader_changed == true)
    {
        server.send('LEADER CHANGED', remote.port, remote.address, function(error)
        {
            if(error)
                console.log('MEH!');
            else
                console.log('Sent leader change to' + `${remote.address}`);
        });
        
        wait++;
        if(wait == 50)
        {
            wait = 0;
            leader_changed = false;
        }
    }
    
    io.on('connection', (socket) => {
        console.log('A user connected');
        socket.emit('data', message);
        // Handle disconnections
        socket.on('disconnect', () => {
            console.log('A user disconnected');
        });
    });


});

// Bind server to port and IP
server.bind(PORT, HOST);


http_server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
>>>>>>> 455a437582625063620e82c45e907dfeb314e37c
=======

});


>>>>>>> Stashed changes
