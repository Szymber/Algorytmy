#include <iostream>
#include <fstream>

#include <ctime>
#include <cmath>
#include <iomanip>
#include <climits>
#define MAX 60000l
#define MLD 1000000000.0


/*------------------------------------------------*/
void Wypisz(int* A, int n, bool nowaLinia = true) {
    for (int i = 0; i < n; i++)
        std::cout << *(A + i) << " ";
	if (nowaLinia)
		std::cout << std::endl;
}

void zamien(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int* wczytajPlik(std::ifstream& plik, int &n) {
    n = 0;
    int tmp, i = 0;
    
    while (plik >> tmp)
        ++n;
    
    int *A = new int[n];
    plik.clear();
    plik.seekg(0,plik.beg);
    
    while (plik >> tmp) {
        *(A + i) = tmp;
        ++i;
    }

    return A;
}

void zapiszPlik(std::ofstream& plik, int* A, int n) {
    for (int i = 0; i < n; i++) 
        plik << *(A + i) << std::endl; 
}
/*------------------------------------------------*/

void Heapify(int *A, int i, int heapSize) {
	int largest;
	int l = 2 * i + 1;
    int r = 2 * (i + 1);
	if (l < heapSize && A[l] > A[i])
		largest = l;
	else
		largest = i;

    if (r < heapSize && A[r] > A[largest])
        largest = r;

    if (largest != i) { 
        zamien((A + i), (A + largest));
        Heapify(A, largest, heapSize);
    }
}

void BuildHeap(int* A, int heapSize) {
    for (int i = heapSize / 2; i >= 0; --i)
        Heapify(A, i, heapSize);
}

void HeapSort(int *A, int n) {
    int heapSize = n;
    BuildHeap(A, heapSize);
    for (int i = n - 1; i >= 1; --i) {
        zamien((A + heapSize - 1), A);
        --heapSize;
        Heapify(A, 0, heapSize);
    }
}

/*__________________________________________*/

void Heapify_i(int *A, int i, int heapSize) {
	int l, r, largest = i;
	
    do {
        i = largest;
        l = 2 * i + 1;
        r = 2 * (i + 1);
        
        if (l < heapSize && A[l] > A[i])
            largest = l;

        if (r < heapSize && A[r] > A[largest])
            largest = r;

        zamien((A + i), (A + largest));

    } while (i != largest);
}

void HeapSort_i(int *A, int n) {
    int heapSize = n;
    BuildHeap(A, heapSize);
    for (int i = n - 1; i >= 1; --i) {
        zamien((A + heapSize - 1), A);
        --heapSize;
        Heapify_i(A, 0, heapSize);
    }
}

/*__________________________________________*/

int* utworzTabliceRosnacych(int n) {
    int *A = new int[n];
    for (int i = 0; i < n; i++)
        A[i] = i + 1;
    return A;
}

int* utworzTabliceMalejacych(int n) {
    int *A = new int[n];
    for (int i = 0, j = n; i < n; i++, --j)
        A[i] = j;
    return A;
}

int* utworzTabliceRownych(int n) {
    int *A = new int[n];
    for (int i = 0; i < n; i++)
        A[i] = 1;
    return A;
}

/*__________________________________________*/


void oszacowanieCzasu(int* (*tworzenieTablicy)(int), double (*Fn)(int)) {
    struct timespec tp0, tp1;
    double Tn;
    for (int i = 12; i < 250000; i *= 2) {

        int *A = tworzenieTablicy(i);
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
        HeapSort(A, i);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);
        Tn=(tp1.tv_sec + tp1.tv_nsec / MLD) - (tp0.tv_sec + tp0.tv_nsec / MLD);
        std::cout << "n: " << std::setw(7) << i << "\t" << "czas: " << std::setw(10) << Tn << "\t" << "wsp: " << Fn(i) / Tn << std::endl;
        delete[] A;
    }
}


int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::ifstream wejscie("in.txt");
    std::ofstream wyjscie("out.txt");
    if (!wejscie.is_open()) {
        std::cout << "Nie mozna otworzyc pliku wejsciowego.\n";
        exit(EXIT_FAILURE);
    }
    if (!wyjscie.is_open())  {
        std::cout << "Nie mozna otworzyc pliku wyjsciowego.\n";
        exit(EXIT_FAILURE);
    }

    int n;
    int *A = wczytajPlik(wejscie, n);
	std::cout << "Wejscie: (n = " << n <<") \n\t";
	Wypisz(A, n);
    HeapSort(A, n);
	std::cout << "Wyjscie: \n\t";
    Wypisz(A, n);
    zapiszPlik(wyjscie, A, n);
    delete[] A;
    wejscie.close();
    wyjscie.close();

    //Testowanie złożoności czasowej
    /*
    oszacowanieCzasu(utworzTabliceRownych, [](int n) -> double {return n;});
    std::cout << "--------------------------------------------\n";
    oszacowanieCzasu(utworzTabliceMalejacych, [](int n) -> double {return log(n)*n;});
    std::cout << "--------------------------------------------\n";
    oszacowanieCzasu(utworzTabliceRosnacych, [](int n) -> double {return log(n)*n;});
    */
	return 0;
}
