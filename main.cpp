#include "TXLib.h"
#include <iostream>
#include <fstream>
#include "dirent.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Button
{
 int x;
 int y;
 const char* text;
 string category;

};

void drawButton(Button btn)
{
  txSetColor (TX_GRAY);
  txSetFillColor (TX_GRAY);
  Win32::RoundRect (txDC(),btn.x+5, btn.y+5, btn.x+155, btn.y+45,30,30);
  txSetColor (TX_BLACK);
  txSetFillColor (TX_WHITE);
  Win32::RoundRect (txDC(), btn.x, btn.y, btn.x+150, btn.y+40,30,30);
  txSetColor (TX_BLACK);
  txSelectFont("Times New Roman",32);
  txDrawText( btn.x, btn.y, btn.x+150, btn.y+40, btn.text);

}

bool click(Button btn)
{
   return(txMouseButtons() == 1 &&
          txMouseX() >= btn.x &&
          txMouseX() <= btn.x + 150 &&
          txMouseY() >= btn.y &&
          txMouseY() <= btn.y + 40);
}

struct Pictures
{
    int x;
    int y;
    string adress;
    HDC image;
    int w;
    int h;
    int w_scr;
    int h_scr;
    string category;
    bool visible;
};

void drawPictures(Pictures pct)
{
    if(pct.visible)
    {
     Win32::TransparentBlt(txDC(),pct.x, pct.y, pct.w_scr, pct.h_scr, pct.image, 0,0, pct.w, pct.h, TX_WHITE);
    }
}

int readFromDir(string adress, Pictures menuPicture[], int COUNT_PICTURES)
{
    DIR *dir;
    struct dirent *ent;
    int lastY = 100;
    if ((dir = opendir (adress.c_str())) != NULL)
    {
      while ((ent = readdir (dir)) != NULL)
      {
        if((string)ent->d_name != "." && (string)ent->d_name != "..")
        {
            menuPicture[COUNT_PICTURES].y = lastY;
            menuPicture[COUNT_PICTURES].adress = adress + (string)ent->d_name;
            COUNT_PICTURES ++;
            lastY +=150;
        }
      }
      closedir (dir);
    }
    return COUNT_PICTURES;

}

int get_w(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int w = *(int*)&headerinfo[18];

    return w;
}

int get_h(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int h = *(int*)&headerinfo[22];

    return h;
}


        //???????????? ???????????????? /???????????????????? ??????????
     string runFileDialog(bool isSave)
     {
         string fileName = "";

         OPENFILENAME ofn;     //common dialog box structure
         TCHAR szFile[260] = {0};     //if using TCHAR macros

         //Initailize OPENFILNAME
         ZeroMemory(&ofn, sizeof(ofn));
         ofn.lStructSize = sizeof(ofn);
         ofn.hwndOwner = txWindow();
         ofn.lpstrFile = szFile;
         ofn.nMaxFile = sizeof(szFile);
         ofn.lpstrFilter = ("Text\0*.TXT\0");
         ofn.nFilterIndex = 1;
         ofn.lpstrFileTitle = NULL;
         ofn.nMaxFileTitle = 0;
         ofn.lpstrInitialDir = NULL;
         ofn.Flags - OFN_PATHMUSTEXIST | OFN_PATHMUSTEXIST;


         if (isSave)
         {
             if (GetSaveFileName(&ofn)== TRUE)
             {
                 fileName = ofn.lpstrFile;

                 if (fileName.find(".txt") > 1000)
                 {
                     fileName = fileName + ".txt" ;


                 }
             }
        }

      else
     {
         if (GetOpenFileName(&ofn) == TRUE)
        {
           fileName = ofn.lpstrFile;
        }

      }
   return fileName;
}

    const int COUNT_BTN = 7;
    const int BTN_SAVE = COUNT_BTN - 2;
    const int BTN_LOAD = COUNT_BTN - 1;


