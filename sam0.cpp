#include <ncurses.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <stdio.h>

using namespace std;

int spot = 0;

char newMenu()
{
  WINDOW *winMenu;
  winMenu = newwin(LINES/2, COLS/2, LINES/4, COLS/4);
  wprintw(winMenu, "Enter (1-4) from the options below: \n");
  wprintw(winMenu, "1. Open \n");
  wprintw(winMenu, "2. Save \n");
  wprintw(winMenu, "3. Save As \n");
  wprintw(winMenu, "4. Exit \n");
  wrefresh(winMenu);
  char input = wgetch(winMenu);
  werase(winMenu);
  wrefresh(winMenu);
  delwin(winMenu);
  return input;
}//newMenu

string askForFile()
{
  WINDOW *winFile;
  winFile = newwin(LINES/4, COLS/2, LINES/4, COLS/4);
  wprintw(winFile, "Enter the name of the file: ");
  wmove(winFile, 1, 0);
  wrefresh(winFile);
  string input;
  char c = wgetch(winFile);
  while (c != '\n')
    {
      input += c;
      c = wgetch(winFile);
    }
  werase(winFile);
  wrefresh(winFile);
  delwin(winFile);
  return input;
}//askForFile

void printBuffer(vector<char> x, WINDOW *y)
{
  for (auto &element:x)
    {
      string temp;
      temp += element;
      wprintw(y, temp.c_str());
    }
}

void printScroll(vector<char> x, WINDOW *y, int b)
{
  
  
  for (int a = 0; lns.size() > a; ++a)
    {
    for(int b = 0; b < lns[a]; ++b){
      string temp;
      temp += x[a];
      wprintw(y, temp.c_str());
      }
    }
}


int main(int argc, char *argv[])
{
  initscr();
  scrollok(stdscr, TRUE);
  printw("F1: Menu");
  move(0, COLS/2 - 8);
  printw("CSCI 1730 Editor!");
  move(LINES - 1, 0);
  printw("<NO NAME>");
  refresh();
  WINDOW *border;
  border = newwin(LINES/(1.05), COLS/(1.05), 1, 1);
  box(border, 0, 0);
  wrefresh(border);
  WINDOW *editor;
  editor = newwin(LINES/(1.1), COLS/(1.1), 2, 2);
  keypad(editor, TRUE);
  int c;
  int inc;
  int ouc;
  bool exit = false;
  vector<char> buffer;
  //char* lines = new char[];
  while (!exit)
    {
      c = wgetch(editor);
      scrollok(editor, TRUE);
      idlok(editor, TRUE);
     
      if( c != KEY_LEFT && c != KEY_RIGHT && c != KEY_UP && c != KEY_DOWN){
        buffer.push_back(c);
        spot++;
      }
      if (c == KEY_F(1))
        {
           int option = newMenu();
           if (option == '1')
             {
               buffer.clear();
               string xfile = askForFile();
               ifstream f(xfile);
               werase(editor);
               wrefresh(editor);
               string line;
               if (f.good())
                 {
                   while (!f.eof())
                     {
                       getline(f, line);
                       const char* temp = line.c_str();
                       for(int x = 0; x < line.length(); ++x){
                         
                         buffer.push_back(temp[x]);
                         spot++;
                       }
                     
                       wprintw(editor, line.c_str());
                       wprintw(editor, "\n");
                    
                     }
                 }
               wrefresh(editor);
               move(LINES - 1, 0);
               printw(xfile.c_str());
               printw("                             ");
               refresh();
               wmove(editor, 0, 0);
             }
           if (option == '2')
             {

             }
           if (option == '3')
             {
               string fileName = askForFile();
               int newFile = creat(fileName.c_str(), 0666);
               char *chBuffer = new char[buffer.size()];
               for (unsigned int x = 0; x < buffer.size(); ++x)
                 {
                   chBuffer[x] = buffer[x];
                 }
               int nbytes = write(newFile, chBuffer, buffer.size()-1);
               move(LINES - 1, 0);
               printw(fileName.c_str());
               printw("                             ");
               refresh();
               delete chBuffer;
             }
           if (option == '4')
             {
               exit = true;
             }
         }
       if (c == 127 || c == 8)
         {
           buffer.erase(buffer.begin() + spot);
           werase(editor);
           printBuffer(buffer, editor);
           wrefresh(editor);
         }
       if (c == KEY_LEFT){
        int x, y;
        getyx(editor, y, x);
        wmove(editor, y,x-1);
        wrefresh(editor);
        spot--;

       }
       if (c == KEY_UP){
        int x, y;
        getyx(editor, y, x);
        wmove(editor, y-1,x);
        wrefresh(editor);
        if(y <=0){
          printScroll(buffer, editor, buffer.size());
          wscrl(editor, -1);
          wrefresh(editor); 
        }
       }
       if (c == KEY_RIGHT){
        int x, y;
        getyx(editor, y, x);
        wmove(editor, y,x+1);
        wrefresh(editor);
        spot++;
       }
       if (c == KEY_DOWN){
        int x, y;
        getyx(editor, y, x);
        wmove(editor, y+1,x);
        wrefresh(editor);
        int p = getmaxy(editor)-1;
       
        if(y >= p){
          scroll(editor);
          //printBuffer(buffer, editor);
          wrefresh(editor);
        }
 
       }
       if ( c == '\n'){
          int x, y;
          getyx(editor, y, x);
          wmove(editor, y+1,0);
          wrefresh(editor);
          int p = getmaxy(editor)-1;
   
          if(y >= p){
            scroll(editor);
            wmove(editor, y+1,0);
            
            //printBuffer(buffer, editor);
            wrefresh(editor);
          }
        
        }
     }
  
  delwin(editor);
  endwin();
  return EXIT_SUCCESS;
}//main
