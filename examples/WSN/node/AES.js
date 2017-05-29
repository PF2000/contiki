var coap = require('coap')
var CryptoJS = require("crypto-js");

//var req = coap.request('coap://[aaaa::212:4b00:60d:b21a]:5683/test/hello')
//var req = coap.request('coap://[aaaa::212:4b00:60d:b305]:5683/test/ola')
var req = coap.request('coap://[aaaa::212:4b00:60d:b305]:5683/test/hello')

var key = CryptoJS.enc.Hex.parse('000102030405060708090A0B0C0D0EFF');
//var data = CryptoJS.enc.Hex.parse('00112233445566778899AABBCCDDEEFF');



// edit this to adjust max packet
req.setOption('Block2', new Buffer([0x02]))

req.on('response', function(res) {

  res.setEncoding('utf8');
  //  console.log(res);
  console.log(res.payload.toString("hex"));
  //console.log('response code', res.code)
  //console.log(res.options[1].value);

  var data = CryptoJS.enc.Hex.parse(res.payload.toString("hex"));

  var encrypted = {};
  encrypted.key = key;
  //encrypted.iv=iv;
  encrypted.ciphertext = data;

  var decrypted3 = CryptoJS.AES.decrypt(encrypted, key, {
    mode: CryptoJS.mode.ECB,
    padding: CryptoJS.pad.NoPadding
  });

  console.log(CryptoJS.enc.Hex.stringify(decrypted3));
  console.log(CryptoJS.enc.Utf8.stringify(decrypted3));

  res.on('end', function() {
    console.log("out");
    process.exit(0);
  });
})

req.end()

