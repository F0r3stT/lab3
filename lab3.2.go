package main

import (
    "bufio"
    "fmt"
    "math"
    "math/rand"
    "os"
    "strconv"
    "strings"
    "time"
)

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))

func randInt(a, b int) int {
    return rnd.Intn(b-a+1) + a
}
func randFloat(a, b float64) float64 {
    return a + rnd.Float64()*(b-a)
}

// решето Эратосфена
func prime(n int) []int {
    isPrime := make([]bool, n+1)
    for i := 2; i <= n; i++ {
        isPrime[i] = true
    }
    for i := 2; i*i <= n; i++ {
        if isPrime[i] {
            for j := i * i; j <= n; j += i {
                isPrime[j] = false
            }
        }
    }
    out := make([]int, 0, n/10)
    for i := 2; i <= n; i++ {
        if isPrime[i] {
            out = append(out, i)
        }
    }
    return out
}

// возведение по модулю
func powMod(a, b, mod int) int {
    res := 1
    a %= mod
    for b > 0 {
        if b&1 != 0 {
            res = (res * a) % mod
        }
        a = (a * a) % mod
        b >>= 1
    }
    return res
}

// ---- Miller-Build и тесты ----

type MillerRes struct {
    n int
    q []int
}

// milerBuild: генерирует n=2*m+1 с помощью простых
func millerBuild(primes []int, b int) MillerRes {
    maxInd := 0
    for maxInd < len(primes) && primes[maxInd] < (1<<(b-1)) {
        maxInd++
    }
    m := 1
    qvec := []int{}
    for {
        idx := randInt(0, maxInd-1)
        pr := randInt(1, b-1)
        tmp := 1
        for i := 0; i < pr; i++ {
            tmp *= primes[idx]
        }
        if tmp > 0 && m <= math.MaxInt32/tmp {
            m *= tmp
            qvec = append(qvec, primes[idx])
        }
        if m > (1 << (b - 2)) {
            if m >= (1 << (b - 1)) {
                m = 1
                qvec = []int{}
            } else {
                break
            }
        }
    }
    return MillerRes{2*m + 1, qvec}
}

// тест Миллера — ферма + добавление проверки q
func millerTest(n int, q []int, t int) bool {
    if n < 2 {
        return false
    }
    aVals := make([]int, 0, t)
    for i := 0; i < t && i < n-3; i++ {
        aVals = append(aVals, randInt(2, n-2))
    }
    for _, a := range aVals {
        if powMod(a, n-1, n) != 1 {
            return false
        }
    }
    for _, g := range q {
        ok := false
        for _, a := range aVals {
            if powMod(a, (n-1)/g, n) != 1 {
                ok = true
                break
            }
        }
        if !ok {
            return false
        }
    }
    return true
}

// Miller–Rabin тест
func millerRabin(n, t int) bool {
    if n < 2 || n%2 == 0 {
        return n == 2
    }
    d := n - 1
    s := 0
    for d%2 == 0 {
        d /= 2
        s++
    }
    for i := 0; i < t; i++ {
        a := randInt(2, n-2)
        x := powMod(a, d, n)
        if x != 1 && x != n-1 {
            composite := true
            for r := 1; r < s; r++ {
                x = powMod(x, 2, n)
                if x == n-1 {
                    composite = false
                    break
                }
            }
            if composite {
                return false
            }
        }
    }
    return true
}

// ---- Pocklington ----

type PockRes struct {
    n int
    q []int
}

func pocklingtonBuild(primes []int, b int) PockRes {
    maxInd := 0
    for maxInd < len(primes) && primes[maxInd] < (1<<((b/2)+1)) {
        maxInd++
    }
    m := 1
    qvec := []int{}
    for {
        idx := randInt(0, maxInd-1)
        powcnt := randInt(1, (b/2)+1)
        tmp := int(math.Pow(float64(primes[idx]), float64(powcnt)))
        if tmp > 0 && m <= math.MaxInt32/tmp {
            m *= tmp
            qvec = append(qvec, primes[idx])
        }
        if m > (1 << (b / 2)) {
            if m >= (1 << ((b/2)+1)) {
                m = 1
                qvec = nil
            } else {
                break
            }
        }
    }
    R := 0
    for {
        low := (1 << ((b/2) - 1)) + 1
        high := (1 << (b / 2)) - 1
        R = randInt(low, high)
        if R%2 == 0 {
            break
        }
    }
    return PockRes{R*m + 1, qvec}
}

