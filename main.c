#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> /* GETPASS */
#define COMPUTADOR 1  /* COMPUTADOR 1 = WINDOWS , COMPUTADOR 2 = UNIX */
#if COMPUTADOR == 1
    #include "stdio.h"
#elif COMPUTADOR == 2
    #define system(x) system("clear") /*  Caso esteja rodando em UNIX */
#endif // COMPUTADOR


struct Alunos{ //ESTRUTURA BASE

    char *nome;
    char *nomeR;
    char *RG;
    char *tel;
    char *ano;
    int RA;

};
struct Admin{//ESTRUTURA LOGIN
    char nome[20];
    char senha[20];
};
//Prot�tipos das fun��es
void incluir();
void pegarDados(struct Alunos *contato);
void guardarDados(FILE *x ,struct Alunos contato);
void listar();
void imprimirContato    ( struct Alunos x, int num );
int lerDadosArquivo (FILE *x, struct Alunos *y);
int  menu( int opc );
void alterar();
void excluir();
void verific();
void newusuario();

int main (){ //Fun��o Principal
    //ajusta o console a 70 letras e 50 linhas
    system("MODE CON cols=70 lines=50");

    //Permite letras como �, �, � etc
    setlocale(LC_ALL, "Portuguese");


    bool sair = false;
    int opc = 0;


   // verific();
    while( !sair ){
           switch ( menu(opc) ) {
           case 1: incluir();   break;
		   case 2: listar ();   break;
		  // case 3: ordenar();   break;
           case 4: excluir();   break;
           case 5: alterar(); break;
           case 6: newusuario(); break;
		    case 0: sair = true; break;
		    default: printf( "N�o existe essa opcao\n" ); getchar(); getchar(); break;
		}// SWITCH

    }// WHILE

    return 0;
}
void incluir(){ //Fun��o para cadastrar um novo membro ao final do arquivo

FILE *fp;
if ((fp = fopen("agenda.dat","ab"))== NULL){
    printf("\nN�o foi possivel abrir o arquivo agenda.txt.\n");
    return;
    }

    struct Alunos contato;
    pegarDados(&contato);
    guardarDados(fp,contato);
    printf("Contato guardado corretamente. ");
    getchar();
    getchar(); // Pausa
    fclose(fp);
    free(contato.nome);
    free(contato.nomeR);
    free(contato.RG);
    free(contato.tel);
    free(contato.ano);
    return;


}
void pegarDados(struct Alunos *contato){ //Salvar dados na estrutura
char opc[10];
char buf[1024];
int tamanho;

system("cls");
do{
        system("cls");
        printf("Digite os dados pedidos \n");
        printf("\nNome...............: ");
            scanf("%1024[^\n]%*c", buf);
            setbuf(stdin,NULL); // LIMPAR BUFFER;
            contato->nome = (char *) calloc(1,strlen(buf)+1);
            strncpy(contato->nome,buf,(strlen(buf)+1));
        printf("\nNome do Responsavel: ");
            scanf("%1024[^\n]%*c", buf);
            setbuf(stdin,NULL); // LIMPAR BUFFER;
            contato->nomeR = (char *) calloc(1,strlen(buf)+1);
            strncpy(contato->nomeR,buf,(strlen(buf)+1));
        setbuf(stdin,NULL);
        printf("\nRG ................: ");
            scanf("%1024[^\n]%*c", buf);
            setbuf(stdin,NULL); // LIMPAR BUFFER;
            contato->RG = (char *) calloc(1,strlen(buf)+1);
            strncpy(contato->RG,buf,(strlen(buf)+1));
        setbuf(stdin,NULL);
        printf("\nTelefone...........: ");
            scanf("%1024[^\n]%*c", buf);
            setbuf(stdin,NULL); // LIMPAR BUFFER;
            contato->tel = (char *) calloc(1,strlen(buf)+1);
            strncpy(contato->tel,buf,(strlen(buf)+1));
        setbuf(stdin,NULL);
        printf("\nAno................: ");
            scanf("%1024[^\n]%*c", buf);
            setbuf(stdin,NULL); // LIMPAR BUFFER;
            contato->ano = (char *) calloc(1,strlen(buf)+1);
            strncpy(contato->ano,buf,(strlen(buf)+1));
        setbuf(stdin,NULL);
        printf("\nRA.................: ");
            scanf(" %d",&contato->RA);
        setbuf(stdin,NULL);
        puts("\n\nCONFIRMA OS DADOS ? (y/n)");
            scanf(" %s", opc);
}while(opc[0] == 'n'|| opc[0] == 'N');

return;

}
void guardarDados(FILE *x ,struct Alunos contato){ // Salvar dados ao final do arquivo
//Escreve os dados tomados pela fun��o pegarDados no arquivo *x
fseek(x, 0L, SEEK_END); //Colocar ponteiro no final do arquivo para n�o sobreescrever o cadastro anterior
fprintf(x,"%s,",contato.nome);
fprintf(x,"%s,",contato.nomeR);
fwrite(&contato.RA,sizeof(int),1,x);
return;

}
void listar(){ // Listar dados gravados
//Fun��o que imprime tdos os contatos cadastrados do arquivo referente
    system("cls");
    int contador = 1;
    FILE *fp;
    fp = fopen("agenda.dat", "rb");
    if(fp == NULL){
        printf("N�o foi possivel abrir o arquivo");
        return;
        }
    struct Alunos contato;
    rewind(fp); // Volta o ponteiro ao come�o do arquivo
    //ler um a um enquando n�o chegar ao fim do arquivo
    while(lerDadosArquivo(fp,&contato))//fread retorna o numero de arquivos lidos como mandou ler s� 1, se nao conseguir retornar� 0(FALSE
    {
    //Evitar lista de contatos excluidos
        if(contato.nome[0] != '*'){
            imprimirContato(contato,contador);
                    }
        contador++;

        }
        getchar(); // PAUSAR A TELA
        fclose(fp);
    return;
}
void imprimirContato    ( struct Alunos x, int num ){ // Fun��o chamada pela fun��o listar, para mostrar na tela dados
    //imprime o contato recebido por referencia.
    printf("%d : Nome:%-20s\t\t\t\tRA:%5d\n",num, x.nome,x.RA);
    return;
    }
