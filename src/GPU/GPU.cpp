//
// Created by nikos on 12-Aug-19.
//

#include <GPU/GPU.hpp>
#include <Memory/Memory.hpp>
#include <util/bitoperations.hpp>

GPU::GPU(Memory &memory) : m_memory(memory){}

void GPU::updateGraphics(int cycles) {
    setLCDStatus();

    if (isLCDEnabled()) {
        scanLineCounter -= cycles;

        if (scanLineCounter <= 0) {
            scanLineCounter = 456;

            m_memory.setLY(m_memory.getLY() + 1);
            uint8_t current_line = m_memory.getLY();

            if (current_line < 144) {
                drawScanLine();
            }
            else if (current_line == 144) {
                // request v-blank interrupt
                m_memory.RequestInterrupt(0);
            }
            else if (current_line > 153) {
                m_memory.setLY(0);
            }
        }
    }
}

bool GPU::isLCDEnabled() {
    uint8_t lcdc = m_memory.getLCDC();
    return checkBit(lcdc, 7);
}

void GPU::setLCDStatus() {
    uint8_t status = m_memory.getSTAT();
    if (!isLCDEnabled()) {
        scanLineCounter = 456;
        m_memory.setLY(0);
        // change stat register to mode 1. bits 1-0: 01
        status = resetBit(status, 1);
        status = setBit(status, 0);
    }
    else {
        uint8_t current_line = m_memory.getLY();
        uint8_t current_mode = status & 0x3;    // mode is encoded in the 2 least bits of stat register
        uint8_t mode;
        bool reqInterrupt = false;

        // v-blank. when in v-blank set mode to 1
        if (current_line >= 144) {
            mode = 1;
            // change stat register to mode 1. bits 1-0: 01
            status = resetBit(status, 1);
            status = setBit(status, 0);
            // if bit 4 is 1 request an interrupt. bit 4 is 1 if interrupts for mode 1 are enabled
            reqInterrupt = checkBit(status, 4);
        }
        else {
            int mode2bounds = 456 - 80;     // first 80 cycles
            int mode3bounds = mode2bounds - 172;

            if (scanLineCounter >= mode2bounds) {
                mode = 2;
                // change stat register to mode 2. bits 1-0: 10
                status = setBit(status, 1);
                status = resetBit(status, 0);
                // if bit 5 is 1 request an interrupt. bit 5 is 1 if interrupts for mode 2 are enabled
                reqInterrupt = checkBit(status, 5);
            }
            else if (scanLineCounter >= mode3bounds) {
                mode = 3;
                // change stat register to mode 3. bits 1-0: 11
                status = setBit(status, 1);
                status = setBit(status, 0);
            }
            else {
                mode = 0;
                // change stat register to mode 0. bits 1-0: 00
                status = resetBit(status, 1);
                status = resetBit(status, 0);
                // if bit 3 is 1 request an interrupt. bit 3 is 1 if interrupts for mode 0 are enabled
                reqInterrupt = checkBit(status, 3);
            }
        }

        if (reqInterrupt && (mode != current_mode)) {
            // request lcdc interrupt
            // lcdc interrupt is in bit 1 (counting from 0)
            m_memory.RequestInterrupt(1);
        }

        // set/reset Coincidence Flag (bit 2 of STAT)
        // 1: LYC = LCDC LY
        // 0: LYC not equal to LCDC LY
        if (m_memory.getLY() == m_memory.getLYC()) {
            status = setBit(status, 2);
            if (checkBit(status, 6)) {
                // request lcdc interrupt
                // lcdc interrupt is in bit 1 (counting from 0)
                m_memory.RequestInterrupt(1);
            }
        }
        else {
            status = resetBit(status, 2);
        }
    }
    m_memory.setSTAT(status);
}

void GPU::drawScanLine() {
    uint8_t lcdc = m_memory.getLCDC();
    if (checkBit(lcdc, 0)) {
        drawBackground();
    }
    if (checkBit(lcdc, 1)) {
        drawSprites();
    }
}

