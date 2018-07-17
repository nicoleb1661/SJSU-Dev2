#include "Interface.hpp"

#ifndef I2CBackpack_H
#define I2CBackpack_H


class I2CBackpack : public Interface 
{

    public:
        I2CBackpack();
        bool Init() override;
        void Set4BitMode() override; 
        void ClearScreen() override;
        void SetPosition(uint8_t row, uint8_t  col) override;
        void ReturnHome() override;
        void PrintChar() override;
        void DisplayCursor() override;
        void NoCursor() override; //No cursor will appear
        void BlinkChar() override; //The current character will blink
        void SolidChar() override; //The current character will not blink
        void SetLineDisplay(uint8_t lines) override; //Select 1, 2, or 4 line display
        bool CheckBusyFlag() override;//Check busy flag
        void DisplayOn() override;
        void DisplayOff() override;
        void ShiftCursorLeft() override;
        void ShiftCursorRight() override;
        void SetFont() override;
        ~I2CBackpack();

    private:
        uint8_t device_address_;
        uint8_t col_;
        uint8_t row_;
        void Write(uint8_t address, uint8_t data);
        void Read(uint8_t address)const;

};

#endif