int lerDadosArquivo (FILE *x, struct Alunos *y){
    char buf[100];
    char buf2[100];// fread no arquivo

   fscanf(x,"%[^\,],%[^\,],",buf,buf2);
    fread(&y->RA,sizeof(int),1,x);
    return;
}
int  menu( int opc ){ // op��o do menu switch main
  //Esse � o menu de op�oes
    system("cls");
    printf("\t\t\t\tEscolha uma op��o\n1-Incluir \n2-Listar \n3-Ordenar \n4-Excluir \n5-Alterar \n0-Sair \n?>>");
    scanf(" %d%*c",&opc );
    return opc; //<-retorna a op�ao para o switch
}
void alterar (){ // Altera um cadastro

FILE *fp;
if((fp= fopen("agenda.txt","r+"))== NULL){
    printf("Erro na abertura do arquivo agenda.txt");
    return;
}

struct Alunos contact;
long int nReg;


puts("Informe o numero do cadastro: "); scanf(" %ld",&nReg);
    system("cls");
if  (fseek(fp,(nReg-1)*sizeof(struct Alunos),SEEK_SET) != 0){ // nReg -1 porque o usuario n�o conhece o indice '0'
    printf("Erro ao posicionar no arquivo!\n");
    return;
    }
if  (lerDadosArquivo(fp,&contact) != 1) //fread vai ler 1 estrutura na qual foi posicionada pelo fseek
{
    puts("Erro na leitura do arquivo");
    return;
}

if   (contact.nome[0] == '*')
{
    printf("Um registro apagado n�o pode ser alterado !\n");
    getchar();
    getchar();
    return;
}

printf("Dados a serem alterados : \n");
    imprimirContato(contact,nReg);
    getchar(); // LIMPAR BUFFER
    getchar();
printf("Novos dados :\n");
    pegarDados(&contact);
// RECUAR UM REGISTRO NO ARQUIVO
fseek(fp,-(long) sizeof(struct Alunos),SEEK_CUR);
//REESCREVER REGISTRO

fwrite(&contact,sizeof(struct Alunos),1,fp);
    puts("Dados gravados!\n");
    getchar();
fclose(fp);
}
void excluir(){
	int num1,i;
	FILE* fp;
	struct Alunos x;
	if((fp=fopen ("agenda.txt","r+"))== NULL){
		puts("Erro ao abrir arquivo.\n");
		return;
	}
	printf("Digite o numero de cadastro a ser excluido >> ");
	scanf("%d",&num1);
	fseek(fp,(num1-1)*sizeof(struct Alunos),SEEK_SET); // COLOCAR PONTEIRO NA ESTRUTURA DESEJADA
if  (lerDadosArquivo(fp,&x) != 1) //fread vai ler 1 estrutura na qual foi posicionada pelo fseek
{
    puts("Erro na leitura do arquivo");
    return;
}
	for (i=0;i<20;i++){
		x.nome[i] = '*';
		x.nomeR[i] = '*';
		x.ano[i] = '*';
		x.RG[i] = '*';
		x.tel[i]= '*';
	}
    x.RA = 0; //Fora do FOR pois RA n�o � um vetor.

	fseek(fp,-(long) sizeof(struct Alunos),SEEK_CUR); // recuar uma estrutura para sobrepola
	fwrite(&x,sizeof(struct Alunos),1,fp);
	    puts("Dados excluidos!\n");
	    getchar();
	    getchar();
	fclose(fp);
}
void verific(){
    FILE *arq;
    arq = fopen("usuarios.txt","r");
    if(arq == NULL){
            newusuario();
        arq = fopen("usuarios.txt","r");

        }
    else
        printf("Arquivo lido com sucesso!\n");

    struct Admin login;
    char senha[12];
    char usuario[12];
    int teste = 0;

    system("cls");
    printf("Para prosseguir digite os dados\nUsuario>> ");
        scanf("%s",usuario);
    printf("\nSenha>> "); scanf(" %s",senha);
     if((strlen(senha))< 4 ){
        printf("Numero de digitos INCORRETO!\n");
        exit(1);
    }
    while((fread(&login,sizeof(struct Admin),1,arq))== 1)
{

        while((strcmp(usuario,login.nome))== 0){
            if((strcmp(senha,login.senha))== 0){
                teste = 1;
                getchar();
                break;
                }
                break;
                                                }
}
    if(teste != 0){
    system("cls");
    if((strcmp(usuario,"admin"))== 0)
        printf("OL� RAFAEL\nSEGUE O MENU DE ADMINISTRADOR>>\n");
    else
    printf("Ol� Senhor(a) %s !\n%s\n%s\nDigite <ENTER> Para Prosseguir...",usuario,__DATE__,__TIME__);
    getchar();
}
    else {
        puts("Dados Incorretos!\n*********** Permiss�o Negada   ***********");
        exit(3);
        }
}
void newusuario(){
    FILE *arq;
    arq = fopen("usuarios.txt","a+");
    if(arq == NULL){
        printf("ERRO AO CRIAR ARQUIVO. CONTACTE O ADMINISTRADOR\n");
        exit(1);
    }
    struct Admin cadastro = {0};
    char passConf[20];
    int ver = 0;
        do{
            ver = 0;
            printf("\nDigite seu nome de usu�rio>> ");
            scanf("%s",cadastro.nome);
            printf("\nDigite sua senha>> "); scanf("%s",cadastro.senha);
            printf("\nConfirme sua senha>> "); scanf("%s",passConf);
            if((strcmp(cadastro.senha,passConf))== 0){
                printf("Senhas conferidas!\n");
            }
            else{
                puts("Senhas n�o s�o iguais!\n");
                ver = 1;
                getchar();
                system("cls");
                }
            if((strlen(cadastro.senha))< 4){
                ver = 1;
                puts("Digitos insuficientes! \n");
                getchar();
                system("cls");
            }
            }while(ver == 1);
        fwrite (&cadastro,sizeof(struct Admin),1,arq);
        puts("\nLOGIN SALVO");
        getchar();
        fclose(arq);

}
