#include "ThreadPool.h"
#include "Integral.h"

int main(int argc, char* argv[])
{
    {
       unsigned int N = 0;

        int number = 100000000;                                                    //N - number of threads(потоки)
        double low_limit, up_limit;                         //low/up_limit - integration limit
        std::stringstream A;

        A << argv[1]<<' '<<argv[2]<<' '<<argv[3];
        A >> N >> low_limit >> up_limit;

        if(argc != 4 || argv[0] <= 0){
            std::cerr<<"Wrong input! Should be like ./ThreadPool Number_of_threads Left_limit Right_limit"<<std::endl;
            return 0;
        }

        ThreadPool pool{N};
        
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
