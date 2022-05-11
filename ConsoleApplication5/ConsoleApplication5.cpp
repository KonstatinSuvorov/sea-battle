#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<conio.h>
#include<clocale>
#include<stdlib.h>
#include<ctime>
#include<iomanip>
#include<cmath>
#include<stdio.h>
#include<windows.h>

enum ConsoleColor { Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };

void SetColor(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

struct
{
    int* strPalubKorabl;//адрес первой палубы
    int pologKorabliy;//положение кораб.
    int palubu;//количество палуб
}Chet, Try1, Try2, Dwa1, Dwa2, Dwa3, ChetP, Try1P, Try2P, Dwa1P, Dwa2P, Dwa3P;//корабли от 4-2--

const int sizeRow = 12, sizeCol = 12;
int PoleIgroka[sizeRow][sizeCol] = { 0 };
int PoleKopuyt[sizeRow][sizeCol] = { 0 };
int* adresaKorabl[sizeRow * sizeCol];//адреса кораблей пользователя
int* adresaKorablPC[sizeRow * sizeCol];
int* adresaHodovPC[sizeRow * sizeCol];
int index = 0;//идекс дла адресов пользователя
int indexPC = 0, indexHodPC = 0;//индекс адресов кораб.РС    -----   индекс адресов ходов РС
int oshibRuchVoda = 0;
int ranen = 0, counterRanen = 1, flag = 0;//ранен или нет ----- количество ранений -----
int* ptrPopadPC1;//адрес первой раненой палубы
int strok = 1, stolb = 1, prohod = 1;//дя стрельбы РС через клетку
int countIG = 0, countPC = 0;//счетчик для определения победителя

int OpredPobed();
int perevod(char);
int poiskAdresa(int*, int, int);//поиск адрессов пользователя
void print();
void printComp();
void HodPolz();
void HodPC();

void main()
{
    srand((unsigned)time(NULL));
    setlocale(LC_CTYPE, "rus");

    void pointKorabl(int, int);//автоматическая растановка кораблей пользователя
    void pointKorablVruchnuy(int, int);//ручная растановка кораблей пользователя
    void pointKorablKomp(int, int);//автоматическая растановка кораблей РС

    for (int i = 0; i < sizeRow; i++) //запись 2(мимо)по периметру поля
    {
        PoleIgroka[i][0] = 2;
        adresaHodovPC[indexHodPC] = &PoleIgroka[i][0];//запись в хода РС чтобы туда уже не ходил
        indexHodPC++;
        PoleIgroka[i][11] = 2;
        adresaHodovPC[indexHodPC] = &PoleIgroka[i][11];
        indexHodPC++;
    }
    for (int j = 1; j < sizeCol - 1; j++)//запись 2(мимо)по периметру поля
    {
        PoleIgroka[0][j] = 2;
        adresaHodovPC[indexHodPC] = &PoleIgroka[0][j];
        indexHodPC++;
        PoleIgroka[11][j] = 2;
        adresaHodovPC[indexHodPC] = &PoleIgroka[11][j];
        indexHodPC++;
    }

    SetColor(White, Black);
    cout << "\n\t\t\tИГРА \"МОРСКОЙ БОЙ\"\n\n";
    cout << "Зеленые прямоугольники  ";
    SetColor(Green, Green);
    cout << "0 ";
    SetColor(White, Black);
    cout << "  Ваши корабли\n";
    cout << "Желтый крест  ";
    SetColor(Yellow, Black);
    cout << 'x';
    SetColor(White, Black);
    cout << "  \"МИМО\"\n";
    cout << "Красный крест  ";
    SetColor(LightRed, Black);
    cout << 'X';
    SetColor(White, Black);
    cout << "  \"ПОПАДАНИЕ\"\n\n";
    char hod[10];//первый ход
    char Vuborrastanov[10];//растановка кораблей игрока /атомат или в ручную
    int paluba = 4, colich = 1;//начало расстановки с 4-х палубного кор.
    print();

    int vubor = 0, hod1 = 0;//для перевода строк в цыфры

    do {
        SetColor(LightGreen, Black);
        cout << "\nВыберети растановку кораблей  - Вручную(1)  - Автоматически(2) __  ";
        SetColor(White, Black);
        cin.clear();
        cin.getline(Vuborrastanov, 9);
        vubor = atoi(Vuborrastanov);
        if (vubor < 1 || vubor>2)
        {
            SetColor(White, LightRed);
            cout << "\n Неверный выбор \n";
            SetColor(White, Black);
        }
    } while (vubor < 1 || vubor>2);

    if (vubor == 1)
    {
        pointKorablVruchnuy(paluba, colich);
    }

    else if (vubor == 2)
    {
        pointKorabl(paluba, colich);
    }
    print();
    pointKorablKomp(paluba, colich);
    printComp();
    do {
        SetColor(LightGreen, Black);
        cout << "\nКто ходит первым Компьютер(1)  Пользователь(2) __  ";
        SetColor(White, Black);
        cin.clear();
        cin.sync();
        cin.getline(hod, 9);
        hod1 = atoi(hod);
        if (hod1 < 1 || hod1>2)
        {
            SetColor(White, LightRed);
            cout << "\n Неверный выбор \n";
            SetColor(White, Black);
        }
    } while (hod1 < 1 || hod1>2);
    if (hod1 == 2)
        HodPolz();
    else if (hod1 == 1)
        HodPC();
    system("pause");
    return;
}

void pointKorabl(int pal, int colum)//автоматическая растановка кораблей пользователя
{
    if (!pal)//палуб 0 - выход
        return;
    int pervayPal = 1;//для записи в структуру
    for (int j = 1; j <= colum; j++)//j количество кораблей одного типа
    {
        int* ptr;//указывает на адрес палубы 
        int row, col;//строка - столбец массива
        int gorver = 1 + rand() % 2;//по горизонтали или по вертикали
        int k = 1, i = pal;//k счетчик i текущяя палуба
        if (gorver == 1)//вертикаль
        {
            do {
                do {
                    row = rand() % 10 + 1;
                } while (row + pal > 11);//чтобы не вышел за пределы массива
                col = rand() % 10 + 1;
                ptr = &PoleIgroka[row][col];
            } while (!poiskAdresa(ptr, pal, gorver));//проверка записан ли такой адресс           
        }
        else if (gorver == 2)//горизонталь
        {
            do {
                do {
                    col = rand() % 10 + 1;
                } while (col + pal > 11);
                row = rand() % 10 + 1;
                ptr = &PoleIgroka[row][col];
            } while (!poiskAdresa(ptr, pal, gorver));//-----------------//вызвать ф-ю        
        }

        switch (gorver)
        {
        case 1:     //вертикаль            
            while (k <= i)//k счетчик i текущяя палуба
            {
                PoleIgroka[row][col] = 1;
                if (pal == 4 && pervayPal == 1)
                {
                    ChetP.strPalubKorabl = &PoleIgroka[row][col];     //запись в структуру адреса только самой первой палубы
                    ChetP.pologKorabliy = 1;//положение корабля
                    ChetP.palubu = 4;//кол. палуб
                    pervayPal++;//чтобы больше не зап. адреса остальн. палуб
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1P.strPalubKorabl = &PoleIgroka[row][col];
                    Try1P.pologKorabliy = 1;
                    Try1P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2P.strPalubKorabl = &PoleIgroka[row][col];
                    Try2P.pologKorabliy = 1;
                    Try2P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa1P.pologKorabliy = 1;
                    Dwa1P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa2P.pologKorabliy = 1;
                    Dwa2P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa3P.pologKorabliy = 1;
                    Dwa3P.palubu = 2;
                    pervayPal++;
                }
                row++;
                i--;
            }
            break;
        case 2:         //горизонталь                
            while (k <= i)
            {
                PoleIgroka[row][col] = 1;
                if (pal == 4 && pervayPal == 1)
                {
                    ChetP.strPalubKorabl = &PoleIgroka[row][col]; //запись в структуру адреса только самой первой палубы
                    ChetP.pologKorabliy = 2;//положение корабля
                    ChetP.palubu = 4;//кол. палуб
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1P.strPalubKorabl = &PoleIgroka[row][col];
                    Try1P.pologKorabliy = 2;
                    Try1P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2P.strPalubKorabl = &PoleIgroka[row][col];
                    Try2P.pologKorabliy = 2;
                    Try2P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa1P.pologKorabliy = 2;
                    Dwa1P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa2P.pologKorabliy = 2;
                    Dwa2P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa3P.pologKorabliy = 2;
                    Dwa3P.palubu = 2;
                    pervayPal++;
                }
                col++;
                i--;
            }
            break;
        }
    }
    pointKorabl(pal - 1, colum + 1);//уменшение палуб - увеличение кол. кор.
}

void print()
{
    if (flag)
        system("cls");
    flag = 1;
    SetColor(LightGreen, Black);
    cout << "\t\t Поле Игрока\n\n";
    SetColor(White, Black);
    SetColor(White, Black);
    cout << "     a   b   c   d   e   f   g   h   i   j\n";
    cout << "   + - + - + - + - + - + - + - + - + - + - +\n";
    for (int i = 1; i < sizeRow - 1; i++)
    {
        cout << setw(2) << i << " ";
        for (int j = 1; j < sizeCol - 1; j++)
        {
            if (!(PoleIgroka[i][j])) {
                cout << "| " << "  ";
                SetColor(White, Black);
            }
            else if (PoleIgroka[i][j] == 1)//если 1 нарисовать корабль
            {
                cout << "|";
                SetColor(Green, Green);
                cout << '0' << "  ";
                SetColor(White, Black);
            }
            else if (PoleIgroka[i][j] == 2)//если 2 мимо желтый крест
            {
                cout << "| ";
                SetColor(Yellow, Black);
                cout << 'x' << " ";
                SetColor(White, Black);
            }
            else if (PoleIgroka[i][j] == 3)//если 3 ранен или убит крас крест
            {
                cout << "|";
                SetColor(LightRed, Black);
                cout << " " << 'X' << " ";
                SetColor(White, Black);
            }
            if (j == 10)
                cout << "|";
        }
        cout << endl;
        cout << "   + - + - + - + - + - + - + - + - + - + - +\n";
    }
}

//----------------------------------------------
int poiskAdresa(int* ptrKorabl, int palub, int vergor)//ищет есть совпадение адресов есть 0 или нет 1
{
    void zapAdres(int*, int, int);//запись адрессов пользователя
    for (int a = 1; a <= palub; a++)//проверка каждой палубы 
    {
        if (vergor == 1)
        {
            for (int i = 0; i < sizeRow * sizeCol; i++)
            {
                if (ptrKorabl == adresaKorabl[i])
                {
                    oshibRuchVoda = 1;// только для ручного ввода
                    return 0;
                }
            }
            *(ptrKorabl += 12);
        }
        else if (vergor == 2)
        {
            for (int i = 0; i < sizeRow * sizeCol; i++)
            {
                if (ptrKorabl == adresaKorabl[i])
                {
                    oshibRuchVoda = 1;// только для ручного ввода
                    return 0;
                }
            }
            *(ptrKorabl += 1);
        }
    }
    zapAdres(ptrKorabl, palub, vergor);//нет совпадений записывает адраса
    return 1;
}

//--------------------------------
void zapAdres(int* pKorabl, int palu, int pologen)//записывает адреса кор. и вокруг них
{
    if (pologen == 1)//если корабль по вертикали
    {
        int* pKorabl1 = &*(pKorabl - 1);
        int* pKorabl2 = &*(pKorabl + 1);
        for (int i = 0; i < palu + 2; i++)//обвод корабля
        {
            adresaKorabl[index] = &*pKorabl;
            index++;
            *(pKorabl -= 12);
            adresaKorabl[index] = &*pKorabl1;
            index++;
            *(pKorabl1 -= 12);
            adresaKorabl[index] = &*pKorabl2;
            index++;
            *(pKorabl2 -= 12);
        }
    }
    else if (pologen == 2)//по горизонтали
    {
        int* pKorabl1 = &*(pKorabl + 12);
        int* pKorabl2 = &*(pKorabl - 12);
        for (int i = 0; i < palu + 2; i++)//обвод корабля
        {
            adresaKorabl[index] = &*pKorabl;
            index++;
            *(pKorabl -= 1);
            adresaKorabl[index] = &*pKorabl1;
            index++;
            *(pKorabl1 -= 1);
            adresaKorabl[index] = &*pKorabl2;
            index++;
            *(pKorabl2 -= 1);
        }
    }
}
//------------------------------------
void pointKorablVruchnuy(int pal, int colum)//растановка в ручную
{
    int* ptr;//указывает на адрес палубы 
    int row, col;//строка - столбец массива
    int gorver;//по горизонтали или по вертикали     
    char Stolbec;

    if (!pal)
        return;
    int pervayPal = 1;
    for (int j = 1; j <= colum; j++)//j количество кораблей одного типа
    {
        do {
            if (pal > 1)
                cout << pal << " -палубный корабль. Введите координаты первой палубы например(а4)_ ";
            else if (pal == 1)
                cout << pal << " -палубный корабль. Введите координаты корабля например(а4)_ ";
            cin.sync();
            scanf("%c%d", &Stolbec, &row);
            col = perevod(Stolbec);
            if ((!col) || (row < 1) || (row > 10))
            {
                SetColor(White, LightRed);
                cout << " Неверные координаты \n";
                SetColor(White, Black);
            }
        } while ((!col) || (row < 1) || (row > 10));
        int k = 1, i = pal;//k счетчик i текущяя палуба      
        if (pal > 1)
        {
            do {
                cout << "Как Вы хотите установить свой " << pal << " палубный корабль \nВертикально(1)  Горизонтально(2) __ ";
                cin >> gorver;
                if (gorver < 1 || gorver>2)
                {
                    SetColor(White, LightRed);
                    cout << " Неверные координаты \n";
                    SetColor(White, Black);
                }
            } while (gorver < 1 || gorver>2);
        }
        if (pal == 1)//для однопалубного
            gorver = 1;
        if (gorver == 1)//вертикаль
        {
            do {
                if (oshibRuchVoda)//проверка адресов
                {
                    SetColor(White, LightRed);
                    cout << " Неверные координаты \n";
                    SetColor(White, Black);
                    do {
                        cout << pal << " -палубный корабль. Введите координаты первой палубы например(а4)_ ";
                        cin.sync();
                        scanf("%c%d", &Stolbec, &row);
                        col = perevod(Stolbec);
                        if ((!col) || (row < 1) || (row > 10))
                        {
                            SetColor(White, LightRed);
                            cout << " Неверные координаты \n";
                            SetColor(White, Black);
                        }
                    } while ((!col) || (row < 1) || (row > 10));
                    oshibRuchVoda = 0;
                }
                if (row + pal > 11)//проверка на выход за пределы массива
                    do {
                        SetColor(White, LightRed);
                        cout << " Неверные координаты \n";
                        SetColor(White, Black);
                        do {
                            cout << pal << " -палубный корабль. Введите координаты первой палубы например(а4)_ ";
                            cin.sync();
                            scanf("%c%d", &Stolbec, &row);
                            col = perevod(Stolbec);
                            if ((!col) || (row < 1) || (row > 10))
                            {
                                SetColor(White, LightRed);
                                cout << " Неверные координаты \n";
                                SetColor(White, Black);
                            }
                        } while ((!col) || (row < 1) || (row > 10));
                    } while (row + pal > 11); //проверка на выход за пределы массива           
                    ptr = &PoleIgroka[row][col];  //первая палуба             
            } while (!poiskAdresa(ptr, pal, gorver)); //проверка на совпадение адресов     
        }
        else if (gorver == 2)//горизонталь
        {
            do {
                if (oshibRuchVoda)//проверка адресов
                {
                    SetColor(White, LightRed);
                    cout << " Неверные координаты \n";
                    SetColor(White, Black);
                    do {
                        cout << pal << " -палубный корабль. Введите координаты первой палубы например(а4)_ ";
                        cin.sync();
                        scanf("%c%d", &Stolbec, &row);
                        col = perevod(Stolbec);
                        if ((!col) || (row < 1) || (row > 10))
                        {
                            SetColor(White, LightRed);
                            cout << " Неверные координаты \n";
                            SetColor(White, Black);
                        }
                    } while ((!col) || (row < 1) || (row > 10));
                    oshibRuchVoda = 0;
                }
                if (col + pal > 11)//проверка на выход за пределы массива
                    do {
                        SetColor(White, LightRed);
                        cout << " Неверные координаты \n";
                        SetColor(White, Black);
                        do {
                            cout << pal << " -палубный корабль. Введите координаты первой палубы например(а4)_ ";
                            scanf("%c%d", &Stolbec, &row);
                            col = perevod(Stolbec);
                            if ((!col) || (row < 1) || (row > 10))
                            {
                                SetColor(White, LightRed);
                                cout << " Неверные координаты \n";
                                SetColor(White, Black);
                            }
                        } while ((!col) || (row < 1) || (row > 10));
                    } while (col + pal > 11);
                    ptr = &PoleIgroka[row][col];
            } while (!poiskAdresa(ptr, pal, gorver));
        }

        switch (gorver)
        {
        case 1:     //вертикаль            
            while (k <= i)//k счетчик i текущяя палуба
            {
                PoleIgroka[row][col] = 1;
                if (pal == 4 && pervayPal == 1)
                {
                    ChetP.strPalubKorabl = &PoleIgroka[row][col]; //запись в структуру адреса только самой первой палубы
                    ChetP.pologKorabliy = 1;//положение корабля
                    ChetP.palubu = 4;//кол. палуб
                    pervayPal++;//чтобы записать только первую палубу 
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1P.strPalubKorabl = &PoleIgroka[row][col];
                    Try1P.pologKorabliy = 1;
                    Try1P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2P.strPalubKorabl = &PoleIgroka[row][col];
                    Try2P.pologKorabliy = 1;
                    Try2P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa1P.pologKorabliy = 1;
                    Dwa1P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa2P.pologKorabliy = 1;
                    Dwa2P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa3P.pologKorabliy = 1;
                    Dwa3P.palubu = 2;
                    pervayPal++;
                }
                row++;
                i--;
            }
            break;
        case 2:         //горизонталь                
            while (k <= i)
            {
                PoleIgroka[row][col] = 1;
                if (pal == 4 && pervayPal == 1)
                {
                    ChetP.strPalubKorabl = &PoleIgroka[row][col];
                    ChetP.pologKorabliy = 2;
                    ChetP.palubu = 4;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1P.strPalubKorabl = &PoleIgroka[row][col];
                    Try1P.pologKorabliy = 2;
                    Try1P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2P.strPalubKorabl = &PoleIgroka[row][col];
                    Try2P.pologKorabliy = 2;
                    Try2P.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa1P.pologKorabliy = 2;
                    Dwa1P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa2P.pologKorabliy = 2;
                    Dwa2P.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3P.strPalubKorabl = &PoleIgroka[row][col];
                    Dwa3P.pologKorabliy = 2;
                    Dwa3P.palubu = 2;
                    pervayPal++;
                }
                col++;
                i--;
            }
            break;
        }
        print();
    }
    pointKorablVruchnuy(pal - 1, colum + 1);//уменшение палуб - увеличение кол. кор.
}

int perevod(char sim)//переводит введ. пользоват. символы
{
    switch (sim)
    {
    case 'a':
        return 1;
        break;
    case 'b':
        return 2;
        break;
    case 'c':
        return 3;
        break;
    case 'd':
        return 4;
        break;
    case 'e':
        return 5;
        break;
    case 'f':
        return 6;
        break;
    case 'g':
        return 7;
        break;
    case 'h':
        return 8;
        break;
    case 'i':
        return 9;
        break;
    case 'j':
        return 10;
        break;
    default:
        return 0;
    }
}
//------------------------------------------------
void HodPolz()//хода польз.
{
    if (!OpredPobed())//проверка на победителя
    {
        SetColor(White, Black);
        cout << "\n\t\t";
        SetColor(White, LightRed);
        cout << " ИГРА ОКОНЧЕНА \n";
        SetColor(LightGreen, Black);
        cout << "\nПобедил Пользователь со счетом Компьютер __ " << countPC << " Пользователь __ " << countIG << endl << endl;
        SetColor(White, Black);
        //system("pause");
        return;
    }
    void proverPopad(int*);
    int stroka, stolbec;
    char stolb;
    do {                      //проверка правильного ввода
        cin.sync();
        SetColor(LightGreen, Black);
        cout << "\nПроизведите выстрел введя координаты (a2)__ ";
        SetColor(White, Black);
        scanf("%c%d", &stolb, &stroka);
        stolbec = perevod(stolb);
        if ((!stolbec) || (stroka < 1) || (stroka > 10))
        {
            SetColor(White, LightRed);
            cout << "\n Неверные координаты \n\n";
            SetColor(White, Black);
        }
    } while ((!stolbec) || (stroka < 1) || (stroka > 10));//-----------
    if (PoleKopuyt[stroka][stolbec] == 1)//если попали
    {
        int* strPopad = &PoleKopuyt[stroka][stolbec];//адрес ранен. кор. чтобы обвести если убит
        PoleKopuyt[stroka][stolbec] = 3;//присв. 3
        proverPopad(strPopad);
        HodPolz();
        if (!OpredPobed())
        {
            //system("pause");
            return;
        }
    }
    else if (PoleKopuyt[stroka][stolbec] != 3 && PoleKopuyt[stroka][stolbec] != 1)//если мимо
    {
        PoleKopuyt[stroka][stolbec] = 2;      //присв. 2
    }

    print();
    printComp();
    HodPC();
    if (!OpredPobed())
    {
        //system("pause");
        return;
    }
}
//----------------------------------------
void proverPopad(int* pPopad)//Поиск корабля в который попали
{
    void UbitKor(int*, int, int);
    if ((*(pPopad + 1) != 1) && (*(pPopad - 1) != 1) && (*(pPopad + 12) != 1) && (*(pPopad - 12) != 1) &&//для однопалубных обвод в случае попадания
        (*(pPopad + 1) != 3) && (*(pPopad - 1) != 3) && (*(pPopad + 12) != 3) && (*(pPopad - 12) != 3))
    {
        *(pPopad + 11) = 2;
        *(pPopad - 11) = 2;
        *(pPopad + 13) = 2;
        *(pPopad - 13) = 2;

        *(pPopad + 1) = 2;
        *(pPopad - 1) = 2;
        *(pPopad + 12) = 2;
        *(pPopad - 12) = 2;
    }
    for (int i = 0, j = 12; i < 4; i++)//проверка в какой корабль попали
    {
        if (&*pPopad == &*(Chet.strPalubKorabl + i) || &*pPopad == &*(Chet.strPalubKorabl + j))//если есть такой адрес
            UbitKor(Chet.strPalubKorabl, Chet.pologKorabliy, Chet.palubu);//проверить убили или ранели
        j += 12;
    }
    for (int i = 0, j = 12; i < 3; i++)
    {
        if (&*pPopad == &*(Try1.strPalubKorabl + i) || &*pPopad == &*(Try1.strPalubKorabl + j))
            UbitKor(Try1.strPalubKorabl, Try1.pologKorabliy, Try1.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 3; i++)
    {
        if (&*pPopad == &*(Try2.strPalubKorabl + i) || &*pPopad == &*(Try2.strPalubKorabl + j))
            UbitKor(Try2.strPalubKorabl, Try2.pologKorabliy, Try2.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa1.strPalubKorabl + i) || &*pPopad == &*(Dwa1.strPalubKorabl + j))
            UbitKor(Dwa1.strPalubKorabl, Dwa1.pologKorabliy, Dwa1.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa2.strPalubKorabl + i) || &*pPopad == &*(Dwa2.strPalubKorabl + j))
            UbitKor(Dwa2.strPalubKorabl, Dwa2.pologKorabliy, Dwa2.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa3.strPalubKorabl + i) || &*pPopad == &*(Dwa3.strPalubKorabl + j))
            UbitKor(Dwa3.strPalubKorabl, Dwa3.pologKorabliy, Dwa3.palubu);
        j += 12;
    }
    print();
    printComp();
}
//-------------------------------------------------------------------
void UbitKor(int* obvodPal, int pologKorabl, int kolPal)//проверка убит ли кор. если да то вызвать ф-ю обвести obvodUbitKor
{
    void obvodUbitKor(int*, int, int);
    int setch = 0;//счетчик
    int* obvod = &*(obvodPal);//адрес первой палубы

    if (pologKorabl == 1)
    {
        for (int i = 0, j = 12; i < kolPal; i++)
        {
            if (*(obvod) == 3)
            {
                *(obvod += j);
                setch++;
            }
            if (setch == kolPal)
                obvodUbitKor(obvodPal, pologKorabl, kolPal);
        }
    }

    else if (pologKorabl == 2)
    {
        for (int i = 0, j = 1; i < kolPal; i++)
        {
            if (*(obvod) == 3)
            {
                *(obvod += j);
                setch++;
            }
            if (setch == kolPal)
                obvodUbitKor(obvodPal, pologKorabl, kolPal);
        }
    }
}
//--------------------------------------------------------------
void obvodUbitKor(int* obvodPalub, int pologKorabley, int kolPalub)//обвод уитого корабля
{
    if (pologKorabley == 1)//вертикаль
    {
        int* pKorabl3 = &*(obvodPalub - 12);
        *pKorabl3 = 2;
        int* pKorabl1 = &*(pKorabl3 - 1);
        int* pKorabl2 = &*(pKorabl3 + 1);
        for (int i = 0; i < kolPalub + 2; i++)
        {
            if (*pKorabl1 != 3)
                *pKorabl1 = 2;
            *(pKorabl1 += 12);
            if (*pKorabl2 != 3)
                *pKorabl2 = 2;
            *(pKorabl2 += 12);
            if (i == kolPalub + 1)
            {
                *(pKorabl2 -= 13);
                *pKorabl2 = 2;
            }
        }
    }

    else if (pologKorabley == 2)//горизонталь
    {
        int* pKorabl3 = &*(obvodPalub - 1);
        *pKorabl3 = 2;
        int* pKorabl1 = &*(pKorabl3 - 12);
        int* pKorabl2 = &*(pKorabl3 + 12);
        for (int i = 0; i < kolPalub + 2; i++)
        {
            if (*pKorabl1 != 3)
                *pKorabl1 = 2;
            *(pKorabl1 += 1);
            if (*pKorabl2 != 3)
                *pKorabl2 = 2;
            *(pKorabl2 += 1);
            if (i == kolPalub + 1)
            {
                *(pKorabl1 += 11);
                *pKorabl1 = 2;
            }
        }
    }
}
//------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void pointKorablKomp(int pal, int colum)
{
    int poiskAdresaKomp(int*, int, int);
    if (!pal)
        return;
    int pervayPal = 1;
    for (int j = 1; j <= colum; j++)//j количество кораблей одного типа
    {
        int* ptr;//указывает на адрес палубы 
        int row, col;//строка - столбец массива
        int gorver = 1 + rand() % 2;//по горизонтали или по вертикали
        int k = 1, i = pal;//k счетчик i текущяя палуба
        if (gorver == 1)//вертикаль
        {
            do {
                do {
                    row = rand() % 10 + 1;
                } while (row + pal > 11);//чтобы не выйти за пределы массива
                col = rand() % 10 + 1;
                ptr = &PoleKopuyt[row][col];//адресс для проверки на совпадение
            } while (!poiskAdresaKomp(ptr, pal, gorver));//-----------------//вызвать ф-ю            
        }
        else if (gorver == 2)//горизонталь
        {
            do {
                do {
                    col = rand() % 10 + 1;
                } while (col + pal > 11);//чтобы не выйти за пределы массива
                row = rand() % 10 + 1;
                ptr = &PoleKopuyt[row][col];//адресс для проверки на совпадение
            } while (!poiskAdresaKomp(ptr, pal, gorver));//-----------------//вызвать ф-ю        
        }

        switch (gorver)
        {
        case 1:     //вертикаль            
            while (k <= i)//k счетчик i текущяя палуба
            {
                PoleKopuyt[row][col] = 1;//устоновка палубы 1
                if (pal == 4 && pervayPal == 1)
                {
                    Chet.strPalubKorabl = &PoleKopuyt[row][col];  //запись в структуру только первой палубы
                    Chet.pologKorabliy = 1;//положение корабл.
                    Chet.palubu = 4;//колич. палуб
                    pervayPal++;//чтобы не входить в блок дважды
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1.strPalubKorabl = &PoleKopuyt[row][col];
                    Try1.pologKorabliy = 1;
                    Try1.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2.strPalubKorabl = &PoleKopuyt[row][col];
                    Try2.pologKorabliy = 1;
                    Try2.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa1.pologKorabliy = 1;
                    Dwa1.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa2.pologKorabliy = 1;
                    Dwa2.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa3.pologKorabliy = 1;
                    Dwa3.palubu = 2;
                    pervayPal++;
                }
                row++;
                i--;
            }
            break;
        case 2:         //горизонталь                
            while (k <= i)
            {
                PoleKopuyt[row][col] = 1;
                if (pal == 4 && pervayPal == 1)
                {
                    Chet.strPalubKorabl = &PoleKopuyt[row][col];
                    Chet.pologKorabliy = 2;
                    Chet.palubu = 4;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 1)
                {
                    Try1.strPalubKorabl = &PoleKopuyt[row][col];
                    Try1.pologKorabliy = 2;
                    Try1.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 3 && pervayPal == 2 && j == 2)
                {
                    Try2.strPalubKorabl = &PoleKopuyt[row][col];
                    Try2.pologKorabliy = 2;
                    Try2.palubu = 3;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 1)
                {
                    Dwa1.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa1.pologKorabliy = 2;
                    Dwa1.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 2 && j == 2)
                {
                    Dwa2.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa2.pologKorabliy = 2;
                    Dwa2.palubu = 2;
                    pervayPal++;
                }
                else if (pal == 2 && pervayPal == 3 && j == 3)
                {
                    Dwa3.strPalubKorabl = &PoleKopuyt[row][col];
                    Dwa3.pologKorabliy = 2;
                    Dwa3.palubu = 2;
                    pervayPal++;
                }
                col++;
                i--;
            }
            break;
        }
    }
    pointKorablKomp(pal - 1, colum + 1);//уменш. палуб - увел. кол. корабл.
}
//----------------------------------------------
void printComp()//печать
{
    SetColor(LightGreen, Black);
    cout << "\n\t\t Поле Компьютера\n\n";
    SetColor(White, Black);
    SetColor(White, Black);
    cout << "     a   b   c   d   e   f   g   h   i   j\n";
    cout << "   + - + - + - + - + - + - + - + - + - + - +\n";
    for (int i = 1; i < sizeRow - 1; i++)
    {
        cout << setw(2) << i << " ";
        for (int j = 1; j < sizeCol - 1; j++)
        {
            if (OpredPobed())
            {
                if (!(PoleKopuyt[i][j])) {
                    cout << "| " << "  ";
                    SetColor(White, Black);
                }
                else if (PoleKopuyt[i][j] == 2)
                {
                    cout << "| ";
                    SetColor(Yellow, Black);
                    cout << 'x' << " ";
                    SetColor(White, Black);
                }
                else if (PoleKopuyt[i][j] == 3)
                {
                    cout << "|";
                    SetColor(LightRed, Black);
                    cout << " " << 'X' << " ";
                    SetColor(White, Black);
                }
                else
                {
                    cout << "|";
                    SetColor(White, Black);
                    cout << ' ' << "  ";
                    SetColor(White, Black);
                }
                if (j == 10)
                    cout << "|";
            }
            else if (!OpredPobed())
            {
                if (!(PoleKopuyt[i][j])) {
                    cout << "| " << "  ";
                    SetColor(White, Black);
                }
                //-------------------------
                else if (PoleKopuyt[i][j] == 1)//если 1 нарисовать корабль
                {
                    cout << "|";
                    SetColor(Green, Green);
                    cout << '0' << "  ";
                    SetColor(White, Black);
                }
                //------------------------
                else if (PoleKopuyt[i][j] == 2)
                {
                    cout << "| ";
                    SetColor(Yellow, Black);
                    cout << 'x' << " ";
                    SetColor(White, Black);
                }
                else if (PoleKopuyt[i][j] == 3)
                {
                    cout << "|";
                    SetColor(LightRed, Black);
                    cout << " " << 'X' << " ";
                    SetColor(White, Black);
                }
                else
                {
                    cout << "|";
                    SetColor(White, Black);
                    cout << ' ' << "  ";
                    SetColor(White, Black);
                }
                if (j == 10)
                    cout << "|";
            }
        }
        cout << endl;
        cout << "   + - + - + - + - + - + - + - + - + - + - +\n";
    }
}
//--------------------------------
int poiskAdresaKomp(int* ptrKorabl, int palub, int vergor)//есть ли уже этот адрес - если нет 1 записать адреса иначе 0 выбр. др.
{
    void zapAdresPC(int*, int, int);
    for (int a = 1; a <= palub; a++)//проверка каждой палубы 
    {
        if (vergor == 1)
        {
            for (int i = 0; i < sizeRow * sizeCol; i++)
            {
                if (ptrKorabl == adresaKorablPC[i])
                    return 0;
            }
            *(ptrKorabl += 12);
        }
        else if (vergor == 2)
        {
            for (int i = 0; i < sizeRow * sizeCol; i++)
            {
                if (ptrKorabl == adresaKorablPC[i])
                    return 0;
            }
            *(ptrKorabl += 1);
        }
    }
    zapAdresPC(ptrKorabl, palub, vergor);
    return 1;
}
//---------------------------------------------------
void zapAdresPC(int* pKorabl, int palu, int pologen)//запись адрессов
{
    if (pologen == 1)//если по вертикали
    {
        int* pKorabl1 = &*(pKorabl - 1);
        int* pKorabl2 = &*(pKorabl + 1);
        for (int i = 0; i < palu + 2; i++)// адреса корабл. и вокруг него
        {
            adresaKorablPC[indexPC] = &*pKorabl;
            indexPC++;
            *(pKorabl -= 12);
            adresaKorablPC[indexPC] = &*pKorabl1;
            indexPC++;
            *(pKorabl1 -= 12);
            adresaKorablPC[indexPC] = &*pKorabl2;
            indexPC++;
            *(pKorabl2 -= 12);
        }
    }
    else if (pologen == 2)//горизонт.
    {
        int* pKorabl1 = &*(pKorabl + 12);
        int* pKorabl2 = &*(pKorabl - 12);
        for (int i = 0; i < palu + 2; i++)
        {
            adresaKorablPC[indexPC] = &*pKorabl;
            indexPC++;
            *(pKorabl -= 1);
            adresaKorablPC[indexPC] = &*pKorabl1;
            indexPC++;
            *(pKorabl1 -= 1);
            adresaKorablPC[indexPC] = &*pKorabl2;
            indexPC++;
            *(pKorabl2 -= 1);
        }
    }
}
//-----------------------------------------
void HodPC()// ход РС
{
    if (!OpredPobed())//проверка на победителя
    {
        SetColor(White, Black);
        cout << "\n\t\t";
        SetColor(White, LightRed);
        cout << " ИГРА ОКОНЧЕНА \n";
        SetColor(LightGreen, Black);
        cout << "\nПобедил Компьютер со счетом Компьютер __ " << countPC << " Пользователь __ " << countIG << endl << endl;
        SetColor(White, Black);
        //system("pause");
        return;
    }
    int hadaPC(int*);
    void proverPopadPC(int*);
    int* ptrPigr;

    if (!ranen)//если не ранил
    {
        for (; prohod <= 2; )//проход1 *_  проход2 _*
        {
            for (; strok < 11; strok++, (prohod == 1) ? ((strok % 2 == 0) ? stolb = 2 : stolb = 1) : ((strok % 2 == 0) ? stolb = 1 : stolb = 2))//как ходить *_ или _*
            {
                for (; stolb < 11; )
                {
                    ptrPigr = &PoleIgroka[strok][stolb];
                    if (!hadaPC(ptrPigr))//чтобы не ходить в то-же место
                    {
                        stolb += 2;
                        continue;
                    }

                    adresaHodovPC[indexHodPC] = &PoleIgroka[strok][stolb];
                    indexHodPC++;
                    if (PoleIgroka[strok][stolb] == 1)//если попал
                    {
                        PoleIgroka[strok][stolb] = 3;//присвоить 3
                        int* ptrPopadPC = &PoleIgroka[strok][stolb];
                        proverPopadPC(ptrPopadPC);  //проверка на ранение      
                        stolb += 2;
                        HodPC();
                        if (!OpredPobed())
                        {
                            //system("pause");
                            return;
                        }
                    }
                    else
                    {
                        PoleIgroka[strok][stolb] = 2; //иначе присв. 2
                        stolb += 2;
                    }

                    print();
                    printComp();
                    SetColor(LightRed, Black);
                    cout << "\nКомпьютер походил !" << endl << endl;
                    SetColor(White, Black);
                    HodPolz();
                    if (!OpredPobed())
                    {
                        //system("pause");
                        return;
                    }
                }
            }
            if (strok == 11 && stolb == 1)
                strok = 1;
            stolb = 2;
            prohod++;
        }
    }

    if (ranen)//если ранен
    {
        int* ptrPopadPC = &*(ptrPopadPC1);
        do {
            if (counterRanen == 1)
                ptrPopadPC = &*(ptrPopadPC1 + 1);//вправо на 1

            if (counterRanen == 2)
                ptrPopadPC = &*(ptrPopadPC1 - 1);//влево на 1

            if (counterRanen == 3)
                if (*(ptrPopadPC1 + 1) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 + 2);//если вправо на 1 !=2 вправо на 2
                else
                    counterRanen++;

            if (counterRanen == 4)
                if (*(ptrPopadPC1 - 1) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 - 2);//если влево на 1 !=2 влево на 2
                else
                    counterRanen++;

            if (counterRanen == 5)
                if (*(ptrPopadPC1 + 1) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 + 3);//если вправо на 1 !=2 вправо на 3
                else
                    counterRanen++;

            if (counterRanen == 6)
                if (*(ptrPopadPC1 - 1) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 - 3);//если влево на 1 !=2 влево на 3
                else
                    counterRanen++;

            if (counterRanen == 7)
                ptrPopadPC = &*(ptrPopadPC1 + 12);//значит корабль стоит вертикально - в низ на 1

            if (counterRanen == 8)
                ptrPopadPC = &*(ptrPopadPC1 - 12);//вверх на 1

            if (counterRanen == 9)
                if (*(ptrPopadPC1 - 12) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 - 24);//если вверх на 1 !=2 то вверх на 2
                else
                    counterRanen++;

            if (counterRanen == 10)
                if (*(ptrPopadPC1 + 12) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 + 24);//если вниз на 1 !=2 то вниз на 2
                else
                    counterRanen++;

            if (counterRanen == 11)
                if (*(ptrPopadPC1 - 12) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 - 36);//если вверх на 1 !=2 то вверх на 3
                else
                    counterRanen++;

            if (counterRanen == 12)
                if (*(ptrPopadPC1 + 12) != 2)
                    ptrPopadPC = &*(ptrPopadPC1 + 36);//если вниз на 1 !=2 то вниз на 3
                else
                    counterRanen++;

            ptrPigr = &*ptrPopadPC;
            counterRanen++;
        } while (!hadaPC(ptrPigr));//проверит ходил ли уже так

        adresaHodovPC[indexHodPC] = &*(ptrPopadPC);//запись хода
        indexHodPC++;

        if (*ptrPopadPC == 1)//если попал
        {
            *ptrPopadPC = 3;
            proverPopadPC(ptrPopadPC);  //проверка      
            HodPC();
            if (!OpredPobed())
            {
                //system("pause");
                return;
            }
        }
        else
        {
            *ptrPopadPC = 2;
        }
        print();
        printComp();
        SetColor(LightRed, Black);
        cout << "\nКомпьютер походил !" << endl << endl;
        SetColor(White, Black);
        HodPolz();
        if (!OpredPobed())
        {
            //system("pause");
            return;
        }
    }
}
//------------------------------------------
int hadaPC(int* ptrPoligr)//сравнение ходов на совпадение
{
    for (int i = 0; i < sizeRow * sizeCol; i++)
    {
        if (ptrPoligr == adresaHodovPC[i])
        {
            return 0;
        }
    }
    return 1;
}
////-------------------------------------------////////////////---------------------------///////////////////------------/////
void UbitKorPC(int* obvodPal, int pologKorabl, int kolPal)//если убит то вызв. ф-ю обвода и сбросить положение ранен 
{
    void obvodUbitKorPC(int*, int, int);
    int setch = 0;
    int* obvod = *&(obvodPal);

    if (pologKorabl == 1)
    {
        for (int i = 0, j = 12; i < kolPal; i++)
        {
            if (*(obvod) == 3)
            {
                setch++;
                ranen = 1;
                if (counterRanen == 1)
                    ptrPopadPC1 = &*(obvod);  // присваивает адрес только первого попадения
            }
            if (setch == kolPal) {//значит убит
                obvodUbitKorPC(obvodPal, pologKorabl, kolPal);//    обвод
                ranen = 0;//сброс ранения
                counterRanen = 1;
            }
            *(obvod += j);////--------///-----///------------------///------------
        }
    }

    else if (pologKorabl == 2)
    {
        for (int i = 0, j = 1; i < kolPal; i++)
        {
            if (*(obvod) == 3)
            {
                setch++;
                ranen = 1;
                if (counterRanen == 1)
                    ptrPopadPC1 = &*(obvod);  //заменил    obvodPal на       obvod               
            }
            if (setch == kolPal) {
                obvodUbitKorPC(obvodPal, pologKorabl, kolPal);
                ranen = 0;
                counterRanen = 1;
            }
            *(obvod += j);//--------///-----///------------------///----------
        }
    }
}
//-----------------------------------------
void proverPopadPC(int* pPopad)//Поиск корабля в который попали
{
    void UbitKorPC(int*, int, int);
    if ((*(pPopad + 1) != 1) && (*(pPopad - 1) != 1) && (*(pPopad + 12) != 1) && (*(pPopad - 12) != 1) &&
        (*(pPopad + 1) != 3) && (*(pPopad - 1) != 3) && (*(pPopad + 12) != 3) && (*(pPopad - 12) != 3))
    {
        if (*(pPopad + 11) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad + 11);
            indexHodPC++;
        }
        if (*(pPopad - 11) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad - 11);
            indexHodPC++;
        }
        if (*(pPopad + 13) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad + 13);
            indexHodPC++;
        }
        if (*(pPopad - 13) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad - 13);
            indexHodPC++;
        }
        if (*(pPopad + 1) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad + 1);
            indexHodPC++;
        }
        if (*(pPopad - 1) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad - 1);
            indexHodPC++;
        }
        if (*(pPopad + 12) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad + 12);
            indexHodPC++;
        }
        if (*(pPopad - 12) != 2) {
            adresaHodovPC[indexHodPC] = &*(pPopad - 12);
            indexHodPC++;
        }
        *(pPopad + 11) = 2;
        *(pPopad - 11) = 2;
        *(pPopad + 13) = 2;
        *(pPopad - 13) = 2;

        *(pPopad + 1) = 2;
        *(pPopad - 1) = 2;
        *(pPopad + 12) = 2;
        *(pPopad - 12) = 2;
    }
    for (int i = 0, j = 12; i < 4; i++)
    {
        if (&*pPopad == &*(ChetP.strPalubKorabl + i) || &*pPopad == &*(ChetP.strPalubKorabl + j))
            UbitKorPC(ChetP.strPalubKorabl, ChetP.pologKorabliy, ChetP.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 3; i++)
    {
        if (&*pPopad == &*(Try1P.strPalubKorabl + i) || &*pPopad == &*(Try1P.strPalubKorabl + j))
            UbitKorPC(Try1P.strPalubKorabl, Try1P.pologKorabliy, Try1P.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 3; i++)
    {
        if (&*pPopad == &*(Try2P.strPalubKorabl + i) || &*pPopad == &*(Try2P.strPalubKorabl + j))
            UbitKorPC(Try2P.strPalubKorabl, Try2P.pologKorabliy, Try2P.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa1P.strPalubKorabl + i) || &*pPopad == &*(Dwa1P.strPalubKorabl + j))
            UbitKorPC(Dwa1P.strPalubKorabl, Dwa1P.pologKorabliy, Dwa1P.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa2P.strPalubKorabl + i) || &*pPopad == &*(Dwa2P.strPalubKorabl + j))
            UbitKorPC(Dwa2P.strPalubKorabl, Dwa2P.pologKorabliy, Dwa2P.palubu);
        j += 12;
    }
    for (int i = 0, j = 12; i < 2; i++)
    {
        if (&*pPopad == &*(Dwa3P.strPalubKorabl + i) || &*pPopad == &*(Dwa3P.strPalubKorabl + j))
            UbitKorPC(Dwa3P.strPalubKorabl, Dwa3P.pologKorabliy, Dwa3P.palubu);
        j += 12;
    }
    print();
    printComp();
}
//-------------------//////////////---------------------/////////////////------------------/////////////////---------------/////////////--
void obvodUbitKorPC(int* obvodPalub, int pologKorabley, int kolPalub)//обводит убитые корабли
{
    if (pologKorabley == 1)
    {
        int* pKorabl3 = &*(obvodPalub - 12);
        if (*pKorabl3 != 2) {
            adresaHodovPC[indexHodPC] = &*(pKorabl3);
            indexHodPC++;
        }
        *pKorabl3 = 2;
        int* pKorabl1 = &*(pKorabl3 - 1);
        int* pKorabl2 = &*(pKorabl3 + 1);
        for (int i = 0; i < kolPalub + 2; i++)
        {
            if (*pKorabl1 != 3) {
                if (*pKorabl1 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl1);
                    indexHodPC++;
                }
                *pKorabl1 = 2;
            }
            *(pKorabl1 += 12);
            if (*pKorabl2 != 3) {
                if (*pKorabl2 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl2);
                    indexHodPC++;
                }
                *pKorabl2 = 2;
            }
            *(pKorabl2 += 12);
            if (i == kolPalub + 1)
            {
                *(pKorabl2 -= 13);
                if (*pKorabl2 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl2);
                    indexHodPC++;
                }
                *pKorabl2 = 2;
            }
        }
    }

    else if (pologKorabley == 2)
    {
        int* pKorabl3 = &*(obvodPalub - 1);
        if (*pKorabl3 != 2) {
            adresaHodovPC[indexHodPC] = &*(pKorabl3);
            indexHodPC++;
        }
        *pKorabl3 = 2;
        int* pKorabl1 = &*(pKorabl3 - 12);
        int* pKorabl2 = &*(pKorabl3 + 12);
        for (int i = 0; i < kolPalub + 2; i++)
        {
            if (*pKorabl1 != 3) {
                if (*pKorabl1 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl1);
                    indexHodPC++;
                }
                *pKorabl1 = 2;
            }
            *(pKorabl1 += 1);
            if (*pKorabl2 != 3) {
                if (*pKorabl2 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl2);
                    indexHodPC++;
                }
                *pKorabl2 = 2;
            }
            *(pKorabl2 += 1);
            if (i == kolPalub + 1)
            {
                *(pKorabl1 += 11);
                if (*pKorabl1 != 2) {
                    adresaHodovPC[indexHodPC] = &*(pKorabl1);
                    indexHodPC++;
                }
                *pKorabl1 = 2;
            }
        }
    }
}
//--------------------------------------
int OpredPobed()
{
    countIG = 0, countPC = 0;
    for (int i = 1; i < sizeRow - 1; i++)
    {
        for (int j = 1; j < sizeCol - 1; j++)
        {
            if (PoleIgroka[i][j] == 3)
                countPC++;
        }
    }

    for (int i = 1; i < sizeRow - 1; i++)
    {
        for (int j = 1; j < sizeCol - 1; j++)
        {
            if (PoleKopuyt[i][j] == 3)
                countIG++;
        }
    }
    if (countPC == 20)
    {
        return 0;
    }
    else if (countIG == 20)
    {
        return 0;
    }
    else
        return 1;
}