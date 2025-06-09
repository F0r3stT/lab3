#include <iostream>
#include <vector>
#include <cmath>
#include <string>


using namespace std;

//факториал
long fact(int num)
{
    long res = 1;
    for(int i = 2; i<=num; i++)
        res *= i;
    
    return res;
}

//наибольший общий делитель с помощью алгоритма евклида
long NOD (long a, long b)
{
    while(b!=0)//цикл работает пока остаток не равен нулю
    {
        long tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

long bincof(int n, int k)
{
    if(k <0 || k > n)
    {
        return 0;
    }
//выч по формуле C(n,k) = n! / (k! *(n-k)!)
    return fact(n)/(fact(k)*fact(n-k));
}
//число стирлинга
long n_stirling(int n, int k)
{
    if(k<0 || k > n)
        return 0;
    if (k== 1 && n > 0) 
        return 1;
    if (k==n )
        return 1;
    if (k==0 && n > 0 )
        return 0;

    long sum = 0;
    //сумма от i до k
    for(int i=0; i <=k; i++)
    {
        //по ф-ле вычисляем бинкоэфф умноженный на i^n
        long ch_sum = bincof(k,i) * pow(i, n);
        if((k-i)% 2 !=0) //проверка знака слагаемого (-1)^(k-i)
        {
            sum -= ch_sum; //вычитаем слагаемое
        }else {
            sum += ch_sum; //если чётное, то (-1) в чётной степени даёт +1
        }
    }
    return sum / fact(k); 
    
}

int main()
{
    int per_a; //кол-во элементов
    int per_b; //кол-во параметров

    while(true)
    {
        cout <<"Введите два целых числа (от 1 до 10): ";
        cin >> per_a >> per_b;
        if(cin.fail())
        {
            cout <<"Ошибка ввода. Введите два целых числа" << endl;
            cin.clear();

        }else if(per_a >=1 && per_b <= 10 && per_b >= 1 & per_b <=10){
            break;
        }else
            cout <<"Ошибка. Числа должны быть в диапазоне от 1 до 10" << endl;
    }

    cout << endl;

    if(per_b == 1){
        cout <<"infinity" << endl;
        return 0;
    }
    
    long gen_num =0; //общий числитель
    long gen_den = pow(per_b -1, per_a+1); //общий знаменатель дроби
    vector<int> vec_k;

    for(int i=0; i<=per_a; i++)
        vec_k.push_back(i);
    
//вычисление суммы
    for(auto k : vec_k)
    {
        long stirling = n_stirling(per_a, k);
        long fakt = (fact(k));
        long n_pow = pow(per_b - 1, per_a - k);

        gen_num += stirling * fakt * per_b * n_pow;
    }

    long del = NOD(gen_num, gen_den);
    long res_num = gen_num / del; //сокращенный числитель и знаменатель получаем делением исходного числа на НОД
    long res_den = gen_den / del;

    cout << "Результат: " << res_num << "/" << res_den << endl;

    return 0;
}