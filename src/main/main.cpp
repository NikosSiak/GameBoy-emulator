#include <iostream>
#include <Emulator/GameBoy.hpp>
#include <Joypad/Joypad.hpp>
#include <GPU/GPU.hpp>
#include <SDL2/SDL.h>

constexpr char const* WINDOW_TITLE = "GameBoy Emulator";
const int WINDOW_WIDTH = 160;
const int WINDOW_HEIGHT = 144;
const int RES_MULT = 4;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

SDL_Color darkest_blue  = {0, 26, 77, 255};
SDL_Color dark_blue     = {0, 51, 153, 255};
SDL_Color light_blue    = {0, 60, 179, 255};
SDL_Color lightest_blue = {0, 68, 204, 255};

bool windowInit();
void keyDown(SDL_Keycode key, Joypad &joypad);
void keyUp(SDL_Keycode key, Joypad &joypad);
void draw(GPU &gpu);
void setRenderColor(SDL_Color color);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Usage: GameBoy_emulator /path/to/rom" << std::endl;
        return 1;
    }

    GameBoy emulator;
    emulator.loadGame(argv[1]);

    Joypad &joypad = emulator.getJoypad();
    GPU &gpu = emulator.getGPU();

    if (!windowInit()) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    bool closeWindow = false;
    SDL_Event e;

    while (!closeWindow) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    closeWindow = true;
                    break;
                case SDL_KEYDOWN:
                    keyDown(e.key.keysym.sym, joypad);
                    break;
                case SDL_KEYUP:
                    keyUp(e.key.keysym.sym, joypad);
                    break;
                default:
                    break;
            }
        }
        emulator.emulate();
        draw(gpu);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

bool windowInit() {
    // TODO: init audio
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH * RES_MULT,
            WINDOW_HEIGHT * RES_MULT,
            0);

    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer" << std::endl;
        return false;
    }

    return true;
}

void keyDown(SDL_Keycode key, Joypad &joypad) {
    switch (key) {
        case SDLK_UP:       joypad.keyPress(Key::Up);       break;
        case SDLK_DOWN:     joypad.keyPress(Key::Down);     break;
        case SDLK_RIGHT:    joypad.keyPress(Key::Right);    break;
        case SDLK_LEFT:     joypad.keyPress(Key::Left);     break;
        case SDLK_z:        joypad.keyPress(Key::A);        break;
        case SDLK_x:        joypad.keyPress(Key::B);        break;
        case SDLK_SPACE:    joypad.keyPress(Key::Start);    break;
        case SDLK_LCTRL:    joypad.keyPress(Key::Select);   break;
    }
}

void keyUp(SDL_Keycode key, Joypad &joypad) {
    switch (key) {
        case SDLK_UP:       joypad.keyRelease(Key::Up);       break;
        case SDLK_DOWN:     joypad.keyRelease(Key::Down);     break;
        case SDLK_RIGHT:    joypad.keyRelease(Key::Right);    break;
        case SDLK_LEFT:     joypad.keyRelease(Key::Left);     break;
        case SDLK_z:        joypad.keyRelease(Key::A);        break;
        case SDLK_x:        joypad.keyRelease(Key::B);        break;
        case SDLK_SPACE:    joypad.keyRelease(Key::Start);    break;
        case SDLK_LCTRL:    joypad.keyRelease(Key::Select);   break;
    }
}

void draw(GPU &gpu) {
    SDL_Rect pixel;

    pixel.w = RES_MULT;
    pixel.h = RES_MULT;

    setRenderColor(darkest_blue);
    SDL_RenderClear(renderer);

    for (int i = 0; i < WINDOW_WIDTH; i++) {
        for (int j = 0; j < WINDOW_HEIGHT; j++) {
            pixel.x = i * RES_MULT;
            pixel.y = j * RES_MULT;
            Colour pixelColour = gpu.getPixelColour(i, j);
            switch (pixelColour) {
                case Colour::White:         setRenderColor(lightest_blue);  break;
                case Colour::Light_Grey:    setRenderColor(light_blue);     break;
                case Colour::Dark_Grey:     setRenderColor(dark_blue);      break;
                case Colour::Black:         setRenderColor(darkest_blue);   break;
            }
            SDL_RenderFillRect(renderer, &pixel);
        }
    }
    SDL_RenderPresent(renderer);
}

void setRenderColor(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
