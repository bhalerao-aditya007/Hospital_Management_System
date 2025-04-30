#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100
typedef struct{
 int priority;
 char name[50];
}Patient;
Patient queue[MAX];  
int size =0;
const char* getCategory(int priority ){
 if(priority >=15) return "Critical" ;
 else if(priority >= 10) return "Severe";
 else if(priority >=5) return "Moderate" ;
 else return "Mild";
}
void insert(int priority , char name[ ]){
 if(size >= MAX){
     printf("Queue is full!\n") ;
     return ;
 }
 queue[size].priority = priority ;
 strcpy(queue[ size ].name , name ) ;
 size++ ;
}
int partition(int low ,int high){
    int pivot = queue[high].priority ;
    int i = low -1 ;
    for(int j=low; j<high ;j++){
        if(queue[j].priority > pivot){
            i++ ;
            Patient temp = queue[i];
            queue[i] = queue[j] ;
            queue[j] = temp;
        }
    }

    Patient temp = queue[i+1];
    queue[i+1] = queue[high] ;
    queue[high] = temp;
    return i+1 ;
}
void quickSort(int low , int high){
    if(low < high){
        int pivotIndex = partition( low ,high );
        quickSort( low, pivotIndex -1 );
        quickSort(pivotIndex+1 , high );
    }
}
void sortQueue( ){
    quickSort( 0 , size - 1 );
}
Patient extractMax(){
    if(size==0){
        printf("No patients in queue!\n");
        Patient empty = {-1, "None"} ;
        return empty;
    }
    sortQueue();
    Patient First_Patient = queue[0];
    for(int i = 0 ; i < size -1 ; i++){
        queue[i] = queue[ i+1 ];
    }
    size-- ;
    return First_Patient;
}
void displaySortedQueue(){
    sortQueue();
    printf("\nOrder of Treatment (Highest to Lowest Priority):\n");
    for(int i=0;i<size;i++){
        printf("Priority Code: %d, Patient: %s (%s)\n" , queue[i].priority , queue[i].name , getCategory(queue[i].priority));
    }
}
int main(){
    insert(1, "Ajay" ) ;
    insert(10,"Atul");
    insert(8 , "Shreya");
    insert(15, "Shankar") ;
    insert(3,"Arjit") ;
    insert(12 , "Lata");
    displaySortedQueue( );
    Patient firstPatient = extractMax( );
    printf("\nFirst Patient to be Treated: %s (%s)\n",firstPatient.name , getCategory( firstPatient.priority ) );
    return 0;
}