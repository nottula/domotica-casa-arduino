#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

//DEFINIZIONE VARIABILI LUMINOSITA'
int lux_camera = 0;

//DEFINIZIONE VARIABILI AUTO MODE LUX
int autolux_camera=0;

//DEFINIZIONE ZONE E PIN
#define PINCAMERA 45
#define PINBAGNONOTTE 46 
#define PINBAGNO 47
#define PINPRANZO 48 
#define PINSALONE 49 
#define PINCUCINA 50 
#define PINESTERNA 51 
#define PINSCALE 52 


//DEFINIZIONE RITORNI CONTROLLO LUCI
int PINRITORNOCAMERA = 22;


//DEFINIZIONE ZONE LUCE

//DEFINIZIONE TIPOLOGIA SENSORE
#define DHTTYPE    DHT11     // DHT 11

// Update these with values suitable for your network.
byte mac[]= { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress server(192, 168, 1, 12);



//INIZIALIZZARE ETHERNET CLIENT
EthernetClient ethClient;
PubSubClient client(ethClient);
/////////////////////////////////////////////////

DHT_Unified dhtcamera(PINCAMERA, DHTTYPE);
DHT_Unified dhtnottebagno(PINBAGNONOTTE, DHTTYPE);
DHT_Unified dhtbagno(PINBAGNO, DHTTYPE);
DHT_Unified dhtpranzo(PINPRANZO, DHTTYPE);
DHT_Unified dhtsalone(PINSALONE, DHTTYPE);
DHT_Unified dhtcucina(PINCUCINA, DHTTYPE);
DHT_Unified dhtesterna(PINESTERNA, DHTTYPE);
DHT_Unified dhtscale(PINSCALE, DHTTYPE);
//DHT_Unified dhtc(PIN_NON_UTILIZZATO, DHTTYPE);

void setup() {
  Serial.begin(9600);

  //INIZIALIZZAZIONE DHT11
  dhtcamera.begin();
  dhtbagno.begin();
  dhtcamera.begin();
  dhtnottebagno.begin();
  sensor_t sensor;

  //INIZIALIZZAZIONE RITORNI LUCE
  pinMode(PINRITORNOCAMERA, INPUT);

  //INIZIALIZZAZIONE MQTT CLIENT
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);

}
uint32_t delayMS;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if(topic ="luce_camera"){
    String str_lux=""; 
      for (int i=0;i<length;i++) {
        str_lux=str_lux+(char)payload[i];
      }
    lux_camera=str_lux.toInt();      
  }
  
  
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  String valore ="";
  char valore_send[5];

  delay(100);
 
  sensors_event_t event;


  if (!client.connected()) {
    reconnect();
  }
  client.loop();
//////////////////////////////////////////////////////
//////////////////BAGNO GIORNO
/////////////////////////////////////////////////////
  dhtbagno.temperature().getEvent(&event);  
   valore=String(event.temperature)+"°C";
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("temp_bagno", valore_send);
          
  dhtbagno.humidity().getEvent(&event);   
   valore=String(event.relative_humidity);
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("um_bagno", valore_send);
//////////////////////////////////////////////////////
//////////////////BAGNO NOTTE
/////////////////////////////////////////////////////
  dhtnottebagno.temperature().getEvent(&event);   
   valore=String(event.temperature)+"°C";
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("notte_temp_bagno", valore_send);
          
  dhtnottebagno.humidity().getEvent(&event);
   valore=String(event.relative_humidity);
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("notte_um_bagno", valore_send);
//////////////////////////////////////////////////////
//////////////////CAMERA
/////////////////////////////////////////////////////
  dhtcamera.temperature().getEvent(&event); 
   valore=String(event.temperature)+"°C";
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("temp_camera", valore_send);
          
  dhtcamera.humidity().getEvent(&event);     
   valore=String(event.relative_humidity);
   //Serial.println(valore);
   valore.toCharArray(valore_send,5);
   client.publish("um_camera", valore_send);



//client.subscribe("temp_camera"); 
//client.subscribe("notte_um_bagno"); 
//client.subscribe("temp_bagno"); 
client.subscribe("luce_camera");
//client.subscribe("notte_um_bagno");
//client.subscribe("notte_temp_bagno");
//client.subscribe("um_bagno"); 

Serial.println(digitalRead(PINRITORNOCAMERA));



/////////////////////////////////////////////////////////
////////////CONTROLLO LUCI
////////////////////////////////////////////////////////

int v = analogRead(A15);
//Serial.println(v);
  
}
