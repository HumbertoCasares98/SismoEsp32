#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const int derA = 18; 
const int derB = 19;
const int derA1 = 22;
const int derB1 = 23; 
int i=0;

// Replace with your network credentials
const char* ssid = "Lab Electronic";
const char* password = "!2345678";

const int output = 2;

String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>SismoSim</title>
  <style>
    
  html {
  font-family: Arial; display: inline-block; text-align: center;
   font-size: 2.5em; /* 40px/16=2.5em */
  width: 100%;
  height:100%;
  background: linear-gradient(-45deg, #23a6d5, #23d5ab);
}


form {
  width: 300px;
  margin: 0 auto;
  text-align: center;
  padding-top: 50px;
}

.value-button {
  display: inline-block;
  border: 1px solid #ddd;
  margin: 10px;
  width: 80px;
  height: 30px;
  text-align: center;
  vertical-align: middle;
  padding: 11px 0;
  color: white;
  background: #000;
  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

.value-button:hover {
  cursor: pointer;
}

form #decrease {
  margin-right: -4px;
  border-radius: 8px 0 0 8px;
}

form #increase {
  margin-left: -4px;
  border-radius: 0 8px 8px 0;
}

form #input-wrap {
  margin: 0px;
  padding: 0px;
}

input#number {
  font-family: Arial; 
   font-size: 1.5em; /
  text-align: center;
  border: none;
  border-bottom: 1px solid #ddd;
  margin: 0px;
  width: 200px;
  height: 120px;
}

input[type=number]::-webkit-inner-spin-button,
input[type=number]::-webkit-outer-spin-button {
    -webkit-appearance: none;
    margin: 0;
}
  </style>
</head>
<body>
  <h2>Simulador de Sismos FI-UAGro</h2>
    
    <form>
        <div class="value-button" id="decrease" onclick="decreaseValue()" value="Decrease Value">-</div>
          <input type="number" id="textSliderValue" min="0" max="255" value="35" disabled />
        <div class="value-button" id="increase" onclick="increaseValue()" value="Increase Value">+</div>
    </form>

  
  <script>
    function updatePWM(val) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value="+val, true);
      xhr.send();
    }

    function increaseValue() {
      var value = parseInt(document.getElementById('textSliderValue').value, 10);
     if(value<255){
        value=value+5;
      }
      document.getElementById('textSliderValue').value = value;
      updatePWM(value);
    }
    
    function decreaseValue() {
      var value = parseInt(document.getElementById('textSliderValue').value, 10);
      if(value==0){
        value=0;
      }else if(value>0){
        value=value-5;
      }
      document.getElementById('textSliderValue').value = value;
      updatePWM(value);
    }
  </script>
  
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(derA, ledChannel);
  
  ledcWrite(ledChannel, sliderValue.toInt());

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
     /* for (i=35;i==255;i+5){
          ledcWrite(ledChannel, i);
          delay(10000);
        }*/
      ledcWrite(ledChannel, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
