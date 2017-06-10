//ECC
const BN = require('bn.js');
var EC = require('elliptic').ec;
var ec = new EC('p192');
var exec = require('child_process').exec;
var fs = require('fs');
var StringBuilder = require('stringbuilder');



//Tem de ir ler a chave publica da box;

var brPubX = '11FA2B68851DEDA9B0CE4D6EFD76F4623DD4600FEB5824EF';
var brPubY = '1B2585D62B7E6055C8534362A55F7F4F6EAB50F376CF18CE';

var newKey = ec.genKeyPair();

var newPriv = newKey.getPrivate().toString(16);
var newPubX = newKey.getPublic().getX().toString(16);
var newPubY = newKey.getPublic().getY().toString(16);

//var newPriv = "b1e6da10e70616b1007f2200069e4ba04c32a3aa709a041";
//var newPubX = "4836afd14762e456abc1f62770b74666d36473fb721f57f3";
//var newPubY = "215928e5cef0001559db7d19505e749b3ea5b5a5c06d2c15";


console.log(newPriv.toString(16));


var main = new StringBuilder();

  // extend de String object
StringBuilder.extend('string');

var filename = '/home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/Keys.c';
var stream = fs.createWriteStream( filename );
main
  .appendLine('#include <stdio.h>')
  .appendLine('#include <stdlib.h>')
  .appendLine('#include <string.h>')
  .appendLine()
  .appendLine()
  .appendLine('uint32_t MYPrivateKey[6] = {0} ;', converterr(newPriv))
  .appendLine('uint32_t brPubKeyX[6] = {0} ;', converterr(brPubX))
  .appendLine('uint32_t brPubKeyY[6] = {0} ; ', converterr(brPubY))
  ;
var stream = fs.createWriteStream( filename, 'utf-8' );
main.pipe(stream);
main.flush();



function converterr(strHEX) {
  var str = "" + strHEX;
  var res = str.match(/.{8}/g);

  var final
  final = '{ '
  res.reverse().forEach(function(entry, idx, array) {
    final += '0x'+ entry
    if (idx != array.length - 1) {
      final += ', '
    }
  });
  final += ' }'

  console.log(final);
  return final;
}

var strIEEE;

var spawn = require('child_process').spawn,
    ls    = spawn('make', ['-C', '/home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/', '-I', 'sudo /home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/', 'er-example-server.upload']);

    ls.stdout.on('data', function (data) {
      console.log('stdout: ' + data.toString());
    });

    ls.stderr.on('data', function (data) {
      if( Boolean(data.toString().match('IEEE')) ){
        strIEEE = data.toString();
        console.log('stderr: ' + data.toString());
      }
        //console.log(data.toString().match('IEEE'));
    });

    ls.on('exit', function (code) {
      console.log('child process exited with code ' + code.toString());
      if(code === 0 )
        wirteToFile();
    });


//exec('make -C /home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/ -I sudo /home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/ clean', function (error, username) {
    //console.log('stdout: %s', username);

    //exec('make -C /home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/ -I sudo /home/user/GIT/contiki/examples/zolertia/tutorial/03-coap/  er-example-server.upload', function (error, username) {
    //    console.log('stdout: %s', username);

    //});
//});


function wirteToFile() {

  var main = new StringBuilder();

    // extend de String object
  StringBuilder.extend('string');
  var MAC = parseMAC(strIEEE);
  console.log(MAC);
  var filename = 'Keys/IP ' + MAC;
  var stream = fs.createWriteStream( filename );
  main
    .appendLine()
    .appendLine('IP: {0}',MAC)
    .appendLine()
    .appendLine('PRIV: {0}', newPriv )
    .appendLine()
    .appendLine('PUBX: {0}',newPubX)
    .appendLine('PUBY: {0}',newPubY)
    ;
  var stream = fs.createWriteStream( filename, 'utf-8' );
  main.pipe(stream);
  main.flush();

}

function parseMAC(sIEEE) {
  var arrIEEE = sIEEE.split("IEEE")[sIEEE.split("IEEE").length-1].split(":");

  return 'aaaa::2'+arrIEEE[6].trim()+":"+arrIEEE[7].trim()+arrIEEE[8].trim()+":"+arrIEEE[1].trim().charAt(1)+arrIEEE[2].trim()+":"+arrIEEE[3].trim()+arrIEEE[4].trim();
}

console.log( newPubX );
console.log( newPubY );
