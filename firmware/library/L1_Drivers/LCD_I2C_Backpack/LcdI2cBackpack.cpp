#include "LcdI2cBackpack.hpp"
#include "utilities.h"

// Slave address: 40h for write, 41h for read
// This backpack requires data transfer in 4-bit mode
// 4-bit transfers require all data to be written twice

const uint8_t LcdI2cBackpack::kNumeral[10] =
{
    [0] = 0b0011'0000,
    [1] = 0b0011'0001,
    [2] = 0b0011'0010,
    [3] = 0b0011'0011,
    [4] = 0b0011'0100,
    [5] = 0b0011'0101,
    [6] = 0b0011'0110,
    [7] = 0b0011'0111,
    [8] = 0b0011'1000,
    [9] = 0b0011'1001
};

const uint8_t LcdI2cBackpack::kAlphabet[26] =
{
    // A = 0
    [0] = 0b0100'0001,
    // B = 1
    [1] = 0b0100'0010,
    // C = 2
    [2] = 0b0100'0011,
    // D = 3
    [3] = 0b0100'0100,
    // E = 4
    [4] = 0b0100'0101,
    // F = 5
    [5] = 0b0100'0110,
    // G = 6
    [6] = 0b0100'0111,
    // H = 7
    [7] = 0b0100'1000,
    // I = 8
    [8] = 0b0100'1001,
    // J = 9
    [9] = 0b0100'1010,
    // K = 10
    [10] = 0b0100'1011,
    // L = 11
    [11] = 0b0100'1100,
    // M = 12
    [12] = 0b0100'1101,
    // N = 13
    [13] = 0b0100'1110,
    // O = 14
    [14] = 0b0100'1111,
    // P = 15
    [15] = 0b0101'0000,
    // Q = 16
    [16] = 0b0101'0001,
    // R = 17
    [17] = 0b0101'0010,
    // S = 18
    [18] = 0b0101'0011,
    // T = 19
    [19] = 0b0101'0100,
    // U = 20
    [20] = 0b0101'0101,
    // V = 21
    [21] = 0b0101'0110,
    // W = 22
    [22] = 0b0101'0111,
    // X = 23
    [23] = 0b0101'1000,
    // Y = 24
    [24] = 0b0101'1001,
    // Z = 25
    [25] = 0b0101'1010
};

const uint8_t LcdI2cBackpack::kSymbol[11] =
{
    // Blank space = 0
    [0] = 0b0010'0000,
    // ! = 1
    [1] = 0b0010'0001,
    // " = 2
    [2] = 0b0010'0010,
    // ' = 3
    [3] = 0b0010'0111,
    // ( = 4
    [4] = 0b0010'1000,
    // ) = 5
    [5] = 0b0010'1001,
    // , = 6
    [6] = 0b0010'1100,
    // . = 7
    [7] = 0b0010'1110,
    // : = 8
    [8] = 0b0011'1010,
    // ; = 9
    [9] = 0b0011'1011,
    // ? = 10
    [10] = 0b0011'1111
};

