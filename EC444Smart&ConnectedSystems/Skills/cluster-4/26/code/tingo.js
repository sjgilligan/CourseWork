const express = require('express');
const app = express();
var fs= require("fs");

var Engine = require('tingodb')();
var db = new Engine.Db('./mydb', {}); // create directory


var logCollection = db.collection('sensorlogs');

let file_data = []; 
//READ FROM FILE 
fs.readFile('./smoke.txt', 'utf8', function (err, data) { //read from file
    // Display the file content
    if(err){ 
        console.error("File not found",err); 
        return; 
    }
    file_data = data.split('\n').slice(1); // skip first line
    
    file_data.forEach(line => {
        const values = line.trim().split('\t'); 

        const time_val = parseInt(values[0]);
        const id_val = parseInt(values[1]);
        const smoke_val = parseInt(values[2]);
        const temp_val = parseFloat(values[3]);  
        final_message = "Formatted data: " + time_val + " " + id_val + " " + smoke_val + " " + temp_val ;
		
        //write to DB
        var logEntry = { time: time_val,  id: id_val, smoke: smoke_val, temp: temp_val };
        console.log("logEntry: " ,logEntry); 
        logCollection.insert(logEntry, function(err, result) {
            if (err) throw err;
            //console.log('ERROR:');
        });
    });
    
	//query DB for ID 1:
    db.collection('sensorlogs', function(err, collection) {
        if (err) throw err;
        const query = { "id":1,"smoke": 1}; 
        collection.find(query).toArray(function(err, returned_data) {
        if (err) throw err;
            console.log("Database says: ", returned_data); 
        });
    });
});

