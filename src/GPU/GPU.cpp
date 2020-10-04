//
// Created by nikos on 12-Aug-19.
//

#include <GPU/GPU.hpp>
#include <GPU/Colour.hpp>
#include <GPU/Tile.hpp>
#include <Memory/Memory.hpp>
#include <utils/bitoperations.hpp>

GPU::GPU(Memory &memory) : m_memory(memory){
    for (int i = 0; i < GAMEBOY_WIDTH; i++)
        for (int j = 0; j < GAMEBOY_HEIGHT; j++)
            pixels[i][j] = Colour::White;
}

void GPU::updateGraphics(int cycles) {

    cyclesCounter += cycles;
    uint8_t status = m_memory.getSTAT();

    switch (currentMode) {
        case Mode::ACCESS_OAM:
            if (cyclesCounter >= CLOCKS_PER_SCANLINE_OAM) {
                cyclesCounter %= CLOCKS_PER_SCANLINE_OAM;
                status = setBit(status, 1);
                status = setBit(status, 0);
                currentMode = Mode::ACCESS_VRAM;
            }
            break;
        case Mode::ACCESS_VRAM:
            if (cyclesCounter >= CLOCKS_PER_SCANLINE_VRAM) {
                cyclesCounter %= CLOCKS_PER_SCANLINE_VRAM;
                currentMode = Mode::HBLANK;

                if (checkBit(status, 3))
                    m_memory.requestInterrupt(1);

                bool lyCoincidenceInterrupt = checkBit(status, 6);
                bool lyCoincidence = checkBit(status, 2);
                if (lyCoincidenceInterrupt && lyCoincidence)
                    m_memory.requestInterrupt(1);

                status = resetBit(status, 1);
                status = resetBit(status, 0);
            }
            break;
        case Mode::HBLANK:
            if (cyclesCounter >= CLOCKS_PER_HBLANK) {
                drawScanLine();
                m_memory.setLY(m_memory.getLY() + 1);

                cyclesCounter %= CLOCKS_PER_HBLANK;

                if (m_memory.getLY() == 144) {
                    currentMode = Mode::VBLANK;
                    status = resetBit(status, 1);
                    status = setBit(status, 0);
                    m_memory.requestInterrupt(0);
                }
                else {
                    currentMode = Mode::ACCESS_OAM;
                    status = setBit(status, 1);
                    status = resetBit(status, 0);
                }
            }
            break;
        case Mode::VBLANK:
            if (cyclesCounter >= CLOCKS_PER_SCANLINE) {
                m_memory.setLY(m_memory.getLY() + 1);

                cyclesCounter %= CLOCKS_PER_SCANLINE;

                if (m_memory.getLY() == 154) {
                    drawSprites();
                    m_memory.setLY(0);
                    currentMode = Mode::ACCESS_OAM;
                    status = setBit(status, 1);
                    status = resetBit(status, 0);
                }
            }
            break;
    }

    m_memory.setSTAT(status);
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
            m_memory.requestInterrupt(1);
        }

        // set/reset Coincidence Flag (bit 2 of STAT)
        // 1: LYC = LCDC LY
        // 0: LYC not equal to LCDC LY
        if (m_memory.getLY() == m_memory.getLYC()) {
            status = setBit(status, 2);
            if (checkBit(status, 6)) {
                // request lcdc interrupt
                // lcdc interrupt is in bit 1 (counting from 0)
                m_memory.requestInterrupt(1);
            }
        }
        else {
            status = resetBit(status, 2);
        }
    }
    m_memory.setSTAT(status);
}

void GPU::drawScanLine() {
    if (!isLCDEnabled())
        return;

    uint8_t lcdc = m_memory.getLCDC();
    if (checkBit(lcdc, 0)) {
        drawBackground();
    }

    if (checkBit(lcdc, 5)) {
        drawWindow();
    }
}

