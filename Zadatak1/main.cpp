#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

void MoveSpaceship(RECT*);
void Draw(HDC, RECT*);
void MoveAsteroid(RECT*);

const int ID_TIMER1 = 1;
const int ID_TIMER2 = 2;

int i = 0;
typedef struct ObjectInfo
{
    int width;
    int height;
    int x;
    int y;

    int dx;
    int dy;
} Object;

Object Sky;
Object Asteroid;
Object Spaceship;

HBITMAP hbmSky;
HBITMAP hbmSpaceship;
HBITMAP hbmSpaceshipMask;
HBITMAP hbmAsteroid ;
HBITMAP hbmAsteroidMask;

int SpaceshipCX = 0;
int SpaceshipCY = 0;

int AsteroidCX = 0;
int AsteroidCY = 0;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_VREDRAW | CS_HREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("eldar_haseljic_zadaca3"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)                  /* handle the messages */
    {
    case WM_SIZE:
    {
        BITMAP bitmap;
        SetTimer(hwnd, ID_TIMER1, 10, NULL);
        hbmSky = (HBITMAP)LoadImage(NULL,"sky.bmp",IMAGE_BITMAP,LOWORD(lParam),HIWORD(lParam),LR_LOADFROMFILE);
        hbmSpaceship = (HBITMAP)LoadImage(NULL,"spaceship_black.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        hbmSpaceshipMask = (HBITMAP)LoadImage(NULL,"spaceship_white.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

        SetTimer(hwnd,ID_TIMER2, 10000, NULL);
        hbmAsteroid = (HBITMAP)LoadImage(NULL,"asteroid_black.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        hbmAsteroidMask = (HBITMAP)LoadImage(NULL,"asteroid_white.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

        GetObject(hbmSpaceship,sizeof(BITMAP),&bitmap);

        Spaceship.width = bitmap.bmWidth/3;
        Spaceship.height = bitmap.bmHeight;
        Spaceship.dx = (LOWORD(lParam)-bitmap.bmWidth/3)/2;
        Spaceship.y = HIWORD(lParam)-bitmap.bmHeight;
        Spaceship.dy = 1;

        GetObject(hbmAsteroid,sizeof(bitmap),&bitmap);
        Asteroid.width = bitmap.bmWidth/2;
        Asteroid.height = bitmap.bmHeight/2;
        Asteroid.x = (LOWORD(lParam)-bitmap.bmWidth/2);
        Asteroid.y = 0;

        GetObject(hbmSky,sizeof(bitmap),&bitmap);

        Sky.width = bitmap.bmWidth;
        Sky.height = bitmap.bmHeight;
        Sky.dx = 0;
        Sky.dy = 0;

        break;
    }
    case WM_TIMER:
    {
        RECT clientRectangle;
        HDC hdc;
        switch(wParam)
        {
        case ID_TIMER1:
        {
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &clientRectangle);

            MoveSpaceship(&clientRectangle);
            MoveAsteroid(&clientRectangle);
            Draw(hdc, &clientRectangle);

            ReleaseDC(hwnd, hdc);

            ++i;
            if(i == 5)
                i =0;
            break;
        }
        case ID_TIMER2:
        {
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &clientRectangle);
            Asteroid.x = clientRectangle.right-Asteroid.width ;
            Asteroid.y = clientRectangle.top ;
            ReleaseDC(hwnd, hdc);
            break;
        }
        }
        break;
    }
    case WM_KEYDOWN:
    {
        switch(wParam)
        {
        case VK_DOWN:
            if(Spaceship.dy > 1)
                --Spaceship.dy;
            break;
        case VK_UP:
            ++Spaceship.dy;
            break;
        case VK_LEFT:
            if(Spaceship.x>0)
                Spaceship.dx -= 15;
            break;
        case VK_RIGHT:
            RECT rect;
            GetClientRect(hwnd,&rect);
            if(Spaceship.x < rect.right - Spaceship.width)
                Spaceship.dx += 15;
            break;

        }
        break;
    }
    case WM_DESTROY:
        KillTimer(hwnd,ID_TIMER1);
        KillTimer(hwnd,ID_TIMER2);
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void MoveAsteroid(RECT* prect)
{
    --Asteroid.x;
    ++Asteroid.y;
}

void MoveSpaceship(RECT* prect)
{
    Spaceship.x = Spaceship.dx;
    Spaceship.y -= Spaceship.dy;
    if(Spaceship.y < prect->top-Spaceship.height)
    {
        Spaceship.y=prect->bottom;
    }
}

void Draw(HDC hdc, RECT* prect)
{
    HDC hdcFirstHelp = CreateCompatibleDC(hdc);
    HBITMAP hbmFirstHelpBitmap = CreateCompatibleBitmap(hdc, prect->right, prect->bottom);
    HBITMAP hbmSaveFirstHelpBitmap = (HBITMAP)SelectObject(hdcFirstHelp,hbmFirstHelpBitmap);

    HDC hdcSecondHelp = CreateCompatibleDC(hdc);

    HBITMAP hbmSecondHelpBitmap = (HBITMAP)SelectObject(hdcSecondHelp, hbmSky);
    BitBlt(hdcFirstHelp, Sky.x, Sky.y, Sky.width, Sky.height, hdcSecondHelp, 0, 0, SRCCOPY);

    (HBITMAP)SelectObject(hdcSecondHelp, hbmSpaceshipMask);
    BitBlt(hdcFirstHelp, Spaceship.x, Spaceship.y,  Spaceship.width,  Spaceship.height, hdcSecondHelp, SpaceshipCX* Spaceship.width, SpaceshipCY*Spaceship.height, SRCAND);

    (HBITMAP)SelectObject(hdcSecondHelp, hbmSpaceship);
    BitBlt(hdcFirstHelp,Spaceship.x, Spaceship.y,  Spaceship.width,  Spaceship.height, hdcSecondHelp, SpaceshipCX* Spaceship.width, SpaceshipCY*Spaceship.height, SRCPAINT);

    SpaceshipCX++;
    if(SpaceshipCX==3)
    {
        SpaceshipCX=0;
    }

    (HBITMAP)SelectObject(hdcSecondHelp, hbmAsteroidMask);
    BitBlt(hdcFirstHelp, Asteroid.x, Asteroid.y,  Asteroid.width,  Asteroid.height, hdcSecondHelp, AsteroidCX* Asteroid.width, AsteroidCY*Asteroid.height, SRCAND);

    (HBITMAP)SelectObject(hdcSecondHelp, hbmAsteroid);
    BitBlt(hdcFirstHelp,Asteroid.x, Asteroid.y,  Asteroid.width,  Asteroid.height, hdcSecondHelp, AsteroidCX* Asteroid.width, AsteroidCY*Asteroid.height,  SRCPAINT);

    if(i==2)
    {

        AsteroidCX++;
        if(AsteroidCX == 2)
        {
            AsteroidCY++;
            if(AsteroidCY == 2)
                AsteroidCY = 0;
            AsteroidCX = 0;
        }
    }
    SelectObject(hdcSecondHelp, hbmSecondHelpBitmap);
    DeleteDC(hdcSecondHelp);

    BitBlt(hdc, 0, 0, prect->right, prect->bottom, hdcFirstHelp, 0, 0, SRCCOPY);

    SelectObject(hdcFirstHelp, hbmSaveFirstHelpBitmap);
    DeleteDC(hdcFirstHelp);
    DeleteObject(hbmFirstHelpBitmap);
}
