//Gabriel Rocha Monaco RA 171092
//Igor Dantas RA 190721
//João Pedro Abrantes RA 190846


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

/* 
    Verificar se CPF é permitido - baixa prioridade
    Alterar o status do carro alugado - Alta prioridade
    Consulta de cliente - Média prioridade (Bom para ver se o carro foi alugado)
 
    Acho que não precisa altrar os valores do arquivo salvo por enquanto
 */


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

typedef struct vip{
    int reg_cli;
    char nome[80];
    char CPF[15];
    char tipo;
}vip;

int verifica_arquivo_carro();
int verifica_arquivo_cliente();
void aloca_carro(carro **p, int q);
void cadastra_carro(carro *p_carro, int q);
void consulta_total(carro *p_carro);
void consulta_se_tem_livre_por_tipo(carro *p_carro);
void grava_cliente(cliente *p, char *str, int pos);
void aloca_cliente(cliente **p, int q);
void grava_carro(carro *p,char *str,int pos);
int busca(carro *p_carro,int num_reg);
void altera(carro *p_carro, int reg_car, cliente *p_cli);
int busca_vago_cliente(cliente *p_cli, int qtde);
void cadastro_cliente(int op_carro, cliente *p_cli, carro *p_carro);
void consulta_de_teste(carro *p_carro);
void gravaClienteAntigo(vip *p_vip, char *str,int pos);
void aloca_vip(vip **p, int q);
int verifica_arquivo_vip();
void deletaCliente(cliente *p_cli, vip *p_vip, int pos);
void consulta_total_cliente(cliente *p_cliente);
int busca_cpf(cliente *p_cli,char *cpf_devolucao);
int buscaCarroPorRegCli(carro *p_carro, int reg_cli);
int devolucao(carro *p_carro, cliente *p_cli);
void colocaDadosDeCarro(carro *p_carro,cliente *p_cli, int pos);
void consulta_historico_cliente(vip *p_vip);
float calculaValorAPagar(carro *p_carro);
int calculaDiasEntreDatas(int dia, int mes, carro *p_carro);
char* escolheCidade();
void consulta_nova(carro *p_carro);
char escolhe_tipo();
int escolhe_entre_numeros(int numero_a, int numero_b);
int verifica_se_esta_livre(carro *p_carro, int dia_busca, int mes_busca);
int tem_no_lugar_certo(carro *p_carro, char *local);
void apresenta_dados_carro(carro *p_carro);

int main()
{
    setlocale(LC_ALL, "");
    carro *p_carro = NULL;
    cliente *p_cli = NULL;
    vip *p_vip = NULL;
    
    aloca_carro(&p_carro, 1);
    aloca_cliente(&p_cli,1);
    aloca_vip(&p_vip, 1);

    int op, op_carro;
    info_carro a;
    int numero;
    do
    {
        printf("\n[1]Cadastrar carro\n[2]Consulta Total/Aluguel de Carro\n[3]Consulta Parcial\n[4]Devolucao\n[5]Consulta Cliente Antigo\n[6]Fim\nOpcao:");
        scanf("%i", &op);
        fflush(stdin);
        switch (op)
        {
        case 1:
            strcpy(a.local_ret, "\nCadastro");
            printf("%s\n", a.local_ret);
            cadastra_carro(p_carro, 1);
            break;
        case 2:
            consulta_total(p_carro);
            printf("\nDigite o registro do carro que deseja alugar: ");
            scanf("%i", &op_carro);
            fflush(stdin);
            //printf("Antes de cadastro cliente");
			cadastro_cliente(op_carro, p_cli,p_carro);
            break;
        case 3:
            consulta_se_tem_livre_por_tipo(p_carro);
            system("pause");
            break;
        case 4:
            int pos = devolucao(p_carro, p_cli);
            if(pos!=-1) deletaCliente(p_cli, p_vip, pos);
            break;
        case 5:
            consulta_historico_cliente(p_vip);
            break;
        case 9:
            consulta_de_teste(p_carro);
            break;
        case 10:
            consulta_total_cliente(p_cli);
            break;
        case 11:
            consulta_nova(p_carro);
            break;
        default:
            printf("\nEscolha uma opcao valida\n");
            break;
        } // switch

    } while (op != 6);
    system("pause");
    return 1;
}

