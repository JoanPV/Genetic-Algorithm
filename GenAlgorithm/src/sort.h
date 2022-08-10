#include <stdio.h>

//void quicksort_method (int [], int [], int, int);

// int main() {
//    int element_list[50],count, counter;
//    int order[50];
//    printf("Please enter the total count of the elements that you want to sort: ");
//    scanf("%d", &count);
//    printf("Please input the elements that has to be sorted:\n");
//
//    for (counter = 0; counter < count; counter++) {
//        scanf("%d", &element_list[counter]);
//        order[counter] = counter;
//    }
//    
//    quicksort_method(element_list, order, 0, count - 1);
//    printf("Output generated after using quick sort\n");
//    for (counter = 0; counter < count; counter++) {
//        printf("%d ", element_list[counter]);
//    }
//    printf("\n");
//    printf("New order after using quick sort\n");
//    for (counter = 0; counter < count; counter++) {
//        printf("%d ", order[counter]);
//    }
//    printf("\n");
//    return 0;
//}

void quicksort_method(double element_list[], int order[], double low, double high){
    double temp;
    int pivot, value1, value2;
    if (low < high) {
        pivot = low;
        value1 = low;
        value2 = high;
        while (value1 < value2) {
            while (element_list[value1] <= element_list[pivot] && value1 <= high){
                value1++;
            }
            while (element_list[value2] > element_list[pivot] && value2 >= low){
                value2--;
            }
            if (value1 < value2) {
                temp = element_list[value1];
                element_list[value1] = element_list[value2];
                element_list[value2] = temp;
                temp = order[value1];
                order[value1] = order[value2];
                order[value2] = temp;
            }
        }
        temp = element_list[value2];
        element_list[value2] = element_list[pivot];
        element_list[pivot] = temp;
        temp = order[value2];
        order[value2] = order[pivot];
        order[pivot] = temp;
        quicksort_method(element_list, order, low, value2 - 1);
        quicksort_method(element_list, order, value2 + 1, high);
    }
}













