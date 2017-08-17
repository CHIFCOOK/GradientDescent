#include <stdio.h>
#include <stdlib.h>

#define ALPHA 0.031
#define NUMBER_OF_EXAMPLES 3
#define NUMBER_OF_VARIABLES 4

typedef struct TrainingSet{
	int n_examples;
	int n_variables;
	double examples[NUMBER_OF_EXAMPLES][NUMBER_OF_VARIABLES];
	double results[NUMBER_OF_EXAMPLES];
}Set;

double Hypothesis(double * thea, int row, Set * T);
double DifferentialOfJ(double * thea,int column, Set * T);
double * MinimizeJ(double * thea, double alpha, Set * T);
double * InitThea(int n);

int main(){
	Set T = {
			NUMBER_OF_EXAMPLES,
			NUMBER_OF_VARIABLES,
			{
				{0.0,3.0,0.0,4.0},
				{0.0,2.0,3.0,1.0},
				{0.0,5.0,-9.0,7.0}
			},
			{
				7.0,
				9.0,
				8.0
			}
	};
	double * thea = InitThea(T.n_variables);
	printf("Caculating...\n");
	MinimizeJ(thea,ALPHA,&T);
	
	for(int i = 0; i<NUMBER_OF_VARIABLES; i++){			
		printf("thea[%d] = %lf\t",i,thea[i]);
	}
	printf("\n");


	int n_input = 0;
	printf("How many examples do you want to predict?\n");
	scanf(" %d",&n_input);

	double input[n_input][NUMBER_OF_VARIABLES];

	for(int i = 0; i< n_input; i++){
		input[i][0] = T.examples[i][0];
		for(int j = 1; j<NUMBER_OF_VARIABLES; j++){
			printf("Please input example[%d], variable[%d]: ",i+1,j);
			scanf(" %lf",&input[i][j]);
		}
	}

	Set test = {
				n_input,
				NUMBER_OF_VARIABLES,
	};
	for(int i = 0; i < n_input; i++){
		for(int j = 0; j<NUMBER_OF_VARIABLES; j++){
			test.examples[i][j] = input[i][j];
		}
	}
	
	for(int i = 0; i < n_input; i++){
		double result = Hypothesis(thea,i,&test);
		printf("Predict result for example[%d]: %lf\n",i+1,result);
	}
	return 0;
}

double * InitThea(int n){
	double * thea = (double*)malloc(n*sizeof(double));
	for(int i = 0 ; i < n ; i++){
		thea[i] = 0.0f;
	}
	return thea;
}

double * MinimizeJ(double * thea,double alpha, Set * T){
	while(1){
		double * temp = (double*)malloc(T->n_variables*sizeof(double));
		for(int i = 0; i< T->n_variables; i++){
			temp[i] = thea[i] - alpha * DifferentialOfJ(thea,i,T);
		}
		
		if(temp[1]==thea[1])break;

		for(int i = 0; i< T->n_variables; i++){
			thea[i] = temp[i];
			//printf("thea[%d] = %f\t",i,thea[i]);
		}
		//printf("\n");
	}
	return thea;
}

double DifferentialOfJ(double * thea,int column, Set * T){
	double sum = 0.0f;
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

double Hypothesis(double * thea, int row, Set * T){
	double ret = 0.0f;
	for(int i = 0; i<T->n_variables; i++){
		ret = ret + ( thea[i] * T->examples[row][i]);
	}
	//printf("Hypothesis(x%d) = %f\n",row,ret);
	return ret;
}
