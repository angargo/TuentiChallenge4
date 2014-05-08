#!/usr/bin/env node

if ((process.version.split('.')[1]|0) < 10) {
	console.log('Please, upgrade your node version to 0.10+');
	process.exit();
}

var net = require('net');
var util = require('util');
var crypto = require('crypto');

var options = {
	'port': 6767,
	'host': '0.0.0.0',
}

const KEYPHRASE = 'YOUR KEYPHRASE';

var dh, secret, state = 0;

var socket = net.connect(options, function() {
	socket.write('hello?');
	state++;
});

socket.on('data', function(data) {

	data = data.toString().trim().split('|');

	if (state == 1 && data[0] == 'hello!') {
		dh = crypto.createDiffieHellman(256);
		dh.generateKeys();
		socket.write(util.format('key|%s|%s\n', dh.getPrime('hex'), dh.getPublicKey('hex')));
		state++;
	} else if (state == 2 && data[0] == 'key') {
		secret = dh.computeSecret(data[1], 'hex');
		var cipher = crypto.createCipheriv('aes-256-ecb', secret, '');
		var keyphrase = cipher.update(KEYPHRASE, 'utf8', 'hex') + cipher.final('hex');
		socket.write(util.format('keyphrase|%s\n', keyphrase));
		state++;
	} else if (state == 3 && data[0] == 'result') {
		var decipher = crypto.createDecipheriv('aes-256-ecb', secret, '');
		var message = decipher.update(data[1], 'hex', 'utf8') + decipher.final('utf8');
		console.log(message);
		socket.end();
	} else {
		console.log('Error');
		socket.end();
	}

});
