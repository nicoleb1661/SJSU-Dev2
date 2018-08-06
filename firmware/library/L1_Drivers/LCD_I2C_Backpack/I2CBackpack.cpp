#include "LcdI2cBackpack.hpp"

// Slave address: 40h for write, 41h for read
// This backpack requires data transfer in 4-bit mode
// 4-bit transfers require all data to be written twice

const uint8_t numeral[10][8] =
{
    [0] = 0b0011'0000,
    [1] =
    [2] =
    [3] =
    [4] =
    [5] =
    [6] =
    [7] =
    [8] =
    [9] =
};

const uint8_t alphabet[26][8] =
{
    // A = 0

    // B = 1

    // C = 2

    // D = 3

    // E = 4

    // F = 5

    // G = 6

    // H = 7

    // I = 8

    // J = 9

    // K = 10

    // L = 11

    // M = 12

    // N = 13

    // O = 14

    // P = 15

    // Q = 16

    // R = 17

    // S = 18

    // T = 19

    // U = 20

    // V = 21

    // W = 22

    // X = 23

    // Y = 24

    // Z = 25
};

const uint8_t symbol[10][8] =
{
    // ! = 0

    // " = 1

    // ' = 2

    // ( = 3

    // ) = 4

    // , = 5

    // . = 6

    // : = 7

    // ; = 8

    // ? = 9
};

LcdI2cBackpack::LcdI2cBackpack(uint8_t address_read, uint8_t address_write)
{
    device_address_read_ = address_read;
    device_address_write_ = address_write;
    display_function_ = 0x00;
    display_control_ = 0x00;
    display_mode_ = 0x00;
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
bool I2CBackpack::Init(FontSize size, DisplayLines lines) //initializes the LCD
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
    // Write start up bits
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
    Write(kClearDisplay, kClearDisplay);
    ReturnHome();
}

void LcdI2cBackpack::ReturnHome()
{
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
    display_function_ |= lines;
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

void LcdI2cBackpack::SetFont(FontSize size)
{
    display_control_ |= size;
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
