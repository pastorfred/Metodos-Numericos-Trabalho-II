//Trabalho 2 da disciplina Métodos Numéricos desenvolvido pelo aluno Leonardo Chou da Rosa
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <sstream>
#include <cfenv>
#include <fstream>

using namespace std;

void programa(string arquivo) {
    //Logica de leitura de arquivo
    int counter = 0; //counter que conta quantidade de linhas do arquivo
    stringstream ss; 
    ifstream arq;
    arq.open(arquivo); //abre o arquivo
    string header;
    string linha;
    getline(arq, header); //remove o cabecalho do arquivo
    while (!arq.eof()) { //enquanto o arquivo nao termina insere todas as linhas em um stringstream
        getline(arq, linha);
        ss << linha << " ";
        counter++;
    }
    arq.close();

    //Definindo o tamanho e zerando as matrizes
    double matrix1[((counter+1)/2)][((counter-1)/2)+1] = {0}; //cria matriz de tamanho de # de linhas-1 /2 +1 (por exemplo, se tiver 19 linhas no arquivo, cria matriz de 10 por 10)
    double matrix2[((counter+1)/2)][1] = {0}; //cria segunda matriz de mesma quantidade de rows que a primeira mas com somente uma coluna
    for(int i = 0; i < ((counter+1)/2); i++) { //zera a primeira matriz (para garantir que nao havera nenhum erro)
        for(int j = 0; j < ((counter+1)/2); j++) {
        matrix1[i][j] = 0;
        }
    }

    //Logica para receber os valores do arquivo
    for (int i = 0; i < counter-1; i++) { //enquanto o stringstream nao esta vazio
    string lixo; //remove caracteres extra
    getline(ss, lixo, '_');
    string sem1; //recebe o valor do primeiro semestre na linha
    getline(ss, sem1, ' ');
    string lixo2; //remove caracteres extra
    getline(ss, lixo2, ' ');
    string valor; //recebe a probabilidade do aluno ir para o outro semestre da linha 
    getline(ss, valor, ' ');
    string lixo3; //remove caracteres extra
    getline(ss, lixo3, ' ');
    string lixo4; //remove caracteres extra
    getline(ss, lixo4, 'e');
    string lixo5; //remove caracteres extra
    if (lixo4 != "Diploma S") { //se lixo4 nao for diploma (padrao)
        getline(ss, lixo5, '_'); //remove caracteres extra
    } 
    string sem2; //recebe o valor do segundo semestre na linha
    if (lixo4 == "Diploma S" || lixo4 == "Diploma") { //se lixo4 for "Diploma S" ou "Diploma" (nao ha numero do semestre, mas sabemos que e o ultimo)
        sem2 = to_string(((counter+1)/2)); //semestre 2 recebe o numero de semestres + 1
    } 
    else { getline(ss, sem2, ' '); } //se nao, recebe o valor do segundo semestre na linha

    //Logica para atribuir os valores recebidos do arquivo nas matrizes
    if (stoi(sem1) == stoi(sem2)) { //se os dois semestres forem iguais
        matrix1[stoi(sem2)-1][stoi(sem2)-1] = 1 - stod(valor); //insere na posicao [sem1-1][sem1-1] o resultado de 1 - o valor
        } else { //se os dois semestres forem diferentes
        matrix1[stoi(sem2)-1][stoi(sem1)-1] = stod(valor); //insere na posicao[sem2-1][sem1-1] o valor da linha
        }
    }
    matrix1[(counter-1)/2][(counter-1)/2] = 1; //apos todos os dados serem atribuidos, insere 1 na posicao final (nao pode sair apos receber diploma)

    //Logica para receber o valor de entrada
    stringstream temp;
    temp << header << endl;
    string aux1; //remove caracteres extra
    getline(temp, aux1, ':');
    string aux2; //remove caracteres extra
    getline(temp, aux2, ' ');
    string alunos; //recebe valor de entrada
    getline(temp, alunos, ' ');
    matrix2[0][0] = stod(alunos); //insere valor de entrada na primeira posicao da segunda matriz

    //Logica para fazer Eliminacao de Gauss
    int k = 0;
    for (int i = 0; i < (counter/2)+1; i++) { //para todas as fileiras
        double multiplicador = matrix1[i+1][k]/matrix1[i][k]; //multiplicador = proxima linha / pivo
        for (int j = 0; j < (counter/2)+1; j++) { //para todas as colunas
            matrix1[i+1][j] -= multiplicador*matrix1[i][j]; //proxima linha = proxima linha - multiplicador*linha atual
        }
        matrix2[i+1][0] -= multiplicador*matrix2[i][0]; //igual anterior, mas para a matriz 2
        k++;
    }

    //Logica para imprimir resultados finais
    double total = 0;
    for (int i = 0; i < (counter/2)+1; i++) { //for utilizado para imprimir valores e calcular numero total de alunos
        if (i < (counter/2)) {
        cout << "Num total de alunos no semestre " << i+1 << " = " << setprecision(0) << fixed << abs(matrix2[i][0]/matrix1[i][i]) << endl; //num no semestre = num estimado no semestre / 1-num de alunos que ficaram no semestre
        } else {cout << "Num total de alunos formados = " << abs(matrix2[i][0]/matrix1[i][i]) << endl;}
        total += abs(matrix2[i][0]/matrix1[i][i]); //total = soma de todos os alunos em cada semestre 
    } 

    double totalsemformados = 0;
    for (int i = 0; i < (counter/2); i++) { //for utilizado para imprimir valores e calcular numero total de alunos
        totalsemformados += abs(matrix2[i][0]/matrix1[i][i]); //total = soma de todos os alunos em cada semestre 
    } 

    cout << endl;
    cout << "Num total de alunos incluindo diplomados = " << setprecision(0) << fixed << total << endl; //imprime o num total de alunos
    cout << "Num total de alunos excluindo diplomados = " << setprecision(0) << fixed << totalsemformados << endl; //imprime o num total de alunos
    cout << "Num total de diplomandos = " << setprecision(0) << fixed << abs(matrix2[(counter/2)][0]/matrix1[(counter/2)][(counter/2)]) << endl; //num total de diplomados = ultimo valor da matriz 2 / ultimo valor da matriz 1
}

int main() {
    programa("caso10.txt");
}