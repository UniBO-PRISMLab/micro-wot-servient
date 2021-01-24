
#include <ArduinoJson.h>
#include "Arduino.h"
#include <WiFi.h>
#include <embeddedWoT_HTTP_LongPoll.h>
#include <embeddedWoT_WebSocket.h>
#include <coap-simple.h>
#include <WebSocketsClient.h>

const char* ssid = "Socc'mel Busàn";
const char* password = "chellol96";
String protocolServer = "http";
int portServer = 80;
String urlServer = "";
String protocolSocket = "ws";
int portSocket = 81;
String urlSocket = "";

String thingName = "semaphore-ws-coap";
String td = "";


DynamicJsonDocument es_doc(20);
// Json Array to store the ip addresses of clients connected to WebSocket channel for Events requests   
JsonArray ipe_arr;
DeserializationError err;

int properties_number = 1;
int objectProperties_number = 0;
int actions_number = 1;
int events_number = 1;

// Properties
const char* property0_name = "semaphore";
bool property0_value = false;

// Actions
const char* action1_name = "statusChanged";
int action1_inputsNumber = 0;
String action1_schema[0] = {};

// Events
const char* event1_name = "hasParkChanged";
bool events_subscriptionSchema[1] = {false};
bool events_dataSchema[1] = {false};
bool events_cancellationSchema[1] = {false};

// Endpoints
String req6 = "/" + thingName + "/events/" + event1_name;
String req5 = "/" + thingName + "/actions/" + action1_name;
String req4 = "/" + thingName + "/properties/" + property0_name;
String req3 = "/" + thingName + "/all/properties";
String req2 = "/" + thingName;
String req1 = "/";

IPAddress ipS;
//Longpoll object handler
embeddedWoT_HTTP_LongPoll *hlp;
//WebSocket object handler
embeddedWoT_WebSocket *wsb;

const int GREENLED = 12;
const int REDLED = 33;
const char* HTTPUrl = "http://192.168.1.156/bike-rack/events/hasParkChanged";
WiFiUDP Udp;
Coap coap(Udp);
WebSocketsClient webSocket;
bool tot_ws = true;
bool tot_coap = true;
int i, j, k, n;

String request1();
String request2();
String request3();
String request4();
String request5(String body);

//HTTP - actions
const int http_actions_num = 1;
const String http_actions_endpoint[http_actions_num] = { req5 };
actions_handler http_actions_callback[http_actions_num] = { request5 };

//WS - actions
const int ws_actions_num = 0;
const String ws_actions_endpoint[ws_actions_num] = {  };
actions_handler ws_actions_callback[ws_actions_num] = {  };

//CoAP - actions
const int coap_actions_num = 0;
const String coap_actions_endpoint[coap_actions_num] = {  };
actions_handler coap_actions_callback[coap_actions_num] = {  };

//HTTP - Properties
const int http_properties_num = 4;
const String http_properties_endpoint[http_properties_num] = { req1, req2, req3, req4 };
properties_handler http_properties_callback[http_properties_num] = { request1, request2, request3, request4 };

//WS - Properties
const int ws_properties_num = 4;
const String ws_properties_endpoint[ws_properties_num] = { req1, req2, req3, req4 };
properties_handler ws_properties_callback[ws_properties_num] = { request1, request2, request3, request4 };

//CoAP - Properties
const int coap_properties_num = 0;
const String coap_properties_endpoint[coap_properties_num] = {  };
properties_handler coap_properties_callback[coap_properties_num] = {  };

//HTTP - events
const int http_events_num = 0;
const String http_events_endpoint[http_events_num] = {  };

//WS - events
const int ws_events_num = 1;
const String ws_events_endpoint[ws_events_num] = { req6 };

//CoAP - events
const int coap_events_num = 0;
const String coap_events_endpoint[coap_events_num] = {  };

