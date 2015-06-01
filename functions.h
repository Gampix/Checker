#ifndef FUNCTIONS
#define FUNCTIONS
#include "widget.h"
#include "qlabel"
#include "ui_widget.h"

#include <stdlib.h>

int hod = 1; // show us which player should move checkers 1 - white 2 -black
int kill_pos = 0; // if kill=1 player can kill one or more enemy checkers
int killed = 0;



struct square
{
    int x;
    int y;
    int col; // 0 - no checker , 1 - white , 2 -black
    int queen; // 0 - not queen , 1 - this checker is queen
    int move;  // 0 - you can't move checker in this square , 1 - you can
    int kill;  // 0 - you can't kill checker in this square , 1 - you can
    struct square * moved; // 0 - you can't move this checker in possible square , 1 - you can
    QLabel* lab;
};

square mass[8][8];

void start_positions() //put checkers in start positions
{
    int i=0,b=0;
    for (i = 0; i <= 7; i++)
    {
        for (b = 0; b <= 3; b++)
        {
            if (i % 2 != 0)
            {
                mass[2*b][i].x = 2 * b;
                mass[2*b][i].y = i;
                mass[2*b][i].queen = 0;
                mass[2*b][i].kill = 0;
                mass[2*b][i].move = 0;
                mass[2*b][i].moved = NULL;
                if (i <= 2) mass[2*b][i].col = 2;
                else if (i <= 4) mass[2*b][i].col = 0;
                else if (i <= 7) mass[2*b][i].col = 1;
            }

            if (i % 2 == 0)
            {   mass[2*b+1][i].queen = 0;
                mass[2*b+1][i].kill = 0;
                mass[2*b+1][i].move = 0;
                mass[2*b+1][i].moved = NULL;
                mass[2*b+1][i].x = (2 * b) + 1;
                mass[2*b+1][i].y = i;
                if (i <= 2) mass[2*b+1][i].col = 2;
                else if (i <= 4) mass[2*b+1][i].col = 0;
                else if (i <= 7) mass[2*b+1][i].col = 1;
            }
        }
    }

}

int check_kill(square* sq, square* mv)  //check can checker sq kill enemy between sq and mv
{
    int posx=(sq->x+mv->x)/2, posy=(sq->y+mv->y)/2;
    if ((mass[posx][posy].col!=hod)&&(mass[posx][posy].col!=0)&&(mv->col==0))
    {
        mv->kill = 1;
        mv->moved = sq;
        return 1;
    }
    else return 0;
}

int check_kill_queen(square* kill,square* sq, square* mv)  //check can checker sq kill enemy between sq and mv
{
    int posx=(sq->x+mv->x)/2, posy=(sq->y+mv->y)/2;
    if ((mass[posx][posy].col!=hod)&&(mass[posx][posy].col!=0)&&(mv->col==0))
    {
        mv->kill = 1;
        mv->moved = kill;
        return 1;
    }
    else return 0;
}

void check_move(square* sq, square* mv) //check can checcker sq move to square mv
{
    if ((*mv).col != 0) return;
    else
    {
        (*mv).move = 1;
        (*mv).moved = sq;
        return;
    }
}

int check_all_kills() //check can all checkers kill anyone.
{
    int i, k, a = 0;
    for (i = 0; i < 8; i++)
    {
        for (k = 0; k < 8; k++)
        {
            if (mass[k][i].col == hod && mass[k][i].queen == 0)
            {
                if ((k + 2 < 8) && (i + 2 < 8)) a = a + check_kill(&(mass[k][i]), &(mass[k+2][i+2]));
                if ((k + 2 < 8) && (i - 2 > 0)) a = a + check_kill(&(mass[k][i]), &(mass[k+2][i-2]));
                if ((k - 2 > 0) && (i + 2 < 8)) a = a + check_kill(&(mass[k][i]), &(mass[k-2][i+2]));
                if ((k - 2 > 0) && (i - 2 > 0)) a = a + check_kill(&(mass[k][i]), &(mass[k-2][i-2]));
            }
            else if (mass[k][i].col == hod && mass[k][i].queen == 1)
            {int x1=0,x2=0,x3=0,x4=0,m;
                for (m=0;m<=7;m++)
                {   if ((k + 2 + m < 8) && (i + 2 + m < 8) && (x1==0))
                        {x1 = x1 + check_kill_queen(&(mass[k][i]), &(mass[k+m][i+m]), &(mass[k+2+m][i+2+m]));
                        a=a+x1;
                        }
                    if ((k + 2 + m < 8) && (i - 2 - m > 0) && (x2==0))
                        {x2 = x2 + check_kill_queen(&(mass[k][i]), &(mass[k+m][i-m]), &(mass[k+2+m][i-2-m]));
                        a=a+x2;
                        }
                    if ((k - 2 - m > 0) && (i + 2 + m < 8) && (x3==0))
                        {x3 = x3 + check_kill_queen(&(mass[k][i]), &(mass[k-m][i+m]), &(mass[k-2-m][i+2+m]));
                        a=a+x3;
                        }
                    if ((k - 2 - m > 0) && (i - 2 + m > 0) && (x4==0))
                        {x4 = x4 + check_kill_queen(&(mass[k][i]), &(mass[k-m][i-m]), &(mass[k-2-m][i-2-m]));
                        a=a+x4;
                        }

                }
            }
        }
    }
    return a;
}

