// Arduino Bluetooth LE Servo Controlled by iOS

#define i2cAddr 0x64  // 0xC8 write, 0xC9 read, 0x64 address
#define regAddrChipId 0x08 // Should read 0x03FC
 
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

// ******
// Takes a 16-bit register address and a 16-bit register value and writes the address to value
int regRead(int addr) {
  /* Write register address */
  char addrAry[2] = {0x8, 0x0};
  int ntx;  // Number of transmitted bytes
  int nrx;  // Number of received bytes
  int regVals[2]; // The two bytes of read out data
  
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
    Serial.print("Receiving byte number:");
    Serial.println(nrx);
    nrx = nrx + 1;
    int c = Wire.read(); // receive a byte as character
    if (nrx < 3) {
      regVals[nrx-1] = c;
    }
    Serial.println(c, HEX);         // print the character
  }
  Serial.print("Total Number of bytes received: ");
  Serial.println(nrx);

  if (nrx < 2) {
    Serial.println("*** Error: regRead - could not read register value" );
    return -1;
  } else {
    return regVals[1] | (regVals[0] << 8);
  }
}
 
void loop() // Continuous loop
{
    // See if new position data is available
//  if (BLE_Shield.available()) {
//    myservo.write(BLE_Shield.read());  // Write position to servo
//  }
  Serial.println("*Beginning Loop");

  /* Read Chip ID */
  int val = regRead(regAddrChipId) ;
  Serial.print("Read out value: ");
  Serial.println(val, HEX);
  
  delay(20000);
  
}
