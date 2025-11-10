//
// UDP socket receive
// Neil Gershenfeld 11/21/15
//
if (process.argv.length != 3) {
   console.log("command line: node udprcv.js port")
   process.exit()
   }
var port = process.argv[2]
var dgram = require("dgram")
var server = dgram.createSocket("udp4")
server.bind(port)
server.on("message",function(message,info) {
   console.log("received '"+message+"' from '"+info.address)
   })
server.on("error",function(err) {
   console.log("error: "+err)
   })
console.log("listening on port "+port)
