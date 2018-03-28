#include "ThreadPool.h"
#include "Integral.h"

int main(int argc, char* argv[])
{
    {
        unsigned int N = 2;

        ThreadPool pool{N};

        int number = 100000000;                                                    //N - number of threads(потоки)
        double low_limit, up_limit;                         //low/up_limit - integration limit
        std::stringstream A;

        A << argv[1]<<' '<<argv[2];
        A >> low_limit >> up_limit;

        if(argc != 3){
            std::cerr<<"Wrong input! Should be like ./a.out Left_limit Right_limit"<<std::endl;
            return 0;
        }

        double limits_of_integration[N+1];

        for(int i = 0; i<=N; i++)
            limits_of_integration[i] = low_limit + i*(up_limit - low_limit)/N;

        int step = number/N;

        auto f1 = pool.enqueue([&]{
            double result =  integrate(limits_of_integration[0], limits_of_integration[1], step);
            return result;
        });

        auto f2 = pool.enqueue([&]{
            double result = integrate(limits_of_integration[1], limits_of_integration[2], step);
            return result;
        });

        std::cout<<(f1.get() + f2.get())<<std::endl;
    }

    return 0;
}