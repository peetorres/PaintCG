/*
Alunos: Pedro Gabriel Evangelista Torres - Matricula: 14.2.4220
*/

#include <GL/glut.h>
#include <iostream>
#include <unistd.h>

using namespace std ;

typedef struct{
    int criado = 0;
    int x[100], y[100];
    int vertices;
    float rp, gp, bp;
    bool preench;
}Poligono;

/*Variaveis Globais*/
int COLUNAS = 600.0;
int LINHAS = 600.0;
int coordx, coordy;
float MULT=1.0;
int displayControle[600][600];
int corPincel;
int cliquedireito = 0;
int tx, ty;
int rot;
int mod = 0;
Poligono figura[100];
int pol = 0; // Variavel de controle de poligonos
int coordenada;
float rn, gn, bn;
int selecaoPoligono = -1;
bool teclaPreench = false;
int selecaoVertice = 0;

/*Menu de Funcoes*/
void pegaCoordenada(int, int);
bool verificaConvexo();
void criarPoligono();
void corCelula(int, int, float&, float&, float&);
void verificaClique(int, int);
void gerenciaMouse(int , int , int , int );
void tecla(unsigned char);
void unit(int, int, double, double, double);
void selecionarVertice(int, int);
void drawGrid();
void displayInicial();
void inicializaMenu();
void menu(int item);
void init();
void limpaQuadro();
void translate();
float calculaArea();
int determinarOrientacao();
void alteraTamanhoJanela();
void drawQuadro();
void eliminar();

void eliminar(){
    Poligono aux;
    for(int i=selecaoPoligono; i<pol-1; i++){
        figura[i] = figura[i+1];
    }
    pol--;
    mod = 0;
    drawQuadro();
}

void tecla(unsigned char tecla, int x, int y){
    if (tecla == 'F'){
            if (teclaPreench == true)
                teclaPreench = false;
            else
                teclaPreench = true;
    }
    else if (tecla == 'f'){
        if (teclaPreench == true)
                teclaPreench = false;
            else
                teclaPreench = true;
    }
}

void alteraTamanhoJanela(GLsizei w, GLsizei h){
   // Evita a divisao por zero
   if(h == 0) h = 1;

   if(w == 0) w = 1;

   // Especifica as dimens›es da Viewport
   glViewport(0, 0, w, h);

   // Inicializa o sistema de coordenadas
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //cout << "w: " << w << endl;
   //cout << "h: " << h << endl;

   // Estabelece a janela de selecao (left, right, bottom, top)
   if (w <= h){
           gluOrtho2D (0.0f, 600.0f, 0.0f, 600.0f*MULT);
           MULT = h/w;
           //cout << "if 1 h/w " <<1.0*h/w << endl;
   }else if ( h <= w){
           gluOrtho2D (0.0f, 600.0f*MULT, 0.0f, 600.0f);
           MULT = w/h;
           //cout << "if 2 w/h " << 1.0*w/h << endl;
   }
    drawQuadro();
}

int determinarOrientacao(){
    float detPos = 0, detNeg = 0;
    float resultado;
    int qvertices = figura[selecaoPoligono].vertices;
    //cout << "Quantidade de vertices do poligono: " << qvertices << endl;
    for(int i=0; i<qvertices; i++){
        detPos += figura[selecaoPoligono].x[i] * figura[selecaoPoligono].y[i+1];
        detNeg += figura[selecaoPoligono].x[qvertices-i] * figura[selecaoPoligono].y[qvertices-i-1];
    }
    resultado = (detPos - detNeg)/2.0;
    //cout << "Valor do resultado: " << resultado << endl;
    if (resultado < 0)
        return 0; // horario
    else
        return 1; // anti horario
}

float calculaArea(){
    float detPos = 0, detNeg = 0;
    float resultado;
    int qvertices = figura[selecaoPoligono].vertices-1;
    for(int i=0; i<qvertices; i++){
        detPos += figura[selecaoPoligono].x[i] * figura[selecaoPoligono].y[i+1];
        detNeg += figura[selecaoPoligono].x[qvertices-i] * figura[selecaoPoligono].y[qvertices-i-1];
    }
    resultado = (detPos - detNeg)/2.0;
    if (resultado < 0)
        resultado = resultado * -1;
    return resultado;
}

