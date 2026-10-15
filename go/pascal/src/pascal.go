package main
import "fmt"

func pascal(i int, j int) int{

    if ((i == j) || (j == 1)) {
        return 1
    } else if ( i < j ) {
        return 0
    } else {
        return pascal(i-1, j) + pascal(i-1, j-1)
    }
}
func main() {
    for i := 0;  i < 10; i++ {
        for j := 0;  j <= i; j++ {
            fmt.Print(pascal(i+1,j+1))
            fmt.Print(" ")
        }
        fmt.Println()
    }
}