void clear_tags()
{
    int i, k;
    for (i = 0; i < 8; i++)
    {
        for (k = 0; k < 8; k++)
        {if ((mass[i][k].move != 0)||(mass[i][k].kill != 0))
            {
                mass[i][k].move = 0;
                mass[i][k].kill = 0;
                mass[i][k].moved = NULL;
                mass[i][k].lab->setPixmap(QPixmap(":new/null_1.png"));
            }
        }

    }
}

void move(square* sq)
{
    if (sq->move != 1) return;
    else
    {
        sq->col = sq->moved->col;
        sq->moved->col=0;
        sq->moved->lab->setPixmap(QPixmap(":new/null_1.png"));
        sq->queen = sq->moved->queen;
        sq->moved->queen = 0;
        clear_tags();
        if ((sq->y == 0 && sq->col == 1) || (sq->y == 7 && sq->col == 2)) sq->queen=1;
    }
}

void kill(square* sq)
{
    int posx, posy;
    if ((*sq).kill != 1) return;
    else
    {   if(sq->moved->queen == 1)
            {sq->col = sq->moved->col;
             sq->moved->col = 0;
             sq->queen = sq->moved->queen;
             sq->moved->queen = 0;
             if (sq->x > sq->moved->x) (posx = sq->x - 1);
             if (sq->x < sq->moved->x) (posx = sq->x + 1);
             if (sq->y > sq->moved->y) (posy = sq->y - 1);
             if (sq->y < sq->moved->y) (posy = sq->y + 1);
             mass[posx][posy].col = 0;
             mass[posx][posy].queen = 0;
             clear_tags();
             return;
            }
        else
            {(*sq).col = (*((*sq).moved)).col;
            (*((*sq).moved)).col = 0;
            (*sq).queen = (*((*sq).moved)).queen;
            (*((*sq).moved)).queen = 0;
            posx = (sq->x + sq->moved->x) / 2;
            posy = (sq->y + sq->moved->y) / 2;
            mass[posx][posy].col = 0;
            mass[posx][posy].queen = 0;
            clear_tags();
            return;
            }
    }

}

void check_hod(square* sq)
{
    int posx = sq->x, posy = sq->y - 1;
    if ((sq->col != hod) || (sq->col == 0)) return;
    if ((sq->col == 1)&&(sq->queen == 0))
        {
        if (posy < 0) return;
        else if ((posx - 1 < 0) && (mass[posx+1][posy].col != 0)) return;
        else if ((posx + 1 > 7) && (mass[posx-1][posy].col != 0)) return;
        if ((mass[posx+1][posy].col == 0) && (posx+1<=7))
            {
                mass[posx+1][posy].move = 1;
                mass[posx+1][posy].moved = sq;
            }
        if ((mass[posx-1][posy].col == 0) && (posx-1>=0))
            {
                mass[posx-1][posy].move = 1;
                mass[posx-1][posy].moved = sq;
            }
        }
    posy = posy + 2;
    if ((sq->col == 2)&&(sq->queen == 0))
        {
            if (posy >7) return;
            else if ((posx - 1 < 0) && (mass[posx+1][posy].col != 0)) return;
            else if ((posx + 1 > 7) && (mass[posx-1][posy].col != 0)) return;
            if ((mass[posx+1][posy].col == 0) && (posx + 1<=7))
            {
                mass[posx+1][posy].move = 1;
                mass[posx+1][posy].moved = (sq);
            }
            if ((mass[posx-1][posy].col == 0) && (posx - 1>=0))
            {
                mass[posx-1][posy].move = 1;
                mass[posx-1][posy].moved = (sq);
            }
        }
    posy = posy - 1;
    if (sq->queen == 1)
        {
        int i;
        for (i=0;i<7;i++)
            {if ((i+posx<8) && (i+posy<8) && (mass[posx+i][posy+i].col==0))
                {
                    mass[posx+i][posy+i].move = 1;
                    mass[posx+i][posy+i].moved = (sq);
                }
            if ((i+posx<8) && (posy-i>=0) && (mass[posx+i][posy-i].col==0))
                {
                    mass[posx+i][posy-i].move = 1;
                    mass[posx+i][posy-i].moved = (sq);
                }
            if ((posx-i>=0) && (posy+i<8) && (mass[posx-i][posy+i].col==0))
                {
                    mass[posx-i][posy+i].move = 1;
                    mass[posx-i][posy+i].moved = (sq);
                }
            if ((posx-i>=0) && (posy-i>=0) && (mass[posx-i][posy-i].col==0))
                {
                    mass[posx-i][posy-i].move = 1;
                    mass[posx-i][posy-i].moved = (sq);
                }

            }

        }

}

