#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <winuser.h>
#include <windef.h>

#define BUF_SIZE 256

TCHAR szName[] = TEXT("Global\MyFileMappingObject");
TCHAR szMsg[] = TEXT("You recieve memory mapped file text!");

//HWND GetConsoleHwnd()
//{
//    HWND hwnd;
//    wchar_t Old[200];
//    GetConsoleTitle(Old, 200);
//    SetConsoleTitle(L"Console");
//    Sleep(40);
//    hwnd = FindWindow(NULL, L"Console");
//    SetConsoleTitle(Old);
//    return hwnd;
//}

//typedef struct tagBITMAPINFOHEADER {
//    DWORD biSize;
//    LONG  biWidth;
//    LONG  biHeight;
//    WORD  biPlanes;
//    WORD  biBitCount;
//    DWORD biCompression;
//    DWORD biSizeImage;
//    LONG  biXPelsPerMeter;
//    LONG  biYPelsPerMeter;
//    DWORD biClrUsed;
//    DWORD biClrImportant;
//}BITMAPINFOHEADER, * PBITMAPINFOHEADER;


//Открытие БМП-файла, возвращает массив битовых данных и через указатель структуру BITMAPINFOHEADER с параметрами битовых данных
//unsigned char* OpenBitmap(LPCWSTR name, BITMAPINFOHEADER* retbih)
//{
//    HANDLE hbmp, mapfile;    //Хэндлы открытых файлов
//    BITMAPFILEHEADER* bfh;          //Файловый заголовок
//    BITMAPINFOHEADER* bih;       //Информационный заголовок файла
//    BITMAPINFO* bi;                 //BITMAPINFO
//    unsigned char* mf, * bits, * newbits;//указатели на блоки памяти
//    unsigned int size;              //Зазмер файла
//    unsigned int index;             //используется для преобразования индексного цвета в truecolor
//    unsigned int newsts, oldsts; //размер новой и старой строки битовой карты в байтах
//    hbmp = CreateFile(name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//открываем файл из командной строки
//    if (hbmp == INVALID_HANDLE_VALUE)  //Если не открылся
//    {
//        return 0;                            //возвращаем ошибку
//    }
//    size = GetFileSize(hbmp, NULL);    //получаем его размер
//    mapfile = CreateFileMapping(hbmp, NULL, PAGE_READWRITE, 0, size, L"MyFileMappingObject"); //получаем хэндл маппирования
//    mf = (unsigned char*)MapViewOfFile(mapfile, FILE_MAP_ALL_ACCESS, 0, 0, size);//Маппируем файл на участок памяти (то есть теперь файл представляет собой массим из байт в памяти)
//    bfh = (BITMAPFILEHEADER*)mf;//инициализируем указатель на файловый заголовок файла
//    if (bfh->bfType != 0x4d42)     //Если сигнатура файла не равна BM
//    {
//        return 0;                               //возвращаем ошибку
//    }
//    bih = (BITMAPINFOHEADER*)(mf + 0xe);            //инициализируем указатель на информационный заголовок файла
//    bi = (BITMAPINFO*)(mf + 0xe);                   //инициализируем указатель на структуру BITMAPINFO
//    bits = mf + bfh->bfOffBits;                     //массив с растровыми данными
//    oldsts = (bfh->bfSize - bfh->bfOffBits) / bih->biHeight;  //длина строки растра в байтах старого файла
//    newsts = bih->biWidth * 3;                              //длина строки растра в байтах нового файла
//    if ((newsts & 3) != 0)newsts = newsts & 0xfffffffc + 4;  //если размер стртки не кратен 4, то дополняем его до 4
//    newbits = new unsigned char[newsts * bih->biHeight];    //Массив нового растра
//    memset(newbits, 0, newsts * bih->biHeight);
//    for (int i = 0; i != bih->biHeight; i++)        //Идем по каждому пикселю
//        for (int j = 0; j != bih->biWidth; j++)
//        {
//            switch (bih->biBitCount)     //и в зависимости от глубины цвета преобразовываем при помощи разных алгоритмов
//            {
//            case 1:                           //для 1 бита на пиксель
//            {
//                index = (bits[i * oldsts + j / 8] >> (7 - j & 7)) & 1;          //получаем цвет пикселя, как индекс в палитре файла
//                newbits[i * newsts + j * 3] = bi->bmiColors[index].rgbBlue;       //Заполняем массив битовых данных соответствующим цветом из палитры
//                newbits[i * newsts + j * 3 + 1] = bi->bmiColors[index].rgbGreen;
//                newbits[i * newsts + j * 3 + 2] = bi->bmiColors[index].rgbRed;
//                break;
//            }
//            case 4:                    //для 4 бит на пиксель
//            {
//                index = (bits[i * oldsts + j / 2] >> ((1 - j % 2) * 4)) & 0xf; //получаем цвет пикселя, как индекс в палитре файла
//                newbits[i * newsts + j * 3] = bi->bmiColors[index].rgbBlue;       //Заполняем массив битовых данных соответствующим цветом из палитры
//                newbits[i * newsts + j * 3 + 1] = bi->bmiColors[index].rgbGreen;
//                newbits[i * newsts + j * 3 + 2] = bi->bmiColors[index].rgbRed;
//                break;
//            }
//            case 8:                   //для 8 бит на пиксель
//            {
//                index = bits[i * oldsts + j];                     //получаем цвет пикселя, как индекс в палитре файла
//                newbits[i * newsts + j * 3] = bi->bmiColors[index].rgbBlue;       //Заполняем массив битовых данных соответствующим цветом из палитры
//                newbits[i * newsts + j * 3 + 1] = bi->bmiColors[index].rgbGreen;
//                newbits[i * newsts + j * 3 + 2] = bi->bmiColors[index].rgbRed;
//                break;
//            }
//            case 16:                  //для 16 бит на пиксель
//            {
//                index = bits[i * oldsts + j * 2] + (bits[i * oldsts + j * 2 + 1] << 8);      //берем 16 бит цвета
//                newbits[i * newsts + j * 3] = (index & 0x1f) << 3;                     //разлагаем на составляющие (красный, зеленый, синий)
//                newbits[i * newsts + j * 3 + 1] = (index & 0x3e0) >> 2;                  //и преобразуем каждую 5-битную составляющую в 8-битную
//                newbits[i * newsts + j * 3 + 2] = (index & 0x7c00) >> 7;
//                break;
//            }
//            case 24:                 //для 24 бит на пиксель
//            {
//                newbits[i * newsts + j * 3] = bits[i * oldsts + j * 3];              //копируем цветовые составляющие из старого файла в новый
//                newbits[i * newsts + j * 3 + 1] = bits[i * oldsts + j * 3 + 1];
//                newbits[i * newsts + j * 3 + 2] = bits[i * oldsts + j * 3 + 2];
//                break;
//            }
//            }
//        }
//
//    retbih->biBitCount = 24;          //количество бит на пиксель
//    retbih->biCompression = BI_RGB;   //без компрессии
//    retbih->biSizeImage = newsts * bih->biHeight;   //размер изображения
//    retbih->biSize = sizeof(BITMAPINFOHEADER);    //размер заголовка
//    retbih->biWidth = bih->biWidth;       //ширина растра
//    retbih->biHeight = bih->biHeight;   //высота растра
//    retbih->biPlanes = 1;                 //количестао плоскостей, должно быть 1
//    retbih->biXPelsPerMeter = bih->biXPelsPerMeter;//разрешение пикселей на метр
//    retbih->biYPelsPerMeter = bih->biYPelsPerMeter;
//
//    UnmapViewOfFile(mf);                 //Закрываем маппирование
//    CloseHandle(mapfile);                //закрываем файл
//    CloseHandle(hbmp);                   //закрываем файл
//    return newbits;                     //возвращаем битовые данные
//}

