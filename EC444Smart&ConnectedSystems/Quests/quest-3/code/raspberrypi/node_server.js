var dgram = require('dgram');
const express = require('express');
const http = require('http');

var PORT = 3333;
var HOST = '192.168.1.147';

var server = dgram.createSocket('udp4');
var client = dgram.createSocket('udp4');

const app = express();
const http_server = http.createServer(app);
const io = require('socket.io')(http_server);

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

let ActivityArr = [0, 0, 0, 0];
let leader = -1;
let leader_changed = 0; 

let data;
let client_address;
let client_port;

let map = new Map();
map.set(0, '192.168.1.142');
map.set(1, '192.168.1.118');
map.set(2, '192.168.1.110');

server.on('message', function(message, remote) {
    console.log(remote.address + ':' + remote.port + '- ' + message);

    data = message.toString().split(' ');

    server.send('OK', remote.port, remote.address, function(error){
        if(error)
            console.log('MEH');
        else
            console.log('Sent to ' + `${remote.address.toString()}`);
    })

    switch(remote.address)
    {
        case '192.168.1.142':
            ActivityArr[0] = parseFloat(data[1]) + parseFloat(data[2]);
            break;
        case '192.168.1.118':
            ActivityArr[1] = parseFloat(data[1]) + parseFloat(data[2]);
            break;
        case '192.168.1.110':
            ActivityArr[2] = parseFloat(data[1]) + parseFloat(data[2]);
            break;
    }

    prev_leader = leader;
    leader = ActivityArr.reduce((maxIndex, currentValue, currentIndex, array) => {
        return currentValue > array[maxIndex] ? currentIndex : maxIndex;
    }, 0);

    if(prev_leader != leader)
    {
        leader_changed = 1;
    }

    if(leader_changed == 1)
    {
        for(let i = 0; i < 3; i++)
        {
            if(i == leader)
            {
                client.send('1 1', 0, 3, '5555', map.get(i), (err) => {
                    if(err)
                        console.error('Cant send :(', err);
                    else
                        console.log(`1 1 sent to ${map.get(i)}`);
                });
                console.log('LEADER: ' + leader);
                console.log('index: ' + i);
            }
            else
            {
                client.send('1 0', 0, 3, '5555', map.get(i), (err) => {
                    if(err)
                        console.error('Cant send :(', err);
                    else
                        console.log('`1 1 sent to ${map.get(i)}`');
                });
                console.log('LEADER: ' + leader);
                console.log('index: ' + i);
            }

        }

        leader_changed = 0; 
    }

    let message2 = remote.address.toString() + ' ' + message.toString();
    io.emit('data', message2);
    
});

server.bind(PORT, HOST);

http_server.listen(8888, () => {
    console.log(`Server is running on http://localhost:8888`);
});

io.on('connection', (socket) => {
    console.log('A user connected');
    //socket.emit('data', message);
    // Handle disconnections
    socket.on('disconnect', () => {
        console.log('A user disconnected');
    });
});
