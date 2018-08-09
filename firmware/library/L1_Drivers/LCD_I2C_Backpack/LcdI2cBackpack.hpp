#pragma once

#include <cstdint>

// Import SJOne libraries to be used for testing
#include "L2_Drivers/base/i2c_base.hpp"

class LcdBackpackInterface
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
    virtual void PrintChar(char input) = 0;
    virtual void CursorControl(bool show_cursor, bool blink_cursor) = 0;
    virtual void SetLineDisplay(DisplayLines lines) = 0;
    virtual bool CheckBusyFlag() = 0;
    virtual void DisplayControl() = 0;
    virtual void SetFont(FontSize size) = 0;
}

class LcdI2cBackpack : public LcdBackpackInterface
{
public:
    //Display entry mode
    static constexpr uint8_t kRightEntry = 0x0000'0000;
    static constexpr uint8_t kLeftEntry = 0x0000'0010;
    // Display control variables
    static constexpr uint8_t kDisplayOff = 0x0000'00;
    static constexpr uint8_t kDisplayOn = 0x0000'0100;
    static constexpr uint8_t kCursorOff = 0x0000'0000;
    static constexpr uint8_t kCursorOn = 0x0000'0010;
    static constexpr uint8_t kBlinkOff = 0x0000'0000;
    static constexpr uint8_t kBlinkOn = 0x0000'0001;
    // Display and cursor shift members
    static constexpr uint8_t kCursorMove = 0x0000'0000;
    static constexpr uint8_t kDisplayMove = 0x0000'1000;
    // Function set members
    static constexpr uint8_t kFourBitMode = 0x0000'0000;
    static constexpr uint8_t kEightBitMode = 0x0001'0000;
    // Enums
    enum class Commands : uint8_t
    {
        kClearDisplay = 0x0000'0001,
        kReturnHome = 0x0000'0010,
        kEntryModeSet = 0x0000'0100,
        kDisplayControl = 0x0000'1000,
        kSetCgramAddress = 0x0100'0000,
        kSetDdramAddress = 0x1000'0000
    };
    enum class BacklightControl : uint8_t
    {
        kBacklightOff = 0x0000'0000,
        kBacklightOn = 0x0000'1000
    };
    enum class BitAssignments : uint8_t
    {
        kEnableBit = 0x0000'0100,
        kReadWriteBit = 0x0000'0010,
        kRegisterSelectBit = 0x0000'0001
    };
    LcdI2cBackpack(uint8_t address_read, uint8_t address_write);
    bool Init() override;
    void Set4BitMode() override;
    void ClearScreen() override;
    void ReturnHome() override;
    void PrintChar() override;
    void CursorControl(bool show_cursor, bool blink_cursor) override; //Replaces NoCursor DisplayCursor, BlinkChar, and SolidChar
    void SetLineDisplay(uint8_t lines) override; //Select 1, 2, or 4 line display
    bool CheckBusyFlag() override;//Check busy flag
    void DisplayControl(bool on, bool show_cursor, bool blink_cursor) override;
    void ShiftCursor() override;
    void SetFont(FontSize size) override;
    ~LcdI2cBackpack();
private:
    uint8_t device_address_read_;
    uint8_t device_address_write_;
    uint8_t display_function_;
    uint8_t display_control_;
    uint8_t display_mode_;
    void Write(uint8_t address, uint8_t data);
    void Read(uint8_t address)const;
};
