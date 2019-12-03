#include <gl/gl.h>
#include <stdio.h>
#define MAT2X(j) ((j)*0.1f-0.3)
#define MAT2Y(i) (0.9-(i)*0.1f)
#include "SOIL.h"
#define TAM 0.1f
#include "func.h"
#define maplimit 10
#include <time.h>


struct posicao_grade
{
    int x;
    int y;
};
int placar[5];
GLuint cubos[6];
GLuint pontos[10];
GLuint grade, start, defeat;
int matriz[][20];
struct posicao_grade g;


static GLuint carregaArqTextura(char *str);
void carregaTexturas() {
    int i;
    char str[50];
    g.x = 2;
    g.y = 2;
    grade = carregaArqTextura(".//sprites//grade.png");
    for ( i = 0; i < 6; i++) {
        sprintf(str,".//sprites//cubo%d.png", i);
        cubos[i] = carregaArqTextura(str);
    }
    for(i=0;i<10;i++){
        sprintf(str,".//sprites//sprite%d.png", i);
        pontos[i] = carregaArqTextura(str);

    }
    defeat = carregaArqTextura(".//sprites//game_over.png");
    start = carregaArqTextura(".//sprites//game_start.png");
}

static GLuint carregaArqTextura(char *str){
    // http://www.lonesock.net/soil.html
    GLuint tex = SOIL_load_OGL_texture
        (
            str,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );

    /* check for an error during the load process */
    if(0 == tex){
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    return tex;
}

void desenhaSprite(float coluna,float linha, GLuint tex){
    coluna = MAT2X(coluna); // fazem as conversões de coluna e linha para as coordenadas do plano
    linha = MAT2Y(linha);   //cartesiano do opengl
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex2f(coluna, linha);
        glTexCoord2f(1.0f,0.0f); glVertex2f(coluna+TAM, linha);
        glTexCoord2f(1.0f,1.0f); glVertex2f(coluna+TAM, linha+TAM);
        glTexCoord2f(0.0f,1.0f); glVertex2f(coluna, linha+TAM);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}

void desenhaDerrota() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, defeat);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f,1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void desenhaInicio() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, start);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f,1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void pontosd();
void desenhaJogo(int matriz[10][20]) {
    int i,j;
    pontosd();
    printf("%d  %d  %d  %d\n\n", placar[0], placar[1], placar[2], placar[3]);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 20; j++) {
            desenhaSprite(i, j, cubos[matriz[i][j]]);
        }
    }

    desenhaSprite(g.x,g.y, grade);
    desenhaSprite(g.x+1,g.y,grade);
}

void pontosd() {
    int i, algs[5];
    int ptsLocal = pont;
    for (i = 0; i < 5; i++) {
        algs[i] = ptsLocal % 10;
        ptsLocal = ptsLocal / 10;
    }
    for (i = 0; i < 5; i++)
        desenhaSprite(0 - i-1, 0, pontos[algs[i]]);
}

//movimentos

void movimenta_esquerda()
{
    if(g.x >= 1)
    {
        g.x--;
    }
}

void movimenta_direita()
{
    if(g.x <= 7)
    {
        g.x++;
    }
}

void movimenta_cima()
{
    if(g.y >= 1)
    {
        g.y--;
    }
}

void movimenta_baixo()
{
    if(g.y <= 18)
    {
        g.y++;
    }
}

void troca()
{
    int q;
    q = matriz [g.x][g.y];
    matriz [g.x][g.y] = matriz[g.x+1][g.y];
    matriz[g.x+1][g.y] = q;
}

