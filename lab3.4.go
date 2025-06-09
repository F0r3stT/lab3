package main

import (
	"fmt"
)

// Функция вычисления игры
func calcGame(n, m int, s []int, dp []int) {
	prefix := make([]int, n+1)

	for i := 0; i < n; i++ {
		prefix[i+1] = prefix[i] + s[i] // вычисление префиксных сумм
	}

	for i := n - 1; i >= 0; i-- {
		maxAdv := 0
		for j := 1; j <= m && i+j <= n; j++ {
			currSum := prefix[i+j] - prefix[i] // сумма взятых элементов
			currAdv := currSum - dp[i+j]       // преимущество игрока
			if currAdv > maxAdv {
				maxAdv = currAdv
			}
		}
		dp[i] = maxAdv
	}
}

func main() {
	var n, m int
	fmt.Print("Введите числа: ")
	_, err := fmt.Scan(&n, &m)
	if err != nil {
		fmt.Println("Ошибка ввода.")
		return
	}

	s := make([]int, n)
	fmt.Println("Последовательность:")
	for i := 0; i < n; i++ {
		fmt.Scan(&s[i])
	}

	dp := make([]int, n+1)
	calcGame(n, m, s, dp)

	fmt.Println()

	if dp[0] > 0 {
		fmt.Println("Результат: 1")
		fmt.Println("Победил Павел")
	} else {
		fmt.Println("Результат: 0")
		fmt.Println("Победила Вика")
	}
}