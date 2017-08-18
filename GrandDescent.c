#include <stdio.h>
#include <stdlib.h>

#define ALPHA 0.0666
#define NUMBER_OF_EXAMPLES 5
#define NUMBER_OF_VARIABLES 6

typedef struct TrainingSet{
	int n_examples;
	int n_variables;
	double examples[NUMBER_OF_EXAMPLES][NUMBER_OF_VARIABLES];
	double labels[NUMBER_OF_EXAMPLES];
}Set;

double Hypothesis(double * thea, int row, Set * T);
double PartialDerivativeOfJ(double * thea,int column, Set * T);
double * MainRepeat(double * thea, double alpha, Set * T);
double * InitThea(int n);

void print(Set * T){
	for(int i = 0; i<NUMBER_OF_EXAMPLES; i++){
		for(int j = 0; j<NUMBER_OF_VARIABLES;j++){
			printf("x[%d][%d] = %lf ; ",i,j,T->examples[i][j]);
		}
		printf(" | y[%d] = %lf\n",i,T->labels[i]);
	}
}

Set * FeatureScaling(Set * T);

int main(){
	int n_switch = 0;
	double d_switch = 0.0;
	while(1){
		printf("press \'1\' for GradientDescent, \'2\' for slove equations: ");
		scanf(" %d",&n_switch);
		if(n_switch == 1 || n_switch == 2)break;
	}
	
	switch(n_switch){
		case 1: 
				d_switch = 1.0;
				break;
		case 2: 
				d_switch = 0.0;
				break;
	}

	Set T = {
			NUMBER_OF_EXAMPLES,
			NUMBER_OF_VARIABLES,
			{
				{d_switch,2,13,5,7,7},
				{d_switch,67,5,7,6,3},
				{d_switch,13,5,82,34,12},
				{d_switch,21,30,67,12,13},
				{d_switch,-13,21,60,30,81}
			},
			{
				1,
				2,
				3,
				4,
				5
			}
	};
	double * thea = InitThea(T.n_variables);
	printf("Feature Scaling...\n");
//--------------------------------------------------
	FeatureScaling(&T);
//--------------------------------------------------
	print(&T);
	printf("Caculating...\n");
	MainRepeat(thea,ALPHA,&T);
	
	for(int i = 0; i<NUMBER_OF_VARIABLES; i++){			
		printf("thea[%d] = %lf ; ",i,thea[i]);
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
//----------------------------------------------
	FeatureScaling(&test);
//----------------------------------------------
	for(int i = 0; i < n_input; i++){
			double result = Hypothesis(thea,i,&test);
			printf("Predict result for example[%d]: %lf\n",i+1,result);
	}
	return 0;
}

Set * FeatureScaling(Set * T){
	double max[NUMBER_OF_VARIABLES];
	double min[NUMBER_OF_VARIABLES];
	double sum[NUMBER_OF_VARIABLES];
	double avg[NUMBER_OF_VARIABLES];

	for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
		max[i] = T->examples[1][i];
		min[i] = T->examples[1][i];
		sum[i] = 0;
		avg[i] = 0;
	}

	for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
		for(int j = 0; j < NUMBER_OF_EXAMPLES; j++){
				sum[i] = sum[i] + T->examples[j][i];
				if(T->examples[j][i] < min[i]){
					min[i] = T->examples[j][i];
				}
				if(T->examples[j][i] >= max[i]){
					max[i] = T->examples[j][i];
				}
			avg[i] = sum[i]/NUMBER_OF_EXAMPLES;
		//printf("min[%d] = %lf\t max%d[] = %lf\t avg[%d] = %lf\n",i,min[i],i,max[i],i,avg[i]);
		} 
		//printf("----------------\n");
		if(max[i] == min[i]){
			max[i] = 1;
			min[i] = 0;
		}
	}

	for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
		for(int j = 0; j < NUMBER_OF_EXAMPLES; j++){
				T->examples[j][i] = (T->examples[j][i]-avg[i])/(max[i]-min[i]);
		}
	}
	
	return T;
}

double * InitThea(int n){
	double * thea = (double*)malloc(n*sizeof(double));
	for(int i = 0 ; i < n ; i++){
		thea[i] = 0.0f;
	}
	return thea;
}

double * MainRepeat(double * thea,double alpha, Set * T){
	int index = 0;
	while(1){
		double * temp = (double*)malloc(T->n_variables*sizeof(double));
		for(int i = 0; i< NUMBER_OF_VARIABLES; i++){
			temp[i] = thea[i] - alpha * PartialDerivativeOfJ(thea,i,T);
		}
		
		for(index = 0; index<NUMBER_OF_VARIABLES; index++){
			if(temp[index]!=0)break;
		}

		if(temp[index] == thea[index])break;

		for(int i = 0; i< NUMBER_OF_VARIABLES; i++){
			thea[i] = temp[i];
	//		printf("thea[%d] = %lf; ",i,thea[i]);
		}
	//	printf("\n");
	}
	return thea;
}

double PartialDerivativeOfJ(double * thea,int column, Set * T){
	double sum = 0.0f;
	for(int i = 0; i<T->n_examples; i++){
		sum = sum + 
				(Hypothesis(thea,i,T)-T->labels[i])
				*
				T->examples[i][column];
	}
	//printf("sum = %lf\n",sum);
	sum = sum/T->n_examples;
	//printf("differential of cost function = %lf\n",sum);
	return sum;
}

double Hypothesis(double * thea, int row, Set * T){
	double ret = 0.0f;
	for(int i = 0; i<NUMBER_OF_VARIABLES; i++){
		ret = ret + (thea[i] * T->examples[row][i]);
	}
	//printf("Hypothesis(x%d) = %lf\n",row,ret);
	return ret;
}
