#pragma once

#include <cstdint>

// Import SJOne libraries to be used for testing
#include "L2_Drivers/base/i2c_base.hpp"

// Pin order RS-RW-E-Backlight_D7-D6-D5-D4
// DB7 behaves as busy flag
class I2cLcdInterface
{
public:
    enum class FontSize : uint8_t
    {
        kSmall = 0b0000'0000,
        kLarge = 0b0000'0100
    };
    enum class DisplayLines : uint8_t
    {
        kOne = 0b0000'0000,
        kTwo = 0b0000'1000
    };
    virtual void Init() = 0;
    virtual void Set4BitMode() = 0;
    virtual void ClearScreen() = 0;
    virtual void ReturnHome() = 0;
    virtual bool PrintChar() = 0;
    virtual void CursorControl(bool show_cursor, bool blink_cursor) = 0;
    virtual uint8_t SetLineDisplay(DisplayLines lines) = 0;
    virtual bool CheckBusyFlag() = 0;
    virtual void DisplayControl() = 0;
    virtual void FunctionSet(FontSize size, DisplayLines lines) = 0;
    virtual uint8_t SetFont(FontSize size) = 0;
}

class LcdI2cBackpack : public I2cLcdInterface
{
public:
    //Display entry mode
    static constexpr uint8_t kRightEntry = 0b0000'0000;
    static constexpr uint8_t kLeftEntry = 0b0000'0010;
    // Display control variables
    static constexpr uint8_t kDisplayOff = 0b0000'0000;
    static constexpr uint8_t kDisplayOn = 0b0000'0100;
    static constexpr uint8_t kCursorOff = 0b0000'0000;
    static constexpr uint8_t kCursorOn = 0b0000'0010;
    static constexpr uint8_t kBlinkOff = 0b0000'0000;
    static constexpr uint8_t kBlinkOn = 0b0000'0001;
    // Display and cursor shift members
    static constexpr uint8_t kCursorMove = 0b0000'0000;
    static constexpr uint8_t kDisplayMove = 0b0000'1000;
    // Function set members
    static constexpr uint8_t kFourBitMode = 0b0000'0010;
    static constexpr uint8_t kEightBitMode = 0b0000'0011;
    // Enums
    enum class Commands : uint8_t
    {
        kClearRegister = 0b0000'0000,
        kClearDisplay = 0b0000'0001,
        kReturnHome = 0b0000'0010,
        kEntryModeSet = 0b0000'0110,
        kDisplayControl = 0b0000'1000,
        kSetCgramAddress = 0b0100'0000,
        kSetDdramAddress = 0b1000'0000
    };
    enum class BacklightControl : uint8_t
    {
        kBacklightOff = 0b0000'0000,
        kBacklightOn = 0b0000'1000
    };
    enum class BitAssignments : uint8_t
    {
        kEnableBit = 0b0010'0000,
        kReadWriteBitSet = 0b0100'0000,
        kReadWriteBitClr = 0b0000'0000,
        kRegisterSelectBit = 0b1000'0000,
        kBusyFlag = 0b0000'1000
    };
    
    static const uint8_t kNumeral[10];
    static const uint8_t kAlphabet[26];
    static const uint8_t kSymbol[11];
    
    LcdI2cBackpack(uint8_t address_read, uint8_t address_write);
    bool Init() override;
    uint8_t Set4BitMode() override;
    void ClearScreen() override;
    void ReturnHome() override;
    bool PrintChar() override;
    void CursorControl(bool show_cursor, bool blink_cursor) override;
    uint8_t SetLineDisplay(DisplayLines lines) override;
    bool CheckBusyFlag() override;
    void DisplayControl() override;
    uint8_t SetFont(FontSize size) override;
    void FunctionSet(FontSize size, DisplayLines lines) override;
    ~LcdI2cBackpack();
private:
    uint8_t device_address_read_;
    uint8_t device_address_write_;
    uint8_t display_function_;
    void Write(uint8_t address, uint8_t data);
    bool Read(uint8_t address)const;
};
