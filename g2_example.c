#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <g2.h>
#include <g2_X11.h>

		/* Função: contagem_especie
			Variaveis: id_especie (identifica a especie), tamanho (tamanho das colonias ou individuo), contagem de especie1 (numero de individuos da especie 1), contagem de especie2 (numero de individuos da especie 2)
			Esta funcao ira ser utilizada para contabilizar o total de individuos de cada especie */
void contagem_especies (int id_especie, int tamanho, int *contagem_especie1, int *contagem_especie2)
{
	if (id_especie == 1)
	*contagem_especie1 = *contagem_especie1 + tamanho;
	else
	*contagem_especie2 = *contagem_especie2 + tamanho;
}

		/* Função: abrirg2
			Variaveis: d (corresponde ao virtual device)
			Esta funcao ira ser utilizada para abrir a janela g2 com as dimensoes 800*600 e para abrir o virtual device. Vai também juntar a memoria fisica à virtual e vai definir o tamanho da fonte de letra e um "refresh" automatico */
void abrirg2 (int *d)
{
   int dev;
   *d = g2_open_vd ();
   dev = g2_open_X11(800, 600);
   g2_attach(*d, dev);
   g2_clear(*d);
   g2_set_font_size(*d,15);

   g2_set_auto_flush(*d,1); 
}

		/* Função: fecharg2
			Variaveis: d (corresponde ao virtual device)
			Esta funcao ira ser utilizada para fechar a janela g2 e o virtual device */

void fecharg2 (int d)
{
   
   g2_flush(d);
   g2_close(d);
}

		/* Função: data_grafico
			Variaveis: dia (corresponde ao valor do dia), mes (corresponde ao valor do mes), ano (corresponde ao valor do ano)
			Esta funcao ira ser utilizada para escrever a data na janela g2 */

void data_grafico (int dia, int mes, int ano, int d)
{      
   char escreve_data[10];
   g2_clear(d);														/*Limpa a janela*/
   g2_pen(d, 1);														/*Escolhe a cor preta*/
   sprintf(escreve_data, "%d/%d/%d", dia, mes, ano);		/*Escreve numa string a data*/
   g2_string(d, 80, 550, escreve_data);						/*Escreve graficamente a string com a data nas coordenadas indicadas*/
}

		/* Função: area_grafico
			Variaveis: xmin (corresponde ao menor valor de x), xmax (maior valor de x), ymin (menor valor de y), ymax(maior valor de y)
			Esta funcao ira ser utilizada para colocar o rectangulo e os valores da area na janela g2 */

void area_grafico (int xmin, int xmax, int ymin, int ymax, int d)
{
   char escreve_xmin[10], escreve_xmax[10], escreve_ymin[10], escreve_ymax[10];

   g2_pen(d, 1);
   g2_rectangle (d , 80, 80, 720, 520);		/*Coloca o rectangulo nas coordenadas indicadas*/
   
   sprintf(escreve_xmin, "%d", xmin);			/*Escreve o xmin, xmax, ymin, ymax em strings e coloca essas strings na janela j2 */
   g2_string(d, 80, 65, escreve_xmin);			/*nas coordenadas indicadas*/
   sprintf(escreve_ymin, "%d", ymin);
   g2_string(d, 70, 80, escreve_ymin);
   sprintf(escreve_xmax, "%d", xmax);
   g2_string(d, 720, 65, escreve_xmax);
   sprintf(escreve_ymax, "%d", ymax);
   g2_string(d, 60, 520, escreve_ymax);
}
		/* Função: converte_x e converte_y
			Variaveis: x (valor da coordenada que queremos converter), xmin (menor valor de x), xmax (maior valor de x), y (valor da coordenada que queremos converter), ymin (menor valor de y), ymax (maior valor de y)
			Esta funcao ira ser utilizada para relacionar as coordenadas que o utilizador coloca nas mensagens com a area e com as dimensoes do rectangulo */

int converte_x(float x, float xmin, float xmax)
{
   int p;
   p = 80 + (x-xmin) *(640)/(xmax-xmin);
   return p;
} 

