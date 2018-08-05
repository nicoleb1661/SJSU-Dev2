
#include "I2CBackpack.hpp"

// Slave address: 40h for write, 41h for read

LcdI2cBackpack::I2CBackpack(uint8_t address_read, uint8_t address_write)
{
    device_address_read_ = address_read;
    device_address_write_ = address_write;
}

/* An internal reset occurs at power on executing the
 * following instructions:
 * 1. Display clear
 * 2. Function set:
 *    8-bit interface data
 *    1-line display
 *    5 x 8 dot char font
 * 3. Display on/off control:
 *    Display display off
 *    Cursor off
 *    Blinking off
 * 4. Entry mode set:
 *    Increment by 1
 *    No shift
 */
bool LcdI2cBackpack::Init() //initializes the LCD
{
    uint8_t clear = 0x00;
    uint8_t function_set = 0x03;
    uint32_t pclk = 48;
    uint32_t bus_rate = 100;
    //call I2C driver init
    init(pclk, bus_rate);
    // Wait for 50ms after Vcc rises to 2.7V
    delay_ms(50);
    // Clear the register
    Write(clear, clear);
    //
    Write(function_set, function_set);
    delay_ms(5);
    Write(function_set, function_set);
    delay_ms(1);
    Write(function_set, function_set);
    // Set interface to 4-bit mode
    Set4BitMode();
    // Set number of lines and font size
    SetFont(size);
    SetLineDisplay(lines);
    // Turn display on with no cursor or blinking
    DisplayControl();
    ClearScreen();
    // Initialize default text direction
    Write(kLeftEntry, kLeftEntry);
    // Set entry mode
    Write(kEntryModeSet, kEntryModeSet);
    return true;
}

// Pin configuration requires 4 bit data transfer
// When 4 bit length is selected, data must be sent or received twice
void LcdI2cBackpack::Set4BitMode()
{
    uint8_t fourBitMode = (0x02);
    Write(fourBitMode, fourBitMode);
}

void LcdI2cBackpack::ClearScreen()
{
    // Set the command to clear screen:
    // 00_00-0000-0001
    
    // Sends 20H to all the DDRAM addresses
    Write(kClearDisplay, kClearDisplay);
    // Set the DDRAM address back to 0 returns the display to its original status 
    ReturnHome();
} 

void LcdI2cBackpack::SetPosition(uint8_t row, uint8_t col)
{

} 

void LcdI2cBackpack::ReturnHome()
{
    // Set the DDRAM address to 0 
    // Return the screen into its original state
    // Move cursor to the left side of the screen (ensure its the first line if using multiple lines)
    Write(kReturnHome, kReturnHome);

}

void LcdI2cBackpack::PrintChar()
{

}

void LcdI2cBackpack::CursorControl(bool show_cursor, bool blink_cursor)
{
    uint8_t cursor_option = 0;

    if(show_cursor) // DB1 == 1
    {
        cursor_option = kCursorOn | kDisplayOn;
        Write(cursor_option, cursor_option);
    }
    else
    {
        cursor_option = kCursorOff | kDisplayOn;
        Write(cursor_option, cursor_option);
    }
    if(blink_cursor) // DB0 == 1
    {
        cursor_option = kBlinkOn | kDisplayOn;
        Write(cursor_option, cursor_option);
    }
    else
    {
        cursor_option = kBlinkOff | kDisplyOn;
        Write(cursor_option, cursor_option);
    }
}


void LcdI2cBackpack::SetLineDisplay(DisplayLines lines)
{
    switch(lines)
    {
        case one:
            break;
        case two:
            break;
        case four:
            break;
        default:
    }
}

bool LcdI2cBackpack::CheckBusyFlag()
{

    return true;
} 

void LcdI2cBackpack::DisplayControl(bool on, bool show_cursor, bool blink_cursor)
{
    if(on) // Turn on display
    {
        Write(kDisplyOn, kDisplayOn);
        CursorControl(show_cursor, blink_cursor);
    }
    else // Turn off display
    {
        Write(kDisplayOff, kDisplayOff);
    }
}

void LcdI2cBackpack::ShiftCursor()
{

}

void LcdI2cBackpack::SetFont(FontSize size)
{
    switch(size)
    {
        case small:
            break;
        case large:
            break;
        default:
    }
}

LcdI2cBackpack::~LcdI2cBackpack()
{

}

void LcdI2cBackpack::Write(uint8_t address, uint8_t data)
{
    writeReg(device_address_write_, address, data);
}

void LcdI2cBackpack::Read(uint8_t address) const
{
    readReg(device_address_read_, address);
}