void press(square* sq)
{   if ((sq->col != hod)&&(sq->move != 1)&&(sq->kill != 1)) return;
    if (kill_pos!=0)
    {
        if (sq->kill == 1)
        {
            kill(sq);
            killed++;
            kill_pos=0;
            clear_tags();
            press(sq);
        }
        else return;
    }
    else (kill_pos=check_all_kills());
    if (kill_pos!=0) return;
    else if (killed!=0)
        {
            if (hod==1) hod=2;
            else hod=1;
            killed=0;
        }
    else if (sq->move == 1)
        {
            move(sq);
            if (hod==1) hod=2;
            else hod=1;
        }

    if ((sq->move != 1) && (sq->kill != 1)) clear_tags();
    if ((sq->move != 1) && (sq->kill != 1) && (sq->col == 0)) return;

    check_hod(sq);
}

void my_print(square sq)
{
    if (sq.col==1 && sq.queen==1) (sq.lab->setPixmap(QPixmap(":new/white_q.png")));
    else if (sq.col==2 && sq.queen==1) (sq.lab->setPixmap(QPixmap(":new/black_q.png")));
    else if (sq.move==1) (sq.lab->setPixmap(QPixmap(":new/move.png")));
    else if (sq.kill==1) (sq.lab->setPixmap(QPixmap(":new/kill.png")));
    else if (sq.col==2) (sq.lab->setPixmap(QPixmap(":new/black.png")));
    else if (sq.col==1) (sq.lab->setPixmap(QPixmap(":new/white.png")));
    else (sq.lab->setPixmap(QPixmap(":new/null_1.png")));
}

void all_print()
{int i, k;
    for(i=0;i<=7;i++)
    {
        for(k=0;k<=7;k++)
        {
            if(((i%2==0)&&(k%2!=0))||((i%2!=0)&&(k%2==0)))
            my_print(mass[k][i]);
        }
    }
}

////////////////////////////////artificial intelligence////////////////////////////////
square mass_copy[8][8];
int peak=0;
int level=0;
int peak1=0;
int peak2=0;
int peak3=0;
int killed_pc=0;int posx,posy;

int kill_pos_pc = 0;

struct hodpc
{int x,y;
 int x1,y1;
 int cost;
 int type; // 0-move 1-kill
};

hodpc mass_hod[20];
hodpc mass_hod1[20];
hodpc mass_hod2[20];
hodpc mass_hod3[20];

hodpc* besthod()
{int i;hodpc* a=&mass_hod[0];int b;
    for(i=0;i<peak;i++)
    {
        if (mass_hod[i].cost>a->cost) a=&mass_hod[i];
        else if (mass_hod[i].cost==a->cost)
        {
            b=rand() % 2;
            if (b==0) a=a;
            if (b==1) a=&mass_hod[i];
        }
    }
    return a;
}

int check_kill_pc(square* sq, square* mv, hodpc* mass_hod, int * peak)  //check can checker sq kill enemy between sq and mv
{
    int posx=(sq->x+mv->x)/2, posy=(sq->y+mv->y)/2;
    if ((mass_copy[posx][posy].col!=hod)&&(mass_copy[posx][posy].col!=0)&&(mv->col==0))
    {
        (mass_hod+*peak)->x=sq->x;
        (mass_hod+*peak)->y=sq->y;
        (mass_hod+*peak)->x1=mv->x;
        (mass_hod+*peak)->y1=mv->y;
        (mass_hod+*peak)->cost=0;
        (mass_hod+*peak)->type=1;
        *peak++;
        return 1;
    }
    else return 0;
}

