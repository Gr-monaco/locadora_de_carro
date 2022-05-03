//Gabriel Rocha Monaco RA 171092
//Igor Dantas RA 190721
//João Pedro Abrantes RA 190846


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//* Informações do carro alugado e seu cliente
//@param sigla A - Alugado R - Reservado
//@param local_ret[20] Local de retirada
//@param reg_cli Número do cliente
//@param dia_ret Dia de Retirada
//@param mes_ret Dia de retirada
//@param dia_dev Dia de devolução
//@param mes_dev Mês de devolução
//@param local_dev[20] Local de devolução
typedef struct info_cliente
{
    char sigla;
    char local_ret[20];
    int reg_cli;
    int dia_ret;
    int mes_ret;
    int dia_dev;
    int mes_dev;
    char local_dev[20];
} info_cliente;

//Informações do cliente
//@param reg_cli Numero do cliente
//@param nome Nome do cliente
//@param CPF CPF do cliente
//@param sigla Sigla do cliente
//@param reg_car Numero do carro alugado
typedef struct cliente{
    int reg_cli;
    char nome[80];
    char CPF[15];
    char sigla;
    int reg_car;
}cliente;

//* Informações do carro
//@param sigla L - Livre A - Alugado R - Reservado
//@param local_ret[20] Local de retirada
typedef struct info_carro
{
    char sigla;
    char local_ret[20];
} info_carro;

//* Estado do carro
//@param car Informações do carro
//@param dados[2] Informações do cliente. Primeiro é do array é
// o cliente alugando, o segundo é o cliente reservado.
union estado
{
    struct info_carro car;
    struct info_cliente dados[2];
};

//* Struct de carro
//@param reg_car Número de registro
//@param modelo[20] Modelo do carro (Limite 20 char)
//@param tipo P - Pequeno M - Médio G - Grande
//@param valorDiaria Preço da diaria
//@param status Estado do carro
typedef struct carro
{
    int reg_car;
    char modelo[20];
    char tipo;
    float diaria;
    union estado status;
} carro;

// Verifica o numero de carros cadastrados.
//@return número da numeração do ultimo carro,
// caso não existe arquivo.bin, o retorno é 0.
int verifica_arquivo()
{
    long int cont = 0;
    FILE *fptr = NULL;
    if ((fptr = fopen("carro.bin", "ab")) == NULL)
        return cont;
    else
    {
        fseek(fptr, 0, 2);                  // posiciona no fim do arquivo
        cont = ftell(fptr) / sizeof(carro); // qtde de elementos
        fclose(fptr);                       // dentro do ELSE por conta do rb
        return cont;
    }
}

// Grava os carros do ponteiro de carros em um arquivo .bin
//@param *p Ponteiro de carros a ser salvado
void grava_carro(carro *p)
{
    FILE *fptr = NULL;
    if ((fptr = fopen("carro.bin", "ab")) == NULL)
        printf("\nErro ao abrir o arquivo");
    else
        fwrite(p, sizeof(carro), 1, fptr);
    fclose(fptr); // fora do ELSE por conta do ab
} // grava_hospede

//* Aloca memória para o ponteiro do carro
//@param **p Endereço do pointeiro de carro
//@param q Quantidade de carros
void aloca_carro(carro **p, int q)
{
    if (((*p = (carro *)realloc(*p, q * sizeof(carro))) == NULL))
        exit(1);
}

// Cadastra carros no sistema
//@param *p_carro Ponteiros de carro aonde se deseja cadastrar
//@param q Quantidade de carros a serem cadastrados. Atualmente sem uso.
void cadastra_carro(carro *p_carro, int q)
{
    // Número de carros cadastrados
    int n;

    n = verifica_arquivo();
    p_carro->reg_car = ++n;
    printf("\nRegistro: %i\n", p_carro->reg_car);
    printf("\nModelo: ");
    gets(p_carro->modelo);
    fflush(stdin);
    printf("\nTipo: ");
    scanf("%c", &(p_carro->tipo));
    fflush(stdin);
    printf("\nDiaria: ");
    scanf("%f", &(p_carro->diaria));
    fflush(stdin);

    p_carro->status.car.sigla = 'L';
    strcpy(p_carro->status.car.local_ret, "Sorocaba");

    grava_carro(p_carro);
}

