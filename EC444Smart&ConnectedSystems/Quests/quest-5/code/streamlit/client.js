const dgram = require('dgram');
const client = dgram.createSocket('udp4');

const express = require('express');
const app = express();

//DATABASE INCLUDES 
var Engine = require('tingodb')();
var db = new Engine.Db('./mydb', {});
var logCollection = db.collection('datalogs');
// Define the server details
const PORT = 41234;
const HOST = '192.168.0.167';

//requests info from server, puts in a database and listens for request from streamlit of querried information to display 
var init = 0;
const INTERVAL = 3000; // Interval in milliseconds (e.g., 1000ms = 1 second)

const requestDataForRobot = (robotID) => {
    const message = Buffer.from(`ROBOTID ${robotID}`);

    client.send(message, PORT, HOST, (err) => {
        if (err) {
            console.error(`Error sending message to Robot ${robotID}: ${err}`);
            client.close();
        } else {
            console.log(`Message sent to Robot ${robotID}: ${message}`);
        }
    });
};

    // Listen for a response from the server
    client.on('message', (msg, rinfo) => {
        const msgstring = msg.toString(); 
        console.log(`Received response from server: ${msgstring}`);
        const dataArray = msgstring.split(','); 
        const IDval = parseInt(dataArray[0]); 
        const  Xval = parseFloat(dataArray[1]);  
        const Zval = parseFloat(dataArray[2]); 
        const thetaval = parseFloat(dataArray[3]); 
        const statusval = dataArray[4]; 
        console.log(`X value: ${Xval}`); 
        var logEntry = {id:IDval, X : Xval, Z : Zval, theta : thetaval, status : statusval} ; 
        //console.log(`Log Entry: ${logEntry.id}`); 
        
       logCollection.insert(logEntry, function(err, result) {
            if (err) throw err;
        });  

        app.get('/api/data', (req, res) => {
            const query = {id: 1, X: 1, Z:1, _id:0 }; 
            logCollection.find({}, query).toArray((err, data) => {
                if (err) {
                    console.error(err);
                    res.status(500).send('Error querying database');
                    return;
                }
                res.json(data); // Return database records as JSON
            });
        });
    });
   
setInterval(() => {
    requestDataForRobot('07');
    requestDataForRobot('08');
}, INTERVAL);

app.listen(3000, () => {
    console.log('Server is running on http://localhost:3000');
});
// Handle process termination gracefully
process.on('SIGINT', () => {
    console.log('Closing UDP client...');
    client.close();
    process.exit();
});

