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



void corCelula(int x, int y, float &rn,float &gn, float &bn){
    int cor = 1;
    //cout << "X " << x << "  Y " << y << "  VALOR:" << cor << endl;
    switch(cor){
        case 1: //destroyer
            rn = 1.0; gn = 0.0; bn = 0.0;
        break;
        case 2: //porta avioes
            rn = 1.0; gn = 1.0; bn = 0.0;
        break;
        case 3: //lancaataque
            rn = 1.0; gn = 0.0; bn = 1.0;
        break;
        case 4: //submarino
            rn = 0.0; gn = 1.0; bn = 1.0;
        break;
        case 5: //corveta
            rn = 0.0; gn = 1.0; bn = 0.0;
        break;
        case 6:{ //bomba
            rn = 0.0; gn = 0.0; bn = 0.0;
        }break;
        case 0: // agua
            rn = 0.0; gn = 0.0; bn = 1.0;
        break;
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

void unit(int x, int y,double r, double g, double b){
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+1,y);
        glVertex2f(x+1,y+1);
        glVertex2f(x,y+1);
    glEnd();
}

void drawGrid(){
    for(int x=0; x<10; x++){
        for(int y=0; y<COLUNAS; y++){
            double r, g, b;
            if (x == 0 || y == 0 ){
                r = 0.5 * x; g = 0.5 * y; b = 0.5;
            }else{
                r = 1/x; g = 0.5*(1/y); b = (1/x)*(1/y);
            }
            unit(x,y,r,g,b);
        }
    }
}

void displayInicial(){
    //Limpa a janela
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glFlush();
    glutMouseFunc(gerenciaMouse);
}

void inicializaMenu(){
    glutCreateMenu(menu);
    glutAddMenuEntry("Criar Poligono", 1);
    glutAddMenuEntry("Sair", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(GLint item){
    switch (item)
        {
        case 1:{ // Modo Facil - GENIUS
            cout << "Criando Poligono!" << endl;
            break;
        }
        //Escolhe modo dificil - GENIUS
        case 2:{
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