int converte_y(float y, float ymin, float ymax)
{
   int p;
   p = 80 + (y-ymin) *(440)/(ymax-ymin);
   return p;
}

		/* Função: Escolha_cor
			Variaveis: id_especie(identifica a especie)
			Esta funcao ira ser utilizada para associar as especies 1 e 8 as cores 4(azul) e 6(verde)*/

int escolha_cor( int id_especie)
{
   int cor;

	if(id_especie==1)
		cor=4;
	else
		cor = 6;

   return cor;
}

		/* Função: individuo_grafico
			Variaveis: id_especie, id_individuo (identifica o individuo), x(coordenada x), y(coordenada y)
			Esta funcao ira ser utilizada para colocar os individuos nas respectivas coordenadas */

void individuo_grafico (int id_especie, int id_individuo, int x, int y, int d)
{
    char escreve_id_individuo[3];
       g2_pen(d, escolha_cor(id_especie));
       sprintf(escreve_id_individuo, "%d", id_individuo);
       g2_string(d, x, y, escreve_id_individuo);
}

		/* Função: colonias_grafico
			Variaveis: id_especie, id_colonia (identifica a colonia), tamanho_colonia (numero de individuos na colonia) x(coordenada x), y(coordenada y)
			Esta funcao ira ser utilizada para colocar as colonias nas respectivas coordenadas */

void colonias_grafico (int id_especie, int id_colonia, int tamanho_colonia, int x, int y, int d)
{ 
   char escreve_id_colonia[3];
   g2_pen(d, escolha_cor(id_especie));
   g2_filled_circle(d, x, y, tamanho_colonia); 			/*O valor correspondente ao raio e o numero de individuos dessa colonia*/
   g2_pen(d, 1);
   sprintf(escreve_id_colonia, "%d", id_colonia);
   g2_string(d, x, y, escreve_id_colonia);
}

		/* Função: troca
			Variaveis: xmin(valor minimo de x), xmax (valor maximo de x)
			Esta funcao ira ser utilizada para trocar os valores de xmin e xmax */

void troca(int *xmin, int*xmax){

   int aux;
   
   aux = *xmax;
   *xmax = *xmin;
   *xmin = aux;

}

		/* Função: enquadramento
			Variaveis: xmax, xmin, ymin, ymax, x, y, palavra (INDIVIDUO ou COLONIA)
			Esta funcao ira ser utilizada para verificar se as coordenadas estão dentro da area estabelecida na mensagem AREA*/

int enquadramento(int xmax, int xmin, int ymin, int ymax, int x, int y, char *palavra){

   if (x>=xmax || x<=xmin || y>=ymax || y<=ymin){
      printf("As coordenadas %s: (%d, %d) não se encontram dentro da AREA establecida.\n", palavra, x, y);
      return 0;
   }
      
   return 1;

}


		/* Função: to_upper
			Variaveis: s (string com os caracteres introduzidos)
			Esta funcao ira ser utilizada para transformar as letras minusculas em maiusculas */

void to_upper(char *s) {

	while (*s != '\0') {
		*s = toupper(*s);
		s++;
	}
}

/*-------------------------------------------------------COLONIA-------------------------------------------------------------------------*/

		/* Função: le_y2
			Variaveis: y2 (coordenada vertical da colonia)
			Esta funcao ira ser utilizada para ler a sexta palavra da mensagem colonia. A função irá verificar se o utilizador introduziu a sexta palavra da mensagem COLONIA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável y2*/

int le_y2(int *y2) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", y2) == 1) {

			if (*y2 >= 0)
				return 1;
			else
				printf("Erro. Y2 '%d' deve ser um valor positivo.\n", *y2);

		}
		else
			printf("Erro. '%s' nao é um dado válido para Y2.\n", palavra);

	}
	else
		printf("Erro. A mensagem COLONIA não tem informação suficiente.\n");


	return 0;
}

		/* Função: le_x2
			Variaveis: x2 (coordenada horizontal da colonia)
			Esta funcao ira ser utilizada para ler a quinta palavra da mensagem colonia. A função irá verificar se o utilizador introduziu a quinta palavra da mensagem COLONIA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável x2 */

