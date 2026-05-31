// ECE 312 Lab 0
// <Ronit Verma>
// <rv24923>
// Spring 2026

#include "Lab0.h"
#include "math.h"
#include "stdlib.h"

/*
    Write a function that takes in an integer x and returns 1 if x is prime and 0 if x is not prime.
*/
int isPrime(int x) {
    //TODO: Your code here

		long a = 0;
		long b;
		long c; 
		if (x<= 1)
			{
		return 0; 
				}
		else
			{
			a = sqrt(x);

			for (b = 2; b <= a; b++)
			{
				c = (x % b);

				if (c == 0)	
			{
				
				return 0;
				}

				} 
				}
				return 1;

}

/*
    Write a function that takes in two legs of a right triange, x and y, and returns the length of the hypotenuse.
*/
double calculateHypotenuse(double x, double y) {
    //TODO: Your code here
	
	double d;
	double e;
	double f;
	double g;

	if (x<0 || y<0) {
	return -1; 
	}
	else {
		if (x == 0 || y == 0) {
		return 0;
		}
			else {
			d = x * x;
			e = y * y;
			f = d + e;
			g = sqrt(f); 
			return g;
			}

}
}

/*
    Write a function that finds the greatest common divisor of two integers x and y.
*/
int gcd(int x, int y) {
    //TODO: Your code here
	int h;

	x = abs(x);
	y = abs(y);
	h = y;

	while (y>0) {
	y = x % y;
	x = h;
	h = y;
	}
	return x;
}
