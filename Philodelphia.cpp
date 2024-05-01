#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    const int number = 5;
    
    // Структура представляет собой массив с мьютексом
    struct Forks
    {
    public:
        Forks(){;}
        std::mutex mu;
    };
    
    // Основная функция задачи, позволяющая задерживать вилки или мысли (тут уж как повезёт)

    // Основной принцип работы - в задержке вилок с помощью мьютексов и таймеров
    auto motions = [](Forks &left_fork, Forks& right_fork, int philosopher_number) {
        while (true){
        std::cout << "Philosopher " << philosopher_number << " is thinking" << endl;

        std::chrono::milliseconds timeout1(2000);
        std::this_thread::sleep_for(timeout1);

        std::unique_lock<std::mutex> llock(left_fork.mu);
        std::unique_lock<std::mutex> rlock(right_fork.mu);
        
        cout << "Philosopher " << philosopher_number << " is eating" << endl;
        
        std::chrono::milliseconds timeout2(4500);
        std::this_thread::sleep_for(timeout2);
        
        cout << "Philosopher " << philosopher_number << " has finished eating" << endl;
        }
    };
    
    //Привозим вилочки с Ашинского металлургического
    Forks fork[number];
    
    //Возрождаем философов
    std::thread philosopher[number];
    
    //Философы начинают думать (ничего себе)
    philosopher[0] = std::thread(motions, std::ref(fork[number-1]), std::ref(fork[0]), 1);
    
    for(int i = 1; i < number; ++i) {
        philosopher[i] = std::thread(motions, std::ref(fork[i-1]), std::ref(fork[i]), i+1);
    }
    
    for(auto &ph: philosopher) {
        ph.join();
    }
    return 0;
}