int le_x2(int *x2, int *y2) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", x2) == 1) {

			if (*x2 >= 0) {

				if (le_y2(y2))
					return 1;
					
			}
			else
				printf("Erro. X2 '%d' deve ser um valor positivo.\n", *x2);
				
		}
		else
			printf("Erro. '%s' nao é um dado válido para X2.\n", palavra);

	}
	else
		printf("Erro. A mensagem COLONIA não contem informação suficiente.\n");

	return 0;

}

		/* Função: le_tamanho_colonia
			Variaveis: tamanho_colonia (numero de individuos da colonia)
			Esta funcao ira ser utilizada para ler a quarta palavra da mensagem colonia. A função irá verificar se o utilizador introduziu a quarta palavra da mensagem COLONIA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável tamanho_colonia */

int le_tamanho_colonia(int *tamanho_colonia, int *x2, int *y2) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", tamanho_colonia) == 1) {

			if (*tamanho_colonia > 0) {

				if (le_x2(x2, y2))
					return 1;
			}
			else
				printf("Erro. TAMANHO_COLONIA '%d' deve ser um valor maior que zero.\n",*tamanho_colonia);

		}
		else
			printf("Erro. '%s' nao é um dado válido para TAMANHO_COLONIA.\n",	palavra);
	}
	else
		printf("Erro. A mensagem COLONIA não tem informação suficiente.\n");

	return 0;
}

		/* Função: le_id_colonia
			Variaveis: id_colonia (identifica a colonia)
			Esta funcao ira ser utilizada para ler a terceira palavra da mensagem colonia. A função irá verificar se o utilizador introduziu a terceira palavra da mensagem COLONIA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável id_colonia */

int le_id_colonia(int *id_colonia, int *tamanho_colonia, int *x2, int *y2) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", id_colonia) == 1) {

			if (*id_colonia > 0) {

				if (le_tamanho_colonia(tamanho_colonia, x2, y2))
					return 1;
			}

			else
				printf("Erro. ID_COLONIA '%d' deve ser um valor maior que zero.\n",*id_colonia);
				
		} else
			printf("Erro. '%s' nao é um dado válido para ID_COLONIA.\n",palavra);
	}
	else
		printf("Erro. A mensagem COLONIA não tem informação suficiente.\n");

	return 0;
}

		/* Função: mensagem_colonia
			Esta funcao ira ser utilizada para ler toda a mensagem COLONIA utilizando as funcoes anteriores para ler as mensagens dessa palavra, atribuindo os valores as variaveis id_especie2, id_colonia, tamanho_colonia, x2 e y2 */

int mensagem_colonia(int *id_especie2, int *id_colonia, int *tamanho_colonia,	int *x2, int *y2) {

	char *palavra;

	/*Leitura da segunda palavra da mensagem COLONIA*/

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		/*Atribuição da palavra à variável id_especie2*/
		if (sscanf(palavra, "%d", id_especie2) == 1) {

			if (*id_especie2 == 1 || *id_especie2 == 8) {
				if (le_id_colonia(id_colonia, tamanho_colonia, x2, y2))
					return 1;
			}
			else
				printf("Erro. ID_ESPECIE2 '%d' deve ser 1 ou 8.\n",*id_especie2);
		}

		else
			printf("Erro. '%s' nao é um dado válido para ID_ESPECIE2.\n",palavra);
	}
	else
		printf("Erro. A mensagem COLONIA não contem informação suficiente.\n");

	return 0;

}



/*-------------------------------------------------INDIVIDUO-----------------------------------------------------------------------*/


		/* Função: le_y1
			Variaveis: y1 (coordenada vertical)
			Esta funcao ira ser utilizada para ler a quinta palavra da mensagem individuo. A função irá verificar se o utilizador introduziu a quinta palavra da mensagem INDIVIDUO e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável y1 */

