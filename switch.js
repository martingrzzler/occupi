const { Gpio } = require( 'onoff' );
const mqtt = require('mqtt');

const client = mqtt.connect('ws://192.168.178.191:1883');
const TOPIC = 'BATHROOM';

const switchIn = new Gpio( '17', 'in', 'both', { debounceTimeout:  10} );

client.on('connect', () => {
  console.log('connected to broker');
  switchIn.watch(onChange);
});


async function onChange(err, val) {
  if (err) {
    throw err;
  }

  const message = val ? 'OCCUPIED' : 'FREE';

  client.publish(TOPIC, message, { retain: true });
  await sleep(100);
  console.log('Updated State of Bathroom');
}

function sleep(millis) {
  return new Promise(resolve => {
    setTimeout(() => { resolve() }, millis);
  });
}