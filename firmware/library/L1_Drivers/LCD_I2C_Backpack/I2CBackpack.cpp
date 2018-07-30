
#include "I2CBackpack.hpp"

// Slave address: 40h for write, 41h for read

I2CBackpack::I2CBackpack(uint8_t address_read, uint8_t address_write)
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
bool I2CBackpack::Init() //initializes the LCD
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
void I2CBackpack::Set4BitMode()
{
    uint8_t fourBitMode = (0x02);
    Write(fourBitMode, fourBitMode);
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


void I2CBackpack::SetLineDisplay(DisplayLines lines)
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

bool I2CBackpack::CheckBusyFlag()
{

    return true;
} 

void I2CBackpack::DisplayControl(bool on, bool show_cursor, bool blink_cursor)
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

void I2CBackpack::ShiftCursor()
{

}

void I2CBackpack::SetFont(FontSize size)
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

I2CBackpack::~I2CBackpack()
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