void drawQuadro(){
    //cout << "Entrou no draw Quadro!" << endl;

    limpaQuadro();
    int auxmod = mod;
    int aux = pol;
    for (int i=0; i<aux; i++){
        pol = i;
        //cout << "Vai chamar a cria poligono com pol: " << pol << endl;
        criarPoligono();
    }
    pol = aux;

    //Parte para desenhar pontos clicados.
    if (figura[pol].criado == 0){
        for(int i=0; i<figura[pol].vertices; i++){
            unit(figura[pol].x[i], figura[pol].y[i], figura[pol].rp, figura[pol].gp, figura[pol].bp);
        }
    }

    drawGrid();
}

void rotacao(){
    int sen, cos, xAux = 0, yAux = 0;
    if (mod == 10){ // -90o
        sen = -1;
        cos = 0;
    }
    else if (mod == 11){ // +90
        sen = 1;
        cos = 0;
    }

    int xx = figura[selecaoPoligono].x[selecaoVertice], yy = figura[selecaoPoligono].y[selecaoVertice];

    tx = -xx;
    ty = -yy;
    translate();


    for (int i=0; i < figura[selecaoPoligono].vertices; i++){
        xAux = (figura[selecaoPoligono].x[i] * cos) - (figura[selecaoPoligono].y[i] * sen);
        yAux = (figura[selecaoPoligono].y[i] * cos) + (figura[selecaoPoligono].x[i] * sen);
        figura[selecaoPoligono].x[i] = xAux;
        figura[selecaoPoligono].y[i] = yAux;
    }

    tx = xx;
    ty = yy;
    translate();

    drawQuadro();
}

void translate(){
    //cout << "Entrou no translate!" << endl;

    if(selecaoPoligono != -1){
        for(int i =0; i<figura[selecaoPoligono].vertices; i++){
            //cout << "Figura x : " << figura[selecaoPoligono].x[i] << "  tx: " << tx << endl;
            //cout << "Figura y : " << figura[selecaoPoligono].y[i] << "  ty: " << ty << endl;
            figura[selecaoPoligono].x[i] = figura[selecaoPoligono].x[i] + tx;
            figura[selecaoPoligono].y[i] = figura[selecaoPoligono].y[i] + ty;

            //cout << "Figura x FINAL: " << figura[selecaoPoligono].x[i] << endl;
            //cout << "Figura y FINAL: " << figura[selecaoPoligono].y[i] << endl;
        }
        drawQuadro();
    }
    //mod = 0;
}

void limparTudo(){
    for(int i=0; i<pol; i++){
        figura[i].vertices = 0;
        figura[i].x[i] = 0;
        figura[i].y[i] = 0;
    }
    limpaQuadro();
    mod = 0;
    pol = 0;
}

bool pontonoPoligono(int x, int y, int poli){
    int i, j = figura[poli].vertices - 1 ;
    bool oddNodes = false;
    //cout << "Vertices do Poligono: " << figura[poli].vertices -1<< endl;
    for (i=0; i<figura[poli].vertices; i++) {
        if ((figura[poli].y[i]<y && figura[poli].y[j]>=y ||
             figura[poli].y[j]<y && figura[poli].y[i]>=y)){
            if (figura[poli].x[i]+(y-figura[poli].y[i])/(figura[poli].y[j]-figura[poli].y[i])*(figura[poli].x[j]-figura[poli].x[i])<x) {
                oddNodes=!oddNodes;
            }
        }
        j=i;
    }
    return oddNodes;
}

bool verificaConvexo(){
    //cout << "Entrou no verifica convexo!" << endl;
    bool sinal, sinalAux;
    int produtoVetorial = 0;
    int pvertices = figura[pol].vertices;
    //cout << "For vai de 0 a " << pvertices << endl;
    for(int i=0; i<pvertices-1;i++){
        //calculando produto vetorial
        int a = i-1, b = i, c = i+1;
        if (a == -1)
            a = pvertices-1;
        if (c == pvertices)
            i = 0;

        //cout << "Verificando produto vetorial: (xb e xa): "<< b << "  " << a << endl;
        //cout << "Verificando produto vetorial: (yc e yb): "<< c << "  " << b << endl;
        //cout << "Verificando produto vetorial: (yb e ya): "<< b << "  " << a << endl;
        //cout << "Verificando produto vetorial: (xc e xb): "<< c << "  " << b << endl;

        produtoVetorial = ((figura[pol].x[b]-figura[pol].x[a])*(figura[pol].y[c]-figura[pol].y[b]))
        - ((figura[pol].y[b]-figura[pol].y[a]) * (figura[pol].x[c]-figura[pol].x[b]));


        if (produtoVetorial >= 0){
            if (sinal == false && i>1){
                return false;
            }
            sinal = true; // positivo
        }
        else{
            if (sinal == true && i>1){
                return false;
            }
            sinal = false; // negativo
        }
    }
    return true;
}

