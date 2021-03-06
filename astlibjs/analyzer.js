///
/// \package astlib
/// \file analyzer.js
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date 24.2.2017
/// \brief Asterix console decoderfor Node.js
///
/// (C) Copyright 2017 R-SYS s.r.o
/// All rights reserved.
///


var   astlib = require('./build/Release/addon.node');
const Utils = require('./asterix_utils.js');
const Channels = require('./channels.js');

Utils.declareAsterixCodes(astlib);

var PORT = 50000;

var asterixRecord = astlib.createAsterixRecord();
var codecs = astlib.enumerateAllCodecs();

function decode(buffer) {
	console.log(buffer);
	var records = astlib.decodeAsterixBuffer('Eurocontrol-48:1.21', buffer);
	if (records)
	{
		for(var i = 0; i < records.length; i++)
		{
			var record = records[i];
			if (record)
			{
				var asterixObject = JSON.parse(astlib.toJson(record));
				console.log("Category: " + record.getCategory() + ", Timestamp: " + record.getTimestamp());
				console.log(asterixObject);
			}
		}
	}
}

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

server.on('error', (err) => {
	  console.log(`server error:\n${err.stack}`);
	  server.close();
	});

server.on('message', (msg, rinfo) => {
  //console.log(`server got: ${msg.toString()} from ${rinfo.address}:${rinfo.port}`);
  decode(msg);
});

server.on('listening', () => {
  var address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});

server.bind(PORT);