void setup() {
    Serial.begin(115200);
    Serial.println();

    // events data
  
    connection(ssid, password);
    
    td = "{\"title\":\"semaphore-ws-coap\",\"id\":\"semaphore-coap\",\"@context\":[\"https://www.w3.org/2019/wot/td/v1\"],\"security\":\"nosec_sc\",\"securityDefinitions\":{\"nosec_sc\":{\"scheme\":\"nosec\"}},\"forms\":[{\"contentType\":\"application/json\",\"href\":\""+urlServer+"/all/properties\",\"op\":[\"readallproperties\",\"readmultipleproperties\"]},{\"contentType\":\"application/json\",\"href\":\""+urlSocket+"/all/properties\",\"op\":[\"readallproperties\",\"readmultipleproperties\"]}],\"links\":[],\"properties\":{\"semaphore\":{\"forms\":[{\"contentType\":\"application/json\",\"href\":\""+urlServer+"/properties/"+property0_name+"\",\"op\":[\"readproperty\"]},{\"contentType\":\"application/json\",\"href\":\""+urlSocket+"/properties/"+property0_name+"\",\"op\":[\"readproperty\"]}],\"type\":\"boolean\",\"items\":{\"type\":\"boolean\"},\"observable\":true,\"readOnly\":true,\"writeOnly\":true}},\"actions\":{\"statusChanged\":{\"forms\":[{\"contentType\":\"application/json\",\"href\":\""+urlServer+"/actions/"+action1_name+"\",\"op\":\"invokeaction\"}],\"safe\":true,\"idempotent\":false}},\"events\":{\"hasParkChanged\":{\"eventName\":\"hasParkChanged\",\"forms\":[{\"contentType\":\"application/json\",\"href\":\""+urlSocket+"/events/"+event1_name+"\",\"op\":[\"subscribeevent\"]}],\"actionsTriggered\":[\"statusChanged\"],\"condition\":\"true\"}}}";

    hlp = new embeddedWoT_HTTP_LongPoll(portServer);

    hlp->exposeActions(http_actions_endpoint, http_actions_callback, http_actions_num);
    hlp->exposeEvents(http_events_endpoint, http_events_num);
    hlp->exposeProperties(http_properties_endpoint, http_properties_callback, http_properties_num);

    hlp->begin();
    wsb = new embeddedWoT_WebSocket(portSocket);

    wsb->bindEventSchema(es_doc);
    wsb->exposeActions(ws_actions_endpoint, ws_actions_callback, ws_actions_num);
    wsb->exposeEvents(ws_events_endpoint, ws_events_num);
    wsb->exposeProperties(ws_properties_endpoint, ws_properties_callback, ws_properties_num);
    Serial.println("Server started");
    Serial.println(urlServer);
    Serial.println(urlSocket);

    // property0_value[0] = false;
// property0_value[1] = false;
// property0_value[2] = false;
// This statement will declare pin 22 as digital output 
pinMode(GREENLED, OUTPUT);
pinMode(REDLED, OUTPUT);
// This statement will declare pin 15 as digital input 
// pinMode(SENSOR0, INPUT);
// pinMode(SENSOR1, INPUT);

// server address, port and URL
// 	webSocket.begin("192.168.1.106", 81, "/bike-rack/events/hasParkChanged");

// 	// event handler
// 	webSocket.onEvent(webSocketEvent);

// 	// use HTTP Basic Authorization this is optional remove if not needed
// //	webSocket.setAuthorization("user", "Password");

// 	// try ever 5000 again if connection has failed
webSocket.begin("192.168.1.158", 81, "/bike-rack-ws/events/hasParkChanged");

// event handler
webSocket.onEvent(webSocketEvent);

// use HTTP Basic Authorization this is optional remove if not needed
//	webSocket.setAuthorization("user", "Password");

// try ever 5000 again if connection has failed
webSocket.setReconnectInterval(5000);

coap.response(callback_response);
coap.start();

delay(4000);
// Serial.println("Eh io l'ho mandato...");
int msgid = coap.get(IPAddress(192, 168, 1, 157), 5683, "bike-rack-coap/events/hasParkChanged");

// Serial.println(msgid);

}    

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  DynamicJsonDocument doc(600);
  // http.end();
  // Serial.println("Speriamo2");
  Serial.println(p);
  deserializeJson(doc, p);
  // extract the values
  JsonObject obj = doc.as<JsonObject>();
  JsonArray array = obj["parks"];
  bool tot_coap = true;
  for(JsonVariant v : array) {
    // Serial.println("Speriamo3-4");
    bool vas = v.as<bool>();
    tot_coap = tot_coap && vas;
  }
  Serial.printf("tot_coap is %s\n", tot_coap ? "true" : "false");
  // Serial.println("Speriamo5");
  if (tot_coap && tot_ws) {
    Serial.println("Rosso");
    digitalWrite(GREENLED, LOW);
    digitalWrite(REDLED, HIGH);   
  } else {
    Serial.println("Verde");
    digitalWrite(GREENLED, HIGH);
    digitalWrite(REDLED, LOW);
  }
  // Serial.println("Speriamo6");
  doc.clear();
}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  DynamicJsonDocument doc(600);
  JsonObject obj;
  JsonArray array;
  JsonVariant v;
  DeserializationError err;
  bool vas;
  
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
     case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("{}");
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      err = deserializeJson(doc, payload);
      // extract the values
      if (!err) {
        obj = doc.as<JsonObject>();
        array = obj["parks"];
        tot_ws = true;
        for(v : array) {
          vas = v.as<bool>();
          tot_ws = tot_ws && vas;
        }
      } else tot_ws = false;
      Serial.printf("tot_ws is %s\n", tot_ws ? "true" : "false");
      if (tot_ws && tot_coap) {
        Serial.println("Rosso");
        digitalWrite(GREENLED, LOW);
        digitalWrite(REDLED, HIGH);   
      } else {
        Serial.println("Verde");
        digitalWrite(GREENLED, HIGH);
        digitalWrite(REDLED, LOW);
      }

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }
  }

