#include <iostream>
#include <string>
#include<omp.h>
#include<sys/time.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3) {
        cout << "Usage: " << argv[0] << " <password> [num_threads]" << endl;
        return 1;
    }

    string target = argv[1];
    int num_threads = 4;

    if (argc == 3) {
        num_threads = atoi(argv[2]);
    }

    omp_set_num_threads(num_threads);
    int threads_used = 1;

#pragma omp parallel
    {
#pragma omp single
        threads_used = omp_get_num_threads();
    }

    string guess(4, 'a');
    bool found = false;

    timeval begin, end;
    gettimeofday(&begin, 0);

// Parallelizes all 4 loops together
#pragma omp parallel for collapse(4)
    for(int i = 0; i < 26; i++) { // First letter
        for(int j = 0; j < 26; j++) { // Second Letter
            for(int k = 0; k < 26; k++) { // Third Letter
                for(int l = 0; l < 26; l++) { // Fourth Letter
                    //If another thread already found the password, continue
                    if (found) {
                        continue;
                    }

                    guess[0] = 'a' + i; // if i = 0, then guess[0] = a
                    guess[1] = 'a' + j;
                    guess[2] = 'a' + k;
                    guess[3] = 'a' + l;

                    // Total possible guesses = 26^4 = 456,976

                    // If the guess matches the target
                    if (guess == target) {
                        #pragma omp critical
                        found = true;
                    }
                }
            }
        }
    }

    gettimeofday(&end, 0);
    double elapsed = (end.tv_sec - begin.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - begin.tv_usec) / 1000.0;
    cout << "Threads: " << threads_used << " Time: " << elapsed << "ms " << endl;

    return 0;
}
