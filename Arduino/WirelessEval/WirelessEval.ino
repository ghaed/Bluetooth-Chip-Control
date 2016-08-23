// Arduino Bluetooth LE Evaluation via iOS for 
// ADI Parts. 
// Uses I2C ports of ADI parts to program the parts as desired
// Packets come from iOS device via BTLE in the following format:
// pkt[7:0]: 1 byte: command ( 0x01 for Write and 0x02 for read)
// pkt[24:8]: 2 bytes: address
// pkt[39:25]: 2 bytes: data (for write and read)  

#define i2cAddr 0x64  // 0xC8 write, 0xC9 read, 0x64 address
#define regAddrChipId 0x08 // Should read 0x03FC
#define regAddrAdcCtrl 0x10 // Example read/write register
typedef enum {Idle, CommandReceived, AddressFirstByteReceived, AddressSecondByteReceived, DataFirstByteReceived} StatusType;
StatusType currentStatus = Idle;

 
#include <SoftwareSerial.h>
#include <Wire.h>

 
int LED = 13;     // Most Arduino boards have an onboard LED on pin 13
SoftwareSerial BLE_Shield(4,5); // Software serial on pins Tx=4 and Rx=5 to interface BTLE board
 
void setup()  // Called only once per startup
{ 
  Serial.begin(9600);
  pinMode(LED, OUTPUT);     // Set pin as an output
  digitalWrite(LED, HIGH);  // Turn on LED (ie set to HIGH voltage)
 
  BLE_Shield.begin(9600);   // Setup the serial port at 9600 bps. This is the BLE Shield default baud rate.

  Wire.begin(); // join i2c bus (address optional for master)
}

 
void loop() // Continuous loop
{
  
  int val;
  int addr;
  int data;
  int cmd;
  //   See if new data is available. Receive all the 5 bytes in a packet
  if (BLE_Shield.available()) {
    printStatus();
    switch(currentStatus) {
      case Idle:
        currentStatus = CommandReceived;
        break;
      case CommandReceived:
        currentStatus = AddressFirstByteReceived;
        break;
      case AddressFirstByteReceived:
        currentStatus = AddressSecondByteReceived;
        break;
      case AddressSecondByteReceived:
        currentStatus = DataFirstByteReceived;
        break;
      case DataFirstByteReceived: 
        currentStatus = Idle;
        break;
      default:
        currentStatus = Idle;
    }
//    myservo.write(BLE_Shield.read());  // Write position to servo
  }
  
//  Serial.println("*Beginning Loop");

//  /* Read Chip ID */
//  val = regRead(regAddrChipId) ;
//  Serial.print("Chip ID: ");
//  Serial.println(val, HEX);
//
//  /* Read ADC Ctrl */
//  val = regRead(regAddrAdcCtrl) ;
//  Serial.print("ADC CTRL: ");
//  Serial.println(val, HEX);
//
//  /* Write ADC Ctrl */
//  int wrval = 0x1234;
//  int nwr = 0;
//  nwr = regWrite(regAddrAdcCtrl, wrval);
//  Serial.print("Wrote this number of bytes in ADCCTRl overwrite: ");
//  Serial.println(nwr, HEX);
//
//  /* Read ADC Ctrl */
//  val = regRead(regAddrAdcCtrl) ;
//  Serial.print("ADC CTRL: ");
//  Serial.println(val, HEX);
//  delay(2000);
}



// ************************************************************
// Takes a 16-bit register address and a 16-bit register value 
// and writes the address to value
int regRead(int addr) {
  /* Write register address */
  byte addrAry[2];
  byte ntx;  // Number of transmitted bytes
  byte nrx;  // Number of received bytes
  byte regVals[2]; // The two bytes of read out data

  addrAry[0] = (addr & 0x00FF);
  addrAry[1] = (addr >> 8) & 0x00FF; 

  Wire.beginTransmission(i2cAddr); // transmit to device via I2C
  ntx = Wire.write(addrAry, 2);        // sends two bytes
  Wire.endTransmission();
  if (ntx != 2) {
    Serial.println("*** Error: regRead - could not write register address" );
    return -1;
  }

  /* Receive Register Contents */
  Wire.requestFrom(i2cAddr, 2);    // request 2 bytes from slave device #8

  nrx = 0;
  while (Wire.available()) { // slave may send less than requested
    nrx = nrx + 1;
    int c = Wire.read(); // receive a byte as character
    if (nrx < 3) {
      regVals[nrx-1] = c;
    }
  }
  
  if (nrx < 2) {
    Serial.println("*** Error: regRead - could not read register value" );
    return -1;
  } else {
    return regVals[1] | (regVals[0] << 8);
  }
}


// ************************************************************
// Takes a 16-bit register address and a 16-bit register value 
// and writes the address to value
int regWrite(int addr, int value) {
  /* Write register address */
  byte ntx;  // Number of transmitted bytes
  byte nrx;  // Number of received bytes
  byte addrAry[2];  // The two bytes of address to be accessed
  byte valAry[2]; // The two bytes of data to be written

  addrAry[0] = (addr & 0x00FF);
  addrAry[1] = (addr >> 8) & 0x00FF; 
  valAry[0] = (value & 0x00FF);
  valAry[1] = (value >> 8) & 0x00FF; 
    
  Wire.beginTransmission(i2cAddr); // transmit to device via I2C
  ntx = Wire.write(addrAry[0]);        // sends two bytes
  ntx += Wire.write(valAry[1]);        // sends two bytes
  ntx += Wire.write(valAry[0]);        // sends two bytes
  Wire.endTransmission();
  



  
  if (ntx != 3) {
    Serial.println("*** Error: regWrite - could not write register values" );
    return -1;
  }
  return ntx;
}

void printStatus() {
  Serial.println(currentStatus);
}

