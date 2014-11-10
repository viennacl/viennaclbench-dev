// Namik Karovic
function displayTime() {
    var time = "";

    var currentTime = new Date();
    var minutes = currentTime.getMinutes();
    var seconds = currentTime.getSeconds();
    var milliseconds = currentTime.getMilliseconds();

    if (minutes < 10) {
        minutes = "0" + minutes;
    }
    if (seconds < 10) {
        seconds = "0" + seconds;
    }
    time += + minutes + ":" + seconds + ":" + milliseconds;
    return time;
}