int le_y1(int *y1) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", y1) == 1) {

			if (*y1 >= 0)
				return 1;
			else
				printf("Erro. Y1 '%d' deve ser um valor positivo.\n", *y1);

		}
		else
			printf("Erro. '%s' nao é um dado válido para Y1.\n", palavra);

	}
	else
		printf("Erro. A mensagem INDIVIDUO não tem informação suficiente.\n");

	return 0;
}

		/* Função: le_x1
			Variaveis: x1 (coordenada horizontal)
			Esta funcao ira ser utilizada para ler a quarta palavra da mensagem individuo. A função irá verificar se o utilizador introduziu a quarta palavra da mensagem INDIVIDUO e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável x1 */

int le_x1(int *x1, int *y1) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", x1) == 1) {

			if (*x1 >= 0) {
				if (le_y1(y1))
					return 1;
			}
			else
				printf("Erro. X1 '%d' deve ser um valor positivo.\n", *x1);
		}
		else
			printf("Erro. '%s' nao é um dado válido para X1.\n", palavra);
	}
	else
		printf("Erro. A mensagem INDIVIDUO não contem informação suficiente.\n");


	return 0;

}

		/* Função: le_id_individuo
			Variaveis: id_individuo (identificao do individuo)
			Esta funcao ira ser utilizada para ler a terceira palavra da mensagem individuo. A função irá verificar se o utilizador introduziu a terceira palavra da mensagem INDIVIDUO e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável id_individuo */

int le_id_individuo(int *id_individuo, int *x1, int *y1) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", id_individuo) == 1) {

			if (*id_individuo > 0) {

				if (le_x1(x1, y1))
					return 1;
			}

			else
				printf("Erro. ID_INDIVIDUO '%d' deve ser um valor maior que zero.\n",*id_individuo);
		}
		else
			printf("Erro. '%s' nao é um dado válido para ID_INDIVIDUO.\n",palavra);
	}
	else
		printf("Erro. A mensagem INDIVIDUO não tem informação suficiente.\n");

	return 0;
}

		/* Função: mensagem_individuo
			Esta funcao ira ser utilizada para ler toda a mensagem INDIVIDUO utilizando as funcoes anteriores para ler as mensagens dessa palavra, atribuindo os valores as variaveis id_especie1, id_individuo, x1 e y1 */

int mensagem_individuo(int *id_especie1, int *id_individuo, int *x1, int *y1) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {		/*Leitura da segunda palavra da mensagem INDIVIDUO*/

		if (sscanf(palavra, "%d", id_especie1) == 1) {

			if (*id_especie1 == 1 || *id_especie1 == 8) {
				if (le_id_individuo(id_individuo, x1, y1))
					return 1;
			} else
				printf("Erro. ID_ESPECIE1 '%d' deve ser 1 ou 8.\n",*id_especie1);
		}

		else
			printf("Erro. '%s' nao é um dado válido para ID_ESPECIE1.\n", palavra);
	}
	else
		printf("Erro. A mensagem INDIVIDUO não contem informação suficiente.\n");


	return 0;

}

/*-------------------------------------------------------AREA-----------------------------------------------------------------------*/

		/* Função: le_ymax
			Variaveis: ymax (coordenada vertical maxima)
			Esta funcao ira ser utilizada para ler a quinta palavra da mensagem area. A função irá verificar se o utilizador introduziu a quinta palavra da mensagem AREA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável ymax */

int le_ymax(int *ymin, int *ymax) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", ymax) == 1) {

			if (*ymax >= 0) {

				if (*ymin > *ymax)
					troca(ymin, ymax);

				if (*ymin == *ymax)
					printf("Erro. YMIX '%d' e YMAX '%d' devem ser diferentes.\n",*ymin, *ymax);
				else
					return 1;
			}
			else
				printf("Erro. YMAX '%d' deve ser um valor positivo.\n", *ymax);

		}
		else
			printf("Erro. '%s' nao é um dado válido para YMAX.\n", palavra);

	}
	else
		printf("Erro. A mensagem AREA não tem informação suficiente.\n");

	return 0;
}

		/* Função: le_ymin
			Variaveis: ymin (coordenada vertical minima)
			Esta funcao ira ser utilizada para ler a quarta palavra da mensagem area. A função irá verificar se o utilizador introduziu a quarta palavra da mensagem AREA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável ymin */

