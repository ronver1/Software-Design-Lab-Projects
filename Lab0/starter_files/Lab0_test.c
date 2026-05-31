#include <stdio.h>
#include "Lab0.h"

/*
    Test main to help you debug Lab0. We HIGHLY recommend you write your own test cases to help you debug and test your code.
*/
int main() {
    //Test isPrime()
    int isPrimeInput = 7;
    int isPrimeExpectedOutput = 1;
    if(isPrime(isPrimeInput) == isPrimeExpectedOutput) {
        printf("isPrime test passed!\n");
    }
    else {
        printf("isPrime test failed. Output was %d, expected %d\n", isPrime(isPrimeInput), isPrimeExpectedOutput);
    }

    //Test calculateHypotenuse()
    double calculateHypotenuseInput1 = 3.0;
    double calculateHypotenuseInput2 = 4.0;
    double calculateHypotenuseExpectedOutput = 5.0;
    if(calculateHypotenuse(calculateHypotenuseInput1, calculateHypotenuseInput2) == calculateHypotenuseExpectedOutput) {
        printf("calculateHypotenuse test passed!\n");
    }
    else {
        printf("calculateHypotenuse test failed. Output was %lf, expected %lf\n", calculateHypotenuse(calculateHypotenuseInput1, calculateHypotenuseInput2), calculateHypotenuseExpectedOutput);
    }

    //Test gcd()
    int gcdInput1 = 10, gcdInput2 = 15;
    int gcdExpectedOutput = 5;
    if(gcd(gcdInput1, gcdInput2) == gcdExpectedOutput) {
        printf("gcd test passed!\n");
    }
    else {
        printf("gcd test failed. Output was %d, expected %d\n", gcd(gcdInput1, gcdInput2), gcdExpectedOutput);
    }

    return 0;
}
