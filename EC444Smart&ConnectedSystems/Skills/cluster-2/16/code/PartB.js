// Serial port example from design pattern
// Uses ESP code in folder: serial-esp-to-node-serialport

const fs = require("fs");
const { SerialPort } = require('serialport');
const port = new SerialPort({ path: 'COM4', baudRate: 115200 });

const { ReadlineParser } = require('@serialport/parser-readline');

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

// Read the port data
port.on("open", () => {
  console.log('Serial port now open');
});

parser.on('data', data => {
  console.log('The word from ESP32:', data);
  fs.appendFile("data.csv", data, (err) => {
    if (err) {
      console.error(err);
    } else {
      console.log('Data successfully written to file');
    }
  });
});