int cakp(hodpc* mass_hod, int* peak) //check can all checkers kill anyone.
{
    int i, k, a = 0;
    for (i = 0; i < 8; i++)
    {
        for (k = 0; k < 8; k++)
        {
            if (mass_copy[k][i].col == hod && mass_copy[k][i].queen == 0)
            {
                if ((k + 2 < 8) && (i + 2 < 8)) a = a + check_kill_pc(&(mass_copy[k][i]), &(mass_copy[k+2][i+2]),mass_hod, peak);
                if ((k + 2 < 8) && (i - 2 >= 0)) a = a + check_kill_pc(&(mass_copy[k][i]), &(mass_copy[k+2][i-2]),mass_hod, peak);
                if ((k - 2 >= 0) && (i + 2 < 8)) a = a + check_kill_pc(&(mass_copy[k][i]), &(mass_copy[k-2][i+2]),mass_hod, peak);
                if ((k - 2 >= 0) && (i - 2 >= 0)) a = a + check_kill_pc(&(mass_copy[k][i]), &(mass_copy[k-2][i-2]),mass_hod, peak);
            }
            /*else if (mass_copy[k][i].col == hod && mass_copy[k][i].queen == 1)
            {
                int x1=0,x2=0,x3=0,x4=0;
                if ((k+1<8)&&(i+1<8)) x1 = check_kill_queen(&(mass_copy[k][i]), &(mass_copy[k+1][i+1]));
                if ((k+1<8)&&(i-1>=0)) x2 = check_kill_queen(&(mass_copy[k][i]), &(mass_copy[k+1][i-1]));
                if ((k-1>=0)&&(i+1<8)) x3 = check_kill_queen(&(mass_copy[k][i]), &(mass_copy[k-1][i+1]));
                if ((k-1>=0)&&(i-1>=0)) x4 = check_kill_queen(&(mass_copy[k][i]), &(mass_copy[k-1][i-1]));
                a=x1+x2+x3+x4;
            }*/
        }
    }
    return a;
}

int check_kills_pc(int i)
{
    if (i==1) return cakp(mass_hod1,&peak1);
    if (i==2) return cakp(mass_hod2,&peak2);
    if (i==3) return cakp(mass_hod3,&peak3);
}

void clear_pc(int i)
{int k;
    if (i==1)
 for(k=0;k<20;k++)
 {
   mass_hod[k].cost=0;
 }
 for(k=0;k<20;k++)
 {
   mass_hod1[k].cost=0;
 }
    if (i==2)
 for(k=0;k<20;k++)
 {
   mass_hod2[k].cost=0;
 }
    if (i==3)
 for(k=0;k<20;k++)
 {
   mass_hod3[k].cost=0;
 }
}

