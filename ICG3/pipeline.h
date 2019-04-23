#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "definitions.h"
#include "objLoader.h"
#include "mygl.h"

using namespace glm;

objLoader *obj;

mat4 matrizModelagem;
mat4 matrizVisualizacao;
mat4 matrizProjecao;
mat4 matrizViewPort;
mat4 matrizTransformacaoFinal;
mat4 allMatrix;

mat4 gerarModelMatrix(float x, float y, float z){

    matrizModelagem = mat4(1.0f);
    mat4 translacao = translate(matrizModelagem, vec3(x,y,z));

    matrizModelagem = scale(translacao, vec3( 1.5f, 1.5f, 1.0f ));

    matrizTransformacaoFinal = matrizModelagem;
}

mat4 getViewMatrix(){

    vec3 posicao = vec3(2.0f, 3.0f, 5.0f);
    vec3 direcao = vec3(-2.0f, -3.0f, -5.0f);
    vec3 up = vec3(0.0f, 1.f, 0.0f);

    vec3 zCamera = -normalize(direcao);
    vec3 xCamera = normalize(cross(up, zCamera));
    vec3 yCamera = normalize(cross(xCamera, yCamera));

    mat4 BT = mat4(vec4(xCamera.x, yCamera.x, zCamera.x, 0.0f),
                         vec4(xCamera.y, yCamera.y, zCamera.y, 0.0f),
                         vec4(xCamera.z, yCamera.z, zCamera.z, 0.0f),
                         vec4(  0.0f,      0.0f,     0.0f,     1.0f));

    mat4 T = mat4(1.0f);
    T[3] = vec4(-posicao, 1.0f);

    matrizVisualizacao = BT * T;

    matrizTransformacaoFinal = matrizVisualizacao * matrizTransformacaoFinal;
}

mat4 gerarMatrizProjecao(float d){
    matrizProjecao = mat4(vec4(1.0f, 0.0f, 0.0f,     0.f),
                            vec4(0.0f, 1.0f, 0.0f,     0.f),
                            vec4(0.0f, 0.0f, 1.0f, -1.0f/d),
                            vec4(0.0f, 0.0f,  d,       1.f));

    matrizTransformacaoFinal = matrizProjecao * matrizTransformacaoFinal;
}

mat4 gerarMatrizViewPort(float x, float y, float z, float d){

    gerarModelMatrix(x, y, z);
    getViewMatrix();
    gerarMatrizProjecao(d);

    mat4 S1 = mat4(1.0f);
    S1[1].y = 1;

    mat4 S2 = mat4(vec4((IMAGE_WIDTH)/2,     0.0f,       0.0f, 0.0f),
                   vec4(    0.0f,      (IMAGE_HEIGHT)/2, 0.0f, 0.0f),
                   vec4(    0.0f,          0.0f,       1.0f, 0.0f),
                   vec4(    0.0f,          0.0f,       0.0f, 1.0f));

    mat4 T = mat4(1.0f);
    T[3] = vec4((IMAGE_WIDTH-1)/2, (IMAGE_HEIGHT-1)/2, 0.0f, 1.0f);



    matrizViewPort = T * S2 * S1;
    matrizTransformacaoFinal = matrizViewPort * matrizTransformacaoFinal;
}

void ObjData(){

    obj = new objLoader();
    obj->load((char *) "Objects/cube.obj");
}

//Inicia o objeto
void InitObj(){

    gerarMatrizViewPort(0.0f, 0.0f, 0.0f, 0.8f);

    for(int i = 0; i < obj->faceCount; i++){
        obj_face *face = obj->faceList[i];

        vec4 primeiroVertice = vec4(obj->vertexList[face->vertex_index[0]]->e[0],
                              obj->vertexList[face->vertex_index[0]]->e[1],
                              obj->vertexList[face->vertex_index[0]]->e[2],
                                                   1.0f);

        vec4 segundoVertice = vec4(obj->vertexList[face->vertex_index[1]]->e[0],
                              obj->vertexList[face->vertex_index[1]]->e[1],
                              obj->vertexList[face->vertex_index[1]]->e[2],
                                                   1.0f);

        vec4 terceiroVertice = vec4(obj->vertexList[face->vertex_index[2]]->e[0],
                                obj->vertexList[face->vertex_index[2]]->e[1],
                                obj->vertexList[face->vertex_index[2]]->e[2],
                                                   1.0f);


        primeiroVertice = matrizTransformacaoFinal * primeiroVertice;
        segundoVertice = matrizTransformacaoFinal * segundoVertice;
        terceiroVertice = matrizTransformacaoFinal * terceiroVertice;

        primeiroVertice = primeiroVertice / primeiroVertice.w;
        segundoVertice = segundoVertice / segundoVertice.w;
        terceiroVertice = terceiroVertice / terceiroVertice.w;

        int cor[4] = {255, 255, 255, 1};
		Posicao primeiraPosicao, segundaPosicao, terceiraPosicao;

		primeiraPosicao.x = (int) primeiroVertice.x;
		primeiraPosicao.y = (int) primeiroVertice.y;

		segundaPosicao.x = (int) segundoVertice.x;
		segundaPosicao.y = (int) segundoVertice.y;

		terceiraPosicao.x = (int) terceiroVertice.x;
		terceiraPosicao.y = (int) terceiroVertice.y;

        drawTriangles(primeiraPosicao, segundaPosicao, terceiraPosicao, cor);

     }
}

void limparFB(){
    memset(FBptr, 0, IMAGE_WIDTH * IMAGE_HEIGHT * 4);
}

