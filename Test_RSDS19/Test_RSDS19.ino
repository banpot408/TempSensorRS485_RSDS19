#include <SoftwareSerial.h>
#include <ModbusMaster.h>

SoftwareSerial mySerial(2, 3); // RX, TX

#define MAX485_DE      4
#define MAX485_RE_NEG  5

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.println("start init serial 0");
  Serial.begin(9600);
  
  while (!Serial) {
    Serial.println("loop for init serial 0"); // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("start init software serial");
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial"); // wait for serial port to connect. Needed for Native USB only
  }
  
  // Modbus slave ID 1
  node.begin(1, mySerial);
  
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop()
{
  uint8_t result;
  uint16_t data[2]; // prepare variable of storage data from sensor
   
  Serial.println("get data");
  result = node.readHoldingRegisters(0, 1); // Read 2 registers starting at 1)
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Temp: ");
    Serial.print(node.getResponseBuffer(0)/10.0f);
    Serial.print("     ");
//    int data_int = -120.5/10;
    uint16_t data_int = node.getResponseBuffer(0)/10;
    Serial.print("Temp INT: ");
    Serial.println(data_int);

  }
  delay(1000);
}