void loop() {
    
delay(40);


coap.loop();
webSocket.loop();


// Serial.println("Speriamo0");
// HTTPClient http;
// Serial.println("Speriamo0.1");

// http.begin(HTTPUrl);
// Serial.println("Speriamo0.2");

// int httpResponseCode = http.GET();
// Serial.println("Speriamo1");
// if (httpResponseCode>0) {
//   String payload = http.getString();
  
//   DynamicJsonDocument doc(600);
//   // http.end();
//   Serial.println("Speriamo2");
//   Serial.println(payload);
//   deserializeJson(doc, payload);
//   // extract the values
//   JsonObject obj = doc.as<JsonObject>();
//   JsonArray array = obj["parks"];
//   bool tot = true;
//   for(JsonVariant v : array) {
//     Serial.println("Speriamo3-4");
//     bool vas = v.as<bool>();
//     tot = tot && vas;
//   }
//   Serial.println("Speriamo5");
//   if (tot) {
//     Serial.println("Rosso");
//     digitalWrite(GREENLED, LOW);
//     digitalWrite(REDLED, HIGH);   
//   } else {
//     Serial.println("Verde");
//     digitalWrite(GREENLED, HIGH);
//     digitalWrite(REDLED, LOW);
//   }
//   Serial.println("Speriamo6");
//   doc.clear();
// }
// Serial.println("Speriamo7");
// // http.end();
// Serial.println("Speriamo8");
    // handle Requests via WebSocket
    wsb->loop();
}

void connection(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    
    Serial.print("\nConnecting to ");
    Serial.print(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        WiFi.begin(ssid, password);
    }

    Serial.println("\nConnected");
    Serial.print("IP address: ");
    ipS = WiFi.localIP();
    Serial.println(ipS);

    urlServer = protocolServer + "://" + ipS.toString() + ":" + portServer + "/" + thingName;
    urlSocket = protocolSocket + "://" + ipS.toString() + ":" + portSocket + "/" + thingName;
}

// Request functions
String request1() {
    String resp = "";

    Serial.println("\nGET Thing URL");
    resp = "[\"" + urlServer + "\"]";
    return resp;
}

String request2() {
    Serial.println("\nGET Thing Description"); 
    return td;
}

String request3() {
    DynamicJsonDocument tmp(2220);
    String resp = "";
    JsonObject obj = tmp.createNestedObject();

    Serial.println("\nGET all properties");
    obj[property0_name] = property0_value;
    serializeJson(obj, resp);

    return resp;
}

String request4() {
    String resp = "";

    Serial.printf("\nGET %s value\n", property0_name);
    resp = "{\"" + (String) property0_name + "\":" + property0_value + "}";
    
    return resp;
}

String request5(String body) {
    DynamicJsonDocument resp_doc(20);
    String resp = "";

    Serial.printf("\nPOST invokeaction %s\n", action1_name);
    Serial.printf("Body received: %s\n", body.c_str());
    
    err = deserializeJson(resp_doc, body);
    if(err) {
        Serial.printf("deserializeJson() failed with code %s", err.c_str());
        resp = err.c_str();
        return resp;
    }
    else {
        statusChanged(); 
        resp = "";
        // hasParkChanged condition
        String ws_msg = "";
         if(true) {
            wsb->sendWebSocketTXT(ws_msg, ws_events_endpoint[0]);
            
        }
    }
    return resp;
}

// handle Input Types
bool handleInputType(String value, String schema) {   
	DynamicJsonDocument schema_doc(20);
    bool validInput = true;

    deserializeJson(schema_doc, schema);
    JsonObject obj_schema = schema_doc.as<JsonObject>();
    String type = obj_schema["type"];
    
    if(value[0] == '"')
        value.remove(0);
    if(value[value.length()-1] == '"')    
        value.remove(value.length()-1);
    
    return validInput;
}

void emitEvent(String txt, String event_endpoint) {
    String endpoint = "/" + thingName + "/events/" + event_endpoint;
    hlp->sendLongPollTXT(txt, endpoint);
    wsb->sendWebSocketTXT(txt, endpoint);
}

// Action functions
void statusChanged() {
	return;
	
}

