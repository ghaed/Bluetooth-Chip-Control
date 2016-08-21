// Arduino Bluetooth LE Servo Controlled by iOS

#define i2cAddr 0x64  // 0xC8 write, 0xC9 read, 0x64 address
#define regAddrChipId 0x08 // Should read 0x03FC
#define regAddrAdcCtrl 0x10 // Example read/write register
 
#include <SoftwareSerial.h>
#include <Wire.h>

 
int LED = 13;     // Most Arduino boards have an onboard LED on pin 13
//SoftwareSerial BLE_Shield(4,5);
 
void setup()  // Called only once per startup
{ 
  Serial.begin(9600);
  pinMode(LED, OUTPUT);     // Set pin as an output
  digitalWrite(LED, HIGH);  // Turn on LED (ie set to HIGH voltage)
 
//  BLE_Shield.begin(9600);   // Setup the serial port at 9600 bps. This is the BLE Shield default baud rate.

  Wire.begin(); // join i2c bus (address optional for master)
}

 
void loop() // Continuous loop
{
  int val;
    // See if new position data is available
//  if (BLE_Shield.available()) {
//    myservo.write(BLE_Shield.read());  // Write position to servo
//  }
  Serial.println("*Beginning Loop");

  /* Read Chip ID */
  val = regRead(regAddrChipId) ;
  Serial.print("Chip ID: ");
  Serial.println(val, HEX);

  /* Read ADC Ctrl */
  val = regRead(regAddrAdcCtrl) ;
  Serial.print("ADC CTRL: ");
  Serial.println(val, HEX);

  /* Write ADC Ctrl */
  int wrval = 0x1234;
  int nwr = 0;
  nwr = regWrite(regAddrAdcCtrl, wrval);
  Serial.print("Wrote this number of bytes in ADCCTRl overwrite: ");
  Serial.println(nwr, HEX);

  /* Read ADC Ctrl */
  val = regRead(regAddrAdcCtrl) ;
  Serial.print("ADC CTRL: ");
  Serial.println(val, HEX);
  delay(2000);
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


