#include "LcdI2cBackpack.hpp"

// Slave address: 40h for write, 41h for read
// This backpack requires data transfer in 4-bit mode
// 4-bit transfers require all data to be written twice

const uint8_t numeral[10][8] =
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

const uint8_t alphabet[26][8] =
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
    [4] = 0b0100'0101
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

const uint8_t symbol[10][8] =
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
    display_control_ = 0b0000'0000;
    display_mode_ = 0b0000'0000;
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
    uint8_t clear = 0b0000'0000;
    uint8_t function_set = 0b0000'0011;
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
    uint8_t fourBitMode = (0b0000'0010);
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

bool LcdI2cBackpack::PrintChar(char input)
{
    uint8_t ascii_value = (uint8_t)input;
    bool char_sent = false;
    
    if(ascii_value >= 65 && ascii_value <= 90) // Letters
    {
        char_sent = true;
    }
    else if(acii_value >= 48 && ascii_value <= 57) // Numbers
    {
        char_sent = true;
    }
    else if(ascii_value >=32 && ascii_value <= 34) // symbols: space, !, "
    {
        char_sent = true;
    }
    else if(ascii_value >= 39 && ascii_value <= 41) // symbols: ', (, )
    {
        char_sent = true;
    }
    else if (ascii_value == 44) // symbol: ,
    {
        char_sent = true;
    }
    else if(ascii_value == 46) // symbol: .
    {
        char_sent = true;
    }
    else if(ascii_value >= 58 && ascii_value <= 59)//:, ;
    {
        char_sent = true;
    }
    else if(ascii_value == 63)// ?
    {
        char_sent = true;
    }
    else
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
