#include <iostream> 
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include <omp.h>  

using namespace std;

const int size = 10000;						//zakres oblicze�
bool *table;								//true - liczba z�o�ona, false - liczba pierwsza
clock_t start;								//czas rozpocz�cia wyznaczania

/*	Alokacja pami�ci na tablic�, domy�lnie wype�niona warto�ciami false*/
void generateTable() { 
	table = (bool*)calloc(size, sizeof(bool));
}


/*		METODA DZIELENIA PRZEZ KOLEJNE LICZBY PIERWSZE			*/
/*	Dla ka�dej liczby z zakresu sprawdzamy czy jest parzysta	*/
/*	lub czy jest podzielna bez reszty przez jak�kolwiek inn�	*/
/*	liczb� pierwsz� z zakresu od 3 do pierwiastek z tej liczby.	*/
void divMethodSeq() { 
	generateTable();

	start = clock();   

	for (long i = 3; i < size; ++i)
	if (i % 2 == 0)
		table[i] = true;
	else {
		for (long j = 3; j <= sqrt((double)i) + 1; j += 2)
		if (!table[j] && !(i % j)) {
			table[i] = true;
			break;
		}
	}
	printf("Dzielenie sekwencyjne:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}

/**/
void divMethodParallel() {
	generateTable();

	start = clock();

#pragma omp parallel for schedule(dynamic, 1)
//#pragma omp parallel for schedule(static, size/omp_get_max_threads()) 
	for (long i = 3; i < size; ++i)
	if (i % 2 == 0)
		table[i] = true;
	else
	for (long j = 3; j <= sqrt((double)i) + 1; j += 2)
	if (!table[j] && !(i % j))    {
		table[i] = true;
		break;
	}
	printf("Dzielenie rownolegle:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}
/*	   METODA WYKRE�LANIA (PODZIA� FUNKCJONALNY)	*/
/*	Dla ka�dej niewykre�lonej jeszcze liczby,		*/
/*	z zakresu od 2 do pierwiastka z tej liczby,		*/
/*	wykre�lamy wszystkie jej wielokrotno�ci.		*/
void tableMethodSeq() { 
	generateTable();

	start = clock(); 

	for (long i = 2; i < (int)(sqrt((double)size)+1); ++i) {
		if (!table[i]) 
		for (long j = 2 * i; j < size; j += i)
			table[j] = true;
	}
	printf("Wykreslanie sekwencyjne:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}

void tableMethodParallel() { 
	generateTable();

	start = clock();   

#pragma omp parallel for schedule(dynamic, 1)
//#pragma omp parallel for schedule(static, size/omp_get_max_threads())
	for (long i = 2; i < (int)(sqrt((double)size)+1); ++i) {
		if (!table[i])       
		for (int j = 2 * i; j < size; j += i)
			table[j] = true;
	}
	printf("Wykreslanie rownolegle:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}

/****//****//****//****//****//****//****//****/

int main() {
	omp_set_num_threads(omp_get_max_threads());

	divMethodSeq();
	tableMethodSeq();

	divMethodParallel();
	tableMethodParallel();

	return 0;
}