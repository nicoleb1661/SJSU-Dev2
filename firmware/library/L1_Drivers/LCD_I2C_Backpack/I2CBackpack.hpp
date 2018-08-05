#pragma once

#include <cstdint>

// Import SJOne libraries to be used for testing
#include "L2_Drivers/base/i2c_base.hpp"

class LcdBackpackInterface
{
public:
    enum FontSize : uint8_t
    {
        kSmall = 0x00,
        kLarge = 0x04,
    };
    enum DisplayLines : uint8_t
    {
        kOne = 0x00,
        kTwo = 0x08,
    };
    virtual void Init() = 0;
    virtual void Set4BitMode() = 0;
    virtual void ClearScreen() = 0;
    virtual void ReturnHome() = 0;
    virtual void PrintChar() = 0;
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
    static constexpr uint8_t kRightEntry = 0x00;
    static constexpr uint8_t kLeftEntry = 0x02;
    // Display control variables
    static constexpr uint8_t kDisplayOff = 0x00;
    static constexpr uint8_t kDisplayOn = 0x04;
    static constexpr uint8_t kCursorOff = 0x00;
    static constexpr uint8_t kCursorOn = 0x02;
    static constexpr uint8_t kBlinkOff = 0x00;
    static constexpr uint8_t kBlinkOn = 0x01;
    // Display and cursor shift members
    static constexpr uint8_t kCursorMove = 0x00;
    static constexpr uint8_t kDisplayMove = 0x08;
    // Function set members
    static constexpr uint8_t kFourBitMode = 0x00;
    static constexpr uint8_t kEightBitMode = 0x10;
    // Enums
    enum Commands : uint8_t
    {
        kClearDisplay = 0x01,
        kReturnHome = 0x02,
        kEntryModeSet = 0x04,
        kDisplayControl = 0x08,
        kFunctionSet = 0x20,
        kSetCgramAddress = 0x40,
        kSetDdramAddress = 0x80,
    };
    enum BacklightControl : uint8_t
    {
        kBacklightOff = 0x00
        kBacklightOn = 0x08,
    };
    enum BitAssignments : uint8_t
    {
        kEnableBit = 0x04;
        kReadWriteBit = 0x02,
        kRegisterSelectBit = 0x01,
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


