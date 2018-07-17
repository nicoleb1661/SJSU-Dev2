#ifndef I2CBackpack_H
#define I2CBackpack_H
#include<stdint.h>

class I2CBackpack
{

    public:
        I2CBackpack();
        bool Init();
        void Set4BitMode();
        void ClearScreen();
        void SetPosition(uint8_t row, uint8_t  col);
        void ReturnHome();
        void PrintChar();
        void DisplayCursor();
        void NoCursor(); //No cursor will appear
        void BlinkChar(); //The current character will blink
        void SolidChar(); //The current character will not blink
        void SetLineDisplay(uint8_t lines); //Select 1, 2, or 4 line display
        bool CheckBusyFlag();//Check busy flag
        void DisplayOn();
        void DisplayOff();
        void ShiftCursorLeft();
        void ShiftCursorRight();
        void SetFont();
        ~I2CBackpack();

    private:
        uint8_t deviceAddress;
        uint8_t col;
        uint8_t row;
        void Write(uint8_t address, uint8_t data);
        void Read(uint8_t address)const;

};

#endif
