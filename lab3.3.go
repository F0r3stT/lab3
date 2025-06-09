package main

import (
	"fmt"
	"math"
	"os"
)

// факториал
func fact(n int) int64 {
	res := int64(1)
	for i := 2; i <= n; i++ {
		res *= int64(i)
	}
	return res
}

// НОД (алгоритм Евклида)
func gcd(a, b int64) int64 {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

// биномиальный коэффициент C(n, k)
func binCoef(n, k int) int64 {
	if k < 0 || k > n {
		return 0
	}
	return fact(n) / (fact(k) * fact(n-k))
}

// число Стирлинга второго рода
func stirling(n, k int) int64 {
	if k < 0 || k > n {
		return 0
	}
	if k == 1 && n > 0 {
		return 1
	}
	if k == n {
		return 1
	}
	if k == 0 && n > 0 {
		return 0
	}

	var sum int64
	for i := 0; i <= k; i++ {
		term := binCoef(k, i) * int64(math.Pow(float64(i), float64(n)))
		if (k-i)%2 != 0 {
			sum -= term
		} else {
			sum += term
		}
	}
	return sum / fact(k)
}

// проверка корректности ввода
func readInput() (int, int) {
	var a, b int
	for {
		fmt.Print("Введите два целых числа (от 1 до 10): ")
		_, err := fmt.Scan(&a, &b)
		if err != nil || a < 1 || b < 1 || a > 10 || b > 10 {
			fmt.Println("Ошибка. Числа должны быть в диапазоне от 1 до 10.")
			continue
		}
		return a, b
	}
}

func main() {
	perA, perB := readInput()

	if perB == 1 {
		fmt.Println("infinity")
		os.Exit(0)
	}

	var genNum int64 = 0
	genDen := int64(math.Pow(float64(perB-1), float64(perA+1)))

	for k := 0; k <= perA; k++ {
		st := stirling(perA, k)
		fk := fact(k)
		pow := int64(math.Pow(float64(perB-1), float64(perA-k)))

		genNum += st * fk * int64(perB) * pow
	}

	// сокращаем дробь
	div := gcd(genNum, genDen)
	num := genNum / div
	den := genDen / div

	fmt.Printf("Результат: %d/%d\n", num, den)
}
