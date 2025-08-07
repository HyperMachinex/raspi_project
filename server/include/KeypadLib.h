#ifndef KEYPADLIB_H
#define KEYPADLIB_H

#include <gpiod.h>
#include <string>

class Keypad {
public:
    Keypad();
    ~Keypad();

    void init();                      // Başlatıcı fonksiyon
    void release();                   // Kaynakları serbest bırak
    char getKey();                    // Basılan tuşu oku

    // GPIO pinlerini değiştirmek istersen
    void setChipName(const std::string& ch);
    void setKeyGPIOCol(int col1, int col2, int col3, int col4);
    void setKeyGPIORow(int row1, int row2, int row3, int row4);

private:
    char key[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    int keyGPIOCol[4] = {6, 13, 19, 26}; // output
    int keyGPIORow[4] = {16, 20, 21, 12}; // input

    gpiod_line* lineKeyCol[4];
    gpiod_line* lineKeyRow[4];
    std::string chipName;
    gpiod_chip* chip;
};

#endif // KEYPADLIB_H
