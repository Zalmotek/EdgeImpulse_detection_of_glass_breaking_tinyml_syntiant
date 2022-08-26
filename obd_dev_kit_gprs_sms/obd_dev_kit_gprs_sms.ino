#include <SoftwareSerial.h>

SoftwareSerial sim800Serial(10, 11); // RX, TX

void setup() {
  pinMode(A0, INPUT); // here we can read the output of the TinyMl Board
  pinMode(A1, OUTPUT); // here we can light up an LED when the detection occurs for debugging purposes

  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  // set the data rate for the SoftwareSerial port
  sim800Serial.begin(4800);
  sim800Serial.println("Hello, world?");
}

void loop() { // run over and over
  updateSerial();
  if (analogRead(A0) > 400) { //TinyML detected a windsheeld breaking sound
    send_SMS();
    digitalWrite(A1, HIGH);
    delay(10000);
  }
  digitalWrite(A1, LOW);
}


void updateSerial() {
  delay(500);
  while (Serial.available()) {
    sim800Serial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (sim800Serial.available()) {
    Serial.write(sim800Serial.read());//Forward what Software Serial received to Serial Port
  }
}

void test_sim800_module() {
  sim800Serial.println("AT");
  updateSerial();
  Serial.println();
  sim800Serial.println("AT+CSQ");
  updateSerial();
  sim800Serial.println("AT+CCID");
  updateSerial();
  sim800Serial.println("AT+CREG?");
  updateSerial();
  sim800Serial.println("ATI");
  updateSerial();
  sim800Serial.println("AT+CBC");
  updateSerial();
}

void send_SMS() {
  sim800Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800Serial.println("AT+CMGS=\"+XXXXXXXXXXXXX\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800Serial.print("Car breaking in progress!"); //text content
  updateSerial();
  Serial.println();
  Serial.println("Message Sent");
  sim800Serial.write(26);
}
