/*
Alunos: Pedro Gabriel Evangelista Torres - Matricula: 14.2.4220
        Vinicius Souza de Almeida        - Matricula: 11.2.8059
*/

#include <GL/glut.h>
#include <iostream>
#include <unistd.h>

using namespace std ;

typedef struct{
    int x[50], y[50];
    int vertices;
}Poligono;


/*Variaveis Globais*/
int COLUNAS = 600.0;
int LINHAS = 600.0;
int coordx, coordy;
int displayControle[600][600];
int corPincel;
int mod = 0;
Poligono figura;
int pol = 0; // Variavel de controle de poligonos
int coordenada;
float rn, gn, bn;

/*Menu de Funcoes*/
void pegaCoordenada(int, int);
void criarPoligono();
void corCelula(int, int, float&, float&, float&);
void verificaClique(int, int);
void gerenciaMouse(int , int , int , int );
void unit(int, int, double, double, double);
void drawGrid();
void displayInicial();
void inicializaMenu();
void menu(int item);
void init();



void pegaCoordenada(int coordx, int coordy){
    float r, g, b;
    int xpos, ypos;
    xpos = coordx;
    ypos = coordy;

    corCelula(coordx, coordy, r, g, b);

    if (coordx>60){
        coordenada ++;
        figura.vertices = coordenada;
        figura.x[coordenada-1] = coordx;
        figura.y[coordenada-1] = coordy;
        cout << "Salvou coordenada " << coordenada << endl;

        //cout << "Conferindo x: Atual = " << coordx << " Primeiro = " << figura.x[0] << endl;
        //cout << "Conferindo y: Atual = " << coordy << " Primeiro = " << figura.y[0] << endl;
    }
    if ((abs(coordx - figura.x[0]) < 5 && abs(coordy - figura.y[0]) < 5 ) && coordenada > 1) {
        criarPoligono();
        cout << "Poligono fechado!" << endl;
        coordenada = 0;
        mod = 0;
    }
    else {
        unit(coordx, 600-coordy,rn,gn,bn);
    }
}

void criarPoligono(){
    pol++;
    cout << "Poligono " << pol << " esta sendo desenhado." << endl;

    corCelula(coordx, coordy, rn,gn,bn);
    glBegin(GL_POLYGON);
        for(int i=0; i<figura.vertices; i++)
            glVertex2f(figura.x[i],600-figura.y[i]);
    glEnd();
    glFlush();
}

void corCelula(int x, int y, float &r,float &g, float &b){
    //cout << "X " << x << "  Y " << y << "  VALOR:" << cor << endl;
    if(x!=0){
        if (x < 60){
            y = 600-y;
            corPincel = y;
        }
        if (x > 60){
            y = corPincel;
            //cout << "Cor Pincel: " << corPincel << endl;
        }
    }

    if(y < 60) {// preto
        r= 0.0,g= 0.0, b= 0.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada Preto" << endl;
    }else if(y < 120){ // azul
        r= 0.0,g= 0.0, b= 1.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Azul" << endl;
    }else if(y < 180){ // verde
        r= 0.0,g= 1.0, b= 0.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Verde" << endl;
    }else if(y < 240){ // vermelho
        r= 1.0,g= 0.0, b= 0.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Vermelho" << endl;
    }else if(y < 300){ // ciano
        r= 0.0,g= 1.0, b= 1.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Ciano" << endl;
    }else if(y < 360){ // amarelo
        r= 1.0,g= 1.0, b= 0.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Amarelo" << endl;
    }else if(y < 420){ // magenta
        r= 1.0,g= 0.0, b= 1.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Magenta" << endl;
    }else if(y < 480){ // verde escuro
        r= 0.2,g= 0.5, b= 0.4;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Verde Oceano" << endl;
    }else if(y < 540){ // vermelho escuro
        r= 0.4,g= 0.0, b= 0.4;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Magenta Escuro" << endl;
    }else if(y < 600){ // azul escuro
        r= 0.4,g= 0.4, b= 1.0;
        if(x<60 && x != 0)
            cout << "Cor selecionada: Roxo" << endl;
    }
}

void verificaClique(int coordx, int coordy){
    int xpos, ypos;
    xpos = coordx;
    ypos = coordy;
    //cout << "xpos: " << xpos << "ypos: " << ypos << endl;
    corCelula(xpos,ypos,rn,gn,bn);
    unit(xpos,600-ypos,rn,gn,bn); // Desenhando o pontinho na tela.
}

void gerenciaMouse(int button, int state, int x, int y){
    if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        coordx = x;
        coordy = y;
        //cout << "valores do mouse " << coordx << " e " << coordy << endl;
        if (mod == 0){
            corCelula(coordx, coordy, rn, gn, bn);
        }
        if (mod == 1){
            pegaCoordenada(coordx, coordy);
        }
  }
}

void unitCor(int x, int y,double r, double g, double b){
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+60,y);
        glVertex2f(x+60,y+60);
        glVertex2f(x,y+60);
    glEnd();
    glFlush();
}

void unit(int x, int y,double r, double g, double b){
    if (x>60){
        glColor3f(r,g,b);
        glPointSize(1);
        glBegin(GL_POINTS);
            glVertex2f(x,y);
        glEnd();
        glFlush();
    }
}

void drawGrid(){
    for(int x=0; x<1; x++){
        for(int y=0; y<LINHAS; y++){
            corCelula(x, y, rn, gn, bn);
            unitCor(x,y,rn,gn,bn);
        }
    }
}

void displayInicial(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glutMouseFunc(gerenciaMouse);
}

void inicializaMenu(){
    glutCreateMenu(menu);
    glutAddMenuEntry("Criar Poligono", 1);
    glutAddMenuEntry("Selecionar Poligono", 2);
    glutAddMenuEntry("Transladar", 3);
    glutAddMenuEntry("Rotacionar", 4);
    glutAddMenuEntry("Calcular Area", 5);
    glutAddMenuEntry("Determinar orientacao", 6);
    glutAddMenuEntry("Eliminar", 7);
    glutAddMenuEntry("Limpar tudo", 8);
    glutAddMenuEntry("Sair", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(GLint item){
    switch (item)
        {
        case 1:{
            cout << "Criando Poligono." << endl;
            mod = 1;
            coordenada = 0;
            break;
        }case 2:{
            cout << "Selecionando Poligono." << endl;
            mod = 2;
            break;
        }
        case 3:{
            cout << "Transladando Poligono." << endl;
            mod = 3;
            break;
        }
        case 4:{
            cout << "Rotacionando" << endl;
            mod = 4;
            break;
        }
        case 5:{
            cout << "Calculando Area" << endl;
            mod = 5;
            break;
        }
        case 6:{
            cout << "Determinando orientacao" << endl;
            mod = 6;
            break;
        }
        case 7:{
            cout << "Eliminar" << endl;
            mod = 7;
            break;
        }
        case 8:{
            cout << "Limpando tudo!" << endl;
            mod = 8;
            break;
        }
        case 9:{
            exit(0);
            break;
        }
        return;
    }
}

void init(){ //substitui o init dos slides
    //Define a cor de background da janela
    glClearColor (1.0, 1.0, 1.0, 0.0);
    //define o sistema de visualizacao de projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0 , COLUNAS , 0.0 , LINHAS , -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    figura.vertices = 0;
    figura.x[0] = 0;
    figura.y[0] = 0;
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(150,150);
    glutCreateWindow("TP II de CG");

    inicializaMenu();
    init();
    glutDisplayFunc(displayInicial);

    glutMainLoop();
    return 0;
}