int _tmain()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;
    
    HANDLE hPicture;
    HANDLE hBmpMappingFile;
    HANDLE hBmpMapFileAddr;
    HDC hdc;
    HDC hMemDC;
    HWND hDlg;
    RECT rWindow;
    BOOL bError;
    HWND hWnd;
    int IDC_BMPFRAME;

  /*  hWnd = CreateWindowEx(0, WC_DIALOG, L"Hello world", WS_VISIBLE | WS_SYSMENU, 500, 500, 200, 200, 0, 0, 0, 0);

    if (hWnd == NULL)
    {
        _tprintf(TEXT("Could not create file LOVE (%d).\n"),
            GetLastError());
        return 1;
    }

    hPicture = CreateFileA("LOVE.bmp", GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hPicture == NULL)
    {
        _tprintf(TEXT("Could not create file LOVE (%d).\n"),
            GetLastError());
        return 1;
    }

    SetConsoleTitle(L"MyConsole");*/

 /*   hDlg = FindWindow(NULL, L"MyConsole");

    if (hDlg == NULL)
    {
        _tprintf(TEXT("Could not find specified window (%d).\n"),
        GetLastError());
            return 1;
    }*/
    
   /* IDC_BMPFRAME = GetDlgCtrlID(hDlg);

   bError = GetClientRect(GetDlgItem(hDlg, IDC_BMPFRAME), &rWindow);

   if (bError == 0)
   {
       _tprintf(TEXT("Could not write window in rect struct (%d).\n"),
           GetLastError());
       return 1;
   }*/


    //hBmpMappingFile = CreateFileMappingA(
    //    hPicture, //описатель файла 
    //    NULL, //аттрибуты защиты объекта ядра 
    //    PAGE_READONLY, //атрибут защиты, присваиваемый 
    //   // страницам физической памяти 
    //    0,
    //    0,
    //    "BmpFile"); //имя объекта ядра 

    //if (hBmpMappingFile == NULL)
    //{
    //    _tprintf(TEXT("Could not create file Mapping for LOVE.BMP (%d).\n"),
    //        GetLastError());
    //    return 1;
    //}

    //hBmpMapFileAddr = MapViewOfFile(
    //    hBmpMappingFile, // описатель объекта «проекция файла» 
    //    FILE_MAP_READ, // вид доступа к данным 
    //    0,
    //    0, 
    //    0);

    //BITMAPFILEHEADER* bFileHeader = (BITMAPFILEHEADER*)hBmpMapFileAddr;

    //BITMAPINFO* bInfo = (BITMAPINFO*)((char*)hBmpMapFileAddr + 14);



    //hdc = GetDC(GetDlgItem(hDlg, IDC_BMPFRAME));
    //hPicture = CreateDIBitmap(hdc, &(bInfo->bmiHeader),
    //    CBM_INIT, (char*)hBmpMapFileAddr + bFileHeader->bfOffBits,
    //    bInfo, DIB_PAL_COLORS);
    //hMemDC = CreateCompatibleDC(hdc);
    //SelectObject(hMemDC, hPicture);
    //StretchBlt(hdc, 0, 0, rWindow.right, rWindow.bottom, hMemDC,
    //    0, 0, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight,
    //    SRCCOPY);
    //ReleaseDC(GetDlgItem(hDlg, IDC_BMPFRAME), hdc);
    //DeleteDC(hMemDC);
    //DeleteObject(hPicture);
    //return (INT_PTR)TRUE;

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        BUF_SIZE,                // maximum object size (low-order DWORD)
        szName);                 // name of mapping object

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        BUF_SIZE);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }


    CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));

    _getch();

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return 0;
}