void pegaCoordenada(int coordx, int coordy){ // Coord dos pontos do poligono
    float r, g, b;

    corCelula(coordx, coordy, r, g, b);
    //cout << "Vertices: " << figura[pol].vertices << endl;
    if(teclaPreench == false)
        figura[pol].preench = false;
    else
        figura[pol].preench = true;

    if (coordx>60*MULT){
        coordenada ++;
        figura[pol].vertices = coordenada;
        figura[pol].x[coordenada-1] = coordx;
        figura[pol].y[coordenada-1] = coordy;
        unit(coordx, coordy, r, g, b);
        //cout << "Salvou coordenada " << coordenada << endl;
        //cout << "Conferindo x: Atual = " << coordx << " Primeiro = " << figura[pol].x[coordenada-1] << endl;
        //cout << "Conferindo y: Atual = " << coordy << " Primeiro = " << figura[pol].y[coordenada-1] << endl;
    }
    //if ((abs(coordx - figura[pol].x[0]) < 5 && abs(coordy - figura[pol].y[0]) < 5 ) && coordenada > 1) {
    if(cliquedireito == 1 && coordenada > 1){
        figura[pol].vertices --;
        figura[pol].rp = r;
        figura[pol].gp = g;
        figura[pol].bp = b;

        if(verificaConvexo() == true){
            //figura[pol].vertices --;
            cout << "Poligono Convexo, desenhando!" << endl;
            //criarPoligono();
            pol ++;
            drawQuadro();
        }
        else{
            cout << "Poligono Nao convexo, apagando." << endl;
            figura[pol].vertices = 0;
            drawQuadro();
        }
        coordenada = 0;
        mod = 0;
        cliquedireito = 0;
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
    else {
        unit(coordx, coordy,r,g,b);
    }
}

void criarPoligono(){ // Cria poligono
    glColor3f(figura[pol].rp,figura[pol].gp,figura[pol].bp);
    if(figura[pol].preench == true){
        glBegin(GL_POLYGON);
            for(int i=0; i<figura[pol].vertices; i++){
                glVertex2f(figura[pol].x[i]*MULT,figura[pol].y[i]*MULT);
                //cout << "Desenhando vertice ponto x: " << figura[pol].x[i] << " ponto y: " << 600-figura[pol].y[i] << endl;
            }
    }
    if(figura[pol].preench == false){
        glPointSize(2);
        glBegin(GL_LINE_LOOP);
            for(int i=0; i<figura[pol].vertices; i++){
                glVertex2f(figura[pol].x[i]*MULT,figura[pol].y[i]*MULT);
                //cout << "Desenhando vertice ponto x: " << figura[pol].x[i] << " ponto y: " << 600-figura[pol].y[i] << endl;
            }
    }
    glEnd();
    glFlush();

    cout << "Poligono " << pol+1 << " desenhado." << endl;
}

void corCelula(int x, int y, float &r,float &g, float &b){ // Pega cor da Palheta (Cor atual)
    if(x!=0){
        if (x < 60*MULT){
            corPincel = y;
        }
        if (x > 60){
            y = corPincel;
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
    //corCelula(xpos,ypos,rn,gn,bn);
    //unit(xpos,600-ypos,rn,gn,bn); // Desenhando o pontinho na tela.
    if (mod == 2){
        for(int i = pol - 1; i>=0; i--){
            if(pontonoPoligono(xpos, ypos, i)){
                cout << "Poligono Selecionado: " << i + 1 << endl;
                selecaoPoligono = i;
                break;
            }
        }
    }
    if (mod == 3){
        //cout << "valores do mouse elseif no motionfunc" << coordx << " e " << 600-coordy << endl;
        tx = coordx - figura[selecaoPoligono].x[0];
        //cout << "TX: " << tx << " coordx: " << coordx << " x0: " << figura[selecaoPoligono].x[0] <<endl;
        ty = LINHAS - coordy - figura[selecaoPoligono].y[0];
        //ty = figura[selecaoPoligono].y[0] - coordy;
        //cout << "TY: " << tx << " coordy: " << coordy << " y0: " << figura[selecaoPoligono].y[0] <<endl;
        //cout << "valor do eixo 0 : x = " << figura[selecaoPoligono].x[0] << " y = " << figura[selecaoPoligono].x[0] << endl;
        translate();
    }
}

void selecionarVertice (int coordx, int coordy){
    //cout << "Entrou no selecionar vertices" << endl;
    for(int i=0; i<figura[selecaoPoligono].vertices; i++){
            if (abs(coordx - figura[selecaoPoligono].x[i]) < 10 && abs(coordy - figura[selecaoPoligono].y[i]) < 10 ){
                selecaoVertice = i;
                cout << "Rotacao em torno do vertice : " << selecaoVertice << endl;
                break;
            }
    }
    rotacao();
}

void gerenciaMouse(int button, int state, int x, int y){
    if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN && mod != 3){
        coordx = (x);
        if(mod != 3){
            coordy = (LINHAS-y);
        }
        //cout << "valores do mouse " << coordx << " e " << coordy << endl;
        if (mod == 0 || mod == 2){
            corCelula(coordx, coordy, rn, gn, bn);
        }
        if (mod == 1){
            glutDetachMenu(GLUT_RIGHT_BUTTON);
            pegaCoordenada(coordx, coordy);
        }
        if (mod == 2){
            verificaClique(coordx, coordy);
        }
        if (mod == 10 || mod == 11){
                selecionarVertice(coordx, coordy);
        }
  }
  if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && mod==1){
        //cout << "Entrou no clique Direito!! " << endl;
        cliquedireito = 1;
        pegaCoordenada(coordx,coordy);
        cliquedireito = 0;
  }
  else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    if(mod == 3){
            glutMotionFunc(verificaClique);
            //mod = 0;
        }
  }
}

