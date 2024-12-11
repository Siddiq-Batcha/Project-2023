const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h2>Information</h2><br>
  <h1>Live BPM Value :<span id="LiveBPM">0</span></h1><br>
  <h1>Patient Condition :<span id="BPMVAL">0</span></h1><br>
  <h1>Patient ECG :<span id="ECG">0</span></h1><br>
  <h1>Patient Pressure : 90 <span id></span></h1><br>
  <br><a href="https://www.armadai.net/">Armada Industrial Automation</a>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
  getData1();
  getData2();
}, 2000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LiveBPM").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
function getData1() {
  var xhttp1 = new XMLHttpRequest();
  xhttp1.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("BPMVAL").innerHTML =
      this.responseText;
    }
  };
  xhttp1.open("GET", "readADC1", true);
  xhttp1.send();
}
function getData2() {
  var xhttp2 = new XMLHttpRequest();
  xhttp2.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ECG").innerHTML =
      this.responseText;
    }
  };
  xhttp2.open("GET", "readECG", true);
  xhttp2.send();
}
</script>
</body>
</html>
)=====";
