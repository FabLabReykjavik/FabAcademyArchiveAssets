//
// UDP socket send
// Neil Gershenfeld 11/21/15
//
if (process.argv.length != 5) {
   console.log("command line: node udpsnd.js address port text")
   process.exit()
   }
var address = process.argv[2]
var port = process.argv[3]
var text = process.argv[4]
var dgram = require("dgram")
var client = dgram.createSocket("udp4")
var message = new Buffer(text)
client.send(message,0,message.length,port,address,function(err,bytes) {
   if (err) throw err
   console.log("sent '"+message+"' to "+address+":"+port)
   client.close()
   })
