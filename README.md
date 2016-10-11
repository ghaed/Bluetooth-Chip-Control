#Bluetooth Chip Control 
A complete implementation of a way to program/control any integrated circuit on an iPhone
The solution includes an iphone app for the front-end and and Arduino microcontroller project for the back-end
##XCode
An iPhone App implemented in Xcode to communicate to a Bluetooth-LE module. The app makes use of core bluetooth API to send register read/write packets to the bluetooth module on an Arduino board. The packets consist of a command, address, and data. Address is assumed to be 16-bit and data is 8-bit.
##Arduino
The arduino project to receive packets from Bluetooth-LE module and relay them over to any IC via the I2C serial bus.
