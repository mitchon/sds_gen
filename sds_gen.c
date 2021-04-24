#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "elipt_cur.h"
#include "hash_lib.h"

//очистить все переменные, используемые gmp
void Clear_GMP (mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP, mpz_t d, mpz_t xQ, mpz_t yQ)
{
	mpz_clear(p);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(m);
	mpz_clear(q);
	mpz_clear(xP);
	mpz_clear(yP);
	mpz_clear(d);
	mpz_clear(xQ);
	mpz_clear(yQ);
}

//получение параметров цифровой подписи
char GetParams (char* path, mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP)
{
	FILE* params;
	if ((params = fopen(path, "r")) == NULL)
	{
		printf("Error. File \"ds_params.sdsp\" not found. Generate or import parameters file\n");
		return -1;
	}
	mpz_inp_str (p, params, 16);
	mpz_inp_str (a, params, 16);
	mpz_inp_str (b, params, 16);
	mpz_inp_str (m, params, 16);
	mpz_inp_str (q, params, 16);
	mpz_inp_str (xP, params, 16);
	mpz_inp_str (yP, params, 16);
	gmp_printf("p = %Zx\n"
			"a = %Zx\n"
			"b = %Zx\n"
			"m = %Zx\n"
			"q = %Zx\n"
			"xP = %Zx\n"
			"yP = %Zx\n", p, a, b, m, q, xP, yP);
	fclose(params);
	return 0;
}

//генератор ключей (не работает)
int GenerateKeys (mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP, char* login, mpz_t d, mpz_t xQ, mpz_t yQ)
{
	return 0;
	//добавить генератор
}

//открыть целевой файл
int OpenTargetFile(char* path, FILE *file)
{
	if ((file = fopen(path, "r")) == NULL)
	{
		printf("Error reading target file");
		return -1;
	}
	return 0;
}

//добавление ЦП к файлу
int AddDSToFile(char* ds, FILE *file)
{
	return 0;
}

//получение ключей из таблицы пользователя
int GetUserKeys(char* login, mpz_t d, mpz_t xQ, mpz_t yQ)
{
	FILE *keys;
	char loginfound = 0;
	int num = 0;
	char buffer[256];
	if ((keys = fopen("accounts.sdsa", "r")) == NULL)
	{
		printf("Error reading accounts info");
		return -1;
	}
	while (feof(keys) == 0 && loginfound == 0)
	{
		fgets(buffer, 256, keys);
		if (num % 4 == 0)
		{
			for (int i=0; i<256; i++)
			{
				if (buffer[i] == 0x0A)
					buffer[i]=0;
			}
			if (strcmp(buffer, login) == 0)
			{
				printf("login: %s\n", login);
				loginfound = 1;
			}
		}
		num++;
	}
	if (feof(keys) != 0 && loginfound == 0)
	{
		printf("User not found\n");
		return -1;
	}
	
	mpz_init(d);
	mpz_init(xQ);
	mpz_init(yQ);
	
	mpz_inp_str (d, keys, 16);
	mpz_inp_str (xQ, keys, 16);
	mpz_inp_str (yQ, keys, 16);
	gmp_printf("d = %Zx\n"
			"xQ = %Zx\n"
			"yQ = %Zx\n", d, xQ, yQ);
	fclose(keys);
	return 0;
}

//генератор ЦП
int GenerateDS (mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP, mpz_t d, mpz_t xQ, mpz_t yQ, unsigned char *ds)
{
	mpz_t e, alpha, k, xC, yC, r, s, tmp;
	gmp_randstate_t state;
	unsigned char s_str[32], r_str[32];
	unsigned char *h;
	mpz_init(e);
	mpz_init(alpha);
	mpz_init(k);
	mpz_init(xC);
	mpz_init(yC);
	mpz_init(r);
	mpz_init(s);
	mpz_init(tmp);
	gmp_randinit_default(state);

	//получение хеш-кода (пока что из примера стандарта)
	unsigned char msg[]={0xfb,0xe2,0xe5,0xf0,0xee,0xe3,0xc8,0x20,0xfb,0xea,0xfa,0xeb,0xef,0x20,0xff,0xfb,0xf0,0xe1,0xe0,0xf0,0xf5,0x20,0xe0,0xed,0x20,0xe8,0xec,0xe0,0xeb,0xe5,0xf0,0xf2,0xf1,0x20,0xff,0xf0,0xee,0xec,0x20,0xf1,0x20,0xfa,0xf2,0xfe,0xe5,0xe2,0x20,0x2c,0xe8,0xf6,0xf3,0xed,0xe2,0x20,0xe8,0xe6,0xee,0xe1,0xe8,0xf0,0xf2,0xd1,0x20,0x2c,0xe8,0xf0,0xf2,0xe5,0xe2,0x20,0xe5,0xd1};
    long len = sizeof(msg)/sizeof(msg[0]);
	h=hash256(msg, len);
    /*printf("h: ");
    for (int i=0; i<32; i++)
        printf("%x ", h[i]);
	printf("\n");*/
	//получить альфа, число, двоичным представлением которого является h
	mpz_import(alpha, 32, 1, 1, 1, 0, h);
	//gmp_printf("alpha = %Zx\n", alpha);

	//получить е (пока что из примера)
	//mpz_mod(e, alpha, q);
	mpz_set_str(e, "2DFBC1B372D89A1188C09C52E0EEC61FCE52032AB1022E8E67ECE6672B043EE5", 16);
	//генерировать k, если r или s равны 0
	while (mpz_cmp_si(r, 0) == 0 || mpz_cmp_si(s, 0) == 0)
	{
		//получить k (пока что из примера)
		//mpz_urandomb(k, state, 256);
		mpz_set_str(k, "77105C9B20BCD3122823C8CF6FCC7B956DE33814E95B7FE64FED924594DCEAB3", 16);
		//C=k*P
		PointMul(p, a, xP, yP, k, xC, yC);
		//r=xCmodq
		mpz_mod(r, xC, q);
		//для того, чтобы пропустить ненужные шаги, если r=0
		if (mpz_cmp_si(r, 0) != 0)
		{
			//s=(rd+ke)modq
			mpz_mul(tmp, r, d);
			mpz_mul(s, k, e);
			mpz_add(s, s, tmp);
			mpz_mod(s, s, q);
			//для того, чтобы пропустить ненужные шаги, если s=0
			if (mpz_cmp_si(s, 0) != 0)
			{
				//преобразование r и s в строки и получение строки цифровой подписи=(r||s)
				mpz_export(r_str, NULL, 1, 1, 1, 0, r);
				mpz_export(s_str, NULL, 1, 1, 1, 0, s);
				for (int i=0; i<32; i++)
				{
					ds[i]=r_str[i];
					ds[i+32]=s_str[i];
				}
				printf("ds = ");
				for (int i = 0; i<64; i++)
					printf("%x ", ds[i]);
				printf("\n");
			}
		}
	}

	mpz_clear(e);
	mpz_clear(alpha);
	mpz_clear(k);
	mpz_clear(xC);
	mpz_clear(yC);
	mpz_clear(r);
	mpz_clear(s);
	mpz_clear(tmp);
	return 0;
}