// Verifica o numero de carros cadastrados.
//@return Número do registro do ultimo carro,
// caso não existe carro.bin, o retorno é 0.
int verifica_arquivo_carro()
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

//Verifica o número de clientes cadastrados.
//@return Número do registro do ultimo cliente,
//caso não exista cliente.bin o retorno é 0.
int verifica_arquivo_cliente()
{
    long int cont = 0;
    FILE *fptr = NULL;
    if ((fptr = fopen("cliente.bin", "ab")) == NULL)
        return cont;
    else
    {
        fseek(fptr, 0, 2);                  // posiciona no fim do arquivo
        cont = ftell(fptr) / sizeof(cliente); // qtde de elementos
        fclose(fptr);                       // dentro do ELSE por conta do rb
        return cont;
    }
}


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

    n = verifica_arquivo_carro();
    p_carro->reg_car = ++n;
    printf("\nRegistro: %i\n", p_carro->reg_car);
    printf("\nModelo: ");
    gets(p_carro->modelo);
    fflush(stdin);
    p_carro->tipo = escolhe_tipo();
    fflush(stdin);
    printf("\nDiaria: ");
    scanf("%f", &(p_carro->diaria));
    fflush(stdin);

    p_carro->status.car.sigla = 'L';
    printf("\nEscolha uma cidade para retirar o carro: ");
    strcpy(p_carro->status.car.local_ret, escolheCidade());

    grava_carro(p_carro,"ab", 1);
}

char escolhe_tipo(){
    char escolha = 'a'; //escolha arbitraria
    char escolha_intermediaria = 'a';
    while (escolha != 'G' && escolha != 'M' && escolha != 'P')
    {

        printf("\nEscolha um tipo: \n[G] - Grande \n[M] - Medio \n[P] - Pequeno\n");
        fflush(stdin);
        scanf("%c", &escolha_intermediaria);
        escolha = toupper(escolha_intermediaria);



        if(escolha != 'G' && escolha != 'M' && escolha != 'P'){
            printf("\nEscolha uma opcao valida\n");
        }
    }

    return escolha;
}

int escolhe_entre_numeros(int numero_a, int numero_b){
    int escolha=numero_a-1; // Garante que a escolha seja inicializada e esteja entre o intervalo a e b
    printf("\nEscolha um numero entre %i e %i\n: ", numero_a, numero_b);
    while(escolha < numero_a || escolha > numero_b){
        fflush(stdin);
        scanf("%i", &escolha);
        if(escolha < numero_a || escolha > numero_b){
            printf("\nEscolha um numero valido.\n");
        }
    }
}


int verifica_se_esta_livre(carro *p_carro, int dia_busca, int mes_busca){
    //Só leva em consideração no mesmo ano, a implementação para garantir diferença de ano fica
    //extremamente impossivel sem o dado de ano
    //Infelizmente não podemos alterar a struct para incluir ano

    int boleano = 0;

    if(p_carro->status.car.sigla=='L'){
        boleano = 1;
        return boleano;
    }

    if(p_carro->status.car.sigla=='A'){
        if((p_carro->status.dados[0].dia_dev < dia_busca && p_carro->status.dados[0].mes_dev == mes_busca) || p_carro->status.dados[0].mes_dev < mes_busca ){
            boleano = 1;
            return boleano;
        }
    }

    if(p_carro->status.car.sigla=='R'){
        if((p_carro->status.dados[1].dia_dev < dia_busca && p_carro->status.dados[1].mes_dev == mes_busca) || p_carro->status.dados[1].mes_dev < mes_busca ){
            boleano = 1;
            return boleano;
        }
    }
    
    return boleano;
}

int tem_no_lugar_certo(carro *p_carro, char *local){
    int retorno = 0;
    
    if(p_carro->status.car.sigla == 'L'){
        if(strcmp(p_carro->status.car.local_ret, local)==0){
            retorno = 1;
            return retorno;
        }
    }

    if(p_carro->status.car.sigla == 'A'){
        if(strcmp(p_carro->status.dados[0].local_dev, local)==0){
            retorno = 1;
            return retorno;
        }
    }

    if(p_carro->status.car.sigla == 'R'){
        if(strcmp(p_carro->status.dados[1].local_dev, local)==0){
            retorno = 1;
            return retorno;
        }
    }

    return retorno;
}