int le_ymin(int *ymin, int *ymax) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", ymin) == 1) {

			if (*ymin >= 0) {
				if (le_ymax(ymin, ymax))
					return 1;
			}
			else
				printf("Erro. YMIN '%d' deve ser um valor positivo.\n", *ymin);
		}
		else 
			printf("Erro. '%s' nao é um dado válido para YMIN.\n", palavra);
	}
	else
		printf("Erro. A mensagem AREA não contem informação suficiente.\n");


	return 0;

}

		/* Função: le_xmax
			Variaveis: xmax (coordenada horizontal maxima)
			Esta funcao ira ser utilizada para ler a terceira palavra da mensagem area. A função irá verificar se o utilizador introduziu a terceira palavra da mensagem AREA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável xmax */

int le_xmax(int *xmin, int *xmax, int *ymin, int *ymax) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", xmax) == 1) {

			if (*xmax >= 0) {

				if (*xmin > *xmax)
					troca(xmin, xmax);

				if (*xmin == *xmax)
					printf("Erro. XMIX '%d' e XMAX '%d' devem ser diferentes.\n",*xmin, *xmax);
				else {
					if (le_ymin(ymin, ymax))
						return 1;
				}
			}
			else
				printf("Erro. XMAX '%d' deve ser um valor positivo.\n", *xmax);

		}
		else
			printf("Erro. '%s' nao é um dado válido para XMAX.\n", palavra);
	}
	else
		printf("Erro. A mensagem AREA não tem informação suficiente.\n");

	return 0;
}

		/* Função: mensagem_area
			Esta funcao ira ser utilizada para ler toda a mensagem AREA utilizando as funcoes anteriores para ler as mensagens dessa palavra, atribuindo os valores as variaveis xmin, xmax, ymin, ymax */

int mensagem_area(FILE *fp, int *xmin, int *xmax, int *ymin, int *ymax) {

	char *palavra;
	char linha[256];
	

	while (fgets(linha, 255, fp) != NULL ) {
	
      /*Ignora linhas vazias*/
		if (linha[0]=='\n')
		   continue;

		/*Converte caracteres para maisculas*/
		to_upper(linha);

		/*Substitui '\n' por '\0' no último caracter da string*/
		linha[strlen(linha) - 1] = '\0';

		/*Leitura da primeira palavra da string*/
		palavra = strtok(linha, " ");

		if (strcmp(palavra, "AREA")) {
			printf("A seguir à mensagem DATA deve seguir-se uma mensagem AREA.\n Introduza AREA.\n");
			continue;
		}

		/*Leitura da segunda palavra da mensagem AREA*/

		if ((palavra = strtok(NULL, " ")) != NULL ) {

			/*Atribuição da palavra à variável xmin*/
			if (sscanf(palavra, "%d", xmin) == 1) {

				if (*xmin >= 0) {
					if (le_xmax(xmin, xmax, ymin, ymax))
						return 1;
				}
				else
					printf("Erro. XMIN '%d' deve ser um valor positivo.\n",*xmin);
			}
			else
				printf("Erro. '%s' nao é um dado válido para XMIN.\n", palavra);
		}
		else
		   printf("Erro. A mensagem AREA não contem informação suficiente.\n");
	}

	return 0;

}

/*-------------------------------------------------------DATA------------------------------------------------------------------------*/

		/* Função: le_ano
			Variaveis: ano
			Esta funcao ira ser utilizada para ler a quarta palavra da mensagem data. A função irá verificar se o utilizador introduziu a quarta palavra da mensagem DATA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável ano */

