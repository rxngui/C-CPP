/*
	Desenvolvido por Mateus Mendonça Monteiro RA: 10.191.159-9
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "meuconio.h"



//Definição das constantes das cores

//Definição dos Registrados
struct TpDicionario{
	char Portugues[25], Ingles[25], Significado[100];
	int Pontos;
};

struct TpPlayer{
	char login[25], Nome[30];
	int Pontos;
	char DataEntrada[10], DataUltAcesso[10];	
};

//Registrar as Palavras que foram mostradas para o jogador/ Tem que existir na TpPessoa
struct TpJogo{
	//chave primaria dupla
	//Login tem que ter em Player e Palavra no DIcionario
	char login[25], Palavra[25];
};

//UltStructValida = ftell(PtrPlay) / sizeof(TpPlayer) - 1;
//QtdeReg= ftell(PtrPlay) / sizeof(TpPlayer);

void execute();

void Moldura(int cor)
{

	int i, CI = 1,LI = 1,CF = 80,LF = 25; 
	textcolor(cor);
	gotoxy(CI,LI); printf("%c",201);
	gotoxy(CF,LF); printf("%c",188);
	gotoxy(CF,LI); printf("%c",187);
	gotoxy(CI,LF); printf("%c",200);
	
	for(i=CI+1;i<CF;i++)
	{
		gotoxy(i,LI); printf("%c",205);
		Sleep(10);
		gotoxy(i,LF); printf("%c",205);
	}
	
	for(i=LI+1;i<LF;i++)
	{
		gotoxy(CI,i); printf("%c",186);
		Sleep(10);
		gotoxy(CF,i); printf("%c",186);
		
	}
}

void getData(char str[10]){
   time_t mytime;
   mytime = time(NULL);

   char aux[2];
   struct tm tm = *localtime(&mytime);

   //Dia
   itoa(tm.tm_mday,aux,10);
   strcpy(str,aux);
   strcat(str,"/");

   //Mes
   itoa((tm.tm_mon + 1),aux,10);
   strcat(str,aux);
   strcat(str,"/");

   //Ano
   itoa((tm.tm_year + 1900),aux,10);
   strcat(str,aux);
}
void clear(){
	system("cls");
}
void getStr(char str[]){
	fflush(stdin);
	gets(str);
}

//Declaração das Funções - Protótipos
int BuscaPalavra(FILE *PtrDic, char Elem[]);
int BBIngles(FILE *PtrDic, char Elem[]); //BuscaExaustiva
void OrdenaDicionarioIngles(char NomeArq[]); //BubbleSort

// *** Dicionario ***

int BBIngles(FILE *PtrDic, char Elem[]){
	TpDicionario Reg;
	fseek(PtrDic,0,2);
	int I = 0, M, F = ftell(PtrDic) / sizeof(TpDicionario) - 1;
	M = F/2;
	
	fseek(PtrDic,M*sizeof(TpDicionario),0);
	fread(&Reg,sizeof(TpDicionario),1,PtrDic);
	
	while(I < F && stricmp(Reg.Ingles,Elem) != 0){
		
		if(stricmp(Elem,Reg.Ingles) > 0)
			I = M + 1;
		else
			F = M - 1;
		
		M = (I+F)/2;
		fseek(PtrDic,M*sizeof(TpDicionario),0);
		fread(&Reg,sizeof(TpDicionario),1,PtrDic);
	}
	if(stricmp(Reg.Ingles,Elem) == 0){
		return M * sizeof(TpDicionario);
	}
	else
		return -1;
}
	
void OrdenaDicionarioIngles(char NomeArq[]){//Bubble Sort

	int i, j, QtdeReg;
	TpDicionario RegI, RegJ;
	FILE *PtrDic = fopen(NomeArq,"rb+");
	clear();
	printf("\n\n\tOrdenando Dicionario em Ingles...");

	fseek(PtrDic,0,2);

	QtdeReg = ftell(PtrDic) / sizeof(TpDicionario);

	for (i = 0; i < QtdeReg-1; i++)
		for(j = i+1; j<QtdeReg; j++)
		{	
			fseek(PtrDic,i*sizeof(TpDicionario),0);
			fread(&RegI,sizeof(TpDicionario),1,PtrDic);

			fseek(PtrDic,j*sizeof(TpDicionario),0);
			fread(&RegJ,sizeof(TpDicionario),1,PtrDic);

			if(stricmp(RegI.Ingles,RegJ.Ingles) > 0)
			{
				fseek(PtrDic,i*sizeof(TpDicionario),0);
				fwrite(&RegJ,sizeof(TpDicionario),1,PtrDic);
				fseek(PtrDic,j*sizeof(TpDicionario),0);
				fwrite(&RegI,sizeof(TpDicionario),1,PtrDic);
			}
		}
	fclose(PtrDic);
	printf("\n\tDicionario Ordenado em Ingles!");
	getch();
}

int BuscaPalavra(FILE *PtrDic, char Palavra[25]){//Busca Exaustiva
	TpDicionario R;
	rewind(PtrDic);
	fread(&R,sizeof(TpDicionario),1,PtrDic);
	while(!feof(PtrDic) && stricmp(Palavra,R.Portugues)!=0)
			fread(&R,sizeof(TpDicionario),1,PtrDic);
	
	if(!feof(PtrDic))
      return ftell(PtrDic) - sizeof(TpDicionario);
    else
      return -1;
}

void CadDicionario(char NomeArq[])
{
	TpDicionario RegDic;
	FILE *PtrDic = fopen(NomeArq,"ab+");
	clear();
	printf("\n\n# # #  Cadastrar Palavras no Dicionario  # # #\n");
	printf("Palavra em Portugues: ");
	getStr(RegDic.Portugues);
	while (strcmp(RegDic.Portugues,"\0")!=0)
	{
		if (BuscaPalavra(PtrDic,RegDic.Portugues)==-1)  //Não encontramos a Palavra
		{
			printf("Palavra em Ingles: ");
			getStr(RegDic.Ingles);
			printf("Significado da Palavra: ");
			
			getStr(RegDic.Significado);
			RegDic.Pontos = (rand()%3+1) * 10;
			printf("Pontos: %d\n\n",RegDic.Pontos);
			fwrite(&RegDic,sizeof(TpDicionario),1,PtrDic);
			printf("\nRegistro armazenado corretamente!\n");
		}
		else
			printf("\nPalavra em Portugues ja CADASTRADA!\n");
		
		Sleep(200);
		printf("\nPalavra em Portugues: ");
		
		getStr(RegDic.Portugues);
	}
	fclose(PtrDic);
}

void RelDicionario(char NomeArq[])
{
	TpDicionario RegDic;
	FILE *PtrDic = fopen(NomeArq,"rb");
	clear();
	printf("\n# # #  CONTEUDO DO DICIONARIO  # # #\n\n");
	printf("--------------------------------------------------------------------");
	printf("\nPortugues \t\t Ingles \t Pontos \t Significado");
	printf("\n--------------------------------------------------------------------");
	fread(&RegDic,sizeof(TpDicionario),1,PtrDic);
	if (feof(PtrDic))
	   printf("\nNao ha Palavras Cadastradas!\n");
    else
    {
		while (!feof(PtrDic))
		{
			printf("\n%s \t\t %s \t %d \t %s",RegDic.Portugues,RegDic.Ingles,RegDic.Pontos,RegDic.Significado);
			Sleep(50);
			
			fread(&RegDic,sizeof(TpDicionario),1,PtrDic);
		}    
    }
    printf("\n--------------------------------------------------------------------");
	printf("\n# # #  Fim do Dicionario  # # #\n");
	getch();
	fclose(PtrDic);
}

void ConsDicionario(char NomeArq[])
{
	TpDicionario RegDic;
	int pos;
	FILE *PtrDic = fopen(NomeArq,"rb");
	clear();
	printf("\n\n# # #  Consultar Palavras no Dicionario  # # #\n");
	printf("Palavra em Portugues: ");
	
	getStr(RegDic.Portugues);
	while (strcmp(RegDic.Portugues,"\0")!=0)
	{
		pos = BuscaPalavra(PtrDic,RegDic.Portugues);
		if (pos==-1)          //Não encontramos a Palavra
		    printf("\nPalavra NAO encontrada!\n");
		else {
			   printf("\n*** DADOS ENCONTRADOS  ***\n");
			   fseek(PtrDic,pos,0);     //Posicionar o PtrDic			   
			   fread(&RegDic,sizeof(TpDicionario),1,PtrDic);   //Ler conteúdo ... carregar qtde bytes em RegDic
			   printf("\nPortugues: %s",RegDic.Portugues);
			   printf("\nIngles: %s",RegDic.Ingles);
			   printf("\nPontos: %d",RegDic.Pontos);
			   printf("\nSignificado: %s\n",RegDic.Significado);
		}
		Sleep(200);
		printf("\n\nConsultar a palavra em Portugues: ");
		
		getStr(RegDic.Portugues);
	}
	fclose(PtrDic);
}

void AltDicionario(char NomeArq[])
{
	TpDicionario RegDic;
	int pos;
	FILE *PtrDic = fopen(NomeArq,"rb+");
	clear();
	printf("\n\n# # #  Alterar Palavras no Dicionario  # # #\n");
	printf("Palavra em Portugues: ");
	
	getStr(RegDic.Portugues);
	while (strcmp(RegDic.Portugues,"\0")!=0)
	{
		pos = BuscaPalavra(PtrDic,RegDic.Portugues);

		if (pos==-1)          //Não encontramos a Palavra
		    printf("\nPalavra NAO encontrada!\n");
		else{
            printf("\n*** DADOS ENCONTRADOS  ***\n");
            fseek(PtrDic,pos,0);     //Posicionar o PtrDic			   
            fread(&RegDic,sizeof(TpDicionario),1,PtrDic);   //Ler conteúdo ... carregar qtde bytes em RegDic
            printf("\nPortugues: %s",RegDic.Portugues);
            printf("\nIngles: %s",RegDic.Ingles);
            printf("\nPontos: %d",RegDic.Pontos);
            printf("\nSignificado: %s\n",RegDic.Significado);
            printf("\nDeseja Alterar (S/N)? ");
            if (toupper(getche())=='S')
            {
                printf("\nNOVA Palavra em Ingles: ");
                
                getStr(RegDic.Ingles);
                printf("\nNOVA Pontuação: ");
                scanf("%d",&RegDic.Pontos);
                printf("\nNOVO Significado: ");
                
                getStr(RegDic.Significado);
                fseek(PtrDic,pos,0);
                fwrite(&RegDic,sizeof(TpDicionario),1,PtrDic);
                printf("\n***Registro ALTERADO corretamente!***\n");
            }	
		}
		Sleep(200);
		printf("\n\nConsultar a palavra em Portugues: ");
		
		getStr(RegDic.Portugues);
	}
	fclose(PtrDic);
}
 
void ExclusaoFisicaDicionario(char NomeArq[])
{
	//Exclusao Logica Fazer dps (Definir somente o status como 1) 
	//porem quando utilizar a palavra já, tmb definir como 0  
	TpDicionario RegDic;
	int pos;
	char AuxPalavra[25];
	FILE *PtrDic = fopen(NomeArq,"rb");
	clear();
	printf("\n\n# # #  Excluir Fisicamente Palavras no Dicionario  # # #\n");
	printf("Palavra em Portugues: ");
	
	getStr(AuxPalavra);
	if(strcmp(AuxPalavra,"\0")!=0)
	{
		pos = BuscaPalavra(PtrDic,AuxPalavra);
		if (pos==-1)          //Não encontramos a Palavra
		{
			printf("\nPalavra NAO encontrada!\n");
			getch();
			fclose(PtrDic);
		}
		else{
			printf("\n*** DADOS ENCONTRADOS  ***\n");
			fseek(PtrDic,pos,0);     //Posicionar o PtrDic			   
			fread(&RegDic,sizeof(TpDicionario),1,PtrDic);   //Ler conteúdo ... carregar qtde bytes em RegDic
			printf("\nPortugues: %s",RegDic.Portugues);
			printf("\nIngles: %s",RegDic.Ingles);
			printf("\nPontos: %d",RegDic.Pontos);
			printf("\nSignificado: %s\n",RegDic.Significado);
			printf("\nConfirma Exclusao (S/N)? ");
			if (toupper(getche())=='S')
			{
				FILE *PtrTemp  = fopen("Temp.dat","wb");
				rewind(PtrDic); 		//fseek(PtrTemp,0,0);
				fread(&RegDic,sizeof(TpDicionario),1,PtrDic);
				while (!feof(PtrDic))
				{
					//Grava todo o arq anterior, menos a palavra a ser excluida
					if(strcmp(AuxPalavra,RegDic.Portugues) != 0)
						fwrite(&RegDic,sizeof(TpDicionario),1,PtrTemp);
					fread(&RegDic,sizeof(TpDicionario),1,PtrDic);
				}
				fclose(PtrDic);
				fclose(PtrTemp);
				remove(NomeArq);
				rename("Temp.dat",NomeArq);
				printf("\n\nRegistro Deletado corretamente!\n\n");
				getch();
			}
			else{
				printf("Exclusao Cancelada! ");
				fclose(PtrDic);
			}	
		}
		Sleep(200);
	}
	else
		fclose(PtrDic);
	
}

void ConsultPalavraIngles(char NomeArq[]){
	TpDicionario RegDic;
	int pos;
	FILE *PtrDic = fopen(NomeArq,"rb");
	clear();
	printf("\n\n# # #  Consultar Palavras no Dicionario  # # #\n");
	printf("Palavra em Ingles: ");
	
	getStr(RegDic.Ingles);
	while (strcmp(RegDic.Ingles,"\0")!=0)
	{
		pos = BBIngles(PtrDic,RegDic.Ingles); //BinarySearch
		if (pos==-1)          //Não encontramos a Palavra
		    printf("\nPalavra NAO encontrada!\n");
		else {
			   printf("\n*** DADOS ENCONTRADOS  ***\n");
			   fseek(PtrDic,pos,0);     //Posicionar o PtrDic			   
			   fread(&RegDic,sizeof(TpDicionario),1,PtrDic);   //Ler conteúdo ... carregar qtde bytes em RegDic
			   printf("\nPortugues: %s",RegDic.Portugues);
			   printf("\nIngles: %s",RegDic.Ingles);
			   printf("\nPontos: %d",RegDic.Pontos);
			   printf("\nSignificado: %s\n",RegDic.Significado);
		}
		Sleep(200);
		printf("\n\nConsultar a palavra em Ingles: ");
		
		getStr(RegDic.Ingles);
	}
	fclose(PtrDic);
}

//Jogador
int BBLogin(FILE *PtrPlay, char Login[]){ //Busca Binaria
	TpPlayer Reg;
	fseek(PtrPlay,0,2);
	int I = 0, M, F = ftell(PtrPlay) / sizeof(TpPlayer) - 1;
	M = F/2;
	
	fseek(PtrPlay,M*sizeof(TpPlayer),0);
	fread(&Reg,sizeof(TpPlayer),1,PtrPlay);
	
	while(I < F && stricmp(Reg.login,Login) != 0){
		
		if(stricmp(Login,Reg.login) > 0)
			I = M + 1;
		else
			F = M - 1;
		
		M = (I+F)/2;
		fseek(PtrPlay,M*sizeof(TpPlayer),0);
		fread(&Reg,sizeof(TpPlayer),1,PtrPlay);
	}
	if(stricmp(Reg.login,Login) == 0){
		return M * sizeof(TpPlayer);
	}
	else
		return -1;
}

int IndexLogin(FILE *ptrPlay,char login[]){ //Busca Sequencial Indexada
    TpPlayer R;
    rewind(ptrPlay);

    fread(&R,sizeof(TpPlayer),1,ptrPlay);
    while(!feof(ptrPlay) && stricmp(login,R.login) > 0)
        fread(&R,sizeof(TpPlayer),1,ptrPlay);

    //Retorna a posicao na onde achou a palavra
    if(!feof(ptrPlay) && stricmp(login,R.login) == 0)
        return ftell(ptrPlay) - sizeof(TpPlayer);
    else
        return -1;
}

int ExhaustiveLogin(FILE *PtrPlay, char Login[25]){//Busca Exaustiva
	TpPlayer R;
	rewind(PtrPlay);
	fread(&R,sizeof(TpPlayer),1,PtrPlay);
	while(!feof(PtrPlay) && stricmp(Login,R.login)!=0)
			fread(&R,sizeof(TpPlayer),1,PtrPlay);
	
	if(!feof(PtrPlay))
      return ftell(PtrPlay) - sizeof(TpPlayer);
    else
      return -1;
}

void OrdenaJogadoresNome(char NomeArq[]){ //Bubble Sort //Fazer um selection sort...

	int i, j, QtdeReg;
	TpPlayer RegI, RegJ;
	FILE *PtrDic = fopen(NomeArq,"rb+");
	clear();
	gotoxy(40,10);
	printf("Ordenando Jogadores...");
	fseek(PtrDic,0,2);

	QtdeReg = ftell(PtrDic) / sizeof(TpPlayer);

	for (i = 0; i < QtdeReg-1; i++)
		for(j = i+1; j<QtdeReg; j++)
		{	
			fseek(PtrDic,i*sizeof(TpPlayer),0);
			fread(&RegI,sizeof(TpPlayer),1,PtrDic);

			fseek(PtrDic,j*sizeof(TpPlayer),0);
			fread(&RegJ,sizeof(TpPlayer),1,PtrDic);

			if(stricmp(RegI.Nome,RegJ.Nome) > 0)
			{
				fseek(PtrDic,i*sizeof(TpPlayer),0);
				fwrite(&RegJ,sizeof(TpPlayer),1,PtrDic);
				fseek(PtrDic,j*sizeof(TpPlayer),0);
				fwrite(&RegI,sizeof(TpPlayer),1,PtrDic);
			}
		}
	gotoxy(40,12);printf("Jogadores Ordenados :) ");
	getch();
	fclose(PtrDic);
}

void insDireta(FILE *PtrPlay){ //Insertion Sort

	TpPlayer RegA, RegB;
	
	fseek(PtrPlay,0,2);

	int QtdeReg = ftell(PtrPlay) / sizeof(TpPlayer);
	
	fseek(PtrPlay,(QtdeReg-2)*sizeof(TpPlayer),0);
	fread(&RegB,sizeof(TpPlayer),1,PtrPlay);	
	fread(&RegA,sizeof(TpPlayer),1,PtrPlay);
	

	while (QtdeReg > 1 && stricmp(RegA.login,RegB.login) < 0){
		fseek(PtrPlay,(QtdeReg-2)*sizeof(TpPlayer),0);

		fwrite(&RegA,sizeof(TpPlayer),1,PtrPlay);
		fwrite(&RegB,sizeof(TpPlayer),1,PtrPlay);

		QtdeReg--;
		if(QtdeReg > 1){
			fseek(PtrPlay,(QtdeReg-2)*sizeof(TpPlayer),0);
			fread(&RegB,sizeof(TpPlayer),1,PtrPlay);	
			fread(&RegA,sizeof(TpPlayer),1,PtrPlay);
		}
	}
}

void CadJogador(char NomeArq[]){
	TpPlayer Player;
	FILE *PtrPlay = fopen(NomeArq,"rb+");

	clear();
	printf("\n\n# # # Cadastrar Nome do Jogador # # #\n");
	printf("Digite o seu Login: ");
	
	getStr(Player.login);
	while (strcmp(Player.login,"\0") != 0){
		if(BBLogin(PtrPlay,Player.login) == -1){
			printf("Digite o seu Nome: ");
			
			getStr(Player.Nome);
			Player.Pontos = 0;
			getData(Player.DataEntrada);
			getData(Player.DataUltAcesso);

			fseek(PtrPlay,0,2);
			fwrite(&Player,sizeof(TpPlayer),1,PtrPlay);
			insDireta(PtrPlay);	
			printf("\n\n*** Cadastro Realizado com Sucesso ***");
		}
		else
			printf("\nLogin ja Cadastrado!\n");
		
		Sleep(1000);
		clear();
		printf("\nDigite o login: ");
		
		getStr(Player.login);
	}
	
	fclose(PtrPlay);
}

void RelJogador(char NomeArq[])
{
	TpPlayer RegPlay;
	FILE *PtrPlay = fopen(NomeArq,"rb");
	clear();
	printf("\n# # #  Jogadores Cadastrados  # # #\n\n");
	printf("-------------------------------------------------------------------------------------------");
	printf("\nLogin \t\t Nome \t\t Pontos \t\t DataEntrada \t Data Ultimo Acesso");
	printf("\n-------------------------------------------------------------------------------------------");
	
	fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
	if (feof(PtrPlay)){  
		printf("\nNao ha Jogadores Cadastrados!\n");
	}
    else
    {
		while (!feof(PtrPlay))
		{
			printf("\n%s \t\t %s \t\t %d \t\t %s \t\t %s",RegPlay.login,RegPlay.Nome,RegPlay.Pontos,RegPlay.DataEntrada,RegPlay.DataUltAcesso);
			Sleep(50);
			
			fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
		}    
    }
    printf("\n-------------------------------------------------------------------------------------------");
	printf("\n# # #  Fim do Dicionario  # # #\n");
	getch();
	fclose(PtrPlay);
}

void ConsultJogador(char NomeArq[]){
	TpPlayer RegPlay;
	int pos;
	FILE *PtrPlay = fopen(NomeArq,"rb");
	clear();
	printf("\n\n# # # Consultar Jogadores # # #\n");
	printf("Digite o Login do Jogador: ");
	getStr(RegPlay.login);
	while (strcmp(RegPlay.login,"\0") != 0){
		pos = IndexLogin(PtrPlay,RegPlay.login);
		if(pos == -1){
			printf("Login nao encontrado :(");
			Sleep(750);
		}
		else{
			printf("\n*** DADOS ENCONTRADOS ***\n");
			fseek(PtrPlay,pos,0); 
			fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
			printf("\nLogin: %s",RegPlay.login);
			printf("\nNome: %s",RegPlay.Nome);
			printf("\nPontos: %d",RegPlay.Pontos);
			printf("\nData de Entrada: %s",RegPlay.DataEntrada);
			printf("\nData de Ultimo Acesso: %s",RegPlay.DataUltAcesso);
			getch();
		}
		printf("\n\nConsultar Login: "); //Colocar um gotoxy(56,24); printf("[Enter] sai da Consulta");
		getStr(RegPlay.login);
	}
	fclose(PtrPlay);
}

void AlteraJogador(char NomeArq[]){
	TpPlayer RegPlay;
	int pos;
	FILE *PtrPlay = fopen(NomeArq,"rb+");
	clear();
	printf("\n\n # # # Alterar Jogador # # #\n");
	printf("Digite o Login: ");
	getStr(RegPlay.login);

	while (strcmp(RegPlay.login,"\0") != 0){
		pos = ExhaustiveLogin(PtrPlay,RegPlay.login);
		if(pos == -1)
			printf("\nLogin nao encontrada!\n");
		else{
			printf("\n*** DADOS ENCONTRADOS ***\n");
			fseek(PtrPlay,pos,0);
			fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
			printf("\nLogin: %s",RegPlay.login);
			printf("\nNome: %s",RegPlay.Nome);
			printf("\nPontos: %d",RegPlay.Pontos);
			printf("\nData de Entrada: %s",RegPlay.DataEntrada);
			printf("\nData de Ultima Acesso: %s",RegPlay.DataUltAcesso);
			getch();
		}
		printf("\n\nConsultar Login: "); //Colocar um gotoxy(56,24); printf("[Enter] sai da Consulta");
		getStr(RegPlay.login);
	}
	fclose(PtrPlay);
}

void ExclusaoFisicaJogadores(char NomeArq[]){
	TpPlayer RegPlay;
	int pos;
	char auxLogin[25];
	FILE *PtrPlay = fopen(NomeArq, "rb");
	clear();
	printf("\n\n # # # Excluir Fisicamente Jogadores # # #\n");
	printf("Digite o Login: ");
	getStr(auxLogin);
	if(strcmp(auxLogin,"\0")!=0){
		pos = BBLogin(PtrPlay,auxLogin);
		if(pos == -1){
			printf("\nNenhum jogador encontrado com esse login!\n");
			getch();
			fclose(PtrPlay);
		}
		else{
			printf("\n*** DADOS ENCONTRADOS ***\n");
			fseek(PtrPlay,pos,0);
			fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
			printf("\nLogin: %s",RegPlay.login);
			printf("\nNome: %s",RegPlay.Nome);
			printf("\nPontos: %d",RegPlay.Pontos);
			printf("\nData de Entrada: %s",RegPlay.DataEntrada);
			printf("\nData de Ultima Acesso: %s",RegPlay.DataUltAcesso);
			printf("\n\n Confirma Exclusao(S/N)?\n>_");
			if (toupper(getche())=='S'){
				FILE *PtrTemp = fopen("Temp.dat","wb");
				rewind(PtrPlay);
				fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
				while (!feof(PtrPlay))
				{
					if(stricmp(auxLogin,RegPlay.login) != 0)
						fwrite(&RegPlay,sizeof(TpPlayer),1,PtrTemp);
					fread(&RegPlay,sizeof(TpPlayer),1,PtrPlay);
				}
				fclose(PtrPlay);
				fclose(PtrTemp);
				remove(NomeArq);
				rename("Temp.dat",NomeArq);
				printf("\n\nRegistro de Jogador Deletado corretamente!\n\n");
				getch();
			}
			else{
				printf("Exclusao Cancelada!");
				fclose(PtrPlay);
			}
		}
		Sleep(750);
	}
	else
		fclose(PtrPlay);
}


int BuscaPalavraPlay(char NomeArq1[],char NomeArq2[], char Login[],char Palavra[]){
	TpPlayer Play;
	TpDicionario Dic;
	FILE *PtrPlay = fopen(NomeArq1,"rb");
	FILE *PtrDic = fopen(NomeArq1,"rb");
	rewind(PtrPlay);
	rewind(PtrDic);
	fread(&Play,sizeof(TpPlayer),1,PtrPlay);
	fread(&Dic,sizeof(TpDicionario),1,PtrDic);
	while(!feof(PtrPlay) && stricmp(Login,Play.login)!=0)
		fread(&Play,sizeof(TpPlayer),1,PtrPlay);

	while(!feof(PtrDic) && stricmp(Palavra,Dic.Portugues)!=0)
		fread(&Dic,sizeof(TpDicionario),1,PtrDic);

	if(!(feof(PtrPlay) && feof(PtrDic))){
		fclose(PtrPlay); fclose(PtrDic);
		return 1;
	}
      
    else{
		fclose(PtrPlay); fclose(PtrDic);
		return -1;
	}
      
}
int ValidaJogo(FILE *arq, TpJogo elem){
    TpJogo reg;
    while(!feof(arq) & !(stricmp(reg.login,reg.Palavra) == 0 && stricmp(reg.Palavra,elem.Palavra) == 0))
        fread(&reg,sizeof(TpJogo),1,arq);

    if(!feof(arq))
        return 1;
    return -1;
}
void sortPalavra(char NomeArq[]){
	TpDicionario Dic;
	FILE *PtrDic = fopen(NomeArq,"rb");
	fseek(PtrDic,0,2);
	
	int qtdReg = ftell(PtrDic) / sizeof(TpDicionario);
	int pos = (rand()%qtdReg+1) * sizeof(TpDicionario);

	fclose(PtrDic);
}
void JogoPortugues(char NomeArq[]){

	
	/*
		Não pode repetir a palavra no Jogador
		Cada Player vai ter seu Banco de Palavras usadas
		while(!feof(Ptr) && !(stricmp(Jogador,Reg.Jogador) == 0 && stricmp(Palavra,RegJogo.Palavra) == 0))
		{
			if()
		}

		COlocar uns gotoxy referenciando PT-BR e USA
	
	*/
}