void consulta_nova(carro *p_carro){
    int dia_entrega = -1;
    int mes_entrega = -1;
    char tipo_busca = ' ';
    char local_busca[30] = "Sem escolha";
    printf("\nEscolha um dia: ");
    dia_entrega = escolhe_entre_numeros(1,30);
    printf("\nEscolha um mes: ");
    mes_entrega = escolhe_entre_numeros(1,12);
    tipo_busca = escolhe_tipo();
    strcpy(local_busca, escolheCidade());

    int i, verifica_dia_livre;
    fflush(stdin);
    FILE *ar = NULL;
    int numero_de_carro = verifica_arquivo_carro();
    if ((ar = fopen("carro.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < numero_de_carro; i++)
        {
            fseek(ar,i*sizeof(carro),0);
            fread(p_carro,sizeof(carro),1,ar);
            verifica_dia_livre = verifica_se_esta_livre(p_carro, dia_entrega, mes_entrega);
            printf("\n %i %c %i %i", p_carro->reg_car, p_carro->tipo, tem_no_lugar_certo(p_carro, local_busca), verifica_dia_livre);
            if(p_carro->tipo==tipo_busca && tem_no_lugar_certo(p_carro, local_busca)==1
                && verifica_dia_livre==1){
                apresenta_dados_carro(p_carro);
            }
        }
    }

}


void apresenta_dados_carro(carro *p_carro){
    if (p_carro->status.car.sigla == 'L')
    {
        printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %.2f\nLocal de Retirada: %s\nStatus: %c\n", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria, p_carro->status.car.local_ret, p_carro->status.car.sigla);
    }
    else if (p_carro->status.car.sigla == 'A')
    {
        printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %.2f\nStatus: %c\n", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria, p_carro->status.car.sigla);
        printf("Local de devolucao: %s\n", p_carro->status.dados[0].local_dev);
        printf("Dia devolucao: %i / %i\n", p_carro->status.dados[0].dia_dev, p_carro->status.dados[0].mes_dev);
    }
    else
    {
        printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %.2f\nStatus: %c\n", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria, p_carro->status.car.sigla);
        printf("Local de devolucao: %s\n", p_carro->status.dados[1].local_dev);
        printf("Dia devolucao: %i / %i\n", p_carro->status.dados[1].dia_dev, p_carro->status.dados[1].mes_dev);
    }
}

//Verifica todos os carros cadastrados.
//Faz um print de todos os carros cadastrados no sistema
//@param p_carro Ponteiro de carros utilizado
void consulta_total(carro *p_carro)
{
	int i;
    FILE *ar = NULL;

    int numero_de_carro = verifica_arquivo_carro();
    if ((ar = fopen("carro.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < numero_de_carro; i++)
        {
            fseek(ar,i*sizeof(carro),0);
            fread(p_carro,sizeof(carro),1,ar);
            apresenta_dados_carro(p_carro);

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
    int numero_de_carro = verifica_arquivo_carro();
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
void grava_cliente(cliente *p, char *str, int pos)
{
    FILE *fptr=NULL;

    if((fptr=fopen("cliente.bin",str))==NULL)	
    printf("\nErro ao abrir o arquivo");
    else
    {
        if(strcmp(str,"rb+")==0)
            fseek(fptr,pos*sizeof(cliente),0);
        fwrite(p,sizeof(cliente),1,fptr);
    }//else
    fclose(fptr);
}

//Aloca memória para o ponteiro de cliente.
//@param p Endereço do ponteiro de Cliente utilizado
//@param q Tamanho desejado do ponteiro
void aloca_cliente(cliente **p, int q)
{
    if (((*p = (cliente *)realloc(*p, q * sizeof(cliente))) == NULL))
        exit(1);
}

void grava_carro(carro *p,char *str,int pos)
{
FILE *fptr=NULL;
if((fptr=fopen("carro.bin",str))==NULL)
  printf("\nErro ao abrir o arquivo");
else
  {
   if(strcmp(str,"rb+")==0)
     fseek(fptr,pos*sizeof(carro),0);
   fwrite(p,sizeof(carro),1,fptr);
  }//else
fclose(fptr); //fora do ELSE por conta do ab ou rb+
}//grava

int busca(carro *p_carro,int num_reg)
{
FILE *fptr=NULL;
int qreg,achou=-1,i;
qreg=verifica_arquivo_carro();
if((fptr=fopen("carro.bin","rb"))==NULL)
  printf("\nErro");
else
  {
   for(i=0;i<qreg;i++)
     {
      fseek(fptr,i*sizeof(carro),0);
      fread(p_carro,sizeof(carro),1,fptr);
      if(p_carro->reg_car==num_reg)
        {
         achou=i;
         i=qreg;  //forca a saida do for
  }//if
  }//for
  fclose(fptr);   //DENTRO do else - abriu com rb   
  }//else
return achou;  //posicao do registro
}//busca


void altera(carro *p_carro, int reg_car, cliente *p_cli){
    //Pega o valor que está no ponteiro

    //A função busca altera o valor do ponteiro, mas o valor antigo está no p_carro
    //É preciso guardar os valores alterados antes porque a posição é essencial para 
    //a função gravar

    int pos = reg_car-1;

    //Esses prints perdidos é para debugging
    //p_carro->status.dados[0].sigla = valor_da_sigla_anterior;
    //if(valor_da_sigla_anterior=='R'){
    //    strcpy(p_carro->status.dados[1].local_dev, "Sorocaba");
    //    strcpy(p_carro->status.dados[1].local_ret, "Sorocaba");
    //}
    //if(valor_da_sigla_anterior=='L'){
    //    strcpy(p_carro->status.dados[0].local_dev, "Sorocaba");
    //    strcpy(p_carro->status.dados[0].local_ret, "Sorocaba");
    //}
    

    //printf("\nSigla no altera: %c" ,p_carro->status.car.sigla);

    //como a sigla está no valor antigo, a gente volta para o valor novo
    grava_carro(p_carro, "rb+",pos);
}

int busca_vago_cliente(cliente *p_cli, int qtde){
    FILE *fptr=NULL;
    int achou=-1,i;
    if((fptr=fopen("cliente.bin","rb"))==NULL)
    return achou;
    else
    {
        for(i=0;i<qtde;i++)
        {
            fseek(fptr,i*sizeof(cliente),0);
            fread(p_cli,sizeof(cliente),1,fptr);
            if(p_cli->reg_cli==-1)   //achou vago
            {
                achou=i;  //posicao do registro vago
                i=qtde;  //forca a saida do for
            }//if
        }//for
    fclose(fptr);   //DENTRO do else - abriu com rb
    return achou;  //posicao do registro	  
    }//else
}

//Cadastra o cliente com o carro desejado.
//@param op_carro Numero de registro do carro
//@param p_cli Ponteiro de Cliente utilizado
void cadastro_cliente(int op_carro, cliente *p_cli, carro *p_carro) { 
	
    printf("\nCPF: ");
    gets(p_cli->CPF);
    char *cpf_a_comparar;
    strcpy(cpf_a_comparar, p_cli->CPF);
    int ve_se_cliente_existe = busca_cpf(p_cli, cpf_a_comparar);
    if(ve_se_cliente_existe!=-1){
        printf("\nCliente esta alugando carro");
        return;
    }
    strcpy(p_cli->CPF,cpf_a_comparar);
    
    int n = verifica_arquivo_cliente(); 
	int ind;
    ind = busca_vago_cliente(p_cli, n);
    if(ind!=-1){
        n=ind;
    }
    p_cli->reg_cli = n+1;

    //printf("ANtes");
    if(p_carro==NULL){
        aloca_carro(&p_carro, 1);
    }
    busca(p_carro, op_carro);
    p_cli->reg_car = op_carro;
    p_cli->sigla = p_carro->tipo;
    printf("\nRegistro: %i\n", p_cli->reg_cli);
    printf("\nNome: ");
    gets(p_cli->nome);
  	
    p_cli->reg_car = op_carro;
    printf("\nModelo: %s",p_carro->modelo);
    printf("\nSigla: %c" ,p_carro->status.car.sigla);
    

    if((p_carro->status.car.sigla) == 'L')
        p_carro->status.car.sigla = 'A';
    else if((p_carro->status.car.sigla) == 'A'){
        p_carro->status.car.sigla = 'R';
    }
    else {
        printf("\nCarro ja reservado!");
        return;
    }

    if(p_carro->status.car.sigla == 'A'){
        colocaDadosDeCarro(p_carro,p_cli, 0);
    }else{
        colocaDadosDeCarro(p_carro,p_cli, 1);
    }
    //p_carro->status.car.sigla='A';
    //printf("\nSigla: %c" ,p_carro->status.car.sigla);    
    //A maracutaia está na função salvar
    altera(p_carro, op_carro, p_cli);
    if(ind==-1){
        grava_cliente(p_cli,"ab", 1);
    }else{
        grava_cliente(p_cli,"rb+", n);
    }
    
}

void consulta_de_teste(carro *p_carro)
{
	int i;
    FILE *ar = NULL;

    if(p_carro == NULL){
        aloca_carro(&p_carro,1);
    }

    int numero_de_carro = verifica_arquivo_carro();
    if ((ar = fopen("carro.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < numero_de_carro; i++)
        {
            fseek(ar,i*sizeof(carro),0);
            fread(p_carro,sizeof(carro),1,ar);
            printf("\nRegistro do Carro: %i\nModelo: %s\nTipo: %c\nDiaria: %f\nStatus: %c", p_carro->reg_car, p_carro->modelo, p_carro->tipo, p_carro->diaria, p_carro->status.dados[0].sigla);
            printf("\nCliente: %i", p_carro->status.dados[0].reg_cli);
        }
    }
}

void gravaClienteAntigo(vip *p_vip, char *str,int pos)
{
FILE *fptr=NULL;
if((fptr=fopen("vip.bin",str))==NULL)
  printf("\nErro ao abrir o arquivo");
else
  {
   if(strcmp(str,"rb+")==0)
     fseek(fptr,pos*sizeof(vip),0);
   fwrite(p_vip,sizeof(vip),1,fptr);
  }//else
fclose(fptr); //fora do ELSE por conta do ab ou rb+
}//grava

void aloca_vip(vip **p, int q)
{
    if (((*p = (vip *)realloc(*p, q * sizeof(vip))) == NULL))
        exit(1);
}

int verifica_arquivo_vip()
{
    long int cont = 0;
    FILE *fptr = NULL;
    if ((fptr = fopen("vip.bin", "ab")) == NULL)
        return cont;
    else
    {
        fseek(fptr, 0, 2);                  // posiciona no fim do arquivo
        cont = ftell(fptr) / sizeof(vip); // qtde de elementos
        fclose(fptr);                       // dentro do ELSE por conta do rb
        return cont;
    }
}

void deletaCliente(cliente *p_cli, vip *p_vip, int pos){
    strcpy(p_vip->nome, p_cli->nome);
    p_vip->reg_cli = p_cli->reg_cli;
    strcpy(p_vip->CPF, p_cli->CPF );
    p_vip->tipo = p_cli->sigla;
    p_cli->reg_cli=-1;
    grava_cliente(p_cli, "rb+", pos);
    int numeroVips = verifica_arquivo_vip();
    gravaClienteAntigo(p_vip, "rb+", numeroVips);
}

void consulta_total_cliente(cliente *p_cliente)
{
	int i;
    FILE *ar = NULL;

    int numero_de_cliente = verifica_arquivo_cliente();
    if ((ar = fopen("cliente.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i <numero_de_cliente; i++)
        {
            fseek(ar,i*sizeof(cliente),0);
            fread(p_cliente,sizeof(cliente),1,ar);
            printf("\nRegistro do cliente: %i\nNome: %s\nCPF: %s\nSigla: %c", p_cliente->reg_cli, p_cliente->nome, p_cliente->CPF, p_cliente->sigla, p_cliente->sigla);
            printf("\nN do registro do carro: %i\n", p_cliente->reg_car);

        }
    }
}

int busca_cpf(cliente *p_cli,char *cpf_devolucao)
{
FILE *fptr=NULL;
int qreg,achou=-1,i;
int sobra;
qreg=verifica_arquivo_cliente();
if((fptr=fopen("cliente.bin","rb"))==NULL)
  printf("\nErro");
else
  {
   for(i=0;i<qreg;i++)
     {
      fseek(fptr,i*sizeof(cliente),0);
      fread(p_cli,sizeof(cliente),1,fptr);
      sobra = strcmp(p_cli->CPF, cpf_devolucao);
      if(sobra==0)
        {
         achou=i;
         i=qreg;  //forca a saida do for
         printf("\nCPF = %s foi encontrado no sistema!\n", p_cli->CPF);
  	}//if
  }//for
  fclose(fptr);   //DENTRO do else - abriu com rb   
  }//else
return achou;  //posicao do registro
}//busca


int buscaCarroPorRegCli(carro *p_carro, int reg_cli){
    int pos =-1;
    FILE *fptr=NULL;
int qreg,achou=-1,i;
int sobra;
qreg=verifica_arquivo_carro();
if((fptr=fopen("carro.bin","rb"))==NULL)
  printf("\nErro");
else
  {
   for(i=0;i<qreg;i++)
     {
      fseek(fptr,i*sizeof(carro),0);
      fread(p_carro,sizeof(carro),1,fptr);
      printf("\n reg cli no carro[0] -> %i",p_carro->status.dados[0].reg_cli);
      if(p_carro->status.dados[0].reg_cli==reg_cli)
        {
         achou=i;
         i=qreg;  //forca a saida do for
         printf("\nCarro foi encontrado no sistema!\n");
  	}//if
  }//for
  fclose(fptr);   //DENTRO do else - abriu com rb   
  }//else
    return achou;
}

int devolucao(carro *p_carro, cliente *p_cli) {
	char cpf_devolucao[20];
	
	printf("Informe seu CPF: ");
	scanf("%s", &cpf_devolucao);
	int lugarDoCliente = busca_cpf(p_cli, cpf_devolucao);
    if(lugarDoCliente==-1){
        printf("\nCliente não encontrado.");
        return -1;
    }
    int localDoCarro = buscaCarroPorRegCli(p_carro, p_cli->reg_cli);//altera a posição do carro para bater com o do cliente

    if (localDoCarro==-1){
        printf("\nCarro com registro não encontrado");
        return -1;
    }
    if(p_carro->status.dados[0].reg_cli != p_cli->reg_cli){
        printf("\nRegistro de cliente do aluguel não bate com registro do cliente");
        return -1;
    }

    float valor = calculaValorAPagar(p_carro);

    if(p_carro->status.car.sigla == 'R'){
        p_carro->status.car.sigla = 'A';

        p_carro->status.dados[0].dia_dev = p_carro->status.dados[1].dia_dev;
        p_carro->status.dados[0].mes_dev = p_carro->status.dados[1].mes_dev;
        strcpy(p_carro->status.dados[0].local_ret,p_carro->status.dados[1].local_ret);
        strcpy(p_carro->status.dados[0].local_dev,p_carro->status.dados[1].local_dev);
        p_carro->status.dados[0].dia_ret = p_carro->status.dados[1].dia_ret;
        p_carro->status.dados[0].mes_ret = p_carro->status.dados[1].mes_ret;
        p_carro->status.dados[0].reg_cli = p_carro->status.dados[1].reg_cli;

        //A sigla está dando erro, parece que a sigla de dados[1] não recebe os valores
        //p_carro->status.dados[0].sigla = p_carro->status.dados[1].sigla;
    }
    else
    {
        p_carro->status.car.sigla = 'L';
        strcpy(p_carro->status.car.local_ret,p_carro->status.dados[0].local_dev);
    }
    altera(p_carro, p_cli->reg_car, p_cli);

    return lugarDoCliente;
}

void colocaDadosDeCarro(carro *p_carro,cliente *p_cli, int pos){
    if (pos==0) p_carro->status.dados[pos].sigla = 'A';
    if (pos==1) p_carro->status.dados[pos].sigla = 'R';
    p_carro->status.dados[pos].reg_cli = p_cli->reg_cli;

    if(pos==1){
        p_carro->status.dados[1].dia_ret = p_carro->status.dados[0].dia_dev + 1 ;

        //Parte que virifica se a data não passa de 30 e 12 para dia e mes
    
        if(p_carro->status.dados[1].dia_ret > 30){
            p_carro->status.dados[1].dia_ret = 1;
            p_carro->status.dados[1].mes_ret +=1;
        }

        if(p_carro->status.dados[1].mes_ret > 12){
            p_carro->status.dados[1].mes_ret = 1;
        }

    }else{
        int dia_ret;
        printf("\nDia de retirada: ");
        dia_ret = escolhe_entre_numeros(1,30);
        p_carro->status.dados[pos].dia_ret = dia_ret;

        int mes_ret;
        printf("\nMes de retirada: ");
        mes_ret=escolhe_entre_numeros(1,12);
        p_carro->status.dados[pos].mes_ret = mes_ret;
    }


    int dia_dev;
    printf("\nDia de devolucao: ");
    dia_dev=escolhe_entre_numeros(1,30);
    p_carro->status.dados[pos].dia_dev = dia_dev;

    int mes_dev;
    printf("\nMes de devolucao: ");
    mes_dev=escolhe_entre_numeros(1,12);
    fflush(stdin);
    p_carro->status.dados[pos].mes_dev = mes_dev;
    
    printf("\nEscolha uma cidade para devolver o carro: ");
    strcpy(p_carro->status.dados[pos].local_dev , escolheCidade());
}

//Verifica todos os carros cadastrados.
//Faz um print de todos os carros cadastrados no sistema
//@param p_carro Ponteiro de carros utilizado
void consulta_historico_cliente(vip *p_vip)
{
	int i;
    FILE *ar = NULL;

    int num_de_vip = verifica_arquivo_vip();
    if ((ar = fopen("vip.bin", "rb")) == NULL)
        printf("\nErro");
    else
    {
        for (i = 0; i < num_de_vip; i++)
        {
            fseek(ar,i*sizeof(vip),0);
            fread(p_vip,sizeof(vip),1,ar);
            printf("\n %s %i %s %s %c\n",p_vip->nome, p_vip->reg_cli, p_vip->nome, p_vip->CPF, p_vip->tipo);
        }
    }
}

int calculaDiasEntreDatas(int dia, int mes, carro *p_carro){
    int dias_utilizados;

//A diaria vai ser mais barata nos meses de 31 dias
    if(p_carro->status.dados[0].mes_ret > mes){ // isso leva em consideração devolução no ano seguinte
        int total_dias_ate_ano_novo = 360 -((p_carro->status.dados[0].mes_ret) -1 )*30 - p_carro->status.dados[0].dia_ret;
        int dias_ate_dev_ano_atual = (mes-1)*30 + dia; 
        
        dias_utilizados = total_dias_ate_ano_novo + dias_ate_dev_ano_atual;
        
    } else {
        int dias_mes = (mes-(p_carro->status.dados[0].mes_ret)) * 30;
        int entreDias = dia - p_carro->status.dados[0].dia_ret;

        dias_utilizados = dias_mes +entreDias;
    }

    return dias_utilizados;
}

float calculaValorAPagar(carro *p_carro){
    float valor;
    int dia_retorno, mes_retorno;
    int temMulta = 0;

    fflush(stdin);
    printf("\nQual o dia do retorno: ");
    scanf("%i", &dia_retorno);
    printf("\nQUal o mes de retorno: ");
    scanf("%i", &mes_retorno);

    float multa = 0;
    int dias_ate_dev = calculaDiasEntreDatas(dia_retorno, mes_retorno, p_carro);
    int dias_ate_multa = calculaDiasEntreDatas(p_carro->status.dados[0].dia_dev,p_carro->status.dados[0].mes_dev, p_carro);
    int dias_utilizados;
    if(dias_ate_multa < dias_ate_dev){
        printf("\nMULTA APLICADA");
        multa = (p_carro->diaria)*2 * (dias_ate_dev - dias_ate_multa);
        printf("\n %.2f", multa);
        dias_utilizados = dias_ate_multa;
    }
    else{
        dias_utilizados = dias_ate_dev;
    }
    float diaria_a_cobrar = p_carro->diaria;

    valor = (diaria_a_cobrar * dias_utilizados + multa);
    printf("\nValor a pagar:  %.2f ", valor);
    return valor;
}

char* escolheCidade(){
    int op_correta = -1;
    int op = 0;
    do{
        printf("Escolha uma cidade \n[1] - Sorocaba [2] - Itu\n");
        scanf("%i", &op);
        switch (op)
        {
        case 1:
            return "Sorocaba";
            op_correta=1;
            break;
        case 2:
            return "Itu";
            op_correta=1;
        default:
            printf("\nPor favor, escolha uma opcao valida:");
            break;
        }
    } while (op_correta==-1);
}