//Verifica todos os carros cadastrados.
//Faz um print de todos os carros cadastrados no sistema
//@param p_carro Ponteiro de carros utilizado
void consulta_total(carro *p_carro)
{
	int i;
    FILE *ar = NULL;
    int numero_de_carro = verifica_arquivo();
    system("cls");
    if ((ar = fopen("carro.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < numero_de_carro; i++)
        {
            fseek(ar,i*sizeof(carro),0);
            fread(p_carro,sizeof(carro),1,ar);
            printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %f", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria);

        }
    }
}

//Verifica se existe carro livre pelo tipo dele.
//Faz print de todos os carros livres.
//@param p_carro Ponteiro de carros utilizado
void consulta_se_tem_livre_por_tipo(carro *p_carro){
    system("cls");
    char op;
    int i;
    printf("Selecione o tamanho: ");
    scanf("%c", &op);
    fflush(stdin);
    FILE *ar = NULL;
    int numero_de_carro = verifica_arquivo();
    if ((ar = fopen("carro.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < numero_de_carro; i++)
        {
            fseek(ar,i*sizeof(carro),0);
            fread(p_carro,sizeof(carro),1,ar);
            if(p_carro->tipo==op){
                printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %f\n", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria);
            }
        }
    }
}

//Salva o cliente em um arquivo .bin
//@param p Ponteiros de clientes utilizado
void grava_cliente(cliente *p)
{
    FILE *fptr = NULL;
    if ((fptr = fopen("cliente.bin", "ab")) == NULL)
        printf("\nErro ao abrir o arquivo");
    else
        fwrite(p, sizeof(cliente), 1, fptr);
    fclose(fptr); // fora do ELSE por conta do ab
}

//Aloca memória para o ponteiro de cliente.
//@param p Endereço do ponteiro de Cliente utilizado
//@param q Tamanho desejado do ponteiro
void aloca_cliente(cliente **p, int q)
{
    if (((*p = (cliente *)realloc(*p, q * sizeof(cliente))) == NULL))
        exit(1);
}


//Cadastra o cliente com o carro desejado.
//@param op_carro Numero de registro do carro
//@param p_cli Ponteiro de Cliente utilizado
void cadastro_cliente(int op_carro, cliente *p_cli) { 
	int n; 
	p_cli->reg_cli = ++n;
    printf("\nRegistro: %i\n", p_cli->reg_car);
    printf("\nNome: ");
    gets(p_cli->nome);
    printf("\nCPF: ");
  	scanf("%s", p_cli->CPF);
    p_cli->reg_car = op_carro;

    /* 
       Acho que precisa setar os valores do carro.
       Não sei como mas precisamos pegar o carro com o numero de registro,
       alterar o union para info_cliente, colocar os valores certos e
       finalmente salvar e sobrescrever os valores antigos.
     */

    grava_cliente(p_cli);
}

int main()
{
    setlocale(LC_ALL, "");
    carro *p_carro = NULL;
    cliente *p_cli = NULL;

    int op, op_carro;
    info_carro a;
    int numero;
    do
    {
        printf("\n[1]Cadastrar carro\n[2]Consulta Total/Aluguel de Carro\n[3]Consulta Parcial\n[4]Fim\nOpcao:");
        scanf("%i", &op);
        fflush(stdin);
        switch (op)
        {
        case 1:
            strcpy(a.local_ret, "\nCadastro");
            printf("%s\n", a.local_ret);
            aloca_carro(&p_carro, 1);
            cadastra_carro(p_carro, 1);
            break;
        case 2:
            consulta_total(p_carro);
            printf("Digite o registro do carro que deseja alugar: ");
            scanf("%i", &op_carro);
			system("cls");
			cadastro_cliente(op_carro, p_cli);
            break;
        case 3:
            consulta_se_tem_livre_por_tipo(p_carro);
            system("pause");
            break;
        } // switch

    } while (op != 4);
    system("pause");
    return 1;
    return 1;
}