void MolduraMenorPlayers()
{
	int i,j;
	
	//bordasMenuSup
	for(j=24; j<61; j++)
	{
		gotoxy(j,8);printf("%c",205);	
	}
	
	//bordasMenuInf
	for(i=24;i<61;i++)
	{
		gotoxy(i,16);printf("%c", 205);
	}
	
	
	//lateral
	for(i=8;i<16;i++)	
	{
		gotoxy(23,i); printf("%c",186);
		gotoxy(60,i); printf("%c",186);
	}
	gotoxy(23,8); printf("%c",201);
	gotoxy(60,8); printf("%c",187);
	gotoxy(23,16); printf("%c", 200);
	gotoxy(60,16); printf("%c",188);
}

char MenuPlayers(){
	clear();
	textcolor(LYELLOW);
	MolduraMenorPlayers();
	textcolor(BLUE);
	gotoxy(23,7); ;printf("----------// MENU Players //----------");
    textcolor(LGREEN);
    gotoxy(25,9); printf("[A] - Cadastro Jogador");
    gotoxy(25,10); printf("[B] - Relatorio Jogador");
    gotoxy(25,11); printf("[C] - Consultar Jogador");
    gotoxy(25,12); printf("[D] - Alterar Jogador");
    gotoxy(25,13); printf("[E] - Excluir Jogador");
    gotoxy(25,14); printf("[F] - Ordenar Jogadores pelo Nome");
    gotoxy(25,15); printf("[Esc] - Voltar");
 	textcolor(LWHITE);
	gotoxy(29,18); printf("Qual opcao voce deseja ?");
	gotoxy(30,19); printf("> ");

	fflush(stdin);
	char opc = toupper(getche());

	return opc;
}

