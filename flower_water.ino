#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DS18B20.h>
DS18B20 ds(13);

#define HOST    "iot.cht.com.tw" // ThingSpeak IP Address: 184.106.153.149
#define PORT    80
const char* mqtt_server = HOST;  // MQTT伺服器位址
const char* mqttUserName = "22784134597";       // 使用者名稱，隨意設定。
const char* mqttPwd = "DKEM09SAZPYM2B23ER";     // MQTT密碼
const char* clientID = "qwe";         // 用戶端ID，隨意設定。
const char* topic = "/v1/device/22784134597/rawdata";
const char* sub_topic = "/v1/device/22784134597/sensor/Motor/csv";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar); // 打印mqtt接收到消息
  }

  Serial.println();
  Serial.println((char)payload[length - 1]);
  char x = (char)payload[length - 1];
  if (x == '1') {
    Serial.println("Watering.");
    off_Motor();
    digitalWrite(12,1);
    delay(2000);
    digitalWrite(12,0);
  }else if(x == '2'){
    Serial.println("update data.");
    Upload_Data();
  }
}

void setup() {
  pinMode(12,1);
  digitalWrite(12,0);
  Serial.begin(74880);
  int time1 = millis();
  if (quick() == -1) {
    Serial.println("connect Error");
  } else {
    int x = millis() - time1;
    Serial.println(String("Useing time:") + x );
  }
  delay(100);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // 等5秒之後再重試
    }
  }
  client.subscribe(sub_topic);
  Upload_Data();
}

void loop() {
  static int count = 0;
  if (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe(sub_topic);
    } else {
      Serial.println("loss connect!");
    }
  }
  client.loop();
  delay(1000);
  count++;
  if(count==30){
    count = 0;
    Upload_Data();
  }
}

void Upload_Data(){
    float temp = ds.getTempC();
    String msgStr = "[{\"id\":\"tmp\", \"value\":[\"" + String(temp) + "\"]}]";
    char json[100];
    // 把String字串轉換成字元陣列格式
    msgStr.toCharArray(json, 100);
    client.publish(topic, json);
    pinMode(14, 1);
    digitalWrite(14, 1);
    delay(10);
    float vol = 100 - analogRead(A0) * 0.1;
    digitalWrite(14, 0);
    msgStr = "[{\"id\":\"hum\", \"value\":[\"" + String(vol) + "\"]}]";
    // 把String字串轉換成字元陣列格式
    msgStr.toCharArray(json, 100);
    client.publish(topic, json);
}
void off_Motor(){
    String msgStr = "[{\"id\":\"Motor\", \"value\":[\"" + String(0) + "\"]}]";
    char json[100];
    // 把String字串轉換成字元陣列格式
    msgStr.toCharArray(json, 100);
    client.publish(topic, json);
}
