#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "html_tool.h"
#include <FS.h>

#ifndef APSSID
#define APSSID "xsjesp"
#define APPSK "thereisnospoon"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);
SoftwareSerial espSerial(4, 5); //RX,TX
String LastMSG = "";
HtmlTool htool;

String getContentType(String filename)
{
  if (server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path)
{
  if (server.hasArg("MESSAGE"))
  {
    String recv = server.arg("MESSAGE");
    Serial.println(recv);
    espSerial.print(server.arg(recv));
    LastMSG = recv;
    server.sendHeader("Location", "/");
    server.send(301);
    return true;
  }
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz"; // 路徑結尾加上".gz"

  // 確認請求路徑或者.gz結尾的資源存在
  if (SPIFFS.exists(path) || SPIFFS.exists(pathWithGz))
  {
    if (SPIFFS.exists(pathWithGz))
    {
      path += ".gz";
    }

    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();

    return true;
  }
  return false;
}

//根目录显示文件
void handleRoot()
{
  Serial.println("Enter handleRoot");
  String header;

  if (server.hasArg("MESSAGE"))
  {
    Serial.println(server.arg("MESSAGE"));
    espSerial.print(server.arg("MESSAGE"));
    server.sendHeader("Location", "/");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  File homepage = SPIFFS.open("index.html", "r");
  size_t sent = server.streamFile(homepage, "text/html");
  Serial.println(homepage);
  homepage.close();
  Serial.println("send ok");
}

void getMessage()
{
}

//no need authentication
void handleNotFound()
{
  if (!handleFileRead(server.uri()))
  {
    server.send(404, "text/plain", "FileNotFound");
  }
}

void handleGetData()
{
    server.send(200,"text/plain",LastMSG);
}
void setup(void)
{
  Serial.begin(115200);
  espSerial.begin(115200);
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("Web server started");


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/get",handleGetData);
  server.onNotFound(handleNotFound);
  const char *headerkeys[] = {"User-Agent", "Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{

  server.handleClient();

}