void GPU::drawBackground() {

    uint16_t tileMapAddress;
    uint16_t tileDataAddress;
    bool isUnsigned = true;

    uint8_t lcdc = m_memory.getLCDC();

    uint8_t scrollY = m_memory.getSCY();
    uint8_t scrollX = m_memory.getSCX();
    uint8_t line = m_memory.getLY();

    bool useTileSetZero = checkBit(lcdc, 4);

    if (useTileSetZero) {
        tileDataAddress = 0x8000;
    }
    else {
        tileDataAddress = 0x8800;
        isUnsigned = false;
    }

    if (checkBit(lcdc, 3)) {
        tileMapAddress = 0x9C00;
    }
    else {
        tileMapAddress = 0x9800;
    }

    for (uint8_t pixel = 0; pixel < GAMEBOY_WIDTH; pixel++) {
        uint32_t posX = pixel + scrollX;
        uint32_t posY = line + scrollY;

        uint32_t bgMapX = posX % 256;
        uint32_t bgMapY = posY % 256;

        uint32_t tileX = bgMapX / 8;
        uint32_t tileY = bgMapY / 8;

        uint32_t tilePixelX = bgMapX % 8;
        uint32_t tilePixelY = bgMapY % 8;

        uint32_t tileIndex = tileY * 32 + tileX;
        uint16_t tileIDAddress = tileMapAddress + tileIndex;

        uint8_t tileID = m_memory.readByte(tileIDAddress);

        uint32_t tileDataMemOffset = useTileSetZero ? tileID * 16 : (static_cast<int8_t>(tileID) + 128) * 16;
        uint32_t tileDataLineOffset = tilePixelY * 2;

        uint16_t tileLineDataStartAddress = tileDataAddress + tileDataMemOffset + tileDataLineOffset;

        uint8_t pixel1 = m_memory.readByte(tileLineDataStartAddress);
        uint8_t pixel2 = m_memory.readByte(tileLineDataStartAddress + 1);

        auto colorID = static_cast<uint8_t>((getBit(pixel2, 7 - tilePixelX) << 1) | getBit(pixel1, 7 - tilePixelX));
        pixels[pixel][line] = getColour(colorID);
    }
}

void GPU::drawWindow() {
    uint8_t lcdc = m_memory.getLCDC();
    bool useTileSetZero = checkBit(lcdc, 4);

    uint16_t tileSetAddress = useTileSetZero ? 0x8000 : 0x8800;
    uint16_t tileMapAddress =  checkBit(lcdc, 6) ? 0x9C00 : 9800;

    uint8_t line = m_memory.getLY();
    uint8_t scrolledY = line - m_memory.getWY();

    if (scrolledY >= GAMEBOY_HEIGHT) {
        return;
    }

    uint8_t windowX = m_memory.getWY();
    for (uint8_t pixel = 0; pixel < GAMEBOY_WIDTH; pixel++) {
        uint32_t scrolledX = pixel + windowX - 7;

        uint32_t tileX = scrolledX / 8;
        uint32_t tileY = scrolledY / 8;

        uint32_t tilePixelX = scrolledX % 8;
        uint32_t tilePixelY = scrolledY % 8;

        uint32_t tileIndex = tileY * 32 + tileX;

        uint16_t tileIDAddress = tileMapAddress + tileIndex;

        uint8_t tileID = m_memory.readByte(tileIDAddress);

        uint32_t tileDataMemOffset = useTileSetZero ? tileID * 16 : (static_cast<int8_t>(tileID) + 128) * 16;

        uint32_t tileDataLineOffset = tilePixelY * 2;

        uint16_t tileLineDataStartAddress = tileSetAddress + tileDataMemOffset + tileDataLineOffset;

        uint8_t pixel1 = m_memory.readByte(tileLineDataStartAddress);
        uint8_t pixel2 = m_memory.readByte(tileLineDataStartAddress + 1);

        auto colorID = static_cast<uint8_t>((getBit(pixel2, 7 - tilePixelX) << 1) | getBit(pixel1, 7 - tilePixelX));
        pixels[pixel][line] = getColour(colorID);
    }
}