func pocklingtonTest(n int, F []int, rounds int) bool {
    for i := 0; i < rounds; i++ {
        a := randInt(2, n-2)
        if powMod(a, n-1, n) != 1 {
            return false
        }
        passed := false
        for _, q := range F {
            if powMod(a, (n-1)/q, n) == 1 {
                passed = true
                break
            }
        }
        if !passed {
            return true
        }
    }
    return false
}

// ---- GOST generator ----

func gostBuild(primes []int, b int) int {
    maxInd := 0
    for maxInd < len(primes) && primes[maxInd] < (1 << (b / 2)) {
        maxInd++
    }
    q := primes[randInt(0, maxInd-1)]
    for {
        ep := randFloat(0, 1)
        nVal := (math.Pow(2, float64(b-1))/float64(q) +
            math.Pow(2, float64(b-1))*ep/float64(q))
        nn := int(math.Ceil(nVal))
        if nn%2 != 0 {
            nn++
        }
        for u := 0; u < 1000; u += 2 {
            p := (nn+u)*q + 1
            if p > (1 << b) {
                break
            }
            if powMod(2, p-1, p) == 1 && powMod(2, nn+u, p) != 1 {
                return p
            }
        }
    }
}

// количество итераций нужных для вероятности ошибки
func calculateIterations(errorProb float64) int {
    it := 0
    curr := 1.0
    for curr > errorProb {
        curr /= 4.0
        it++
    }
    return it
}

func main() {
    primes := prime(500)

    fmt.Print("Введите размер чисел в битах (7-31): ")
    reader := bufio.NewReader(os.Stdin)
    line, _ := reader.ReadString('\n')
    line = strings.TrimSpace(line)
    b, err := strconv.Atoi(line)
    if err != nil || b < 2 || b > 31 {
        fmt.Println("Ошибка: некорректный размер!")
        return
    }

    const tErr = 0.1
    iters := calculateIterations(tErr)
    fmt.Printf("Используем %d итераций теста для вероятности ошибки <= %v\n", iters, tErr)

    type resT struct {
        val, k int
    }

    // Метод Миллера
    {
        var results []resT
        k := 0
        for len(results) < 10 {
            mr := millerBuild(primes, b)
            n := mr.n
            found := false
            for _, r := range results {
                if r.val == n {
                    found = true
                    break
                }
            }
            if found {
                k++
                continue
            }
            if millerTest(n, mr.q, iters) {
                results = append(results, resT{n, k})
                k = 0
            } else {
                k++
            }
        }
        fmt.Println("\nМетод Миллера:")
        fmt.Println("----------------------------------------")
        fmt.Println("|  №  |   Число   | Проверка |    k   |")
        for i, r := range results {
            fmt.Printf("| %3d | %9d |    +     | %6d |\n", i+1, r.val, r.k)
        }
    }

    // метод Поклингтона
    {
        var results []resT
        k := 0
        for len(results) < 10 {
            pr := pocklingtonBuild(primes, b)
            n := pr.n
            found := false
            for _, r := range results {
                if r.val == n {
                    found = true
                    break
                }
            }
            if found {
                k++
                continue
            }
            if pocklingtonTest(n, pr.q, iters) {
                results = append(results, resT{n, k})
                k = 0
            } else {
                k++
            }
        }
        fmt.Println("\nМетод Поклингтона:")
        fmt.Println("----------------------------------------")
        fmt.Println("|  №  |   Число   | Проверка |    k   |")
        for i, r := range results {
            fmt.Printf("| %3d | %9d |    +     | %6d |\n", i+1, r.val, r.k)
        }
    }

    // ГОСТ
    {
        var results []resT
        k := 0
        for len(results) < 10 {
            g := gostBuild(primes, b)
            found := false
            for _, r := range results {
                if r.val == g {
                    found = true
                    break
                }
            }
            if found {
                k++
                continue
            }
            if millerRabin(g, iters) {
                results = append(results, resT{g, k})
                k = 0
            } else {
                k++
            }
        }
        fmt.Println("\nМетод ГОСТ:")
        fmt.Println("----------------------------------------")
        fmt.Println("|  №  |   Число   | Проверка |    k   |")
        for i, r := range results {
            fmt.Printf("| %3d | %9d |    +     | %6d |\n", i+1, r.val, r.k)
        }
    }
}
