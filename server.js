const express = require('express');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const app = express();
const port = 3000;

const arduinoSerialPort = new SerialPort('/dev/cu.usbmodem11101', { baudRate: 9600 });
const parser = arduinoSerialPort.pipe(new Readline({ delimiter: '\n' }));

app.use(express.json());

arduinoSerialPort.on('open', () => {
  console.log('Serial Port Open');
});

parser.on('data', data => {
  console.log(data);
});

// app.get('/', (req, res) => {
//   res.send('Hello World from Node.js server!');
// });

app.post('/send', (req, res) => {
  const { command } = req.body;
  arduinoSerialPort.write(command, (err) => {
    if (err) {
      return res.status(500).send('Failed to send command to Arduino');
    }
    res.send('Command sent to Arduino');
  });
});

app.listen(port, () => {
  console.log(`Node server: http://localhost:${port}`);
});