int le_ano(int *ano) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", ano) == 1) {

			if (*ano < 2000 || *ano > 2100)			/*Restringe o ano a 2000 e 2100 */
				printf("Erro. O ANO '%d' indicado deve estar compreendido entre 2000 e 2100.\n",*ano);
			else
				return 1;
		}
		else
			printf("Erro. '%s' nao é um dado válido para ANO.\n", palavra);

	}
	else
		printf("Erro. A mensagem DATA não contem informação suficiente.\n");

	return 0;
}

		/* Função: le_mes
			Variaveis: mes
			Esta funcao ira ser utilizada para ler a terceira palavra da mensagem data. A função irá verificar se o utilizador introduziu a terceira palavra da mensagem DATA e se tal acontecer, vai atribuir essa palavra(que deve ser um inteiro) à variável mes */


int le_mes(int dia, int *mes, int *ano) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {

		if (sscanf(palavra, "%d", mes) == 1) {

			if (*mes > 12 || *mes < 1)				/*Restringe o mes a 1 e 12*/
				printf("Erro. O MES '%d' indicado deve estar compreendido entre 1 e 12.\n",*mes);
			else {

				switch (*mes) {
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					if (dia > 31 || dia < 1) {		/*Restringe os dias a 1 e 31 nos meses indicados*/
						printf("Erro. Para o mês '%d' a informação DIA '%d' deve estar compreendida entre 1 e 31.\n",*mes, dia);
						return 0;
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					if (dia > 30 || dia < 1) {		/*Restringe os dias a 1 e 30 nos meses indicados*/
						printf("Erro. Para o mês '%d' a informação DIA '%d' deve estar compreendida entre 1 e 30.\n",*mes, dia);
						return 0;
					}
					break;
				case 2:
					if (dia > 28 || dia < 1) {		/*Restringe os dias a 1 e 28 no mes fevereiro*/
						printf("Erro.  Para o mês '%d' a informação DIA '%d' deve estar compreendida entre 1 e 28.\n",*mes, dia);
						return 0;
					}
					break;
				}

				if (le_ano(ano))
					return 1;

			}
		}
		else
			printf("Erro. '%s' nao é um dado válido para MES.\n", palavra);
	}
	else
		printf("Erro. A mensagem DATA não tem informação suficiente.\n");

	return 0;
}

		/* Função: mensagem_data
			Esta funcao ira ser utilizada para ler toda a mensagem DATA utilizando as funcoes anteriores para ler as mensagens dessa palavra, atribuindo os valores as variaveis dia, mes, ano */

int mensagem_data(int *dia, int *mes, int *ano) {

	char *palavra;

	if ((palavra = strtok(NULL, " ")) != NULL ) {		/*Leitura da segunda palavra da mensagem - DIA*/

		if (sscanf(palavra, "%d", dia) == 1) {
			if (le_mes(*dia, mes, ano))
				return 1;
		}
		else
			printf("Erro. '%s' nao é um dado válido para DIA.\n", palavra);
	}
	else
		printf("Erro. A mensagem DATA não contem informação suficiente.\n");

	return 0;

}