void unitCor(float x, float y,double r, double g, double b){
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+60,y);
        glVertex2f(x+60,y+60);
        glVertex2f(x,y+60);
    glEnd();
}

void limpaQuadro(){
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glVertex2f(60,0);
        glVertex2f(600,0);
        glVertex2f(600,600);
        glVertex2f(60,600);
    glEnd();
    glFlush();
}

void unit(int x, int y,double r, double g, double b){
    if (x>60){
        glColor3f(r,g,b);
        glPointSize(2);
        glBegin(GL_POINTS);
            glVertex2f(x,y);
        glEnd();
    }
    glFlush();
}

void drawGrid(){
    //cout << "Entrou no drawGrid" << endl;
    for(int x=0; x<1; x++){
        for(int y=0; y<(LINHAS-59)*MULT; y++){
            corCelula(x, y, rn, gn, bn);
            unitCor(x*MULT,y*MULT,rn,gn,bn);
        }
    }
}

void displayInicial(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawQuadro();
    glutMouseFunc(gerenciaMouse);
    glutKeyboardFunc(tecla);
}

void inicializaMenu(){
    GLint subMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Rotacao de +90o", 10);
    glutAddMenuEntry("Rotacao de -90o", 11);

    glutCreateMenu(menu);
    glutAddMenuEntry("Criar Poligono", 1);
    glutAddMenuEntry("Selecionar Poligono", 2);
    glutAddMenuEntry("Transladar", 3);
    glutAddSubMenu("Rotacionar", subMenu);
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
            tx = 0;
            ty = 0;
            break;
        }
        case 10:{
            cout << "Rotacionando +90º" << endl;
            mod = 10;
            rot = 1;
            break;
        }
        case 11:{
            cout << "Rotacionando -90º" << endl;
            mod = 11;
            rot = 0;
            break;
        }
        case 5:{
            cout << "Calculando Area" << endl;
            mod = 5;
            cout << "Area do poligono " << selecaoPoligono+1 << " = " << calculaArea() << endl;
            break;
        }
        case 6:{
            cout << "Determinando orientacao" << endl;
            mod = 6;
            if (determinarOrientacao() == 0){
                cout << "Sentido do poligono: Horario" << endl;
            }
            else if (determinarOrientacao() == 1){
                cout << "Sentido do poligono: Anti-Horario" << endl;
            }
            break;
        }
        case 7:{
            cout << "Eliminar" << endl;
            mod = 7;
            eliminar();
            break;
        }
        case 8:{
            cout << "Limpando tudo!" << endl;
            mod = 8;
            limparTudo();
            break;
        }
        case 9:{
            exit(0);
            break;
        }
        return;
    }
}

void init(){
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0 , COLUNAS*MULT , 0.0 , LINHAS*MULT , -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    for(int i=0; i<100; i++){
        figura[i].vertices = 0;
        figura[i].x[i] = 0;
        figura[i].y[i] = 0;
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(0,0);
    glutCreateWindow("TP II de CG");

    inicializaMenu();
    glutDisplayFunc(displayInicial);
    glutReshapeFunc(alteraTamanhoJanela);

    init();
    glutMainLoop();
    return 0;
}
