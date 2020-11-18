const status = document.querySelector('#status');
const img = document.querySelector('#status-img');

if ('serviceWorker' in navigator) {
  navigator.serviceWorker.register('/sw.js')
    .then((reg) => console.log('service worker regsitered', reg))
    .catch((err) => console.log('service worker not registered', err));
}

connectToBroker();


function connectToBroker() {
  const client = mqtt.connect('wss://occupi.dns.army/mqtt');
  const TOPIC = 'BATHROOM';

  client.on('message', (topic, message) => {
    message = message.toString();
    status.textContent = message;
    const src = message == 'OCCUPIED' ? '/images/closed.png' : '/images/open.png';
    img.src = src;

  });

  client.on('connect', (data) => {
    client.subscribe(TOPIC);
    console.log(data);
    console.log(`Subscriber listening for topic: ${TOPIC}`);
  });
}