//apaga as cominacaoes
void apaga()
{
    int i,j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 20; j++) {
            if (matriz[i][j] != 5) {
                if(i<=4 &&  matriz[i][j] == matriz[i+1][j] && matriz[i][j] == matriz[i+2][j]
                   && matriz[i][j] == matriz[i+3][j]&& matriz[i][j] == matriz[i+4][j]
                   && matriz[i][j] == matriz[i+5][j] )
                   {
                        matriz[i][j]= 5;
                        matriz[i+1][j]=5;
                        matriz[i+2][j]=5;
                        matriz[i+3][j]=5;
                        matriz[i+4][j]=5;
                        matriz[i+5][j]=5;
                        pont+=200;
                   }
                if(i<=5 &&  matriz[i][j] == matriz[i+1][j] && matriz[i][j] == matriz[i+2][j]
                   && matriz[i][j] == matriz[i+3][j]&& matriz[i][j] == matriz[i+4][j])
                    {
                        matriz[i][j]= 5;
                        matriz[i+1][j]=5;
                        matriz[i+2][j]=5;
                        matriz[i+3][j]=5;
                        matriz[i+4][j]=5;
                        pont+=150;
                    }

                if(i<=6 &&  matriz[i][j] == matriz[i+1][j] && matriz[i][j] == matriz[i+2][j]
                   && matriz[i][j] == matriz[i+3][j])
                    {
                        matriz[i][j]= 5;
                        matriz[i+1][j]=5;
                        matriz[i+2][j]=5;
                        matriz[i+3][j]=5;
                        pont+=100;
                    }

                if(i<=7 &&  matriz[i][j] == matriz[i+1][j] && matriz[i][j] == matriz[i+2][j] )
                    {
                        matriz[i][j]= 5;
                        matriz[i+1][j]=5;
                        matriz[i+2][j]=5;
                        pont+=50;
                    }

                if(j <= 17 && matriz[i][j] == matriz[i][j+1] && matriz[i][j] == matriz[i][j+2]
                   && matriz[i][j] == matriz[i][j+3] && matriz[i][j] == matriz[i][j+4]
                   && matriz[i][j] == matriz[i][j+5] )
                    {
                        matriz[i][j]= 5;
                        matriz[i][j+1]=5;
                        matriz[i][j+2]=5;
                        matriz[i][j+3]=5;
                        matriz[i][j+4]=5;
                        matriz[i][j+5]=5;
                        pont+=200;
                    }

                if(j <= 17 && matriz[i][j] == matriz[i][j+1] && matriz[i][j] == matriz[i][j+2]
                   && matriz[i][j] == matriz[i][j+3] && matriz[i][j] == matriz[i][j+4])
                    {
                        matriz[i][j]= 5;
                        matriz[i][j+1]=5;
                        matriz[i][j+2]=5;
                        matriz[i][j+3]=5;
                        matriz[i][j+4]=5;
                        pont+=150;
                    }

                if(j <= 17 && matriz[i][j] == matriz[i][j+1] && matriz[i][j] == matriz[i][j+2]
                   && matriz[i][j] == matriz[i][j+3])
                   {
                        matriz[i][j]= 5;
                        matriz[i][j+1]=5;
                        matriz[i][j+2]=5;
                        matriz[i][j+3]=5;
                        pont+=100;
                   }

                if(j <= 17 && matriz[i][j] == matriz[i][j+1] && matriz[i][j] == matriz[i][j+2] )
                   {
                        matriz[i][j]= 5;
                        matriz[i][j+1]=5;
                        matriz[i][j+2]=5;
                        pont+=50;
                   }
            }
        }

  }
}

// muda os blocos que combinam ao iniciar
void muda (){
    int i ,j;
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 20; j++) {
                if(matriz[i][j] != 5 )
                {


                    while((j > 1 && matriz[i][j] == matriz[i][j-1] && matriz[i][j] == matriz[i][j-2])
                        ||(i > 1 && matriz[i][j] == matriz[i-1][j] && matriz[i][j] == matriz[i-2][j]))
                    {

                        matriz[i][j] = rand()% 5;
                    }
                }
            }
        }
}


//faz os blocos cairem
void gravidade()
{
 int i,j;
    for (i = 0; i < 10; i++)
    {
        for (j = 1; j <20 ; j++)
            {
            if(matriz[i][j] == 5 && matriz[i][j-1] != 5)
            {
                printf("%d %d\n",i,j);
                matriz[i][j]=matriz[i][j-1];
                matriz[i][j-1]=5;
            }
        }
    }
}

//faz blocos subirem
void subir ()
{
 int i,j;
 for (i = 0; i < 10; i++) {
    if (matriz[i][0] != 5)
        statusJogo = 3;
 }
    for(i = 0; i < 10 ;i++)
    {
        for(j = 0; j < 20 ;j++)
        {
            matriz[i][j] = matriz[i][j+1];
        }
    }
    for(i = 0; i < 10 ;i++)
    {
        matriz[i][19]= rand() %5;
        muda();
    }
}
