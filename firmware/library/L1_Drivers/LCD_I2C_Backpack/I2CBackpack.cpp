
#include "I2CBackpack.hpp"

//Slave address: 40h for write, 41h for read

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

//Pin configuration requires 4 bit data transfer
void I2CBackpack::Set4BitMode()
{
    Write(0x2, 0x0);
}

void I2CBackpack::ClearScreen()
{
    //Set the command to clear screen:
    //00_00-0000-0001
    
    //Sends 20H to all the DDRAM addresses
    Write(0x01, 0x00);
    //Set the DDRAM address back to 0 returns the display to its original status 
    ReturnHome();
} 

void I2CBackpack::SetPosition(uint8_t row, uint8_t col)
{

} 

void I2CBackpack::ReturnHome()
{
    //Set the DDRAM address to 0 
    //Return the screen into its original state
    //Move cursor to the left side of the screen (ensure its the first line if using multiple lines)
    Write(0x02, 0x00);

}

void I2CBackpack::PrintChar()
{

}

void I2CBackpack::CursorControl(bool show_cursor, bool blink_cursor)
{

}


void I2CBackpack::SetLineDisplay(uint8_t lines) //Select 1, 2, or 4 lines
{

}

bool I2CBackpack::CheckBusyFlag()
{

    return true;
} 

void I2CBackpack::DisplayControl()
{
    //D is 1 to turn on display
    //C 1 to display the cursor 
    //B 1 to display blinking cursor 
}

void I2CBackpack::ShiftCursor()
{

}

void I2CBackpack::SetFont()
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
