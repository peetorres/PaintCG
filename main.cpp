/*
Alunos: Pedro Gabriel Evangelista Torres - Matricula: 14.2.4220
        Vinicius Souza de Almeida        - Matricula: 11.2.8059
*/

#include <GL/glut.h>
#include <iostream>
#include <unistd.h>

using namespace std ;

/*Variaveis Globais*/
int COLUNAS = 600.0;
int LINHAS = 600.0;
GLint coordx, coordy;
int displayControle[600][600];
int corPincel;

/*Menu de Funcoes*/
void corCelula(int, int, float&, float&, float&);
void verificaClique(int, int);
void gerenciaMouse(int , int , int , int );
void unit(int, int, double, double, double);
void drawGrid();
void displayInicial();
void inicializaMenu();
void menu(int item);
void init();

void corCelula(int x, int y, float &r,float &g, float &b){
    //cout << "X " << x << "  Y " << y << "  VALOR:" << cor << endl;
    if (x < 60){
        if(y < 60) // preto
            r= 0.0,g= 0.0, b= 0.0;
        else if(y < 120) // azul
            r= 0.0,g= 0.0, b= 1.0;
        else if(y < 180) // verde
            r= 0.0,g= 1.0, b= 0.0;
        else if(y < 240) // vermelho
            r= 1.0,g= 0.0, b= 0.0;
        else if(y < 300) // ciano
            r= 0.0,g= 1.0, b= 1.0;
        else if(y < 360) // amarelo
            r= 1.0,g= 1.0, b= 0.0;
        else if(y < 420) // magenta
            r= 1.0,g= 0.0, b= 1.0;
        else if(y < 480) // verde escuro
            r= 0.2,g= 0.5, b= 0.4;
        else if(y < 540) // vermelho escuro
            r= 0.4,g= 0.0, b= 0.4;
        else if(y < 600) // azul escuro
            r= 0.4,g= 0.4, b= 1.0;

        corPincel = y;
    }
}

void verificaClique(int coordx, int coordy){
    float r, g, b;
    int xpos, ypos;
    xpos = coordx;
    ypos = coordy;
    cout << "xpos: " << xpos << "ypos: " << ypos << endl;
    corCelula(xpos,ypos,r,g,b);
    unit(xpos,600-ypos,r,g,b);
    glFlush();
}

void gerenciaMouse(int button, int state, int x, int y){
    if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        coordx = x;
        coordy = y;
        //cout << "valores do mouse " << coordx << " e " << coordy << endl;
        verificaClique(coordx, coordy);
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
        glBegin(GL_POLYGON);
            glVertex2f(x,y);
            glVertex2f(x+1,y);
            glVertex2f(x+1,y+1);
            glVertex2f(x,y+1);
        glEnd();
        glFlush();
    }
}

void drawGrid(){
    for(int x=0; x<1; x++){
        for(int y=0; y<LINHAS; y++){
            float r, g, b;
            corCelula(x, y, r, g, b);
            unitCor(x,y,r,g,b);
        }
    }
}

void displayInicial(){
    //Limpa a janela
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
            cout << "Criando Poligono!" << endl;
            break;
        }case 2:{
            cout << "Selecionando" << endl;
            break;
        }
        case 3:{
            cout << "Transladando" << endl;
            break;
        }
        case 4:{
            cout << "Rotacionando" << endl;
            break;
        }
        case 5:{
            cout << "Calculando Area" << endl;
            break;
        }
        case 6:{
            cout << "Determinando orientacao" << endl;
            break;
        }
        case 7:{
            cout << "Eliminar" << endl;
            break;
        }
        case 8:{
            cout << "Limpando tudo!" << endl;
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
