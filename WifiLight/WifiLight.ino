//==============================================================================
//
//  Copyright (C) 2018 Víctor Navarro <victormna.developer@gmail.com>
//
//  This file is part of WifiLight.
//
//  WifiLight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  WifiLight is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with WifiLight.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ArduinoOTA.h>


//==============================================================================
#include "WifiLightConfig.h"

const int PwmPin = LIGHT_PIN;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// config static IP
const IPAddress ip(IP);
const IPAddress gateway(GATEWAY);
const IPAddress subnet(SUBNET);

const int port = SERVER_PORT;

const char *ArduinoOtaPassword = ARDUINOOTA_PASSWORD;

const char *nodeName = NODENAME;

#define HTTP_OK 200


//==============================================================================
MDNSResponder mdns;
ESP8266WebServer server(port);
String webPage;
int Light_Level;


//==============================================================================
void CreateWebpage(void)
{
  webPage  = "<!DOCTYPE html><html><head>";
  webPage += "<!-- WifiLight v0.1 -->";
  webPage += "<style>";
  
  webPage += ".button{";
  webPage += "background-color:#503070;";
  webPage += "border:none;";
  webPage += "color:white;";
  webPage += "text-align:center;";
  webPage += "font-size:100px;";
  webPage += "width:90%;";
  webPage += "height:18vh;";
  webPage += "display:block;";
  webPage += "}";

  webPage += ".buttonOn{";
  webPage += "background-color:#50E030;";
  webPage += "border:none;";
  webPage += "color:black;";
  webPage += "text-align:center;";
  webPage += "font-size:120px;";
  webPage += "width:90%;";
  webPage += "height:18vh;";
  webPage += "display:block;";
  webPage += "}";  
  
  webPage += "</style>";
  webPage += "</head><body><center>";

  webPage += "<a href=\"?level=0\"><button class=\"button";
  if(Light_Level == 0)
  {
    webPage += "On";
  }  
  webPage += "\">OFF</button></a>";
  
  webPage += "<a href=\"?level=256\"><button class=\"button";
  if(Light_Level == 256)
  {
    webPage += "On";
  }    
  webPage += "\">25%</button></a>";
  
  webPage += "<a href=\"?level=512\"><button class=\"button";
  if(Light_Level == 512)
  {
    webPage += "On";
  }    
  webPage += "\">50%</button></a>";
  
  webPage += "<a href=\"?level=768\"><button class=\"button";
  if(Light_Level == 768)
  {
    webPage += "On";
  }    
  webPage += "\">75%</button></a>";
  
  webPage += "<a href=\"?level=1024\"><button class=\"button";
  if(Light_Level == 1024)
  {
    webPage += "On";
  }   
  webPage += "\">ON</button></a>";

  webPage += "</center></body></html>";   
}


//==============================================================================
void setPwmLight(int value)
{
  Light_Level = value;
  analogWrite(PwmPin, Light_Level);
}


//==============================================================================
void proccessWebRequest(void)
{
    if(server.arg("level") != "")
    {
      setPwmLight(server.arg("level").toInt());
    }
    CreateWebpage();
    server.send(HTTP_OK, "text/html", webPage);
    delay(100);  
}


//==============================================================================
void setup() 
{
  setPwmLight(0);
 
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(5000);
    ESP.restart();
  }

  // No authentication by default
  ArduinoOTA.setPassword(ArduinoOtaPassword);
  ArduinoOTA.begin();
  
  mdns.begin(nodeName, WiFi.localIP());
 
  server.on("/", proccessWebRequest);
  server.begin();
}


//==============================================================================
void loop()
{
  ArduinoOTA.handle();

  server.handleClient();

  delay(100);  
}