void selPlayers(char NomeArq[]){
	char opc;
	do{
		opc = MenuPlayers();
		switch(opc)
			{
				case 'A':
					CadJogador(NomeArq);
					break;
				case 'B':
					RelJogador(NomeArq);
					break;
				case 'C':
					ConsultJogador(NomeArq);
					break;
				case 'D':
					AlteraJogador(NomeArq);
					break;
				case 'E':
					ExclusaoFisicaJogadores(NomeArq);
					break;
				case 'X':
					//Ordena Jogadores pelo Nome
					OrdenaJogadoresNome(NomeArq);
					break;
				case 27:
					execute();
					break;
				default:
					break;
			}
	}while(opc != 27);
	
}

char MenuDicionario(){
	printf("***DICIONARIO:");
	printf("[A] Inserir Registro no Dicionario");
	printf("[B] Relatorio de Dicionario");
	printf("[C] Consultar Dicionario pela palavra em Portugues");
	printf("[D] Alterar dados de Dicionario");
	printf("[E] Excluir Registro de Dicionario");
	printf("[F] Ordenar Dicionario");
	printf("[G] Consulta Palavra em Ingles");
	fflush(stdin);
	char opc = toupper(getche());

	return opc;
}

void selDicionario(char NomeArq[]){
	char opc;
	do{
		opc = MenuDicionario();
		switch(opc)
			{
				case 'A':
					CadDicionario(NomeArq);
					break;
				case 'B':
					RelDicionario(NomeArq);
					break;
				case 'C':
					ConsDicionario(NomeArq);
					break;
				case 'D':
					AltDicionario(NomeArq);
					break;
				case 'E':
					ExclusaoFisicaDicionario(NomeArq);
					break;
				case 'F':
					//Ordena Jogadores pelo Nome
					OrdenaDicionarioIngles(NomeArq);
					break;
				case 'G':
					OrdenaDicionarioIngles(NomeArq);
					ConsultPalavraIngles(NomeArq);
					break;
				case 27:
					execute();
					break;	
				default:
					break;
			}
	}while(opc != 27);
	
}

