const express = require('express');
const app = express();
const path = require('path');

app.set('port', 3000);
app.set('view engine', 'html');
// 정적으로 경로를 지정해주어서 그 안의 파일들을 모두 불러와주는 것.
app.use('/', express.static(path.join(__dirname, 'view')));

app.listen(app.get('port'), () => {
    console.log(app.get('port'), '번 포트에서 대기 중');
});