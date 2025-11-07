//
// hello.ws-blink.js
//    WebSocket blink hello-world test server
//
// Neil Gershenfeld 11/24/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
const WebSocketServer = require('ws').Server
const ws = new WebSocketServer({port:12345})
ws.on('connection',function connection(ws) {
   console.log('connect')
   ws.on('close',function close() {
      console.log('close');
      ws.send('close');
      ws.close()
      });
   ws.on('message',function message(data) {
      console.log('received: %s',data);
      ws.send('received "'+data+'"');
      });
   });
