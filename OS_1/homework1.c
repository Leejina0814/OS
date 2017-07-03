#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
void *sorting(void *p);
void *merge(void *p);

int arr[1001];
int sol[1001];
int i = 0;
static int cnt = 0;
typedef struct{
int from_index;
int to_index; 
}parameters;

typedef struct{
int first;
int first_last;
int second;
int second_last;
int third;
int third_last;
}data_index;


int main(){

freopen("input.txt", "r", stdin);
while(scanf("%d", &arr[i]) != EOF){
	i++;
}


pthread_t thr1, thr2, thr3, thr4;

parameters *data1 = (parameters *)malloc(sizeof(parameters));
parameters *data2 = (parameters *)malloc(sizeof(parameters));
parameters *data3 = (parameters *)malloc(sizeof(parameters));

data_index *datas = (data_index *)malloc(sizeof(data_index));
int num = i%3;

if(num == 2){
	data1->from_index = 0;
	data1->to_index = (i/3)-1;
	datas->first = 0;
	datas->first_last = (i/3)-1;

	data2->from_index = (i/3);
	data2->to_index = (i/3)+(i/3);
	datas->second = (i/3);
	datas->second_last = (i/3)+(i/3);

	data3->from_index = (i/3)+(i/3)+1;
	data3->to_index = i-1;
	datas->third = (i/3)+(i/3)+1;
	datas->third_last = i-1;
}
else{
	data1->from_index = 0;
	data1->to_index = (i/3)-1;
	datas->first = 0;
	datas->first_last = (i/3)-1;

	data2->from_index = (i/3);
	data2->to_index = (i/3)+(i/3)-1;
	datas->second = i/3;
	datas->second_last = (i/3)+(i/3)-1;

	data3->from_index = (i/3)+(i/3);
	data3->to_index = i-1;
	datas->third = (i/3)+(i/3);
	datas->third_last = i-1;

}



pthread_create(&thr1, NULL, sorting, data1);
pthread_create(&thr2, NULL, sorting, data2);
pthread_create(&thr3, NULL, sorting, data3);

while(cnt != 3){}
pthread_join(thr1, NULL);
pthread_join(thr2, NULL);
pthread_join(thr3, NULL);



pthread_create(&thr4, NULL, merge, datas);

pthread_join(thr4, NULL);

for(int j =0; j<i; j++)
	printf("%d ", sol[j]);
printf("\n");

}


void swap(int *a, int *b){

	int temp = *a;
	*a = *b;
	*b = temp;
}

void *sorting(void *p){

	
	parameters* temp_p = (parameters*)p;
	int i, j;
	for(i = temp_p->from_index; i<temp_p->to_index;i++){
		for(j = i+1; j<=temp_p->to_index; j++){
			if(arr[i]> arr[j]){
				swap(&arr[i], &arr[j]);
	}}}
	cnt++;
	pthread_exit(NULL);
}

int array1(int *a, int *b, int *c){
	int temp;
	temp = arr[*a]<arr[*b] ? arr[*a] : arr[*b];
	temp = arr[*c]<temp? arr[*c]: temp;

	if(temp == arr[*a])
		(*a)++;
	else if(temp == arr[*b])
		(*b)++;
	else
		(*c)++;

	return temp;
}

int array2(int *a, int *b){
	int temp;
	temp = arr[*a]<arr[*b] ? arr[*a] : arr[*b];
	if(temp == arr[*a])
		(*a)++;
	else
		(*b)++;
	return temp;
}


void *merge(void *p){
	data_index* temp_p = (data_index*)p;
	int one = temp_p->first;
	int one_last = temp_p->first_last;
	int two = temp_p->second;
	int two_last = temp_p->second_last;
	int three = temp_p->third;
	int three_last = temp_p->third_last;

	for(int j =0; j<i; j++){
		if(one == one_last+1){
			for(int k = j; k<i; k++){
				if(two == two_last+1){
					for(int n = three; n<three_last+1; n++){
						sol[j] = arr[n];
						j++;
					}
				}
				if(three == three_last+1){
					for(int n = two; n<two_last+1; n++){
						sol[j] = arr[n];
						j++;
					}
				}
				sol[j] = array2(&two, &three);
				j++;
			}
		}

		if(two == two_last+1){
			for(int k = j; k<i; k++){
				if(one == one_last+1){
					for(int n = three; n<three_last+1; n++){
						sol[j] = arr[n];
						j++;
					}
				}
				if(three == three_last+1){
					for(int n = one; n<one_last+1; n++){
						sol[j] = arr[n];
						j++;
					}

				}
				sol[j] = array2(&one, &three);
				j++;
			}
		}

		if(three == three_last+1){
			for(int k = j; k<i; k++){
				if(one == one_last+1){
					for(int n = two; n<two_last+1; n++){
						sol[j] = arr[n];
						j++;
					}
				}
				if(two == two_last+1){
					for(int n = one; n<one_last+1; n++){
						sol[j] = arr[n];
						j++;
					}
				}
				sol[j] = array2(&one, &two);
				j++;
			}
		}
		sol[j] = array1(&one, &two, &three);
	}
	pthread_exit(NULL);
}
