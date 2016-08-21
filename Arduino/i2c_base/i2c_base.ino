// Arduino Bluetooth LE Servo Controlled by iOS

#define i2c_wr_addr 0x64  // 0xC8 write, 0xC9 read, 0x64 address
#define i2c_rd_addr 0x64
//#define regaddr_chipid [0x0,0x8] // Should read 0x03FC
 
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
  int ntx, nrx;
  char regaddr_chipid[2] = {0x8, 0x0};
    // See if new position data is available
//  if (BLE_Shield.available()) {
//    myservo.write(BLE_Shield.read());  // Write position to servo
//  }
  Serial.println("*Beginning Loop");

  /* Write register address */
  Wire.beginTransmission(i2c_wr_addr); // transmit to device #8
  ntx = Wire.write(regaddr_chipid, 2);        // sends five bytes
  Wire.endTransmission();
  Serial.print("Number of Bytes written:");
  Serial.println(ntx);
  
  /* Receive Register Contents */
  Wire.requestFrom(i2c_rd_addr, 2);    // request 1 bytes from slave device #8
//  char c = Wire.read(); // receive a byte as character
//  Serial.println(c, HEX);         // print the character

  nrx = 0;
  while (Wire.available()) { // slave may send less than requested
    Serial.print("Receiving byte number:");
    Serial.println(nrx);
    nrx = nrx + 1;
    int c = Wire.read(); // receive a byte as character
    Serial.println(c, HEX);         // print the character
  }
  Serial.print("Total Number of bytes received: ");
  Serial.println(nrx);
  delay(20000);
  
}
