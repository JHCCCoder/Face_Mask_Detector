// relative path to the sensor data for
// get/post:
var serverPath = "/sensor/:80";

// callback when the Web page has been loaded
$(document).ready(function() {
    window.intervalId = setInterval(function() {
        // callback for interval timer for every second
        $.getJSON(serverPath,function(result){
            var path = "data:image/jpg;base64,"+result.mat.replace(/[\r\n]/g,'');
            $("#imageBase").html('<img src="'+path+'"/>');

        });
    }, 1000);
});