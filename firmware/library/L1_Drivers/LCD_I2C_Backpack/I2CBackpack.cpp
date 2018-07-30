
#include "I2CBackpack.hpp"

// Slave address: 40h for write, 41h for read

I2CBackpack::I2CBackpack(uint8_t address_read, uint8_t address_write)
{
    device_address_read_ = address_read;
    device_address_write_ = address_write;
}

bool I2CBackpack::Init() //initializes the LCD
{
    uint32_t pclk = 48;
    uint32_t bus_rate = 100;
    
    //delay to allow LCD internal reset to complete
    delay_ms(10);
    //call I2C driver init
    init(pclk, bus_rate);
    //Set all Backpack ports to Output HIGH
    Write(0xF, 0xF);
    return true;
}

// Pin configuration requires 4 bit data transfer
void I2CBackpack::Set4BitMode()
{
    Write(0x2, 0x0);
}

void I2CBackpack::ClearScreen()
{
    // Set the command to clear screen:
    // 00_00-0000-0001
    
    // Sends 20H to all the DDRAM addresses
    Write(kClearDisplay, kClearDisplay);
    // Set the DDRAM address back to 0 returns the display to its original status 
    ReturnHome();
} 

void I2CBackpack::SetPosition(uint8_t row, uint8_t col)
{

} 

void I2CBackpack::ReturnHome()
{
    // Set the DDRAM address to 0 
    // Return the screen into its original state
    // Move cursor to the left side of the screen (ensure its the first line if using multiple lines)
    Write(kReturnHome, kReturnHome);

}

void I2CBackpack::PrintChar()
{

}

void I2CBackpack::CursorControl(bool show_cursor, bool blink_cursor)
{
    uint8_t cursor_option = 0;

    if(show_cursor){  // DB1 == 1
        cursor_option = kCursorOn | kDisplayOn;
        Write(0x00, cursor_option);
    }else{
        cursor_option = kCursorOff | kDisplayOn;
        Write(0x00, cursor_option);
    }
    if(blink_cursor){  // DB0 == 1
        cursor_option = kBlinkOff | kDisplayOff;
        Write(0x00, cursor_option);
    }else{
        cursor_option = kBlinkOff | kDisplyOn;
        Write(0x00, cursor_option);
    }
}


void I2CBackpack::SetLineDisplay(DisplayLines lines)
{

}

bool I2CBackpack::CheckBusyFlag()
{

    return true;
} 

void I2CBackpack::DisplayControl(bool on, bool show_cursor, bool blink_cursor)
{
    // D is 1 to turn on display
    // C 1 to display the cursor 
    // B 1 to display blinking cursor 
    if(on){  // Turn on display
        if(show_cursor || blink_cursor){  
            Write(0x00, kDisplayOn);
            CursorControl(show_cursor, blink_cursor);
        }
        else{  // If both are false, cursor won't show or blink
            CursorControl(show_cursor, blink_cursor);
        }
    }else{  // Turn off display
        Write(0x00, kDisplayOff);
    }
}

void I2CBackpack::ShiftCursor()
{

}

void I2CBackpack::SetFont(FontSize size)
{
    
}

void I2CBackpack::Write(uint8_t address, uint8_t data)
{
    writeReg(device_address_write_, address, data);
}

void I2CBackpack::Read(uint8_t address) const
{
    readReg(device_address_read_, address);
}