// Each sprite has 4 bytes of attributes associated to it in the oam, these are
// 0: Sprite Y Position: Position of the sprite on the Y axis of the viewing display minus 16
// 1: Sprite X Position: Position of the sprite on the X axis of the viewing display minus 8
// 2: Pattern number: This is the sprite identifier used for looking up the sprite data in memory region 0x8000-0x8FFF
// 3: Attributes: These are the attributes of the sprite.
void GPU::drawSprites() {

    uint8_t sizeMultiplier = 1;

    uint8_t lcdc = m_memory.getLCDC();
    // if bit 2 is 1 sprite size is 8x16
    if (checkBit(lcdc, 2)) {
        sizeMultiplier = 2;
    }

    for (int sprite = 0 ; sprite < 40; sprite++) {
        // sprite takes 4 bytes in the sprite attributes table (OAM) address space: 0xFE00-0xFE9F
        uint8_t index = sprite * 4;
        uint8_t posY = m_memory.readByte(0xFE00 + index);
        uint8_t posX = m_memory.readByte(0xFE00 + index + 1);

        if (posY == 0 || posY >= 160) return;
        if (posX == 0 || posX >= 168) return;

        uint16_t tileSetLocation = 0x8000;

        uint8_t tileLocation = m_memory.readByte(0xFE00 + index + 2);
        // attributes
        // Bit7: Sprite to Background Priority
        // Bit6: Y flip
        // Bit5: X flip
        // Bit4: Palette number
        // Bit3: Not used in standard gameboy
        // Bit2-0: Not used in standard gameboy
        uint8_t attributes = m_memory.readByte(0xFE00 + index + 3);

//        bool usePalette1 = checkBit(attributes, 4);
        bool flipX = checkBit(attributes, 5);
        bool flipY = checkBit(attributes, 6);
        bool behindBg = checkBit(attributes, 7);

        uint32_t tileOffset = tileLocation * 16;
        uint16_t patternAddress = tileSetLocation + tileOffset;


        Tile tile(patternAddress, m_memory, sizeMultiplier);
        int startY = posY - 16;
        int startX = posX - 8;

        for (uint32_t y = 0; y < TILE_HEIGHT_PX * sizeMultiplier; y++) {
            for (uint32_t x = 0; x < TILE_WIDTH_PX; x++) {
                uint32_t maybeFlippedY = !flipY ? y : (TILE_HEIGHT_PX * sizeMultiplier) - y - 1;
                uint32_t maybeFlippedX = !flipX ? x : TILE_WIDTH_PX - x - 1;

                Colour colour = tile.getPixel(maybeFlippedX, maybeFlippedY);

                if (colour == Colour::White) continue;

                int screenX = startX + x;
                int screenY = startY +y;

                if (!(screenX < GAMEBOY_WIDTH && screenY < GAMEBOY_HEIGHT)) continue;

                if (behindBg) continue;

                pixels[screenX][screenY] = colour;
            }
        }

//        uint8_t scanLine = m_memory.getLY();
//
//        uint8_t sizeY = 8;
//        if (sprite8x16) {
//            sizeY = 16;
//        }
//
//        if ((scanLine >= posY) && (scanLine < (posY + sizeY))) {
//            int line = scanLine - posY;
//            // read the sprite in backwards in the y axis
//            if (flipY) {
//                line -= sizeY ;
//                line *= -1 ;
//            }
//
//            line *= 2;
//            uint16_t dataAddress = (0x8000 + (tileLocation * 16)) + line;
//            uint8_t data1 = m_memory.readByte(dataAddress);
//            uint8_t data2 = m_memory.readByte(dataAddress + 1);
//
//            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
//                int colourBit = tilePixel;
//                if (flipX) {
//                    colourBit -= 7;
//                    colourBit *= -1;
//                }
//
//                int colourNum = getBit(data2,colourBit);
//                colourNum <<= 1;
//                colourNum |= getBit(data1,colourBit);
//
//
//                uint16_t colourAddress = checkBit(attributes, 4) ? 0xFF49 : 0xFF48;
//                Colour colour = getColour(colourNum, colourAddress);
//
//                // white is transparent for sprites
//                if (colour != Colour::White) {
//                    int pixX = -tilePixel;
//                    pixX += 7;
//                    int pixel = posX + pixX ;
//                    if (scanLine >= 0 && scanLine <= 143 && pixel >= 0 && pixel <= 159) {
//                        pixels[pixel][scanLine] = colour;
//                    }
//                }
//            }
//        }
    }
}

//Colour GPU::getColour(uint8_t colourNum, uint16_t address) {
//    Colour colour = Colour::Black;
//    uint8_t palette = m_memory.readByte(address);
//    int hi, lo;
//
//    switch (colourNum) {
//        case 0: hi = 1; lo = 0; break;
//        case 1: hi = 3; lo = 2; break;
//        case 2: hi = 5; lo = 4; break;
//        case 3: hi = 7; lo = 6; break;
//    }
//
//    int colourID = getBit(palette, hi);
//    colourID <<= 1;
//    colourID |= getBit(palette, lo);
//
//    switch (colourID) {
//        case 0: colour = Colour::White;      break;
//        case 1: colour = Colour::Light_Grey; break;
//        case 2: colour = Colour::Dark_Grey;  break;
//        case 3: colour = Colour::Black;      break;
//    }
//
//    return colour;
//
//    switch (colourNum) {
//        case 0: return Colour::White;
//        case 1: return Colour::Light_Grey;
//        case 2: return Colour::Dark_Grey;
//        default: return Colour::Black;
//    }
//}

Colour GPU::getPixelColour(int i, int j) {
    return pixels[i][j];
}