char MenuJogoPalavras(){
	printf("***JOGO:");
	printf("[A] Jogar em Portugues");
	printf("[B] Jogar em Ingles");
	printf("[C] Consultar Palavras que sairam para determinado Jogador");
	printf("[D] Alterar Jogos");
	printf("[E] Excluir Jogos");

	fflush(stdin);
	char opc = toupper(getche());

	return opc;
}

void selJogo(char NomeArq[]){
	char opc;
	do{
		opc = MenuJogoPalavras();
		switch(opc)
			{
				case 'A':
					JogoPortugues(NomeArq);
					break;
				case 'B':
					//JogoIngles(NomeArq);
					break;
				case 'C':
					//ConsultPalavraPlayer(NomeArq);
					break;
				case 'D':
					//AlteraGame(NomeArq);
					break;
				case 'E':
					//ExcluiGame(NomeArq);
					break;	
				default:
					break;
			}
	}while(opc != 27);
	
}

char MenuRelatorios(){
	printf("***RELATORIOS ESPECIFICOS:");
	printf("[A] Ranking Geral de Jogadores");
	printf("[B] Jogadores que iniciam com uma determinada letra");
	printf("[C] Ranking de Palavras que mais foram resolvidas em Portugues");
	printf("[D] Geral: Jogadores e palavras que sairam");
	printf("[E] Geral Ordenado: Palavras e os Jogadores que as jogaram");

	fflush(stdin);
	char opc = toupper(getche());

	return opc;
}

