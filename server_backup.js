var express = require('express');
var http = require('http');

var app = express();
var path = require('path');

var server = http.createServer(app);
// 서버 포트 3000으로 설정
server.listen(3000);

// 시리얼 포트 설정
// /dev/cu.debug-console : 아두이노가 연결된 포트로 설정해야하지만 아두이노 없는 관계로 임시로 지정
var serialPort = require('serialport');
var sp = new serialPort('/dev/cu.debug-console',{
    baudRate : 9600,
    dataBits : 8,
    parity : 'none',
    stopBits: 1,
    flowControl: false
})
sp.on('open', function () {
    console.log('open serial communication');
})
// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.use(express.static(path.join(__dirname, 'public')));

app.get('/',function(req,res) {
    res.status(200).render('controller2.ejs');
})

app.get('/controller/:id',function(req,res){
    console.log(req.params.id);
    sp.write(req.params.id) ;
    res.status(200).send('Motor Controll Ok');
})
