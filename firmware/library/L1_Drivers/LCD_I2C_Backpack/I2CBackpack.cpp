
#include "I2CBackpack.hpp"

bool I2CBackpack::init() //initializes the LCD
{

    return true;
}

void I2CBackpack::set4BitMode()
{

}

void I2CBackpack::clearScreen()
{
    //Set the command to clear screen:
    //00_00-0000-0001
    
    //Sends 20H to all the DDRAM addresses
    write(0x7F, 0x20);
    //Set the DDRAM address back to 0 returns the display to its original status 
    returnHome();
} 

void I2CBackpack::setPosition(uint8_t row, uint8_t col)
{

} 

void I2CBackpack::returnHome()
{
    //Set the DDRAM address to 0 
    //Return the screen into its original state
    //Move cursor to the left side of the screen (ensure its the first line if using multiple lines)

}

void I2CBackpack::printChar()
{

}

void I2CBackpack::displayCursor()
{

}

void I2CBackpack::noCursor()  //No cursor will show on the display
{

}

void I2CBackpack::blinkChar() //The current character will blink 
{

}

void I2CBackpack::solidChar() //The current character won’t blink
{

}

void I2CBackpack::setLineDisplay(uint8_t lines) //Select 1, 2, or 4 lines
{

}

bool I2CBackpack::checkBusyFlag()
{

    return true;
} 

void I2CBackpack::displayOn()
{

}

void I2CBackpack::displayOff()
{

}

void I2CBackpack::shiftCursorLeft()
{

}

void I2CBackpack::shiftCursorRight()
{

}

void I2CBackpack::setFont()
{
    
} 