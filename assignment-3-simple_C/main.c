#include <stdio.h>
 
const int first[] = {1, 2, 3, 4, 5};
const int second[] = {6, 7, 8, 9, 10};
 
int scalar(const int* first, const int* second, size_t length) {
    size_t i;
    int result = 0;
    for(i = 0; i < length; i++) {
        result += (first[i] * second[i]);
    }
    return result;
}
 
int is_prime(unsigned long n) {
    size_t i;
    for(i = 2; i < n / 2; i++) {
        if(n % i == 0) {
            return 0;
        }
    }
    return 1;
}
 
void print_array(const int* arr, size_t length) {
    size_t i;
    for(i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
 
 
 
int main(int argc, char** argv) {
    unsigned long n = 0;
    size_t first_length = sizeof(first) / sizeof(int);
    size_t second_length = sizeof(second) / sizeof(int);
    size_t length = first_length > second_length ? second_length : first_length;
    
    print_array(first, first_length);
    print_array(second, second_length);
    printf( "A scalar product of two vectors is: %d \n", scalar(first, second, length));
    printf("Input unsigned long: ");
    scanf("%lu", &n);
    printf("%lu is prime?\n", n);
    if(is_prime(n)) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }
    return 0;
}
