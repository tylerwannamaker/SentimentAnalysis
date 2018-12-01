 #include <stdio.h>
#include <string.h>
#include<ctype.h>
#include<math.h>

char *stringlwr(char *str)
{
	unsigned char *p = (unsigned char *)str;

	while (*p) {
		*p = tolower((unsigned char)*p);
		p++;
	}

	return str;
}
void clearInput(char *input) {
	input[0] = '\0';
}
const char * expertsys_rating(float expert_rating){
	if (expert_rating >= 0.8f)
		return "Certainly Positive";
	else if (expert_rating >= 0.6f)
		return "Mostly Positive";
	else if (expert_rating >= 0.4f)
		return "Essentially Indifferent or Unable to Determine";
	else if (expert_rating >= 0.2f)
		return "Mostly Negative";
	else
		return "Certainly Negative";
}

int main() {
	FILE *reviewfile = fopen("review_input.txt", "r");
	FILE *kwAmpFile = fopen("Keywords/Amplifier_Keywords.txt", "r");
	FILE *kwNegatingFile = fopen("Keywords/Negating_Keywords.txt", "r");
	FILE *kwNegativeFile = fopen("Keywords/Negative_Keywords.txt", "r");
	FILE *kwPosFile = fopen("Keywords/Positive_Keywords.txt", "r");
	FILE *fn= fopen("review_output_svm.txt", "w");
	if (fn == NULL) {
		printf("Error opening output file!\n");
		return 1;
	}
	// ----- RATING WEIGHTS ----- //
	float amp_pos = 1.5f;
	float amp_neg = -0.5f;
	float pos = 1.0f;
	float neg = 0.0f;
    
    float ideal_prating = 1;
    float ideal_nrating = 0;
    float ideal_erating = 0.5;
    float Accuracy = 0.0f;
    float Average= 0.0f;

	int rating =  0;
	int positivecount = 0;
	int negativecount = 0;
	int keywordcount = 0;
	

	char Name[50] = { "" };
	char input[8000]= {""};
	char rev[10090] = { "" };

	char keywords_amplifier[2000] = { "" };
	char keywords_negating[2000] = { "" };
	char keywords_negative[2000] = { "" };
	char keywords_positive[2000] = { "" };
    
    int P_morethan_N_with_PR = 0;
    int P_morethan_N_with_NR = 0;
    int P_equal_N_with_PR = 0;
    int P_equal_N_with_NR = 0;
    int P_lessthan_N_with_PR = 0;
    int P_lessthan_N_with_NR = 0;

    float P_morethan_N_PositiveProbability = 0.0f;
    float P_equalthan_N_PositiveProbability = 0.0f;
    float P_lessthan_N_PositiveProbability = 0.0f;

	fscanf(kwAmpFile, "%s", &input);
	do {
		strcat(keywords_amplifier, input);
		strcat(keywords_amplifier, " ");
		clearInput(input);
		fscanf(kwAmpFile, "%s", &input);
	} while (strstr(input, "fileend") == NULL);
	printf("Amplifiers: %s\n", keywords_amplifier);
	fclose(kwAmpFile);
	clearInput(input);
	
	fscanf(kwNegatingFile, "%s", &input);
	do {
		strcat(keywords_negating, input);
		strcat(keywords_negating, " ");
		clearInput(input);
		fscanf(kwNegatingFile, "%s", &input);
	} while (strstr(input, "fileend") == NULL);
	printf("Negating: %s\n", keywords_negating);
	fclose(kwNegatingFile);
	clearInput(input);

	fscanf(kwNegativeFile, "%s", &input);
	do {
		strcat(keywords_negative, input);
		strcat(keywords_negative, " ");
		clearInput(input);
		fscanf(kwNegativeFile, "%s", &input);
	} while (strstr(input, "fileend") == NULL);
	printf("Negatives: %s\n", keywords_negative);
	fclose(kwNegativeFile);
	clearInput(input);

	fscanf(kwPosFile, "%s", &input);
	do {
		strcat(keywords_positive, input);
		strcat(keywords_positive, " ");
		clearInput(input);
		fscanf(kwPosFile, "%s", &input);
	} while (strstr(input, "fileend") == NULL);
	fclose(kwPosFile);
	printf("Positives: %s\n", keywords_positive);
	
	//Cycle through words in review input file
	do {
		rev[0] = '\0';

		positivecount = negativecount = keywordcount =0.0f;
		clearInput(input);
		fscanf(reviewfile, "%s", &input);
		//printf("input: %s |", input);
		if (strstr(input, "name") !=NULL) {
			fscanf(reviewfile, "%s", &Name);
			//printf("name: %s |", Name);
		}
		else if (strstr(input, "review")!=NULL) {
			fscanf(reviewfile, "%s", &input);

			while (strstr(input, "overallrating") == NULL) {
				stringlwr(input);
				strcat(input, " ");
				//printf("r:%s|", input);

				//Negated Words
				if (strstr(keywords_negating, input) != NULL) {
					//printf("Negating with %s", input);
					strcat(rev, input);
					fscanf(reviewfile, "%s", &input);
					strcat(input, " ");
					if (strstr(keywords_negative, input) != NULL) {
						++positivecount;
						++keywordcount;
						//printf(" negative of %s", input);
					}
					else if (strstr(keywords_positive, input) != NULL) {
						++negativecount;
						++keywordcount;
						//printf(" positive of %s", input);
					}
				}

				// Amplified Words
				else if (strstr(keywords_amplifier, input) != NULL) {
					//printf("Amped with %s", input);
					strcat(rev, input);
					fscanf(reviewfile, "%s", &input);
					strcat(input, " ");
					if (strstr(keywords_negative, input) != NULL) {
						//printf("neg of %s", input);
						++negativecount;
						++keywordcount;
					}
					else if (strstr(keywords_positive, input) != NULL) {
						//printf("pos of %s", input);
						++positivecount;
						++keywordcount;
					}
				}

				// Standard Words
				else if (strstr(keywords_positive, input) != NULL) {
					//printf("Positive with %s", input);
					++positivecount;
					++keywordcount;
				}
				else if (strstr(keywords_negative, input) != NULL) {
					//printf("Negative with %s", input);
					++negativecount;
					++keywordcount;
				}
					strcat(rev, input);
					fscanf(reviewfile, "%s", &input);
			}

			//printf("text: %s", rev);
		}if (strstr(input, "overallrating") != NULL) {
			fscanf(reviewfile, "%d", &rating);
			
            
            if (positivecount > negativecount){
                if (rating >=3){
                P_morethan_N_with_PR++;
                    printf("\nP>N_PR\n");
                }
                else{
                    P_morethan_N_with_NR++;
               printf("\nP>N_NR\n");
                }
            }
            else if (positivecount < negativecount){
                if (rating >= 3){
                    P_lessthan_N_with_PR++;
                    //printf("\n\nNeg! %d stars %d pos - %d neg\n\n", rating, positivecount, negativecount);
                    printf("\nP<N_PR\n");
                }
                else{
                    P_lessthan_N_with_NR++;
               //printf("\n\nNegx2! %d stars %d pos - %d neg\n\n", rating, positivecount, negativecount);
                    printf("\nP<N_NR\n");
                }
            }
            else {
                if (rating >= 3){
                    P_equal_N_with_PR++;
                printf("\nP=N_PR\n");
                }
                else{
                    P_equal_N_with_NR++;
                printf("\nP=N_NR\n");
                }
                
            
            }

			//printf("or: %d |", rating);
			//if (expert_rating == 0.50f) {
            
            printf("Name: %s\nReview: %s\nRating: %d\nKeywords: %d, Positive-Negative: %d-%d\n ", Name, rev, rating, keywordcount, positivecount, negativecount);
            
			//printf("%s\n\n", expertsys_rating(expert_rating));
			//}
			//-----FOR EXPERT OUTPUT-----//
            fprintf(fn, "Name: %s\nReview: %s\nRating: %d\nKeywords: %d, Positive-Negative: %d-%d\n", Name, rev, rating, keywordcount, positivecount, negativecount);
            

			//-----FOR SVM PARSE-----//
			//fprintf(fn, "%d\n%s\nend\n", rating, rev);
		}
    

        
	} while (strstr(input, "fileend")==NULL);
    
    //(Probability when Pos. gt Neg.: %f Probability when Pos. tt Neg.: %f Probability when Pos. eq Neg.: %f) P_morethan_N_PositiveProbability, P_equalthan_N_PositiveProbability, P_lessthan_N_PositiveProbability);
	//fprintf(fn, "\n\nCorrect for positive, 3 stars: %d, Incorrect: %d. Accuracy: %f\n Undetermined: %d. Total: %d", correct, incorrect, (float)correct / (float)(incorrect + correct + undetermined), undetermined, undetermined + correct + incorrect);
    P_equalthan_N_PositiveProbability = (float)(P_equal_N_with_PR) / (float)( P_equal_N_with_NR + P_equal_N_with_PR);
    P_lessthan_N_PositiveProbability = (float)P_lessthan_N_with_PR / (float)(P_lessthan_N_with_PR + P_lessthan_N_with_NR);
    P_morethan_N_PositiveProbability = (float)P_morethan_N_with_PR / (float)( P_morethan_N_with_PR + P_morethan_N_with_NR);
    printf("\n===================Below are Total number of reviews under three cases=================== \
           \nCase 1: Positive > Negative \
           \nCase 2: Positive = Negative \
           \nCase 3: Positive < Negative\n");
           
    printf("\nCase 1: Total Positive Reviews - %d       Total Negative Reviews: %d", P_morethan_N_with_PR, P_morethan_N_with_NR);
    printf("\nCase 2: Total Positive Reviews - %d       Total Negative Reviews: %d", P_equal_N_with_PR, P_equal_N_with_NR);
    printf("\nCase 3: Total Positive Reviews - %d       Total Negative Reviews: %d\n", P_lessthan_N_with_PR, P_lessthan_N_with_NR);

    printf("\nProbability of Postive Reviews: %f\
           \nProbability of Nuetral Reviews: %f\
           \nProbability of Negative Reviews: %f \n" \
           , P_morethan_N_PositiveProbability, P_equalthan_N_PositiveProbability, P_lessthan_N_PositiveProbability);
    
    fprintf(fn, "\n===================Below are Total number of reviews under three cases=================== \
            \nCase 1: Positive > Negative \
            \nCase 2: Positive = Negative \
            \nCase 3: Positive < Negative\n" );
    fprintf(fn, "\nCase 1: Total Positive Reviews - %d       Total Negative Reviews: %d", P_morethan_N_with_PR, P_morethan_N_with_NR);
    fprintf(fn, "\nCase 2: Total Positive Reviews - %d       Total Negative Reviews: %d", P_equal_N_with_PR, P_equal_N_with_NR);
    fprintf(fn, "\nCase 3: Total Positive Reviews - %d       Total Negative Reviews: %d\n", P_lessthan_N_with_PR, P_lessthan_N_with_NR);
    fprintf(fn, "\nProbability of Postive Reviews: %f\
           \nProbability of Nuetral Reviews: %f\
           \nProbability of Negative Reviews: %f \n" \
           , P_morethan_N_PositiveProbability, P_equalthan_N_PositiveProbability, P_lessthan_N_PositiveProbability);
    
    P_morethan_N_PositiveProbability = fabs(P_morethan_N_PositiveProbability - ideal_prating);
    P_lessthan_N_PositiveProbability = fabs(P_lessthan_N_PositiveProbability - ideal_nrating);
    P_equalthan_N_PositiveProbability = fabs(P_equalthan_N_PositiveProbability - ideal_erating);
    
    
    printf("P>N: %f P=N: %f P<N: %f", P_morethan_N_PositiveProbability, P_equalthan_N_PositiveProbability, P_lessthan_N_PositiveProbability);
    
    Average = ((P_morethan_N_PositiveProbability + P_lessthan_N_PositiveProbability + P_equalthan_N_PositiveProbability) /3.0);
    
    Accuracy = (1 - Average);
    printf("\nAccuracy:%f\n", Accuracy);
    
	
	fclose(fn);
	fclose(reviewfile);
	return 0;
}
