#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 12, 177 };
byte gateway[] = { 192, 168, 12, 1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server = EthernetServer(80);
EthernetClient client;
int error = 0;

void setup() {
  Serial.begin(9600);
  for (int thisPin = 31; thisPin < 47; thisPin++)  {
    pinMode(thisPin, OUTPUT);
    digitalWrite(thisPin, HIGH);
  }
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop() {
  String str;
  client = server.available();
  if (client == true) {
    str = client.readStringUntil('\n');
    ParseRestUrl(str);
    delay(1);
    client.stop();
  }
}

void ParseRestUrl(String url) {
  String first, second, third, fourth;
  if (url.startsWith("GET")) {
    url.replace("GET /", "");
    url.replace(" HTTP/1.1", "");
    url.trim();
    int firstslash = url.indexOf('/');
    int secondslash = url.indexOf('/', firstslash + 1);
    int thirdslash = url.indexOf('/', secondslash + 1);
    if (firstslash == -1) {
      first = url;
    } else {
      first = url.substring(0, firstslash);
      if (secondslash == -1) {
        second = url.substring(firstslash + 1);
      } else {
        second = url.substring(firstslash + 1, secondslash);
        if (thirdslash == -1) {
          third = url.substring(secondslash + 1);
        } else {
          third = url.substring(secondslash + 1, thirdslash);
          fourth = url.substring(thirdslash + 1);
        }
      }
    }

    if (first == "as" && second == "status") {
      AntSwitchStatus();
    } else if (first == "as" && second == "set" && fourth == "on") {
      if (third == "1") { SetAntSwitch(1);
      } else if (third == "2") {
          SetAntSwitch(2);
      } else if (third == "3") {
          SetAntSwitch(3);
      } else if (third == "4") {
          SetAntSwitch(4);
      } else if (third == "5") {
          SetAntSwitch(5);
      } else if (third == "6") {
          SetAntSwitch(6);
      } else if (third == "7") {
          SetAntSwitch(7);
      } else if (third == "8") {
          SetAntSwitch(8);
      }
    } else if (first == "k9ay" && second == "status") {
      StatusK9AY();
    } else if (first == "k9ay" && second == "set") {
      if (third == "dir") {
        if (fourth == "ne") {
          SetK9AY(1);
        } else if (fourth == "sw") {
          SetK9AY(2);
        } else if (fourth == "se") {
          SetK9AY(3);
        } else if (fourth == "nw") {
          SetK9AY(4);
        }
      } else if (third == "preamp") {
        if (fourth == "on") {
          SetK9AY(5);
        } else if (fourth == "off") {
          SetK9AY(6);
        }
      } else if (third == "load") {
        if (fourth == "680") {
          SetK9AY(7);
        } else if (fourth == "572") {
          SetK9AY(8);
        } else if (fourth == "530") {
          SetK9AY(9);
        } else if (fourth == "434") {
          SetK9AY(10);
        }
      }
    } else {
      client.println("HTTP/1.0 404 Not Found");
      client.println("");
     }
  } else {
    client.println("HTTP/1.1 405 Method Not Allowed");
    client.println("Connection: close");
  }
}

void AntSwitchStatus() {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print("ASS ");
  for (int thisPin = 31; thisPin < 39; thisPin++)  {
    client.print(digitalRead(thisPin));
  }
}

void SetAntSwitch(int pin) {
  for (int thisPin = 31; thisPin < 39; thisPin++)  {
    digitalWrite(thisPin, HIGH);
  }
  if (pin != 0) {
    digitalWrite(pin + 30, LOW);
  }
  AntSwitchStatus();
}

void SetK9AY(int pin) {
  StatusK9AY();
}

void StatusK9AY() {
}

