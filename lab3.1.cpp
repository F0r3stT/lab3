#include <iostream>
#include <cmath>   
#include <iomanip> 

using namespace std;

const double e = 1e-9;//эпсилон чтоб задать допустимую погрешность
//и сравнить числа с плавающей точкой
double f(double x_val, double gr_x, double peak_y) {
    double abs_x = abs(x_val);

    

    //граничные точки +/- 2
    if (abs(abs_x - gr_x) < e) {
        return 0.0;
    }
    //значения за пределами граничащих точек
    if (abs_x > gr_x) {
        return (abs_x - gr_x) / 3.0;
    }
 
    //основная функция ln(1/tg(x/2))
    double tg_arg = x_val / 2.0;
    double tg_val = tan(tg_arg);
    
    return log(abs(1.0 / tg_val));
}

int main() {
    double x_st = -5.0;
    double x_end = 5.0;
    double step = 0.5;

    double gr_x = 2.0; //функция меняет поведение   
    double peak_y = 3.0; //максимальный y в x=0
   
    cout << setw(10) << "x" << setw(10) << "y" << endl;
    
    //добавляем e/2.0 чтобы защитить от ошибок округления
    for (double x = x_st; x <= x_end + e / 2.0;x += step) {
        
        double y = f(x, gr_x, peak_y); 
        if(x ==0.0){
            cout << x << setw(10) << fixed << setprecision(4) << "inf" << endl;
            continue;
        }
        cout << setw(10) << fixed << setprecision(4) << x;

        cout << setw(10) << fixed << setprecision(4) << y << endl;
    }
    return 0; 
}