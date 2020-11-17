const aedes = require('aedes')();
const { createServer } = require('aedes-server-factory');
const PORT = 1883;
const WEBSOCKET_PORT = 8883;

const serverOverWebsocket = createServer(aedes, { ws: true });
const server = createServer(aedes);



async function start() {

  await server.listen(PORT);
  console.log(`BROKER is listening on PORT ${PORT}`);
  await serverOverWebsocket.listen(WEBSOCKET_PORT);
  console.log(`WEBSOCKET is listening on PORT ${WEBSOCKET_PORT}`);
}

aedes.on('clientReady', () => {
  console.log('new client has connected and is ready');
  console.log(`connected clients: ${aedes.connectedClients}`);
});

aedes.on('clientDisconnect', () => {
  console.log('one client has disconnected');
  console.log(`connected clients: ${aedes.connectedClients}`);
});

aedes.on('publish', (packet, client) => {
  console.log(`publishing packet on channel ${packet.topic}`);
  console.log(`message: ${packet.payload.toString()}`);
})

start();