LcdI2cBackpack::LcdI2cBackpack(uint8_t address_read, uint8_t address_write)
{
    device_address_read_ = address_read;
    device_address_write_ = address_write;
    display_function_ = 0b0000'0000;
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
bool LcdI2cBackpack::Init(I2cLcdInterface::FontSize size, I2cLcdInterface::DisplayLines lines) //initializes the LCD
{
    bool is_on = true;
    bool show_cursor = false;
    bool blink_cursor = false;
    
    // Wait for 50ms after Vcc rises to 2.7V
    delay_ms(50);
    // Clear the register
    Write((uint8_t)Commands::kClearRegister, (uint8_t)Commands::kClearRegister);
    // Write start up bits
    Write(kEightBitMode, kEightBitMode);
    delay_ms(5);
    Write(kEightBitMode, kEightBitMode);
    delay_ms(1);
    Write(kEightBitMode, kEightBitMode);
    // Set interface to 4-bit mode
    // Set number of lines and font size
    FunctionSet(size, lines);
    // Turn display on with no cursor or blinking
    DisplayControl(is_on, show_cursor, blink_cursor);
    ClearScreen();
    // Initialize default text direction
    Write(kLeftEntry, kLeftEntry);
    // Set entry mode
    Write((uint8_t)Commands::kEntryModeSet, (uint8_t)Commands::kEntryModeSet);
    return true;
}

// Pin configuration requires 4 bit data transfer
// When 4 bit length is selected, data must be sent or received twice
uint8_t LcdI2cBackpack::Set4BitMode()
{
    return display_function_ |= kFourBitMode;
}

void LcdI2cBackpack::ClearScreen()
{
    Write((uint8_t)Commands::kClearDisplay, (uint8_t)Commands::kClearDisplay);
    ReturnHome();
}

void LcdI2cBackpack::ReturnHome()
{
    Write((uint8_t)Commands::kReturnHome, (uint8_t)Commands::kReturnHome);
}

bool LcdI2cBackpack::PrintChar(char input)
{
    uint8_t ascii_value = (uint8_t)input;
    bool char_sent = false;
    
    if(ascii_value >= 65 && ascii_value <= 90) // Letters
    {
        Write(kAlphabet[ascii_value - 65], kAlphabet[ascii_value - 65]);
        char_sent = true;
    }
    else if(acii_value >= 48 && ascii_value <= 57) // Numbers 0 - 9
    {
        Write(kNumeral[ascii_value - 48], kNUmeral[ascii_value - 48]);
        char_sent = true;
    }
    else if(ascii_value >=32 && ascii_value <= 34) // symbols-> space !  "
    {
        Write(kSymbol[ascii_value - 32], kSymbol[ascii_value - 32]);
        char_sent = true;
    }
    else if(ascii_value >= 39 && ascii_value <= 41) // symbols-> '  (  )
    {
        Write(kSymbol[ascii_value - 36], kSymbol[ascii_value - 36]);
        char_sent = true;
    }
    else if (ascii_value == 44) // symbol-> ,
    {
        Write(kSymbol[6], kSymbol[6]);
        char_sent = true;
    }
    else if(ascii_value == 46) // symbol-> .
    {
        Write(kSymbol[7], kSymbol[7]);
        char_sent = true;
    }
    else if(ascii_value >= 58 && ascii_value <= 59)// Symbols-> :  ;
    {
        Write(kSymbol[ascii_value - 50], kSymbol[ascii_value - 50]);
        char_sent = true;
    }
    else if(ascii_value == 63)// Symbol-> ?
    {
        Write(kSymbol[10], kSymbol[10]);
        char_sent = true;
    }
    else // Not a valid input
    {
        char_sent = false;
    }
    
    return char_sent;
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
        cursor_option = kBlinkOff | kDisplayOn;
        Write(cursor_option, cursor_option);
    }
}

uint8_t LcdI2cBackpack::SetLineDisplay(I2cLcdInterface::DisplayLines lines) //Select 1, 2, or 4 lines
{
    return (display_function_ |= (uint8_t)lines);
    //TODO: Error check if font is large then can only be one line
}

bool LcdI2cBackpack::CheckBusyFlag()
{
    bool busy_flag_set;
    
    
    Write((uint8_t)BitAssignments::kReadWriteBitSet, (uint8_t)BitAssignments::kReadWriteBitSet);
    if(Read((uint8_t)BitAssignments::kBusyFlag))
    {
        busy_flag_set = true;
    }
    else
    {
        busy_flag_set = false;
    }
    Write((uint8_t)BitAssignments::kReadWriteBitClr, (uint8_t)BitAssignments::kReadWriteBitClr);
    return busy_flag_set;
}

void LcdI2cBackpack::DisplayControl(bool is_on, bool show_cursor, bool blink_cursor)
{
    if(is_on) // Turn on display
    {
        Write(kDisplayOn, kDisplayOn);
        CursorControl(show_cursor, blink_cursor);
    }
    else // Turn off display
    {
        Write(kDisplayOff, kDisplayOff);
    }
}

uint8_t LcdI2cBackpack::SetFont(I2cLcdInterface::FontSize size)
{
    return (display_function_ |= (uint8_t)size);
}

void LcdI2cBackpack::FunctionSet(I2cLcdInterface::FontSize size, I2cLcdInterface::DisplayLines lines)
{
    display_function_ |= Set4BitMode();
    display_function_ |= SetFont(size);
    if(size == FontSize::kSmall)
    {
        display_function_ |= SetLineDisplay(lines);
    }
    else
    {
        display_function_ |= (uint8_t)DisplayLines::kTwo;
    }
    Write(display_function_, display_function_);
}

LcdI2cBackpack::~LcdI2cBackpack()
{
    
}

void LcdI2cBackpack::Write(uint8_t address, uint8_t data)
{
    i2c->writeReg(device_address_write_, address, data);
}

bool LcdI2cBackpack::Read(uint8_t address) const
{
    return i2c->readReg(device_address_read_, address);
}
