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
  <h4>Stress Monitor</h4><br>
  <h1>Location:<span id="Touch11">0</span></h1><br>
  <h1>Location:<span id="Touch12">0</span></h1><br>
  <h1>Location:<span id="Touch13">0</span></h1><br>
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
      document.getElementById("Touch11").innerHTML =
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
      document.getElementById("Touch12").innerHTML =
      this.responseText;
    }
  };
  xhttp1.open("GET", "readADC2", true);
  xhttp1.send();
}
function getData2() {
  var xhttp2 = new XMLHttpRequest();
  xhttp2.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Touch13").innerHTML =
      this.responseText;
    }
  };
  xhttp2.open("GET", "readADC3", true);
  xhttp2.send();
}
</script>
</body>
</html>
)=====";
