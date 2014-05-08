#!/usr/bin/env node

if ((process.version.split('.')[1]|0) < 10) {
  console.log('Please, upgrade your node version to 0.10+');
  process.exit();
}

var net = require('net');
var util = require('util');
var crypto = require('crypto');
var fs = require('fs');

//Connexion with Man-in-the-Middle
var options = {
  'port': 6969,
  'host': '54.83.207.90',
}

var dh, dh2, secret, keyphrase, state = 0;

var socket = net.connect(options, function() {
  //Read Keyphrase from file
  KEYPHRASE = fs.readFileSync('./ch06.in').toString();
});

socket.on('data', function(data) {
  
  data = data.toString().trim().split(':');
  //data[0] = FROM->TO
  //data[1] is the actual data sent
  
  //Split data[1] by '|'
  data2 = data[1].toString().trim().split('|');
  
  if (state <= 1) {
    //CLIENT->SERVER: hello?
    //SERVER->CLIENT: hello!
    socket.write(data[1]);
    ++state;
  }
  else if (state == 2) {
    //The key is going from server to client
    
    //Pretending to be server, compute the response to this key
    dh2 = crypto.createDiffieHellman(data2[1], 'hex');
    dh2.generateKeys();
    secret2 = dh2.computeSecret(data2[2], 'hex');
    backkey = dh2.getPublicKey('hex');
    
    //Pretending to be the client, I send my key to the server
    dh = crypto.createDiffieHellman(256);
    dh.generateKeys();
    socket.write(util.format('key|%s|%s\n', dh.getPrime('hex'), dh.getPublicKey('hex')));
    ++state;
  }
  else if (state == 3) {
    //The response to my key comes from the server
    
    //Pretend to be the client, I compute the client's secret and keyphrase
    secret = dh.computeSecret(data2[1], 'hex');
    var cipher = crypto.createCipheriv('aes-256-ecb', secret, '');
    keyphrase = cipher.update(KEYPHRASE, 'utf8', 'hex') + cipher.final('hex');
    
    //Replace the response to my key with the response to client's key
    socket.write(util.format('key|%s\n', backkey));
    ++state;
  }
  else if (state == 4) {
    //The client gives me its keyphrase, I send my own keyphrase
    socket.write(util.format('keyphrase|%s\n', keyphrase));
    ++state;
  }
  else if (state == 5) {
    //The server answers with my secret message
    var decipher = crypto.createDecipheriv('aes-256-ecb', secret, '');
    var message = decipher.update(data2[1], 'hex', 'utf8') + decipher.final('utf8');
    
    //Print secret message
    console.log(message);
    socket.end();
  }

});
