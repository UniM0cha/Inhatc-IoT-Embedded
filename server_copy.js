const http = require('http');
const fs = require('fs');

const server = http.createServer((reqest, response) => {
    fs.readFile('./index.html', (err,data) => {
        if (err){
            throw err;
        }
        response.end(data);
    })
});

server.listen(3000);
server.on('listening', () => {
    console.log('3000번 포트에서 서버 대기중!');
});
server.on('error', (error) => {
    console.error(error);
});