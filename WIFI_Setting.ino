// ===== hOST setup =====
const char* host = "iot.cht.com.tw";  // 連線的主機域名

String API_KEY = "PKFUSKSXW09F774TGM";  // iot的 api key
String deviceId = "22023668135";    // iot的 裝置ID
String url = "/iot/v1/device/" + deviceId + "/rawdata"; // 請求的頁面地址

void Init_STA_Mode() {
  //=========連接WIFI===========
  //WiFi.forceSleepWake();
  WiFi.begin("WTF");
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  //monitorWiFi();
  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());//顯示IP位址
  delay(10);
}
/**
   上傳資料到onenet
*/
void postDataToOnenet(float soil_hum, float soil_tmp, float temp, float hum, float brightness, int rssi) {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClient client;          // HTTP
  //  WiFiClientSecure client;    // HTTPS
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, 80)) {   // 判斷連線情況
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(url);

  // 傳送POST請求
  // 組拼url地址
  // 組拼HTTPS請求的Header
  String getStr = String("POST ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: ESP32\r\n" +
                  "CK:" + API_KEY + "\r\n" +
                  "Connection: close\r\n";
  String jsonStr = String("") + "[{" +
                   "\"id\":\"soil_hum\"," +
                   "\"value\":[\"" + soil_hum +
                   "\"]},{" +
                   "\"id\":\"soil_tmp\"," +
                   "\"value\":[\"" + soil_tmp +
                   "\"]},{" +
                   "\"id\":\"temp\"," +
                   "\"value\":[\"" + temp +
                   "\"]},{" +
                   "\"id\":\"humidity\"," +
                   "\"value\":[\"" + hum +
                   "\"]},{" +
                   "\"id\":\"brightness\"," +
                   "\"value\":[\"" + brightness +
                   "\"]},{" +
                   "\"id\":\"RSSI\"," +
                   "\"value\":[\"" + rssi +
                   "\"]}]";


  client.print(getStr);   // 傳送Headers頭
  client.print(String("") + "Content-Length:" + jsonStr.length() + "\r\n\r\n"); // 傳送Header頭-資料內容長度(注意:\r\n\r\n是結尾)
  client.print(jsonStr);  // 傳送json資料

  Serial.println("request sent");

  Serial.println("==========");
  Serial.println("send was:");
  Serial.println(jsonStr);   // 打印發送的請求資料
  String line = client.readStringUntil('\n');
  Serial.println("reply was:");
  Serial.println(line);   // 列印接受到的資料
  Serial.println("==========");
}
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
void monitorWiFi()
{
  //wifiMulti.addAP("Taco_GreenHouse_Ext", "AndreJulia");
  //wifiMulti.addAP("Taco_GreenHouse", "AndreJulia");
  wifiMulti.addAP("handsome_LIN", "20922092");
  //wifiMulti.addAP("WTF", "qwery1250");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
      Serial.print("Looking for WiFi ");
    }
    Serial.print(".");
    delay(500);
  }
  if (connectioWasAlive == false)
  {
    connectioWasAlive = true;
    Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
  }
}
void wifi_sleep() {
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

}
