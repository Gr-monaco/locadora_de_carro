#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

//* Informações do cliente
//@param sigla A - Alugado R - Reservado
//@param local_ret[20] Local de retirada
//@param reg_cli Número do cliente
//@param dia_ret Dia de Retirada
//@param mes_ret Dia de retirada
//@param dia_dev Dia de devolução
//@param mes_dev Mês de devolução
typedef struct info_cliente{
    char sigla;
    char local_ret[20];
    int reg_cli;
    int dia_ret;
    int mes_ret;
    int dia_dev;
    int mes_dev;
    char local_dev[20];
} info_cliente; 

//* Informações do carro
//@param sigla L - Livre A - Alugado R - Reservado
//@param local_ret[20] Local de retirada
typedef struct info_carro{
    char sigla; //Sigla lololo
    char local_ret[20];
} info_carro ;

union estado{
    struct info_carro car;
   // struct info_cliente dados[2];
};

//* Struct de carro
//@param reg_car Número de registro
//@param modelo Modelo do carro (Limite 20 char)
//@param tipo
//@param valorDiaria Preço da diaria
typedef struct carro {
    int reg_car;
    char modelo[20];
    char tipo;
    float diaria;

} carro;

//* Aloca memória para o ponteiro do carro
//@param **p Endereço do pointeiro de carro
//@param q Quantidade de carros
void aloca_carro(carro **p, int q){
    if(((*p=(carro*)realloc(*p,q*sizeof(carro)))==NULL)) exit(1);
}

int main(){
    setlocale(LC_ALL,"");
    info_carro a;
    return 1;
}