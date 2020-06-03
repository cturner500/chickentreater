#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EMailSender.h>


//for SMS:
#include <WiFiClientSecure.h>
//const char* host = "maker.ifttt.com";
//const int httpsPort = 443;

const char* ssid = "<ENTER YOUR SSID HERE>";
const char* password = "<ENTER WIFI PASSWORD HERE>";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
int LEDPin = 2;
Servo servo;
 

//WiFiClient espClient;

EMailSender emailSend(<GMAILADDRESS>,<GMAILPASSWORD>);
arduino-dfa5de

void setup(void){
  
  //the HTML of the web page
  page = "<center><img src=\"https://i0.wp.com/www.smokintsmokehouse.com/wp-content/uploads/2015/02/happy-chicken.png?resize=300%2C293\"><br><h1>SolarChickens.com Chicken Treater!</h1><p>&nbsp;<a href=\"1treat\"><button>Push here to treat the Chickens!</button></a><br><br><br>Consider a $1 donation to <a href=\"https://www.classy.org/give/184866/#!/donation/checkout\">Pets in Need</a>?</p></center>";
  //make the LED pin output and initially turned off
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  servo.attach(5); //D1
  servo.write(0);
   
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Soft MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
   
 server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/1treat", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
    digitalWrite(LEDPin, HIGH);
    servo.write(180);
    delay(1000);
    servo.write(0);
    delay(5000);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(LEDPin, LOW);

    //EMAIL
    EMailSender::EMailMessage message;
    message.subject = "Chickens Treated!";
    message.message = "Chickens treated";
    EMailSender::Response resp = emailSend.send(<EMAIL ADDRESS>, message);
    Serial.println("Sending status: ");
    Serial.println(resp.code); 
    Serial.println(resp.desc);
    Serial.println(resp.status);


    });
    
  server.on("/2treat", [](){
    server.send(200, "text/html", page);
    servo.write(0);
    delay(1000); 
  });
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();
}
