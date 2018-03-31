#include "ThreadPool.h"
#include "Integral.h"

int main(int argc, char* argv[])
{
    {
        unsigned int N, number_of_threads;

        int number = 100000000;
        double low_limit, up_limit;                         //low/up_limit - integration limit
        std::stringstream A;

        A << argv[1] << ' ' << argv[2] << ' ' << argv[3] << ' ' << argv[4];
        A >> number_of_threads >> N >> low_limit >> up_limit;

        if (argc != 5 || argv[1] <= 0 || argv[2] <= 0) {
            std::cerr << "Wrong input! Should be like ./ThreadPool Number_of_threads Number_of_tasks Left_limit Right_limit" << std::endl;
            return 0;
        }

        double limits_of_integration[N + 1];

        for (int i = 0; i <= N; i++)
            limits_of_integration[i] = low_limit + i * (up_limit - low_limit) / N;

        int step = number / N;

        ThreadPool pool{number_of_threads};

        auto f1 = pool.enqueue([&] {
            double result = integrate(limits_of_integration[0], limits_of_integration[1], step);
            return result;
        });

        decltype(f1) results[N-1];

        for(int i = 0; i<N-1; i++){
            results[i] = pool.enqueue([&, i] {
                double result = integrate(limits_of_integration[i+1], limits_of_integration[i+2], step);
                return result;
            });
        }

        double result = f1.get();

        for(auto &task : results)
            result += task.get();

        std::cout<<result<<std::endl;
    }

    return 0;
}
