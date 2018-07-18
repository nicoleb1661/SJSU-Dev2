#pragma once

#include <cstdint>
//Import SJOne libraries to be used for testing
#include "L2_Drivers/base/i2c_base.hpp"

#ifndef BackpackInterface_H
#define BackpackInterface_H

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
    virtual void DisplayOn();
    virtual void DisplayOff();
    virtual void ShiftCursorLeft();
    virtual void ShiftCursorRight();
    virtual void SetFont();
}

#endif /* end of include */
