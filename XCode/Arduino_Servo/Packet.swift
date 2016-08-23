//
//  Packet.swift
//  Arduino_Servo
//
//  Created by Saeedeh Salimian on 8/23/16.
//  Copyright © 2016 Razeware LLC. All rights reserved.
//

import Foundation


// Packet Model: 
// Packets come from iOS device via BTLE in the following format:
// pkt[7:0]: 1 byte: command ( 0x01 for Write and 0x02 for read)
// pkt[24:8]: 2 bytes: address
// pkt[39:25]: 2 bytes: data (for write and read)
// Note: Bits [7:0] come first from the air

enum Command: Int {
    case Nop = 0
    case Write
    case Read
}

struct Packet {
    private var addressLowByte: UInt8;
    private var addressHighByte: UInt8;
    private var dataLowByte: UInt8;
    private var dataHighByte: UInt8;
    private var commandByte: UInt8;
    
    var allBits: [UInt8] {
        get {
            return [commandByte, addressLowByte, addressHighByte, dataLowByte, dataHighByte]
        }
    }
    
    var address: UInt16 {
        get {
            return UInt16(addressLowByte) | (UInt16(addressHighByte) << 8)
        }
        set {
            self.addressLowByte = UInt8(newValue & 0x00FF)
            self.addressHighByte = UInt8((newValue >> 8) & 0x0FF)
        }
    }
    
    var data: UInt16 {
        get {
            return UInt16(dataLowByte) | (UInt16(dataHighByte) << 8)
        }
        set {
            self.dataLowByte = UInt8(newValue & 0x00FF)
            self.dataHighByte = UInt8((newValue >> 8) & 0x0FF)
        }
    }
    
    var command: Command {
        set {
            self.commandByte = UInt8(newValue.rawValue)
        }
        get {
            return Command.init(rawValue: Int(self.commandByte))!
        }
    }
    
    init (address: Int, data: Int, command: Command)  {
        self.addressLowByte = UInt8(address & 0x00FF)
        self.addressHighByte = UInt8((address >> 8) & 0x0FF)
        self.dataLowByte = UInt8(data & 0x00FF)
        self.dataHighByte = UInt8((data >> 8) & 0x0FF)
        self.commandByte = UInt8(command.rawValue)
    }
    
}