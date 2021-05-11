const express = require('express');
const app = express();
const path = require('path');

app.set('port', 3001);
app.set('view engine', 'html');
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, './index.html'));
}, (req, res) => {
    throw new Error('에러다 이놈아');
});

app.listen(app.get('port'), () => {
    console.log(app.get('port'), '번 포트에서 대기 중');
});