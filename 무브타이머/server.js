var http = require('http');
var fs = require('fs');

var server = http.createServer(onRequest);
// 서버 포트 3000으로 설정
server.listen(3000);

function send404Message(response){
    response.writeHead(404, {"Content-Type" : "text/plain"});  //단순한 글자 출력
    response.write("404 ERROR...");
    response.end();
}

function onRequest(request, response){
    if(request.url == '/'){
        url = '/index.html';
        response.writeHead(200);
        response.end(fs.readFileSync(__dirname + url));
        console.log("웹페이지 불러오기");
    } else{
        send404Message(response);
    }
}