int main(void) {

   /*Declaração de variáveis*/

	char linha[256];
	char *palavra;
	char *mensagens[] = { "DATA", "INDIVIDUO", "COLONIA", "AREA" };
	
	int d;
	int valor = 1, valor2 = 0, i;
	int dia, mes, ano;
	int xmin, xmax, ymin, ymax;
	int id_especie1, id_individuo, x1, y1;
	int id_especie2, id_colonia, tamanho_colonia, x2, y2;
	int contagem_especie1 = 0, contagem_especie2 = 0;

	FILE *fp, *fp2;


/*Abertura do canal*/

/*
   if ( (fp = fopen("./pipe", "r")) == NULL)
   {
      printf("Erro na abertura do canal.\n");
      return 0;
   }
*/
/*Abertura do virtual g2*/
	abrirg2(&d);

/*Abretura do ficheiro estatisticas*/

	fp2 = fopen ("estatisticas.dat", "a");
	if (fp2 == NULL){
		printf("Erro na abertura do ficheiro estatisticas\n");
		return 0;
	}


	/*Leitura da string a partir do canal*/

	while (fgets(linha, 255, fp) != NULL ) {
	
	   /*Ignora linhas vazias*/
		if (linha[0]=='\n')
		   continue;

		/*Converte caracteres para maisculas*/
		to_upper(linha);

		/*Substitui '\n' por '\0' no último caracter da string*/
		linha[strlen(linha) - 1] = '\0';

		/*Leitura da primeira palavra da string*/
		palavra = strtok(linha, " ");
		
		/*Comparação da primeira palavra com a mensagem pretendida*/
		for (i = 0; i < 4; i++)
			if (!strcmp(palavra, mensagens[i]))
				break;
				
		/* Verifica se a primeira mensagem no canal é a mensagem DATA */
		if (valor && i != 0) {
			printf("A primeira mensagem no canal deve ser DATA.\n");
			continue;
		} else
			valor = 0;

		switch (i) {

		/* DATA*/
		case 0:
		   
		
			if (mensagem_data(&dia, &mes, &ano)) {
				printf("Dia %d\n", dia);
				printf("Mes %d\n", mes);
				printf("Ano %d\n\n", ano);
				data_grafico (dia, mes, ano, d);

				/*Escreve no ficheiro estatistcas a contagem das especies respectiva à data anterior*/
				/*Da primeira vez que se regista uma mensagem DATA não executa*/
				if(valor2){
			      fprintf (fp2, "%d %d\n", contagem_especie1, contagem_especie2);
			      contagem_especie1 = 0;
			      contagem_especie2 = 0;
			   }
			   valor2 = 1;
			   fprintf (fp2, "%d %d %d\n", dia, mes, ano);


				/* AREA */
				if (mensagem_area(fp, &xmin, &xmax, &ymin, &ymax)) {
					printf("XMIN %d\n", xmin);
					printf("XMAX %d\n", xmax);
					printf("YMIN %d\n", ymin);
					printf("YMAX %d\n\n", ymax);
					area_grafico (xmin, xmax, ymin, ymax, d);
				}
			}
			break;
			/* INDIVIDUO */
		case 1:
			
			if (mensagem_individuo(&id_especie1, &id_individuo, &x1, &y1)) {
			
			   if(enquadramento(xmax, xmin, ymin, ymax, x1, y1, palavra)){
				   printf("ID_ESPECIE1 %d\n", id_especie1);
				   printf("ID_INDIVIDUO %d\n", id_individuo);
				   printf("X1 %d\n", x1);
				   printf("Y1 %d\n\n", y1);
				   individuo_grafico (id_especie1, id_individuo, converte_x(x1, xmin, xmax), converte_y(y1, ymin, ymax), d);
					contagem_especies (id_especie1, 1, &contagem_especie1, &contagem_especie2);
				}
			}
			break;
			/* COLONIA */
		case 2:
			if (mensagem_colonia(&id_especie2, &id_colonia,&tamanho_colonia, &x2, &y2)) {
				
				if(enquadramento(xmax, xmin, ymin, ymax, x2, y2, palavra)){
				   printf("ID_ESPECIE2 %d\n", id_especie2);
				   printf("ID_COLONIA %d\n", id_colonia);
				   printf("TAMANHO_COLONIA %d\n", id_colonia);
				   printf("X2 %d\n", x2);
				   printf("Y2 %d\n\n", y2);
				   colonias_grafico (id_especie2, id_colonia, tamanho_colonia, converte_x(x2, xmin, xmax), converte_y(y2, ymin, ymax), d);
					contagem_especies (id_especie2, tamanho_colonia, &contagem_especie1, &contagem_especie2);
				}
			}
			break;
		case 3:
			printf("A mensagem AREA deve seguir-se a uma mensagem DATA.\n Introduza DATA.\n");
			break;
		default:
			printf("%s nao é um comando.\n", palavra);
			break;

		} /* switch */

	}/* while */
	
	
	fprintf (fp2, "%d %d\n", contagem_especie1, contagem_especie2);

	printf("Programa terminado.\n");

   /*Fecho do virtual g2*/
	 fecharg2 (d);

   /* Fecho do ficheiro estatisticas */
	fclose(fp2);
	
	/*Fecho do canal*/
	fclose(fp);

	return 0;
}
