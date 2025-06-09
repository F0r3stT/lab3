#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

void calc_game(int n, int m, vector<int> & s, vector<int> &dp)
{
    vector<int> prefix(n+1, 0);
    for(int i=0; i<n; i++)
    {
        prefix[i+1] = prefix[i] + s[i]; //вычисляет накопительные суммы
    }
    //с конца чтоб узнать все исходы
    for(int i= n-1; i>=0; i--) //вычисляет значения для динамического массива
    {
        int max_adv = 0;
        for(int j=1; j <= m && i +j <=n; j++) //перебор ходов игрока
        {
            int curr_s = prefix[i+j] - prefix[i]; //префиксная сумма

            int curr_adv = curr_s - dp[i+j]; //преимущество для игрока

            max_adv = max(max_adv, curr_adv);
        }
        dp[i] = max_adv; //самый оптимальный ход записываем
    }
}

int main()
{
    cout << "Введите числа: ";
    int n, m;
    cin >> n >> m;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1,5);

    //игровая последовательность
    vector<int> s(n);
    cout << "Последовательность:";
    for(auto &v: s){
        cin >> v;
    }
    cout << endl;

    vector<int> dp(n+1,0);
    calc_game(n,m,s,dp);
    if(dp[0] >0)
    {
        cout << "Результат: " << 1  << endl;
        cout << "Победил Павел" << endl;
    }
    else{
        cout <<"Результат: " << 0 << endl;
        cout << "Победила Вика" << endl;
    }
}