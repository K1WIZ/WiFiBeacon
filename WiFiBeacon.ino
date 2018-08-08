#include <ESP8266WiFi.h> //more about beacon frames https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/

extern "C" {
  #include "user_interface.h"
}

const int switchPin = 13;   // bring LOW to switch
int pinState = LOW;

void setup() {
  delay(10000);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
}



void loop() {
  pinState = digitalRead(switchPin);
  digitalWrite(LED_BUILTIN, HIGH);
  switch (pinState) {
    case LOW:
      sendFuzzedBeacon("BOMB DETONATOR",10); //sends beacon frames with 10 different SSID all starting with 'BOMB DETONATOR' and ending with whitespaces (spaces and/or tabs)
      break;
    case HIGH:
      PorkRoll();
      break;
    } 
  
  //sendRandomBeacon(10); //sends beacon frames with 10 character long random SSID
 
  digitalWrite(LED_BUILTIN, LOW);   // Flash the LED once after every transmit
  delay(1);  
}

void sendFuzzedBeacon(char* baseSsid, int nr) {
  int baseLen = strlen(baseSsid);
  int i=0;
  for(int j=0; j < 32 - baseLen; j++) { //32 is the maximum length of the SSID
    for(int k=0; k < pow(2,j); k++) {
      int kk = k;
      String ssid = baseSsid;
      for(int l=0; l < j; l++) {
        if(kk%2 == 1) ssid += " "; //add a space
        else ssid += "\t"; //add a tab
        kk /= 2;
      }
      char charBufSsid[33];
      ssid.toCharArray(charBufSsid, 33);
      sendBeacon(charBufSsid);
      if(++i >= nr) return; 
    }
  }
}

void sendRandomBeacon(int len) {
  char ssid[len+1];
  randomString(len, ssid);
  sendBeacon(ssid);
}

void randomString(int len, char* ssid) {
  String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
  for(int i = 0; i < len; i++) {
    ssid[i] = alfa[random(65)];
  }
}

void sendBeacon(char* ssid) {
    // Randomize channel //
    byte channel = random(1,12); 
    wifi_set_channel(channel);

    uint8_t packet[128] = { 0x80, 0x00, //Frame Control 
                        0x00, 0x00, //Duration
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
                /*22*/  0xc0, 0x6c, //Seq-ctl
                //Frame body starts here
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
                /*32*/  0xFF, 0x00, //Beacon interval
                /*34*/  0x01, 0x04, //Capability info
                /* SSID */
                /*36*/  0x00
                };

    int ssidLen = strlen(ssid);
    packet[37] = ssidLen;

    for(int i = 0; i < ssidLen; i++) {
      packet[38+i] = ssid[i];
    }

    uint8_t postSSID[13] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
                        0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };

    for(int i = 0; i < 12; i++) {
      packet[38 + ssidLen + i] = postSSID[i];
    }

    packet[50 + ssidLen] = channel;

    // Randomize SRC MAC
    packet[10] = packet[16] = random(256);
    packet[11] = packet[17] = random(256);
    packet[12] = packet[18] = random(256);
    packet[13] = packet[19] = random(256);
    packet[14] = packet[20] = random(256);
    packet[15] = packet[21] = random(256);

    int packetSize = 51 + ssidLen;

    wifi_send_pkt_freedom(packet, packetSize, 0);
    wifi_send_pkt_freedom(packet, packetSize, 0);
    wifi_send_pkt_freedom(packet, packetSize, 0);
    delay(1);
}

void PorkRoll() {
  sendBeacon("_X FILES");
  sendBeacon("_House of LANister");
  sendBeacon("_GET OFF MY WIFI");
  sendBeacon("_Untrusted Network");
  sendBeacon("_BOMB DETONATOR");     //    COMMENT OR REMOVE THIS ONE IF USING NEAR AIRPORTS!  DON'T BE A DICKHEAD!
  sendBeacon("_DEA UNIT 9");
  sendBeacon("_STARSHIP ENTERPRISE");
  sendBeacon("_DIRTY OLD MAN");
  sendBeacon("_ENCRYPTED FOR YOUR PLEASURE");
  sendBeacon("_HOMELAND SECURITY");
  sendBeacon("_FAST N FREE");
  sendBeacon("_USED CONDOM");
  sendBeacon("_DIRTY DIAPER");
  sendBeacon("_IT'S CRAPTASTIC!");
  sendBeacon("_I FEEL NAUGHTY");
  sendBeacon("_MIND CONTROL");
  sendBeacon("_IT HURTS WHEN I PEE");
  sendBeacon("_IM UNDER YOUR BED");
  sendBeacon("_FBI SURVEILLANCE VAN 69");
  sendBeacon("_IM WATCHING YOU NOW");
  sendBeacon("_SKYNET GLOBAL DEFENCE NETWORK");
  sendBeacon("_PLEASE CLICK FOR ID THEFT");
  sendBeacon("_VIRUSES R US");
  sendBeacon("_MAKE AMERICA GREAT AGAIN");
  }
