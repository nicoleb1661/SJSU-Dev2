#pragma once

#include <cstdint>

// Import SJOne libraries to be used for testing
#include "L2_Drivers/base/i2c_base.hpp"

class BackpackInterface
{
 public:
    virtual void Init();
    virtual void Set4BitMode();
    virtual void ClearScreen();
    virtual void SetPosition(uint8_t row, uint8_t col);
    virtual void ReturnHome();
    virtual void PrintChar();
    virtual void CursorControl(bool show_cursor, bool blink_cursor);
    virtual void SetLineDisplay(uint8_t lines);
    virtual bool CheckBusyFlag();
    virtual void DisplayControl();
    virtual void ShiftCursor();
    virtual void SetFont();
}

class I2CBackpack : public BackpackInterface
{
 public:
    I2CBackpack();
    bool Init() override;
    void Set4BitMode() override;
    void ClearScreen() override;
    void SetPosition(uint8_t row, uint8_t  col) override;
    void ReturnHome() override;
    void PrintChar() override;
    // CursorControl() Replaces NoCursor, DisplayCursor, BlinkChar, and SolidChar
    void CursorControl(bool show_cursor, bool blink_cursor) override;
    // Select 1, 2, or 4 line display
    void SetLineDisplay(uint8_t lines) override;
    bool CheckBusyFlag() override;
    void DisplayControl() override;
    void ShiftCursor() override;
    void SetFont() override;
    ~I2CBackpack();
 private:
    uint8_t device_address_;
    uint8_t col_;
    uint8_t row_;
    void Write(uint8_t address, uint8_t data);
    void Read(uint8_t address)const;

};


