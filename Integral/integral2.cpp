#include "iostream"
#include "thread"
#include "sstream"
#include "vector"
#include "cmath"

double result = 0;

double function(double x){                                                        //function value calculation
    return x*x*exp(x) - tan(sqrt(x));
}

void integrate(double lower_limit, double upper_limit, int number){               //number - number of integr steps
    double current_result = 0;                                                    //calculating integral
    double step = (upper_limit-lower_limit)/number;
    for(int i = 0; i<number; i++)
        current_result += step * function(lower_limit+(i+1)*step);
    result += current_result;
}


int main(int argc, char* argv[])
{
    int N, number = 100000000;                                                    //N - number of threads(потоки)
    double limits_of_integration[N], low_limit, up_limit;                         //low/up_limit - integration limit
    std::stringstream A;

    A << argv[1]<<' '<<argv[2]<<' '<<argv[3];
    A >> N >> low_limit >> up_limit;

    if(N <= 0 || argc != 4){
        std::cerr<<"Wrong input!"<<std::endl;
        return 0;
    }

    for(int i = 0; i<=N; i++)
        limits_of_integration[i] = low_limit + i*(up_limit - low_limit)/N;

    int step = number/N;
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();                     //this and 56, 58 lines - time calculation

    for(int i = 0; i<N; i++){
        if (i != N-1){
            threads.push_back(std::thread(integrate, limits_of_integration[i], limits_of_integration[i+1], step));
        }
        else{
            threads.push_back(std::thread(integrate, limits_of_integration[i], limits_of_integration[i+1], number - (N-1)*step));
        }
    }

    for(auto &th : threads)
        th.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<result<<std::endl;
    std::cerr << std::chrono::duration<unsigned long int, std::nano>(end - start).count() <<std::endl;

    return 0;
}
