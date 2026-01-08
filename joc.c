#include <stdlib.h>
#include <stdio.h>  
#include <time.h>    
#include <unistd.h>
#include <ncurses.h>

// dimensiunile tablei de joc
#define latime 10
#define inaltime 20

//matricea cu toate piesele tetris in toate rotatiile lor
//[tip piesa][rotatie][rand][coloana]
int piese[7][4][4][4] = {

    // 0 - I 
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        }
    },

    // 1 - O 
    {
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },

    // 2 - T
    {
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },

    // 3 - S 
    {
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        }
    },

    // 4 - Z 
    {
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },

    // 5 - J 
    {
        {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },

    // 6 - L 
    {
        {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    }
};

//structura care descrie o piesa curenta
typedef struct{
  int tip;       //tipul piesei 
  int rotatie;   //rotatia curenta 
  int x, y;      //pozitia pe tabla
} Piesa;

//muta piesa in jos cu o pozitie
void deplasareJos(Piesa *p){
  p->y++;
}

//muta piesa la dreapta cu o pozitie
void deplasareDreapta(Piesa *p){
  p->x++;
}

//muta piesa la stanga cu o pozitie
void deplasareStanga(Piesa *p){
  p->x--;
}

//roteste piesa cu 90 de grade in sensul acelor de ceasornic
void rotire(Piesa *p){
  p->rotatie = (p->rotatie + 1) % 4;
}

//verifica daca piesa se suprapune cu tabla sau iese din margini
//returneaza 1 daca exista coliziune, 0 altfel
int coliziuneCuTabla(Piesa p, int tabla[inaltime][latime]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            //verifica fiecare bloc ocupat din piesa
            if(piese[p.tip][p.rotatie][i][j]){
                int y = p.y + i;
                int x = p.x + j;
                //verifica daca iese din margini
                if(x<0 || x>=latime || y>=inaltime) return 1;
                //verifica daca se suprapune cu un bloc deja fixat
                if(y >= 0 && tabla[y][x]) return 1;
            }
        }
    }
    return 0;
}

//fixeaza piesa pe tabla (cand nu mai poate cadea)
void fixarePiesa(Piesa p, int tabla[inaltime][latime]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(piese[p.tip][p.rotatie][i][j]){
                tabla[p.y+i][p.x+j]=1;
            }
        }
    }
}

//sterge liniile complete si actualizeaza scorul
void stergeLinii(int tabla[inaltime][latime], int *scor){
    for(int i=inaltime-1;i>=0;i--){
        //verifica daca linia este completa
        int full = 1;
        for(int j=0;j<latime;j++){
            if(tabla[i][j]==0){
                full = 0;
                break;
            }
        }
        //daca linia e completa, o sterge si coboara tot ce e deasupra
        if(full){
            (*scor)++;
            //muta toate liniile de deasupra in jos
            for(int k=i;k>0;k--){
                for(int j=0;j<latime;j++){
                    tabla[k][j] = tabla[k-1][j];
                }
            }
            //goleste prima linie
            for(int j=0;j<latime;j++) tabla[0][j]=0;
            i++; //verifica din nou aceeasi linie (acum contine linia de deasupra)
        }
    }
}

//deseneaza tabla de joc, piesa curenta si informatii
void afisare(int tabla[inaltime][latime], Piesa p, int scor){
    clear();
    //deseneaza fiecare celula din tabla
    for(int i=0;i<inaltime;i++){
        for(int j=0;j<latime;j++){
            int bloc = tabla[i][j];
            //verifica daca piesa curenta ocupa aceasta pozitie
            for(int ii=0;ii<4;ii++){
                for(int jj=0;jj<4;jj++){
                    if(piese[p.tip][p.rotatie][ii][jj]){
                        if(i == p.y+ii && j == p.x+jj)
                            bloc = 1;
                    }
                }
            }
            //deseneaza '#' pentru bloc ocupat, '.' pentru gol
            mvaddch(i, j, bloc ? '#' : '.');
        }
    }

    //afiseaza scorul si controalele in dreapta tablei
    mvprintw(0, latime+2, "Scor: %d", scor);
    mvprintw(2, latime+2, "Controale:");
    mvprintw(3, latime+2, "A/D - Stanga/Dreapta");
    mvprintw(4, latime+2, "W - Rotire");
    mvprintw(5, latime+2, "S - Jos rapid");
    mvprintw(6, latime+2, "Q - Iesire");
    
    refresh();
}

int main(){
    //initializeaza generatorul de numere aleatoare
    srand(time(NULL));

    //initializeaza ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  
    curs_set(0);            

    //initializeaza tabla goala
    int tabla[inaltime][latime] = {0};
    int scor = 0;

    //creeaza prima piesa aleatoare
    Piesa p;
    p.tip = rand() % 7;
    p.rotatie = 0;
    p.x = latime/2 - 2;
    p.y = 0;

    int counter = 0;
    int viteza = 5; //cat de repede cade piesa 

    //bucla principala a jocului
    while(1){
        afisare(tabla, p, scor);

        //citeste input-ul jucatorului
        int ch = getch();
        Piesa temp = p;
        
        //miscare la stanga
        if(ch == 'a' || ch == 'A' || ch == KEY_LEFT){
            deplasareStanga(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                p = temp;
            }
        }
        //miscare la dreapta
        else if(ch == 'd' || ch == 'D' || ch == KEY_RIGHT){
            deplasareDreapta(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                p = temp;
            }
        }
        //rotire
        else if(ch == 'w' || ch == 'W' || ch == KEY_UP){
            rotire(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                p = temp;
            }
        }
        //miscare rapida in jos
        else if(ch == 's' || ch == 'S' || ch == KEY_DOWN){
            deplasareJos(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                p = temp;
            }
        }
        //iesire din joc
        else if(ch == 'q' || ch == 'Q'){
            break;
        }

        //gravitate automata - piesa cade in jos periodic
        counter++;
        if(counter >= viteza){
            counter = 0;
            temp = p;
            temp.y++;

            //incearca sa coboare piesa
            if(!coliziuneCuTabla(temp, tabla)){
                p = temp;
            } else {
                //piesa a atins fundul sau o alta piesa
                fixarePiesa(p, tabla);
                stergeLinii(tabla, &scor);

                //genereaza o noua piesa
                p.tip = rand() % 7;
                p.rotatie = 0;
                p.x = latime/2 - 2;
                p.y = 0;

                //verifica daca noua piesa se suprapune cu tabla (game over)
                if(coliziuneCuTabla(p, tabla)){
                    mvprintw(7, latime+2, "Jocul s a terminat");
                    mvprintw(8, latime/2, "Scor: %d", scor);
                    refresh();
                    napms(3000);
                    break;
                }
            }
        }
        
        //delay intre frame-uri (50ms)
        napms(50);
    }

    //curata ncurses
    endwin();
    return 0;
}
