#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;


mt19937 mRand(random_device{}());
int rand(int a, int b) {
    return uniform_int_distribution<int>(a, b)(mRand);
}
double rd(double a, double b) {
    return uniform_real_distribution<double>(a, b)(mRand);
}

//генерация простых чисел до n с помощью решета Эратосфена
vector<int> prime(int n)
{
    vector<bool> isPrime(n+1, true);

    //путь до корень n
    for(int i =2; i* i <= n; i++)
    if(isPrime[i]){
        for(int j = i*i; j<=n; j+=i)
        {
            isPrime[j] = false;
        }
    }

    vector<int> primes;
    for(int i=2; i<=n;i++)
    {
        if(isPrime[i])
        {
            primes.push_back(i);
        }
    }
    return primes;
}
//возведение в степерь по модулю
long powMod(long a, long b, int mod)
{
    long res = 1;
    a %= mod;
    while(b >0)
    {
        if(b & 1) //проверка бита
            res = (res*a)%mod; //умножаем на текущее значене

        a = (a * a )%mod;
        b >>= 1; //сдвигаем b вправо на 1 бит
    }
    return res;
}

//генерация миллера (числа n вида 2*m+1)
pair<int, vector<int>> millerBuild(vector<int>& prime, int b)
{
    int maxInd = 0; 
    while(maxInd < prime.size() && prime[maxInd] < (1 << (b - 1)))
        maxInd++; //определяем верхнюю границу индексов простых чисел
    
    int m=1; //произведение степеней простых чисел
    vector<int> p; 

    while(true) //цикл генерации
    {
        int idx = rand(0, maxInd -1); //случайный индекс простого числа
        int pr = rand(1,b-1); //случ степень
        long tmp = 1;
        for(int i=0; i < pr; i++)
            tmp *= prime[idx];
        
        if(tmp > 0 && m <= INT32_MAX / tmp) //проверка на отсутствие переполения числа
        {
            m *= tmp; //умножаем на вычисденныъ степенной множитель
            p.push_back(prime[idx]);
        }
        //проверка на превышение нижней и верхней границы
        if(m> (1<< (b - 2))){ // 2^b-2
            if(m >= (1 << (b - 1))){ 
                m=1;
                p.clear(); //сбрасываем, т.к m не подходит
            }else   
                break;
        }
       }
       return {2 * m + 1, p}; //возвращаем готовую пару
    }

//тест Миллера на простоту
bool miller_test(int n, vector<int> q, int t)
{
    if (n<2)
        return false;
    
    vector<int> a_val;
    for(int i=0; i<min(t,n-3);i++)
    {
        a_val.push_back(rand(2,n-2)); //заносим случайные числа 
        //главное чтоб не больше чисел чем в диапазоне
    }
    //проверка a^(n-1) mod n ==1
    for(int a : a_val)
    { //тест ферма a^n-1 = 1(mod n)
        if(powMod(a,n-1,n)!=1){
            return false; //число составное если тест не выполнился
        }
    }

    for(int g : q)
    {
        bool isPrime = false; //наличие подходящего числа
        for(int v : a_val)
        {
            if (powMod(v,(n-1) / g , n)!= 1) //проверка Теоремы ферма
        // v^(n-1)/g если будет показан a mod(n) не делитель для всех простых g, то v равно самому n-1
            {
                isPrime = true;
                break;
            }
        }
        if (!isPrime){
            return false;
        }
    }
    return true;
}

bool miller_rabin(int n, int t)
{
    //проверка чётных и малых чисел
    if(n < 2 || n% 2 ==0)
        return n==2; //вернёт true если двойка


    int d = n - 1; // нечётное число
    int s = 0; //степень двойки
    while (d % 2 == 0) {
            d /= 2;
            s++;
        }
    
    
    for (int i = 0; i < t; ++i) {
        int a = rand(2, n - 2);// выбираем случайное число и вычисляем x = a^d
        long x = powMod(a, d, n);
    
    
        if (x != 1 && x != n - 1){ //полвеояем остальные члены последовательности
            
    
        bool isComposit = true; //составность числа
        //проверка на простое
        for (int r = 1; r < s; ++r) { 
            x = powMod(x, 2, n); //поиск по степени двойки
            if (x == n - 1) {
                isComposit = false;
                break;
            }
        }
        //если x не становится n-1 ни на одном шаге, то число составное
            if (isComposit) 
                return false;
        }
    }
    return true;
}


pair<int, vector<int>> pocklingtonBuild(const vector<int>& primes, int b) {
    int maxInd = 0;
    while (maxInd < primes.size() && primes[maxInd] < (1 << ((b / 2) + 1)))//верхняя граница
        maxInd++;


    int m = 1; //произведение простых множителей
    vector<int> q;


    while (true) {
        int num = rand(0, maxInd - 1);
        int power = rand(1, (b / 2) + 1);
        long temp = pow(primes[num], power); //простое число в степени


        if (temp > 0 && m <= INT32_MAX / temp) {
            m *= temp;
            q.push_back(primes[num]);
        }

//строится число, разрер которого на 1 бит больше половины требуемого размера для простого числа
        if (m > (1 << (b / 2))) {
            if (m >= (1 << ((b / 2) + 1))) {
                m = 1;
                q.clear();
            } else break;
        }
    }
    

    int R;
    //генерирует число, пока не попадётся четный вариант 
    do { //генерируем число размером b/2 то есть (если b = 16 то 8 бит от 128 до 255)
        //сдвигаем верхнюю и нижнюю границу диапазона и генерируем число в этом промежутке
        R = rand((1 << ((b / 2) - 1)) + 1, (1 << (b / 2)) - 1);
    } while (R % 2 != 0);
    //возвращем число собранное по формуле
    return {R * m + 1, q};
}

