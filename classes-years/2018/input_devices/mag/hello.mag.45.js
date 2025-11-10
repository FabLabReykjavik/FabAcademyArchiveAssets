//
// hello.mag.45.js
//
// read and send magnetic field
//    npm install ws and serialport
//
// Neil Gershenfeld 
// (c) Massachusetts Institute of Technology 2014
// 
// This work may be reproduced, modified, distributed, performed, and 
// displayed for any purpose, but must acknowledge the fab modules 
// project. Copyright is retained and must be preserved. The work is 
// provided as is; no warranty is provided, and users accept all 
// liability.
//
var server_port = '1234'
var client_address = '::ffff:127.0.0.1'
var serial_port = "/dev/ttyUSB0"
var baud = 9600
var samples = 100
//
// open serial port
//
var SerialPort = require("serialport")
var sp = new SerialPort(serial_port,{baudRate:baud})
sp.on('error',function(err) {
   console.log("error: "+err)
   process.exit(-1)
   })
//
// look for framing and then update field value
//
var byte2 = 0
var byte3 = 0
var byte4 = 0
var byte5 = 0
var byte6 = 0
var byte7 = 0
var value = 0
var ByteLength = SerialPort.parsers.ByteLength
var parser = sp.pipe(new ByteLength({length:1}));
parser.on('data',function(data) {
   byte1 = byte2
   byte2 = byte3
   byte3 = byte4
   byte4 = byte5
   byte5 = byte6
   byte6 = byte7
   byte7 = data[0]
   if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)) {
      value = (byte5 + 256*byte6 + 256*256*byte7)/samples
      }
   })
//
// wait for socket request and then send field value
//
console.log("listening for connections from "+client_address+" on "+server_port)
var Server = require('ws').Server
wss = new Server({port:server_port})
wss.on('connection', function(ws) {
   if (ws._socket.remoteAddress != client_address) {
      console.log("error: client address "+ws._socket.remoteAddress+" doesn't match")
      return
      }
   console.log("connected to "+client_address+" on port "+server_port)
   ws.on('message', function(data) {
      ws.send(JSON.stringify(value.toFixed(1)))
      })
   })
