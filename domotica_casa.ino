
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


//DEFINIZIONE ZONE E PIN
#define PINCAMERA 46 
#define PINBAGNO 47
#define PINPRANZO 48 
#define PINSALONE 49 
#define PINCUCINA 50 
#define PINESTERNA 51 
#define PINSCALE 52 
//#define PIN_NON_UTILIZZATO 53 

//DEFINIZIONE TIPOLOGIA SENSORE
#define DHTTYPE    DHT11     // DHT 11

// Update these with values suitable for your network.
byte mac[]= { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress server(192, 168, 1, 11);




//INIZIALIZZARE ETHERNET CLIENT
EthernetClient ethClient;
PubSubClient client(ethClient);
/////////////////////////////////////////////////



DHT_Unified dhtcamera(PINCAMERA, DHTTYPE);
DHT_Unified dhtbagno(PINBAGNO, DHTTYPE);
DHT_Unified dhtpranzo(PINPRANZO, DHTTYPE);
DHT_Unified dhtsalone(PINSALONE, DHTTYPE);
DHT_Unified dhtcucina(PINCUCINA, DHTTYPE);
DHT_Unified dhtesterna(PINESTERNA, DHTTYPE);
DHT_Unified dhtscale(PINSCALE, DHTTYPE);
//DHT_Unified dhtc(PIN_NON_UTILIZZATO, DHTTYPE);

uint32_t delayMS;



void setup() {
  Serial.begin(9600);
  dhtcamera.begin();
  dhtbagno.begin();
  sensor_t sensor;
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);

}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
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
  
  // Delay between measurements.
  delay(1000);
  // Get temperature event and print its value.
  sensors_event_t event;
  dhtcamera.temperature().getEvent(&event);
  Serial.println("******* CAMERA ********* ");
  Serial.println(event.temperature);
  
  dhtcamera.humidity().getEvent(&event);
  Serial.println(event.relative_humidity);
  Serial.println("--------------------------");


  

  dhtbagno.temperature().getEvent(&event);
  Serial.println("******* BAGNO ********* ");
  Serial.println(event.temperature);
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.subscribe("temp_bagno");
    
    //float tempc1;
   // var char buffero;
    
   // String temperatura (floatToString(buffero, tempc1, 1));
   valore=String(event.temperature)+"°C";
   Serial.println(valore);
   valore.toCharArray(valore_send,5);
   if(client.publish("temp_bagno", valore_send))
        
  {
    Serial.println("Publish message success");
    Serial.print(valore);
  }
  else
  {
    Serial.println("Could not send message :(");
  }

 
  // Dont overload the server!
  delay(4000);

  

  
  dhtbagno.humidity().getEvent(&event);
  Serial.println(event.relative_humidity);
  Serial.println("--------------------------");

  client.subscribe("um_bagno");
    
    //float tempc1;
   // var char buffero;
    
   // String temperatura (floatToString(buffero, tempc1, 1));
   valore=String(event.relative_humidity);
   Serial.println(valore);
   valore.toCharArray(valore_send,5);
   if(client.publish("um_bagno", valore_send))
        
  {
    Serial.println("Publish message success");
    Serial.print(valore);
  }
  else
  {
    Serial.println("Could not send message :(");
  }
 client.disconnect();













 }
  