bool pocklington_test(int n, const vector<int>& F, int rounds) {

    for (int i = 0; i < rounds; ++i) {
        int a = rand(2, n - 2);
        if (powMod(a, n - 1, n) != 1) //проверка по малой теореме ферма
            return false;//если a^n-1 mod n !=1 то оно составное

        bool passed = false;
        for (int q : F) { //перебор простых делителей n-1
            if (powMod(a, (n - 1) / q, n) == 1) { //условие от обратного т.к
            //ищем число a для которого условие a^(n-1)/q != 1 выполняется для всех делителей
                passed = true; 
                break;
            }
        }
        if (!passed) 
            return true; //n — вероятно простое
    }
    return false; //не прошло тест
}
//гост, переход от меньшего простого к большему
int gostBuild(const vector<int>& primes, int b) {

    int maxInd = 0;
    while (maxInd < primes.size() && primes[maxInd] < (1 << (b / 2)))
        maxInd++; //до какого индекса в primes находятся подходящие простые числа
        
    int q = primes[rand(0, maxInd - 1)]; //генерация базового простое число

        // Вместо 1000 попыток, ищем пока не найдем
        while (true) {
        double ep = rd(0,1); //кси
        //вычисление n из формулы 2^(t-1)/q + 2^(t-1)ξ)/q
        double n_val =( pow(2, b-1)/q) + (pow(2, b - 1) * ep/q);
        int n = ceil(n_val);//округляет в большую сторону
        if (n % 2 != 0) 
            n++;

    
        for (int u = 0; u < 1000; u += 2) {//перебираем четные числа пока не получим простое число
            int p = (n + u) * q + 1; //условное значение на простое число
                if (p > (1 << b)){ //проверка на превышение числа заданной битности (p>2^b)
                    break;
                }
                //по теореме Диемитко проверку
                // 2^p-1 == 1 mod p и 2^N+u != 1 mod p
                if (powMod(2, p - 1, p) == 1 && powMod(2, n + u, p) != 1)
                    return p;
            }
        }
    
}
//вычисление количества проводимых тестов на простату, чтобы вероятность ошибки была мала
int calculateIterations(double errorProb) {
    int it =0;
    double currProb = 1.0; //текущая вероятность ошибки
    while(currProb > errorProb)
    {
        currProb /=4.0; //одна итерация = уменьшение вероятности ошибки
        it++;
    }
    return it;
}


int main()
{
    vector<int> prs = prime(500);

    int b;
    cout << "Введите размер чисел в битах (7-31): ";
    cin >> b;

    if (b < 2 || b > 31) {
        cout << "Ошибка: некорректный размер!" << endl;
        return 1;
    }
    const double t_Error = 0.1; //вероятность ошибки
    const int testIt = calculateIterations(t_Error);
    cout << "Используем " << testIt << " итераций теста для вероятности ошибки <= " << t_Error << endl;

    vector<pair<int, int>> res;
    //Метод Миллера
    {
        res.clear();
        int k = 0;

        while (res.size() < 10) {
            auto [n, q] = millerBuild(prs, b); //генерация числа используя список простых
            bool found = false;
            for(auto &p : res){
                if(p.first == n){ //проверка на дубликат
                    found = true;
                    break;
                }
            }
        if (found)
            continue;

        if(miller_test(n,q,testIt)){
            res.push_back({n,k});
            k=0;
        } else
        {
            k++;
        }
    }

        cout << "\nМетод Миллера:\n";
        cout << "----------------------------------------\n";
        cout << "|  №  |   Число   | Проверка |    k   |\n";


        for (size_t i = 0; i < res.size(); ++i) {
            bool check = miller_rabin(res[i].first, testIt); // по миллеру--рабину проверяем массив и насколько серьёзно
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << res[i].first  << setw(9) 
                 << (check ? '+' : '-') << setw(6) << res[i].second << " |\n";
        }
    }


    //метод поклингтона
    {
        res.clear();

        int k = 0;

        while (res.size() < 10) {
            auto [n, q] = pocklingtonBuild(prs, b);
            bool found = false;

            for(auto &p : res)
            {
                if(p.first == n){
                    found == true;
                    break;
                }
            }
            if (found)
                continue;
            if (pocklington_test(n,q,testIt))
            {
                res.push_back({n,k});
                k=0;
            }else{
                k++;
            }
        }


        cout << "\nМетод Поклингтона:\n";
        cout << "----------------------------------------\n";
        cout << "|  №  |   Число   | Проверка |    k   |\n";


        for (size_t i = 0; i < res.size(); ++i) {
            bool check = miller_rabin(res[i].first, testIt);
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << res[i].first << setw(9) 
                 << (check ? '+' : '-') << setw(6) << res[i].second << " |\n";
        }
    }


    //гост
    {
        res.clear();
        int k = 0;


        while (res.size() < 10) {
            int g = gostBuild(prs, b);
            bool found = false;
            for(auto &p : res)
            {
                if(p.first== g){
                    found == true;
                    break;
                }
            }

            if(found)
                continue;
            if(miller_rabin(g,testIt))
            { //финальная проверка методом Миллера-рабина
                res.push_back({g,k});
                k=0;
            }else
                k++;
        }

        cout << "\nМетод ГОСТ:\n";
        cout << "----------------------------------------\n";
        cout << "|  №  |   Число   | Проверка |    k   |\n";
        // в госте K - проходит проверку с первой попытки и по этому оно всегда равно 0

        for (size_t i = 0; i < res.size(); ++i) {
            bool check = miller_rabin(res[i].first, testIt);
            cout << "| " << setw(3) << i + 1 << " | " << setw(9) << res[i].first << setw(9) 
                 << (check ? '+' : '-') << setw(6) << res[i].second << " |\n";
        }

    }

    return 0;
}
