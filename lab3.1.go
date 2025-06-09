package main

import (
	"fmt"
	"math"
)

const (
	epsilon   = 1e-9  // допустимая погрешность
	xStart    = -5.0  // начальное значение x
	xEnd      = 5.0   // конечное значение x
	step      = 0.5   // шаг изменения x
	boundaryX = 2.0   // граница, где функция меняется
	peakY     = 3.0   // значение y при x = 0
)

// Функция, рассчитывающая значение y в зависимости от x
func computeY(x float64) float64 {
	absX := math.Abs(x)

	// x ≈ 0
	if absX < epsilon {
		return peakY
	}

	// |x| ≈ границе (например, 2.0)
	if math.Abs(absX-boundaryX) < epsilon {
		return 0.0
	}

	// |x| > границы: линейная часть
	if absX > boundaryX {
		return (absX - boundaryX) / 3.0
	}

	// Основная функция: y = ln(1 / |tg(x / 2)|)
	tgArg := x / 2.0
	tgVal := math.Tan(tgArg)

	return math.Log(math.Abs(1.0 / tgVal))
}

func main() {
	fmt.Printf("%10s%10s\n", "x", "y")

	for x := xStart; x <= xEnd+epsilon/2.0; x += step {
		y := computeY(x)
		fmt.Printf("%10.4f%10.4f\n", x, y)
	}
}
