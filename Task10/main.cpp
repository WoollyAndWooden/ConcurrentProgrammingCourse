#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;

bool isPrime(int);
vector<int> findPrimes(int, int, int);

int main(int argc, char ** argv) {
    int start = 1;
    int upper_bound = stoi(argv[2]); 
    int numThreads = stoi(argv[1]);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<int> primes = findPrimes(start, upper_bound, numThreads);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Found " << primes.size() << " twin primes." << " Shouldn't be more than " << (6.0 / 5.0) * upper_bound / log(upper_bound) << endl;
    cout << "Duration: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;

    for(int i = 0; i < primes.size(); i++)
    {
        cout << primes[i] << endl;
    }

    return 0;
}

bool isPrime(int number) {
    if (number < 2)
        return false;
    for (int i = 2; i <= sqrt(number); ++i) {
        if (number % i == 0)
            return false;
    }
    return true;
}

vector<int> findPrimes(int start, int end, int numThreads) {
    vector<int> primes;

    #pragma omp parallel num_threads(numThreads)
    {
        vector<int> local_primes; // Local vector for each thread

        #pragma omp for nowait
        for (int i = start; i <= end - 2; ++i) {
            if (isPrime(i) && isPrime(i + 2)) {
                local_primes.push_back(i);
                local_primes.push_back(i + 2);
            }
        }

        // Merge local results into the main vector
        #pragma omp critical
        primes.insert(primes.end(), local_primes.begin(), local_primes.end());
    }

    return primes;
}
