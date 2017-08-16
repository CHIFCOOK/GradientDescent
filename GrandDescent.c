#include <stdio.h>
#include <stdlib.h>

#define ALPHA 0.003
#define ROW 3
#define COLUMN 4

typedef struct TrainingSet{
	int n_examples;
	int n_variables;
	float examples[ROW][COLUMN];
	float results[ROW];
}Set;

float Hypothesis(float * thea, int row, Set * T);
float DifferentialOfJ(float * thea,int column, Set * T);
float * MinimizeJ(float * thea,float alpha, Set * T);
float * InitThea(int n);

int main(){
	Set T = {
			ROW,
			COLUMN,
			{
				{1.1f,4.4f,6.2f,2.7f},
				{1.5f,2.3f,3.0f,4.0f},
				{1.4f,4.3f,2.0f,5.6f}
			},
			{
				1.2f,
				3.5f,
				4.2f
			}
	};
	float * thea = InitThea(T.n_variables);
	MinimizeJ(thea,ALPHA,&T);
	return 0;
}

float * InitThea(int n){
	float * thea = (float *)malloc(n*sizeof(float));
	for(int i = 0 ; i < n ; i++){
		thea[i] = 0.0f;
	}
	return thea;
}

float * MinimizeJ(float * thea,float alpha, Set * T){
	while(1){
		float * temp = (float *)malloc(T->n_variables*sizeof(float));
		for(int i = 0; i< T->n_variables; i++){
			temp[i] = thea[i] - alpha * DifferentialOfJ(thea,i,T);
		}
		
		if(temp[1]==thea[1])break;

		for(int i = 0; i< T->n_variables; i++){
			thea[i] = temp[i];
			printf("thea[%d] = %f\t",i,thea[i]);
		}
		printf("\n");
	}
	return thea;
}

float DifferentialOfJ(float * thea,int column, Set * T){
	float sum = 0.0f;
	for(int i = 0; i<T->n_examples; i++){
		sum = sum + 
				(Hypothesis(thea,i,T)-T->results[i])
				*
				T->examples[i][column];
	}
	//printf("sum = %f\n",sum);
	sum = sum/T->n_examples;
	//printf("differential of cost function = %f\n",sum);
	return sum;
}

float Hypothesis(float * thea, int row, Set * T){
	float ret = 0.0f;
	for(int i = 0; i<T->n_variables; i++){
		ret = ret + ( thea[i] * T->examples[row][i]);
	}
	//printf("Hypothesis(x%d) = %f\n",row,ret);
	return ret;
}