void chp(square* sq, hodpc* mass_hod, int* peak)
{
    int posx = sq->x, posy = sq->y - 1;
    if ((sq->col != hod) || (sq->col == 0)) return;
    if ((sq->col == 1)&&(sq->queen == 0))
        {
        if (posy < 0) return;
        else if ((posx - 1 < 0) && (mass[posx+1][posy].col != 0)) return;
        else if ((posx + 1 > 7) && (mass[posx-1][posy].col != 0)) return;
        if ((mass[posx+1][posy].col == 0) && (posx+1<=7))
            {
            (mass_hod+*peak)->x=sq->x;
            (mass_hod+*peak)->y=sq->y;
            (mass_hod+*peak)->x1=posx+1;
            (mass_hod+*peak)->y1=posy;
            (mass_hod+*peak)->cost=0;
            (mass_hod+*peak)->type=0;
            (*peak)++;
            }
        if ((mass[posx-1][posy].col == 0) && (posx-1>=0))
            {
            (mass_hod+*peak)->x=sq->x;
            (mass_hod+*peak)->y=sq->y;
            (mass_hod+*peak)->x1=posx-1;
            (mass_hod+*peak)->y1=posy;
            (mass_hod+*peak)->cost=0;
            (mass_hod+*peak)->type=0;
            (*peak)++;
            }
        }
    posy = posy + 2;
    if ((sq->col == 2)&&(sq->queen == 0))
        {
            if (posy >7) return;
            else if ((posx - 1 < 0) && (mass_copy[posx+1][posy].col != 0)) return;
            else if ((posx + 1 > 7) && (mass_copy[posx-1][posy].col != 0)) return;
            if ((mass_copy[posx+1][posy].col == 0) && (posx + 1<=7))
            {
                (mass_hod+*peak)->x=sq->x;
                (mass_hod+*peak)->y=sq->y;
                (mass_hod+*peak)->x1=posx+1;
                (mass_hod+*peak)->y1=posy;
                (mass_hod+*peak)->cost=0;
                (mass_hod+*peak)->type=0;
                (*peak)++;
            }
            if ((mass_copy[posx-1][posy].col == 0) && (posx - 1>=0))
            {
                (mass_hod+*peak)->x=sq->x;
                (mass_hod+*peak)->y=sq->y;
                (mass_hod+*peak)->x1=posx-1;
                (mass_hod+*peak)->y1=posy;
                (mass_hod+*peak)->cost=0;
                (mass_hod+*peak)->type=0;
                (*peak)++;
            }
        }
    posy = posy - 1;
    /*if (sq->queen == 1)
        {
        int i;
        for (i=0;i<7;i++)
            {if ((i+posx<8) && (i+posy<8) && (mass_copy[posx+i][posy+i].col==0))
                {
                    mass_hod[peak].x=sq->x;
                    mass_hod[peak].y=sq->y;
                    mass_hod[peak].x1=posx+i;
                    mass_hod[peak].y1=posy+i;
                    mass_hod[peak].cost=0;
                    peak++;
                }
            if ((i+posx<8) && (posy-i>=0) && (mass_copy[posx+i][posy-i].col==0))
                {
                    mass_hod[peak].x=sq->x;
                    mass_hod[peak].y=sq->y;
                    mass_hod[peak].x1=posx+i;
                    mass_hod[peak].y1=posy-i;
                    mass_hod[peak].cost=0;
                    peak++;
                }
            if ((posx-i>=0) && (posy+i<8) && (mass_copy[posx-i][posy+i].col==0))
                {
                    mass_hod[peak].x=sq->x;
                    mass_hod[peak].y=sq->y;
                    mass_hod[peak].x1=posx-i;
                    mass_hod[peak].y1=posy+i;
                    mass_hod[peak].cost=0;
                    peak++;
                }
            if ((posx-i>=0) && (posy-i>=0) && (mass_copy[posx-i][posy-i].col==0))
                {
                    mass_hod[peak].x=sq->x;
                    mass_hod[peak].y=sq->y;
                    mass_hod[peak].x1=posx-i;
                    mass_hod[peak].y1=posy-i;
                    mass_hod[peak].cost=0;
                    peak++;
                }
            }
        }*/
}

void check_hod_pc(square* sq, int i)
{
    if (i==1) chp(sq,mass_hod1,&peak1);
    if (i==2) chp(sq,mass_hod2,&peak2);
    if (i==3) chp(sq,mass_hod3,&peak3);
}

void copy()
{int i,k;
 for(i=0;i<8;i++)
    {
     for(k=0;k<8;k++)
        {
            mass_copy[i][k].col=mass[i][k].col;
            mass_copy[i][k].x=mass[i][k].x;
            mass_copy[i][k].y=mass[i][k].y;
            mass_copy[i][k].queen=mass[i][k].queen;
        }
    }
}

void move(hodpc* bazan)
{
    int x,y,x1,y1;x=bazan->x;y=bazan->y;x1=bazan->x1;y1=bazan->y1;
    mass[x1][y1].col=mass[x][y].col;
    mass[x1][y1].queen=mass[x][y].queen;
    mass[x][y].col=0;
    mass[x][y].queen=0;
    hod=1;
}

void move_in_mind(hodpc* bazan)
{
    int x,y,x1,y1;x=bazan->x;y=bazan->y;x1=bazan->x1;y1=bazan->y1;
    mass_copy[x1][y1].col=mass_copy[x][y].col;
    mass_copy[x1][y1].queen=mass_copy[x][y].queen;
    mass_copy[x][y].col=0;
    mass_copy[x][y].queen=0;
}

