function lerp(posA, posB, time){
   var pos;
   pos = (posA - posB)*time;
   return pos;
}

const clamp = (val, min, max) => Math.min(Math.max(val, min), max)

var sliderBF = document.getElementById("SlideBF");
var ABoxBF = document.getElementById("ABoxBF");
var BBoxBF = document.getElementById("BBoxBF");

sliderBF.oninput = function moveBoxesBF(){
   ABoxBF.style.left = lerp(50,0, (sliderBF.value/100)) + '%'; 
   BBoxBF.style.left = lerp(100,0, (sliderBF.value/100)) + '%'; 
}

var sliderAF = document.getElementById("SliderAF");
var ABoxAF = document.getElementById("ABoxAF");
var BBoxAF = document.getElementById("BBoxAF");
sliderAF.oninput = function moveBoxesAF(){
   ABoxAF.style.left = lerp(50,0, clamp((sliderAF.value*2 / 100), 0, 1)) + '%'; 
   BBoxAF.style.left = lerp(100 ,0,  clamp((sliderAF.value*2 / 100 / 2), 0, 1)) + '%'; 
}