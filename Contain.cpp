#include <iostream> 
#include <mutex> 
#include <vector>
#include <thread>
#include <stdarg.h>
#include <chrono>

using namespace std;

//Создаём класс потокобезопасного вектора с участием мьютекса
template <typename T>
class container 
{
    std::recursive_mutex lock;
    std::vector<T> nums;
public:
    //Функция, позволяющая нам добавить один элемент в конец вектора
    void push(T element) {
        std::lock_guard<std::recursive_mutex> locker(lock);
        nums.push_back(element);
    }
    //Примерно то же самое, только для нескольких элементов
    void pushmore(int num, ...){
        va_list arguments;
        va_start(arguments, num);
        for (int i = 0; i < num; i++){
        std::lock_guard<std::recursive_mutex> locker(lock);
            push(va_arg(arguments, T));
        }
        va_end(arguments); 
    }
    // Функция для удаления элемента из конца вектора
    T pop() 
    { 
        // Блокируем
        std::lock_guard<std::recursive_mutex> locker(lock); 
        // Получаем элемент
        T item = nums[nums.size()-1]; 
        nums.pop_back();
        return item; 
    }
    //Смотрим, что мы добавили     
    void print(){
        std::unique_lock<std::recursive_mutex> locker(lock);
        for(auto n: nums)
            std::cout << n << std::endl;
        std::cout << "\n";
    }
};

//Функция для перехода к добавлению (и генерация случайных чисел)
void pushFunction(container<int> &c)
{
    srand((unsigned int)time(0));
    c.pushmore(3, rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
}

//Функция для перехода к удалению
void popFunction(container<int> &c)
{
    std::cout << c.pop() << " ";
}
 
int main()
{
    //Создаём объект класса
    container<int> cont;
    //Параллелим потоки для добавления элементов, затем смотрим, что добавили
    std::thread t1(pushFunction, std::ref(cont));
    std::thread t2(pushFunction, std::ref(cont));
    std::thread t3(pushFunction, std::ref(cont));
    t1.join();
    t2.join();
    t3.join();
    cont.print();
    //Ввиду запуска в одно и то же время, у нас генерируются одинаковые случайные числа

    //Чиллим
    std::chrono::milliseconds chill(2000);
    std::this_thread::sleep_for(chill);  

    //Параллелим потоки для проверки функции удаления элементов
    std::thread tt1(popFunction, std::ref(cont));
    std::thread tt2(popFunction, std::ref(cont));
    std::thread tt3(popFunction, std::ref(cont));
    tt1.join();
    tt2.join();
    tt3.join();
    return 0;
}



