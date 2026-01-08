#include <stdlib.h>
#include <stdio.h>  
#include <time.h>    
#include <unistd.h>
#include <ncurses.h>
#include <errno.h>

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
  int tip;       
  int rotatie;   
  int x, y;      
} Piesa;

//functie pentru alocarea tablei de joc
int** alocareTabla(int h, int l){
    int** tabla = (int**)malloc(h * sizeof(int*));
    if(tabla == NULL){
        perror("Eroare la alocarea memoriei pentru tabla");
        return NULL;
    }
    
    for(int i = 0; i < h; i++){
        tabla[i] = (int*)calloc(l, sizeof(int));
        if(tabla[i] == NULL){
            perror("Eroare la alocarea memoriei pentru rand");
            //dezaloca ce s-a alocat deja
            for(int j = 0; j < i; j++){
                free(tabla[j]);
            }
            free(tabla);
            return NULL;
        }
    }
    return tabla;
}

//functie pentru dezalocarea tablei
void dezalocareTabla(int** tabla, int h){
    if(tabla == NULL) return;
    
    for(int i = 0; i < h; i++){
        free(tabla[i]);
    }
    free(tabla);
}

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
int coliziuneCuTabla(Piesa p, int** tabla){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(piese[p.tip][p.rotatie][i][j]){
                int y = p.y + i;
                int x = p.x + j;
                if(x<0 || x>=latime || y>=inaltime) return 1;
                if(y >= 0 && tabla[y][x]) return 1;
            }
        }
    }
    return 0;
}

//fixeaza piesa pe tabla
void fixarePiesa(Piesa p, int** tabla){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(piese[p.tip][p.rotatie][i][j]){
                tabla[p.y+i][p.x+j]=1;
            }
        }
    }
}

//sterge liniile complete si actualizeaza scorul
void stergeLinii(int** tabla, int *scor){
    for(int i=inaltime-1;i>=0;i--){
        int full = 1;
        for(int j=0;j<latime;j++){
            if(tabla[i][j]==0){
                full = 0;
                break;
            }
        }
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
            i++;
        }
    }
}

//deseneaza tabla de joc
void afisare(int** tabla, Piesa p, int scor){
    clear();
    for(int i=0;i<inaltime;i++){
        for(int j=0;j<latime;j++){
            int bloc = tabla[i][j];
            for(int ii=0;ii<4;ii++){
                for(int jj=0;jj<4;jj++){
                    if(piese[p.tip][p.rotatie][ii][jj]){
                        if(i == p.y+ii && j == p.x+jj)
                            bloc = 1;
                    }
                }
            }
            mvaddch(i, j, bloc ? '#' : '.');
        }
    }

    mvprintw(0, latime+2, "Scor: %d", scor);
    mvprintw(2, latime+2, "Controale:");
    mvprintw(3, latime+2, "A/D - Stanga/Dreapta");
    mvprintw(4, latime+2, "W - Rotire");
    mvprintw(5, latime+2, "S - Jos rapid");
    mvprintw(6, latime+2, "Q - Iesire");
    
    refresh();
}

//functie pentru alocarea unei noi piese
Piesa* alocaPiesa(){
    Piesa* p = (Piesa*)malloc(sizeof(Piesa));
    if(p == NULL){
        perror("Eroare la alocarea memoriei pentru piesa");
        return NULL;
    }
    return p;
}

int main(){
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  
    curs_set(0);            

    //aloca memoria pentru tabla
    int** tabla = alocareTabla(inaltime, latime);
    if(tabla == NULL){
        endwin();
        fprintf(stderr, "Nu s-a putut aloca memoria pentru tabla de joc\n");
        return 1;
    }

    int* scor = (int*)malloc(sizeof(int));
    if(scor == NULL){
        perror("Eroare la alocarea memoriei pentru scor");
        dezalocareTabla(tabla, inaltime);
        endwin();
        return 1;
    }
    *scor = 0;

    //aloca prima piesa
    Piesa* p = alocaPiesa();
    if(p == NULL){
        free(scor);
        dezalocareTabla(tabla, inaltime);
        endwin();
        return 1;
    }
    
    p->tip = rand() % 7;
    p->rotatie = 0;
    p->x = latime/2 - 2;
    p->y = 0;

    int counter = 0;
    int viteza = 5;

    //bucla principala
    while(1){
        afisare(tabla, *p, *scor);

        int ch = getch();
        Piesa temp = *p;
        
        if(ch == 'a' || ch == 'A' || ch == KEY_LEFT){
            deplasareStanga(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                *p = temp;
            }
        }
        else if(ch == 'd' || ch == 'D' || ch == KEY_RIGHT){
            deplasareDreapta(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                *p = temp;
            }
        }
        else if(ch == 'w' || ch == 'W' || ch == KEY_UP){
            rotire(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                *p = temp;
            }
        }
        else if(ch == 's' || ch == 'S' || ch == KEY_DOWN){
            deplasareJos(&temp);
            if(!coliziuneCuTabla(temp, tabla)){
                *p = temp;
            }
        }
        else if(ch == 'q' || ch == 'Q'){
            break;
        }

        counter++;
        if(counter >= viteza){
            counter = 0;
            temp = *p;
            temp.y++;

            if(!coliziuneCuTabla(temp, tabla)){
                *p = temp;
            } else {
                fixarePiesa(*p, tabla);
                stergeLinii(tabla, scor);

                //genereaza noua piesa
                p->tip = rand() % 7;
                p->rotatie = 0;
                p->x = latime/2 - 2;
                p->y = 0;

                if(coliziuneCuTabla(*p, tabla)){
                    mvprintw(7, latime+2, "Jocul s a terminat");
                    mvprintw(8, latime+2, "Scor: %d", *scor);
                    refresh();
                    napms(3000);
                    break;
                }
            }
        }
        
        napms(50);
    }

    //curata memoria
    free(p);
    free(scor);
    dezalocareTabla(tabla, inaltime);
    endwin();
    
    return 0;
}