int main(int argc, char** argv)
{
	//аргументов нет
	if (argv[1] == NULL)
	{
		printf("Welcome to Simple DS digital signature generator app\n");
		printf("Digital signature algorythm standart is GOST 34.10-2018\n");
		printf("Hash-functions generator standert is GOST 34.11-2018\n");
		printf("Use \"-h\" to open list of options\n");
		return 0;
	}
	//аргумент -h, вывод гайда
	if (strcmp(argv[1], "-h") == 0 && argc == 2)
	{
		printf("-h				Get help;\n");
		printf("-ug <login>			Generate private and public keys for user and add user info to table;\n");
		printf("-ds <login> <path to file>	Generate digital signature for chosen file using current parameters;\n");
		printf("-p				Generate random parameters of digital signature and blank public user info table;\n");
		printf("-p <path to file>		Use signature parameters and table from .sdsp file;\n");
		printf("Note that person verifying your signature should be provided with same signature parameters and your user info shold be contained in his table.\n");
		printf("In case of loosing this data, its backups are stored in ds_params.log file\n");
		printf("Parameters used by default at the start of this program are contained in ds_params.spsd\n");
		return 0;
	}
	//-h с лишними параметрами
	else if (strcmp(argv[1], "-h") == 0 && argc != 2)
	{
		printf("Option \"-h\" does not support any other arguments");
		return 0;
	}

	mpz_t p, a, b, m, q, xP, yP, d, xQ, yQ;
	mpz_init(p);
	mpz_init(a);
	mpz_init(b);
	mpz_init(m);
	mpz_init(q);
	mpz_init(xP);
	mpz_init(yP);
	mpz_init(d);
	mpz_init(xQ);
	mpz_init(yQ);
	//статус открытого файла
	char parfstatus = 0;
	//получение параметров из файла
	parfstatus += GetParams("ds_params.sdsp", p, a, b, m, q, xP, yP);

	//создание пользователя, генерация ключей (пока не работает)
	if (strcmp(argv[1], "-ug") == 0 && argc == 3)
	{
		printf("It doesnt work right now");
		if (parfstatus == 0)
		{
			GenerateKeys(p, a, b, m, q, xP, yP, argv[2], d, xQ, yQ);
		}
		else
			printf("Get the parameters first!");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}
	//неверное кол-во параметров
	else if (strcmp(argv[1], "-ug") == 0 && argc != 3)
	{
		printf("Incorrect number of arguments");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}

	//подписание файла
	if (strcmp(argv[1], "-ds") == 0 && argc == 4)
	{
		if (parfstatus == 0)
		{
			char fail = 0;
			FILE *target;
			//получение ключей из файла пользователя
			fail += GetUserKeys(argv[2], d, xQ, yQ);
			//открытие целевого файла
			fail += OpenTargetFile(argv[3], target);
			if (fail == 0)
			{
				//генерация подписи, добавление к файлу
				unsigned char ds[64];
				GenerateDS(p, a, b, m, q, xP, yP, d, xQ, yQ, ds);
				AddDSToFile(ds, target);
			}
		}
		//если параметры не получены
		else
			printf("Get the parameters first!");
		//очистить все переменные, используемые gmp
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}
	//кол-во параметров неверно
	else if (strcmp(argv[1], "-ds") == 0 && argc != 4)
	{
		printf("Incorrect number of arguments");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}

	//генерация случ. параметров
	if (strcmp(argv[1], "-p") == 0 && argc == 2)
	{
		printf("It doesnt work right now");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}
	//параметры из файла
	else if (strcmp(argv[1], "-p") == 0 && argc == 3)
	{
		printf("It doesnt work right now");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}
	else if (strcmp(argv[1], "-p") == 0 && argc > 3)
	{
		printf("Incorrect number of arguments");
		Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
		return 0;
	}

	//параметр не найден
	printf("No such parameter. Use \"-h\" to open list of options");
	//очистить все переменные, используемые gmp
	Clear_GMP(p, a, b, m, q, xP, yP, d, xQ, yQ);
	return 0;
}