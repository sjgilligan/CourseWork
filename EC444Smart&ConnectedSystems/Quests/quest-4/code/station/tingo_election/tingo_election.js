const express = require('express');
const http = require('http');
var dgram = require('dgram');

var Engine = require('tingodb')();
var db = new Engine.Db('./election_db', {});
var electionCollection = db.collection('election_data');

var PORT = 3333;
var HOST = '192.168.1.147'; //INSERT SERVER IP ADDRESS HERE

var server = dgram.createSocket('udp4');

const app = express();
const http_server = http.createServer(app);
const io = require('socket.io')(http_server);

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/webclient.html');
});

let collectionArr = [];
let blueTotal = 0;
let redTotal = 0;

// Create server that listens on a port
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});


server.on('message', function(message, remote) {
    
    console.log(`Message ${message} received from ${remote.address}:${remote.port}`);
    let data = message.toString().split(' ');
    
    const currentTime = new Date().toLocaleTimeString('en-US', {
        hour: 'numeric',
        minute: '2-digit',
        hour12: true,
        timeZone: 'America/Los_Angeles'  // Set the desired time zone here
    });
    console.log(`CURRENT TIME IS ${currentTime}\n`);

    if(data[1] == 'R')
        redTotal++;
    else if(data[1] == 'B')
        blueTotal++;
    
    if(data[0] == '132' || data[0] == '142' || data[0] == '110' || data[0] == '118')
    {
        var entry = {ID: data[0], Vote: data[1], Time: currentTime};
        electionCollection.insert(entry, function(err, result) {
            if(err) console.log("ERROR");

            collectionArr.push(entry);

            console.log('SUCCESSFULLY LOGGED INTO DATABASE');
    });
    }

    server.send('MESSAGE RECEIVED', remote.port, remote.address, function(err) {
        if(err) console.log('FAILED TO SEND');
    });

});

server.bind(PORT, HOST);

io.on('connection', (socket) => {
    console.log('A user connected');
    
    // Emit the initial data to the newly connected client
    socket.emit('data', collectionArr);

    // Listen for "update" event
    socket.on('update', () => {
        // Fetch the latest data from the database
        electionCollection.find().toArray((err, items) => {
            if (err) return console.error('Error fetching data:', err);

            collectionArr = items; // Update in-memory array
            io.emit('data', collectionArr); // Send to all clients
        });
    });

    // Listen for "reset database" event
    socket.on('reset database', () => {
        electionCollection.remove({}, {multi: true}, (err) => {
            if (err) return console.error('Couldn\'t clear database:', err);
            
            collectionArr = []; // Clear in-memory array as well
            blueTotal = 0;
            redTotal = 0;
            console.log("Cleared election database");
            io.emit('data', collectionArr); // Notify clients of the reset
        });
    });

    // Handle client disconnect
    socket.on('disconnect', () => {
        console.log('A user disconnected');
    });
});

http_server.listen(7777, () => {
    console.log(`Server is running on http://localhost:7777`);
});