int main()
{
    txCreateWindow (1200,800);
    txDisableAutoPause();
    txTextCursor(false);
    int COUNT_PICTURES = 0;
    int nCentralPic = 0;
    HDC Fon;

    int vybor = -1;
    bool mouse_free;
    char str[100];


    //?????????? ????????????
    Button btn [COUNT_BTN];
    btn[0] = {50,  50, "??????","??????"};
    btn[1] = {230, 50,"???????","???????"};
    btn[2] = {410, 50, "????", "????"};
    btn[3] = {590, 50, "????","????"};
    btn[4] = {770, 50, "?????","?????"};
    btn[5] = {950, 50, "?????????"};
    btn[6] = {1130, 50,  "?????????"};

    //?????????? ???????????????? ????????
    Pictures menuPictures[100];

    Pictures centralPictures[100];


    COUNT_PICTURES = readFromDir("pictures/??????/", menuPictures, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("pictures/???????/", menuPictures, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("pictures/????/", menuPictures, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("pictures/?????/", menuPictures, COUNT_PICTURES);
    COUNT_PICTURES = readFromDir("pictures/????/", menuPictures, COUNT_PICTURES);



    for(int npic=0; npic < COUNT_PICTURES; npic++)
    {
        menuPictures[npic].x = 50;

        menuPictures[npic].image = txLoadImage (menuPictures[npic].adress.c_str());

        menuPictures[npic].w = get_w(menuPictures[npic].adress);
        menuPictures[npic].h = get_h(menuPictures[npic].adress);

        menuPictures[npic].w_scr = menuPictures[npic].w;
        menuPictures[npic].h_scr = menuPictures[npic].h;

        int pos_1 = menuPictures[npic].adress.find("/");
        int pos_2 = menuPictures[npic].adress.find("/", pos_1+1);
        menuPictures[npic].category = menuPictures[npic].adress.substr(pos_1+1, pos_2-pos_1-1);

        menuPictures[npic].visible = false;

    }

    Fon = txLoadImage("pictures/fon/fon.bmp");

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        txBegin();
        txSetColor (TX_WHITE);
        txSetFillColor (TX_BLACK);

        txClear();

    txBitBlt(txDC(), 0, 0, 1200, 800, Fon);

        txRectangle(200, 100, 1150, 750);

        for(int nk=0; nk<COUNT_BTN; nk++)
        {
            drawButton(btn[nk]);
        }

       for(int npic=0; npic < COUNT_PICTURES; npic++)
       {
           drawPictures(menuPictures[npic]);
       }

       for(int npic=0; npic < COUNT_PICTURES; npic++)
       {
           drawPictures(centralPictures[npic]);
       }

        //?????????????????? ????????-???????????????? ???? ?????????????????? ????????????
        for(int nknopka=0; nknopka<COUNT_BTN; nknopka++)
        {
            if(click(btn[nknopka]))
            {
                for(int npic=0; npic < COUNT_PICTURES; npic++)
                {
                    menuPictures[npic].visible = false;

                    if(menuPictures[npic].category == btn[nknopka].category)
                    {
                        menuPictures[npic].visible = true;
                    }

                }
            }
        }

       //
       for(int npic=0; npic < COUNT_PICTURES; npic++)
       {
           if(txMouseButtons() == 1 &&
              menuPictures[npic].visible &&
              txMouseX() >= menuPictures[npic].x &&
              txMouseX() <= menuPictures[npic].x + menuPictures[npic].w_scr &&
              txMouseY() >= menuPictures[npic].y &&
              txMouseY() <= menuPictures[npic].y + menuPictures[npic].h_scr)
              {
                 while(txMouseButtons() == 1)
                  {
                     txSleep(50);
                  }

                   centralPictures[nCentralPic] = {250,
                                              200,
                                              menuPictures[npic].adress,
                                              menuPictures[npic].image,
                                              menuPictures[npic].w,
                                              menuPictures[npic].h,
                                              menuPictures[npic].w,
                                              menuPictures[npic].h,
                                              menuPictures[npic].category,
                                              menuPictures[npic].visible
                                             };

                 nCentralPic++;
              }
       }

        //?????????? ?????????????????????? ????????????????
        for(int npic=0; npic < nCentralPic; npic++)
       {
           if(txMouseButtons() == 1 &&
              centralPictures[npic].visible &&
              txMouseX() >= centralPictures[npic].x &&
              txMouseX() <= centralPictures[npic].x + centralPictures[npic].w &&
              txMouseY() >= centralPictures[npic].y &&
              txMouseY() <= centralPictures[npic].y + centralPictures[npic].h)
              {

                  vybor=npic;
                  mouse_free = false;
              }

       }
       sprintf(str,"Sohranit= %d nCentralPic= %d COUNT_PICTURES= %d", vybor, nCentralPic, COUNT_PICTURES);
       txTextOut(0,0, str);

       //?????????????????? ?????????????????????? ????????????????
       if(vybor>=0)
       {
          if(GetAsyncKeyState(VK_RIGHT)) centralPictures[vybor].x += 3;
          if(GetAsyncKeyState(VK_LEFT))  centralPictures[vybor].x -= 3;
          if(GetAsyncKeyState(VK_UP))    centralPictures[vybor].y -= 3;
          if(GetAsyncKeyState(VK_DOWN))  centralPictures[vybor].y += 3;
       }

       //fff
       if(vybor>=0)
       {
         if(txMouseButtons() == 1 && !mouse_free)
         {
          centralPictures [vybor].x = txMouseX() - centralPictures[vybor].w/2;
          centralPictures [vybor].y = txMouseY() - centralPictures[vybor].h/2;
         }

          {
             if(txMouseButtons() != 1)
              {
                  mouse_free = true;
              }
          }
       }

       if(vybor>=0 && GetAsyncKeyState(VK_DELETE))
       {
            centralPictures[vybor] = centralPictures[nCentralPic - 1];
            nCentralPic--;
            vybor=-1;
            mouse_free = true;
        }


          //??????????????????
        if(click(btn[BTN_SAVE]))
        {
            string fileName = runFileDialog(true);
            if (fileName != "")
            {
                ofstream fout; //?????? ??? ???? ??????????
                fout.open(fileName); //??????? ???? ??? ??????
                for (int npic = 0; npic < nCentralPic; npic++)
                {
                    if (centralPictures[npic].visible)
                    {
                        fout << centralPictures[npic].x << endl; //??? ?? ????????
                        fout << centralPictures[npic].y << endl;
                        fout << centralPictures[npic].adress << endl;
                    }
                }
                fout.close();            //??????? ????

                txMessageBox("??????????????????", "??????????????", MB_ICONINFORMATION);
            }

        }


        //?????????
        if (click(btn[BTN_LOAD]))
        {
            string fileName = runFileDialog(false);
            if (fileName != "")
            {
                for (int npic = 0; npic < COUNT_PICTURES; npic++)
                {
                    centralPictures[npic].visible = false;
                }

                char buff[50];              // ???? ????? ????????? ?????
                ifstream fin(fileName);      //??????? ???? ??? ??????
                while (fin.good())
                {
                    fin.getline(buff, 50); // ??????? ?????? ?? ?????
                    int x = atoi(buff);
                    fin.getline(buff, 50); // ??????? ?????? ?? ?????
                    int y = atoi(buff);
                    fin.getline(buff, 50); // ??????? ?????? ?? ?????
                    string adress = (buff);

                    for (int npic = 0; npic < COUNT_PICTURES; npic++)
                    {
                        if (menuPictures[npic].adress == adress)
                        {

                            centralPictures[nCentralPic] =   {x,
                                                              y,
                                                              menuPictures[npic].adress,
                                                              menuPictures[npic].image,
                                                              menuPictures[npic].w,
                                                              menuPictures[npic].h,
                                                              menuPictures[npic].w,
                                                              menuPictures[npic].h,
                                                              menuPictures[npic].category,
                                                              true
                                                             };
                            nCentralPic++;
                        }
                    }
                }
                fin.close();
            }
        }

        txEnd();
        txSleep(50);

    }

    txDeleteDC(Fon);
    for(int npic=0; npic < COUNT_PICTURES; npic++)
       {
           txDeleteDC(menuPictures[npic].image);
       }

    for(int npic=0; npic < nCentralPic; npic++)
       {
           txDeleteDC(centralPictures[npic].image);
       }


return 0;
}



