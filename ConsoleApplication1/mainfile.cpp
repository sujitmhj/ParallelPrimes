#include <iostream> 
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include <omp.h>  

using namespace std;

const int size = 1000000;
bool *table;
clock_t start;

void generateTable() { 
	table = (bool*)calloc(size, sizeof(bool));
}

void divMethod() { 
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

void tableMethod() { 
	generateTable();

	start = clock(); 

	for (long i = 2; i < (int)(sqrt((double)size)+1); ++i) {
		if (!table[i]) 
		for (long j = 2 * i; j < size; j += i)
			table[j] = true;
	}
	printf("Wykreslanie sekwencyjne:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}

void divMethodFunctional() {
	generateTable();

	start = clock(); 

	#pragma omp parallel for schedule(dynamic, 1) 
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

void tableMethodFunctional() { 
	generateTable();

	start = clock();   

	#pragma omp parallel for schedule(dynamic, 1)
	for (long i = 2; i < (int)(sqrt((double)size)+1); ++i) {
		if (!table[i])       
		for (int j = 2 * i; j < size; j += i)
			table[j] = true;
	}
	printf("Wykreslanie rownolegle:\t%f sekund\n", ((double)(clock() - start) / 1000.0));
}

int main() {
	omp_set_num_threads(omp_get_max_threads());

	divMethod();
	tableMethod();

	divMethodFunctional();
	tableMethodFunctional();
	
	system("PAUSE");   
	return 0;
}