void GPU::drawBackground() {

    uint16_t tile_map_address;

    uint16_t tileData;
    bool isUnsigned = true;

    uint8_t lcdc = m_memory.getLCDC();

    uint8_t scrollY = m_memory.getSCY();
    uint8_t scrollX = m_memory.getSCX();
    uint8_t windowY = m_memory.getWY();
    uint8_t windowX = m_memory.getWX();

    bool drawWindow = false;

    // if window is enabled
    if (checkBit(lcdc, 5)) {
        // if current scan line (LY) is inside the viewing area
        if (windowY <= m_memory.getLY()) {
            drawWindow = true;
        }
    }

    if (checkBit(lcdc, 4)) {
        tileData = 0x8000;
    }
    else {
        tileData = 0x8800;
        isUnsigned = false;
    }

    uint8_t bit = drawWindow ? 6 : 3;
    if (checkBit(lcdc, bit)) {
        tile_map_address = 0x9C00;
    }
    else {
        tile_map_address = 0x9800;
    }

    uint8_t posY;
    if (drawWindow) {
        posY = m_memory.getLY() - windowY;
    }
    else {
        posY = scrollY + m_memory.getLY();
    }

    uint16_t tileRow = (posY / 8) * 32;
    for (int pixel = 0; pixel < 160; pixel++) {
        uint8_t posX = pixel + scrollX;
        if (drawWindow) {
            posX = pixel - windowX;
        }

        uint8_t tileCol = posX / 8;
        int16_t tileNum;
        uint16_t tileLocation = tileData;

        uint16_t tileAddress = tile_map_address + tileRow + tileCol;
        if (isUnsigned) {
            tileNum = m_memory.readByte(tileAddress);
            tileLocation += (tileNum * 16);
        }
        else {
            tileNum = static_cast<int8_t>(m_memory.readByte(tileAddress));
            tileLocation += ((tileNum + 128) * 16);
        }

        uint8_t line = posY % 8;
        line *= 2;
        uint8_t data1 = m_memory.readByte(tileLocation + line);
        uint8_t data2 = m_memory.readByte(tileLocation + line + 1);

        int colourBit = posX % 8;
        colourBit -= 7;
        colourBit *= -1;

        int colourNum = getBit(data2, colourBit);
        colourNum <<= 1;
        colourNum |= getBit(data1, colourBit);

        Colour colour = getColour(colourNum, 0xFF47); // memory[0xFF47] = BG & Window Palette Data

        uint8_t ly = m_memory.getLY();
        if (ly >= 0 && ly <= 143 && pixel >= 0 && pixel <= 159) {
            pixels[pixel][ly] = colour;
        }
    }

}

// Each sprite has 4 bytes of attributes associated to it in the oam, these are
// 0: Sprite Y Position: Position of the sprite on the Y axis of the viewing display minus 16
// 1: Sprite X Position: Position of the sprite on the X axis of the viewing display minus 8
// 2: Pattern number: This is the sprite identifier used for looking up the sprite data in memory region 0x8000-0x8FFF
// 3: Attributes: These are the attributes of the sprite, discussed later.
void GPU::drawSprites() {

    bool sprite8x16 = false;

    uint8_t lcdc = m_memory.getLCDC();
    // if bit 2 is 1 sprite size is 8x16
    if (checkBit(lcdc, 2)) {
        sprite8x16 = true;
    }

    for (int sprite = 0 ; sprite < 40; sprite++) {
        // sprite takes 4 bytes in the sprite attributes table (OAM) address space: 0xFE00-0xFE9F
        uint8_t index = sprite * 4;
        uint8_t posY = m_memory.readByte(0xFE00 + index) - 16;
        uint8_t posX = m_memory.readByte(0xFE00 + index + 1) - 8;
        uint8_t tileLocation = m_memory.readByte(0xFE00 + index + 2);
        // attributes
        // Bit7: Sprite to Background Priority
        // Bit6: Y flip
        // Bit5: X flip
        // Bit4: Palette number
        // Bit3: Not used in standard gameboy
        // Bit2-0: Not used in standard gameboy
        uint8_t attributes = m_memory.readByte(0xFE00 + index + 3);

        bool flipY = checkBit(attributes, 6);
        bool flipX = checkBit(attributes, 5);

        uint8_t scanLine = m_memory.getLY();

        uint8_t sizeY = 8;
        if (sprite8x16) {
            sizeY = 16;
        }

        if ((scanLine >= posY) && (scanLine < (posY + sizeY))) {
            int line = scanLine - posY;
            // read the sprite in backwards in the y axis
            if (flipY) {
                line -= sizeY ;
                line *= -1 ;
            }

            line *= 2;
            uint16_t dataAddress = (0x8000 + (tileLocation * 16)) + line;
            uint8_t data1 = m_memory.readByte(dataAddress);
            uint8_t data2 = m_memory.readByte(dataAddress + 1);

            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
                int colourBit = tilePixel;
                if (flipX) {
                    colourBit -= 7;
                    colourBit *= -1;
                }

                int colourNum = getBit(data2,colourBit);
                colourNum <<= 1;
                colourNum |= getBit(data1,colourBit);


                uint16_t colourAddress = checkBit(attributes, 4) ? 0xFF49 : 0xFF48;
                Colour colour = getColour(colourNum, colourAddress);

                // white is transparent for sprites
                if (colour != Colour::White) {
                    int pixX = -tilePixel;
                    pixX += 7;
                    int pixel = posX + pixX ;
                    if (scanLine >= 0 && scanLine <= 143 && pixel >= 0 && pixel <= 159) {
                        pixels[pixel][scanLine] = colour;
                    }
                }
            }
        }
    }
}

Colour GPU::getColour(uint8_t colourNum, uint16_t address) {
    Colour colour = Colour::Black;
    uint8_t palette = m_memory.readByte(address);
    int hi, lo;

    switch (colourNum) {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 6; break;
    }

    int colourID = getBit(palette, hi);
    colourID <<= 1;
    colourID |= getBit(palette, lo);

    switch (colourID) {
        case 0: colour = Colour::White;      break;
        case 1: colour = Colour::Light_Grey; break;
        case 2: colour = Colour::Dark_Grey;  break;
        case 3: colour = Colour::Black;      break;
    }

    return colour;
}
