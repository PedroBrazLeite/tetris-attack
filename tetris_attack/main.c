#include <windows.h>
#include <gl/gl.h>
#include <gl/gl.h>
#include <time.h>
#include "SOIL.h"
#define TAM 0.1f
#include "func.h"


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
int matriz[10][20];


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          720, // dimensões da janelas
                          720,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    // execução de códigos que devem ficar fora do loop
    statusJogo = 1;
    srand(time(0));
    int i,j;
    for (i = 0; i < 10 ;i++)
    {
        for(j = 0; j < 10 ;j++)
        {
            matriz[i][j] = 5;
        }
    }

    for (i = 0; i < 10 ;i++)
    {
        for (j = 10; j < 20 ;j++)
        {
            matriz[i][j] = rand() % 5;
        }
    }
    muda();
    carregaTexturas();
    pont=0;

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            /* OpenGL animation code goes here */
            static int tempo = 0;
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            switch (statusJogo) {
                case 1:
                    desenhaInicio();
                    break;
                case 2:
                    tempo++;
                    tempo = tempo %400;
                    if(tempo == 0)
                    {
                        subir();
                    }
                    desenhaJogo(matriz);
                    gravidade();
                    apaga();
                    break;
                case 3:
                    desenhaDerrota();
                    break;
            }
            SwapBuffers(hDC);

            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;

                case VK_LEFT:
                    movimenta_esquerda();
                    break;

                    case VK_RIGHT:
                    movimenta_direita();
                    break;

                    case VK_UP:
                    movimenta_cima();
                    break;

                    case VK_DOWN:
                    movimenta_baixo();
                    break;

                    case VK_SPACE:
                    switch (statusJogo) {
                        int i, j;
                        case 1:
                            statusJogo = 2;
                            break;
                        case 2:
                            troca();
                            break;
                        case 3:
                            for (i = 0; i < 10 ;i++)
                            {
                                for(j = 0; j < 10 ;j++)
                                {
                                    matriz[i][j] = 5;
                                }
                            }

                            for (i = 0; i < 10 ;i++)
                            {
                                for (j = 10; j < 20 ;j++)
                                {
                                    matriz[i][j] = rand() % 5;
                                }
                            }
                            muda();
                            pont=0;
                            statusJogo = 2;
                            break;
                    }
                    break;

            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

