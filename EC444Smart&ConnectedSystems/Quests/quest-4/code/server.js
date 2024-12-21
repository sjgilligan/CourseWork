// UDP from the ESP32s part
var dgram = require('dgram');


const PORT = 5555; 
// Create socket
var server1 = dgram.createSocket('udp4');

const express = require('express');
const app = express();
var fs= require("fs");


//DATABASE INCLUDES 
var Engine = require('tingodb')();
var db = new Engine.Db('./mydb', {}); // CREATE DIRECTORY CALLED 'mydb' IN FILE SPACE 


var logCollection = db.collection('sensorlogs');
// UDP traffic from ESP32, no acknowlegement or error checking
// Create server that listens on a port
server1.on('listening', function () {
    var address = server1.address();
    console.log('UDP Server1 listening on ' + address.address + ":" + address.port);
});



let file_data = []; 
//READ FROM FILE 
fs.readFile('/Users/Eve/EC444/smoke.txt', 'utf8', function (err, data) { //read from file smoke.txt
    // Display the file content
    if(err){ 
        console.error("File not found",err); 
        return; 
    }
    file_data = data.split('\n').slice(1); // Skip the first line (header)
    //console.log("file_data: ", file_data); 
    
    file_data.forEach(line => {
        const values = line.trim().split('\t'); // Split each line by spaces

        const time_val = parseInt(values[0]);
        //console.log("time_val: " , time_val);
        const id_val = parseInt(values[1]);
        const smoke_val = parseInt(values[2]);
        const temp_val = parseFloat(values[3]);  
        final_message = "Formatted data: " + time_val + " " + id_val + " " + smoke_val + " " + temp_val ;
        //console.log(final_message); // send to console debug
        //WRITING TO DB
        var logEntry = { time: time_val,  id: id_val, smoke: smoke_val,
        temp: temp_val };
        console.log("logEntry: " ,logEntry); 
        logCollection.insert(logEntry, function(err, result) {
            if (err) throw err;
            console.log('ERROR:');
        });
    });
    //QUERY DB 
        // Now read back (the whole DB!)
    db.collection('sensorlogs', function(err, collection) {
        if (err) throw err;
        const query = { "id":1,"smoke": 1}; 
        collection.find(query).toArray(function(err, returned_data) {
        if (err) throw err;
            console.log("Database says: ", returned_data); 
        });
    });
});


server1.bind(PORT); 
