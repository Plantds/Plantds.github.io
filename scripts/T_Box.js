var TIME = document.getElementById("Time");
var TBOX = document.getElementById("TBOX");

var id = null;
var timeFlow = 1;
TIME.innerHTML = timeFlow;
function MOVETBOX() {
    var present = 1;
    clearInterval(id);
    id = setInterval(frame, 25);
    function frame() {
      if (present <= 0) {
        timeFlow = 1;
      } else {
        present += timeFlow; 
        TBOX.style.left = present + '%';
        if(present >= 100)
            timeFlow = -1;
        TIME.innerHTML = timeFlow;
      }
    }
}