void selRelat(){
	char opc;
	do{
		opc = MenuRelatorios();
		switch(opc)
			{
				case 'A':
					//RankingGeral();
					break;
				case 'B':
					//PlayerStartWithLetter("Archives\\Jogo.dat");
					break;
				case 'C':
					//RankingDasPalavrasPT("Archives\\Jogo.dat");
					break;
				case 'D':
					//RankGeralPlayersWords("Archives\\Jogo.dat");
					break;
				case 'E':
					//RankGeralOrdenado("Archives\\Jogo.dat");
					break;	
				default:
					break;
			}
	}while(opc != 27);
	
}

char Menu(void){

	clear();
 	Moldura(PURPLE);

	textcolor(LYELLOW);
	printf("|1| MANIPULAR JOGADORES");

	textcolor(LBLUE);
	printf("|2| MANIPULAR DICIONARIO");

	textcolor(LGREEN);
	printf("|3| JOGO DAS PALAVRAS");
	
	textcolor(LILAC);
	printf("|4| RELATORIOS ESPECIFICOS");
	
	textcolor(RED);
	printf("[Esc] Sair do Programa");

	textcolor(LWHITE);
	printf("Qual a opcao desejada? ");

	fflush(stdin);
	return toupper(getche());
}

void VerifArquivos(void)
{
	FILE *PtrArq;
	PtrArq = fopen("Archives\\Dicionario.dat","ab");
	fclose(PtrArq);
	PtrArq = fopen("Archives\\Player.dat","ab");
	fclose(PtrArq);
	PtrArq = fopen("Archives\\Jogo.dat","ab");
	fclose(PtrArq);
}


void execute(){
	char op;
	VerifArquivos(); //Verifica a existência dos Arquivos

	do
	{
		op = Menu();
		switch(op)
		{
			//Jogadores
			case '1':
				selPlayers("Archives\\Player.dat");
				break;
			case '2':
				selDicionario("Archives\\Dicionario.dat");
				break;
			case '3':
				selJogo("Archives\\Jogo.dat");
				break;
			case '4':
				selRelat();
				break;
			default:
				break;
		}
	}while(op!=27);
}

int main(void)
{
	//execute();
	//MenuPlayers();
	TpDicionario Dic;
	FILE *PtrDic = fopen("Archives\\Dicionario.dat","rb");
	fseek(PtrDic,0,2);
	
	int qtdReg = ftell(PtrDic) / sizeof(TpDicionario);
	int pos = ((rand()%qtdReg+1)-1) * sizeof(TpDicionario);

	clear();
	printf("size(Dic): %d",sizeof(TpDicionario));
	printf("\nQtd Registros: %d",qtdReg);
	printf("\nPos rand Word: %d",pos);
	
	
	fclose(PtrDic);
	getch();
	return 0;
}
