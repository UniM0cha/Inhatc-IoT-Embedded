var btnPlay = $("#btn-play"),
    btnPause = $("#btn-pause"),
    btnRefresh = $("#btn-refresh"),
    btnMinus = $("#btn-minus"),
    btnPlus = $("#btn-plus"),
    txtMinute = $("#minute");

var minute = 45,	// default value
	remainTime,
	timer,
	run = false;

var canvas = document.getElementById("remain-time");
var ctx = canvas.getContext("2d");
ctx.strokeStyle = "#ff0000";
ctx.lineWidth = 90;

btnPause.hide();
setMinutes();

btnMinus.on("click", clickMinus);
btnPlus.on("click", clickPlus);
btnPlay.on("click", clickPlay);
btnPause.on("click", clickPause);
btnRefresh.on("click", clickRefresh);

function setMinutes() {
	txtMinute.text(minute);
	remainTime = minute * 60;
	if (minute === 60) remainTime--;
	drawArc();
}

function clickMinus() {
	if (minute > 1) minute--;
	clickPause();
	setMinutes();
}

function clickPlus() {
	if (minute < 60) minute++;
	clickPause();
	setMinutes();
}

function clickPlay() {
	run = true;
	btnPlay.hide();
	btnPause.show();
	timer = setInterval(runTimer, 10);	// 1000으로 만들면 원래 타이머
}

function clickPause() {
	run = false;
	btnPlay.show();
	btnPause.hide();
	clearInterval(timer);
	$(".time-timer").removeClass("blink");
}

function clickRefresh() {
	clickPause();
	setMinutes();
}

function drawArc() {
	var remainPercent = (60 * 60 - remainTime) / (60 * 60);
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	ctx.beginPath();
	ctx.arc(110, 110, 65, 1.5*Math.PI, (1.5*Math.PI) - remainPercent*(2*Math.PI));
	ctx.stroke();
}

function runTimer() {
	if (remainTime === 0) {
		clearInterval(timer);
		$(".time-timer").addClass("blink");
	} else {
		remainTime--;
		drawArc();
	}
}