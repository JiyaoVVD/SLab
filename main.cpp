#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <graphics.h>
#include <conio.h>


typedef COLORREF Color;


#define WIDTH 640
#define HEIGHT 480


struct FrameBuffer{
    unsigned width;
    unsigned height;
    Color *buffer;
};


FrameBuffer& CreateFrameBuffer(unsigned width, unsigned height){
    auto *buffer = new Color[width * height]{0};
    auto *frameBuffer = new FrameBuffer{
            width,
            height,
            buffer
    };
    return *frameBuffer;
}


void RenderFrameBuffer(FrameBuffer& frameBuffer){
    for(int i = 0; i < frameBuffer.height; i++){
        for(int j = 0; j < frameBuffer.width; j++){
            putpixel(j, i, frameBuffer.buffer[i * frameBuffer.width + j]);
        }
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    initgraph(WIDTH, HEIGHT);

    FrameBuffer frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);

    while(!_kbhit()){
        RenderFrameBuffer(frameBuffer);
        Sleep(20);
    }
    closegraph();
    return 0;
}
