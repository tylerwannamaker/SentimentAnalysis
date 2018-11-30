#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
char *stringlwr(char *str)
{
	unsigned char *p = (unsigned char *)str;
	while(*p){
		*p=tolower((unsigned char)*p);
		p++;
	}
	return str;
}

void clearInput(char *input){
	input[0]='\0';
}

const char * expertsys_rating(float expert_rating){
	if(expert_rating>=0.6f)
		return "Likely Positive";
	else if(expert_rating>=0.4f)
		return "Essentially Indifferent or unable to determine.";
	else
		return "Likely Negative";
}


void *threadFunc(int num){
//printf("\nMsg from thread %d :)\n", num);

FILE *reviewfile=fopen("review_input_0.txt", "r");
FILE *fn=fopen("rev_0.txt","w");
switch(num){
	case 1:
		reviewfile=fopen("review_input_1.txt", "r");
		fn=fopen("rev_1.txt","w");
		break;
	case 2:
		reviewfile=fopen("review_input_2.txt","r");
		fn=fopen("rev_2.txt","w");
		break;
	case 3:
		reviewfile=fopen("review_input_3.txt","r");
		fn=fopen("rev_3.txt","w");
}
//printf("\nLets go from %d", num);

float amp_pos=1.5f;
float amp_neg=-0.5f;
float pos=1.0f;
float neg=0.0f;

int rating=0;
float weighted_rating=0.0f;
float expert_rating=0.0f;
int positivecount=0;
int negativecount=0;
int keywordcount=0;
int correct=0;
int incorrect=0;
int undetermined=0;

char Name[50]={""};
char input[8000]={""};
char rev[10090]={""};

FILE *kwAmpFile=fopen("Keywords/Amplifier_Keywords.txt","r");
FILE *kwNegatingFile=fopen("Keywords/Negating_Keywords.txt","r");
FILE *kwNegativeFile=fopen("Keywords/Negative_Keywords.txt","r");
FILE *kwPosFile=fopen("Keywords/Positive_Keywords.txt","r");


char keywords_amplifier[2000]={""};
char keywords_negating[2000]={""};
char keywords_negative[2000]={""};
char keywords_positive[2000]={""};

fscanf(kwAmpFile,"%s", &input);
do{
	strcat(keywords_amplifier, input);
	strcat(keywords_amplifier, " ");
	clearInput(input);
	fscanf(kwAmpFile, "%s", &input);
}while(strstr(input, "fileend")==NULL);
fclose(kwAmpFile);
clearInput(input);
//printf("\nAmp from %d", num);

fscanf(kwNegatingFile, "%s", &input);
do{
	strcat(keywords_negating, input);
	strcat(keywords_negating, " ");
	clearInput(input);
	fscanf(kwNegatingFile, "%s", &input);
}while(strstr(input, "fileend")==NULL);
fclose(kwNegatingFile);
clearInput(input);
//printf("\nNegating from %d", num);

fscanf(kwNegativeFile, "%s", &input);
do{
	strcat(keywords_negative, input);
	strcat(keywords_negative, " ");
	clearInput(input);
	fscanf(kwNegativeFile, "%s", &input);
}while(strstr(input, "fileend")==NULL);
fclose(kwNegativeFile);
clearInput(input);
//printf("\nNeg from %d", num);

fscanf(kwPosFile, "%s", &input);
do{
	strcat(keywords_positive, input);
	strcat(keywords_positive, " ");
	clearInput(input);
	fscanf(kwPosFile, "%s", &input);
}while(strstr(input, "fileend")==NULL);
fclose(kwPosFile);
clearInput(input);
//printf("\nPos from %d", num);

//Cycle reviews
do{
	rev[0]='\0';

	positivecount = negativecount = keywordcount = expert_rating = weighted_rating = 0.0f;
	clearInput(input);
	fscanf(reviewfile, "%s", &input);
	
	if(strstr(input, "name")!=NULL){
	fscanf(reviewfile, "%s", &Name);
	}
	else if(strstr(input, "review")!=NULL){
		fscanf(reviewfile, "%s", &input);
		while(strstr(input, "overallrating")==NULL){
		stringlwr(input);
		strcat(input, " ");
		
			//Negated
			if(strstr(keywords_negating, input)!=NULL){
			strcat(rev, input);
			fscanf(reviewfile, "%s", &input);
			strcat(input, " ");
				if(strstr(keywords_negative, input)!=NULL){
				++positivecount;
				++keywordcount;
				weighted_rating+=pos;
				}
				else if(strstr(keywords_positive, input)!=NULL){
					++negativecount;
					++keywordcount;
					weighted_rating+=neg;
				}
			}
			//Amplified
			if(strstr(keywords_amplifier, input)!=NULL){
			strcat(rev, input);
			fscanf(reviewfile, "%s", &input);
			strcat(input, " ");
				if(strstr(keywords_negative, input)!=NULL){
					++negativecount;
					++keywordcount;
					weighted_rating+=amp_neg;
				}
				else if(strstr(keywords_positive, input)!=NULL){
					++positivecount;
					++keywordcount;
					weighted_rating+=amp_pos;
				}
			}
			//Standard Words
			else if(strstr(keywords_positive, input)!=NULL){
				++positivecount;
				++keywordcount;
				weighted_rating+=pos;
			}
			else if(strstr(keywords_negative, input)!=NULL){
				++negativecount;
				++keywordcount;
				weighted_rating+=neg;
			}
			strcat(rev, input);
			fscanf(reviewfile, "%s", &input);
		}
		}if(strstr(input, "overallrating")!=NULL){
			fscanf(reviewfile, "%d", &rating);
			expert_rating = weighted_rating / (float)keywordcount;
			if(keywordcount ==0)
				expert_rating=0.5f;
			if((rating>=3&& expert_rating>0.5f)||(rating<=2 && expert_rating < 0.5f))
				++correct;
			else if(expert_rating==0.5f)
				++undetermined;
			else
				++incorrect;

		//	printf("Name: %s\nReview: %s\nRating: %d\nKeywords: %d, Positive-Negative: %d-%d\nComputed Rating: %f\n", Name, rev, rating, keywordcount, positivecount, negativecount, expert_rating);
			//printf("%s\n\n", expertsys_rating(expert_rating));
			
			fprintf(fn, "Name: %s\nReview: %s\nRating: %d\nKeywords: %d, P-N: %d-%d\nComputed Rating: %f\n\n", Name, rev, rating, keywordcount, positivecount, negativecount, expert_rating);

		}

	

}while(strstr(input, "fileend")==NULL);
printf("Thread %d results: Correct: %d, Incorrect: %d, Undetermined: %d, Accuracy: %f, Total: %d", num, correct, incorrect, undetermined, (float)correct/(float)(incorrect+correct+undetermined), undetermined+correct+incorrect);

fclose(fn);
fclose(reviewfile);



printf("\nDone with %d\n", num);
pthread_exit(NULL);
}

int main(){
	int rc=0;
	pthread_t threads[4];
	int t=0;
	for( t=0; t<4; t++){
		printf("\nStarting thread %d\n", t);
		rc=pthread_create(threads+t, NULL, threadFunc, (void *) t);
		if(rc){
			printf("ERROR: %d\n", rc);
			exit(-1);
		}
	}
	for(t=0;t<4;t++){
		pthread_join(threads[t], NULL);
	}
	pthread_exit(NULL);

}
