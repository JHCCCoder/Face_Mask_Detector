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
    }, 50);

    $("#driver").click(function(event){
        var coordinate_x1 = $("#coordinate_x1").val();
        var coordinate_y1 = $("#coordinate_y1").val();
        var coordinate_x2 = $("#coordinate_x2").val();
        var coordinate_y2 = $("#coordinate_y2").val();
        $.post(
            serverPath,
            {
                coordinate_x1: coordinate_x1,
                coordinate_y1: coordinate_y1,
                coordinate_x2: coordinate_x2,
                coordinate_y2: coordinate_y2
            }
        );

    });
});