void kill_pc(hodpc* bazan)
{
    int x,y,x1,y1,posx,posy;x=bazan->x;y=bazan->y;x1=bazan->x1;y1=bazan->y1;posx=(bazan->x+bazan->x1)/2;posy=(bazan->y+bazan->y1)/2;
    mass[x1][y1].col=mass[x][y].col;
    mass[x1][y1].queen=mass[x][y].queen;
    mass[x][y].col=0;
    mass[x][y].queen=0;
    mass[posx][posy].col=0;
    mass[posx][posy].queen=0;
}

void kill_pc_in_mind(hodpc* bazan)
{
    int x,y,x1,y1,posx,posy;x=bazan->x;y=bazan->y;x1=bazan->x1;y1=bazan->y1;posx=(bazan->x+bazan->x1)/2;posy=(bazan->y+bazan->y1)/2;
    mass_copy[x1][y1].col=mass_copy[x][y].col;
    mass_copy[x1][y1].queen=mass_copy[x][y].queen;
    mass_copy[x][y].col=0;
    mass_copy[x][y].queen=0;
    mass_copy[posx][posy].col=0;
    mass_copy[posx][posy].queen=0;
}


void assesment(int number, hodpc* mass_hod)
{
    int x=(mass_hod+number)->x1;
    if ((mass_hod+number)->type==1) (mass_hod+number)->cost=(mass_hod+number)->cost+10;//
    if ((x>=3) && (x<=4)) (mass_hod+number)->cost=(mass_hod+number)->cost+1;         //
    else if ((x>=1) && (x<=6)) (mass_hod+number)->cost=(mass_hod+number)->cost+2;    //Possition assesment
    else if ((x==0) || (x==7)) (mass_hod+number)->cost=(mass_hod+number)->cost+3;    //
    //if ((mass_hod+number)->type==0) move_in_mind(mass_hod+number);
    //if ((mass_hod+number)->type==1) kill_pc_in_mind(mass_hod+number);
   /* level++;
    if (level<4)
    {
        kill_pos_pc=0;
        kill_pos_pc=check_kills_pc(level);
        int i,k;
        for(i=0;i<8;i++)
        {
            for(k=0;k<8;k++)
        {
           if (kill_pos_pc==0) check_hod_pc(&mass[i][k],i);
           kill_pos=0;
        }
        }
    }*/

}


void pc_hod()
{
    copy();int i,k;hodpc* best;

    kill_pos_pc=cakp(mass_hod,&peak);

    if (kill_pos_pc==0 && killed_pc==0)
    {
        for(i=0;i<8;i++)
        {
            for(k=0;k<8;k++)
            {
                chp(&mass_copy[i][k],mass_hod,&peak);
            }
        }
    }
    for(i=0;i<peak;i++)
    {
        assesment(i,mass_hod);
    }
    best=besthod();
    if (best->type==0 && killed_pc==0)
    {
        move(best);
        clear_pc(0);
        peak=0;
        return;
    }
    if (best->type==1 && killed_pc==0)
    {
        posx=best->x1; posy=best->y1;
        kill_pc(best);
        clear_pc(0);peak=0;
        if (cakp(mass_hod,&peak)!=0)
        {
            killed_pc=1;
            pc_hod();
        }

        hod=1;
        return;
    }
    if (killed_pc == 1)
    {
        for(i=0;i<peak;i++)
        {
            if ((mass_hod[i].x==posx) && (mass_hod[i].y==posy)) kill_pc(&(mass_hod[i]));
            if (cakp(mass_hod,&peak)!=0)
            {
                killed_pc=1;
                pc_hod();
            }
            else killed_pc=0;
            return;
        }
    }
}

void press(square* sq)
{   if ((sq->col != hod)&&(sq->move != 1)&&(sq->kill != 1)) return;
    if (kill_pos!=0)
    {
        if (sq->kill == 1)
        {
            kill(sq);
            killed++;
            kill_pos=0;
            clear_tags();
            press(sq);
            return;
        }
        else return;
    }
    else (kill_pos=check_all_kills());
    if (kill_pos!=0) return;
    else if (killed!=0)
        {
            if (hod==1)
            {
                hod=2; ///////////////////////////////
                pc_hod();
                killed=0;
                return;
            }

            else hod=1;
        }
    else if (sq->move == 1)
        {
            move(sq);
            if (hod==1)
            {
                hod=2;
                pc_hod();
                return;
            }
            else hod=1;
        }

    if ((sq->move != 1) && (sq->kill != 1)) clear_tags();
    if ((sq->move != 1) && (sq->kill != 1) && (sq->col == 0)) return;

    check_hod(sq);
}


#endif // FUNCTIONS


