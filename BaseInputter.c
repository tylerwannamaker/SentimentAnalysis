#include <stdio.h>
#include <string.h>
#include<ctype.h>

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
	


	int rating =  0;
	float weighted_rating = 0.0f;
	float expert_rating = 0.0f;
	int positivecount = 0;
	int negativecount = 0;
	int keywordcount = 0;
	int correct = 0;
	int incorrect = 0;
	int undetermined = 0;

	char Name[50] = { "" };
	char input[8000]= {""};
	char rev[10090] = { "" };

	char keywords_amplifier[2000] = { "" };
	char keywords_negating[2000] = { "" };
	char keywords_negative[2000] = { "" };
	char keywords_positive[2000] = { "" };


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

		positivecount = negativecount = keywordcount = expert_rating = weighted_rating =0.0f;
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
						weighted_rating += pos;
						//printf(" negative of %s", input);
					}
					else if (strstr(keywords_positive, input) != NULL) {
						++negativecount;
						++keywordcount;
						weighted_rating += neg;
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
						weighted_rating += amp_neg;
					}
					else if (strstr(keywords_positive, input) != NULL) {
						//printf("pos of %s", input);
						++positivecount;
						++keywordcount;
						weighted_rating += amp_pos;
					}
				}

				// Standard Words
				else if (strstr(keywords_positive, input) != NULL) {
					//printf("Positive with %s", input);
					++positivecount;
					++keywordcount;
					weighted_rating += pos;
				}
				else if (strstr(keywords_negative, input) != NULL) {
					//printf("Negative with %s", input);
					++negativecount;
					++keywordcount;
					weighted_rating += neg;
				}
					strcat(rev, input);
					fscanf(reviewfile, "%s", &input);
			}

			//printf("text: %s", rev);
		}if (strstr(input, "overallrating") != NULL) {
			fscanf(reviewfile, "%d", &rating);
			expert_rating = weighted_rating / (float)keywordcount;
			if (keywordcount == 0) {
				expert_rating = 0.5f;
			}
			if ((rating >= 3 && expert_rating > 0.50f) || (rating <= 2 && expert_rating < 0.50f)) {
				++correct;
			}
			else if (expert_rating == 0.50f) {
				++undetermined;
			}
			else {
				++incorrect;
			}

			//printf("or: %d |", rating);
			//if (expert_rating == 0.50f) {
			printf("Name: %s\nReview: %s\nRating: %d\nKeywords: %d, Positive-Negative: %d-%d\nComputed Rating:%f\n", Name, rev, rating, keywordcount, positivecount, negativecount, expert_rating);
			printf("%s\n\n", expertsys_rating(expert_rating));
			//}
			//-----FOR EXPERT OUTPUT-----//
			fprintf(fn, "Name: %s\nReview: %s\nRating: %d\nKeywords: %d, Positive-Negative: %d-%d\nComputed Rating:%f\n\n", Name, rev, rating, keywordcount, positivecount, negativecount,expert_rating);

			//-----FOR SVM PARSE-----//
			//fprintf(fn, "%d\n%s\nend\n", rating, rev);
		}
		
	} while (strstr(input, "fileend")==NULL);
	printf("\n\nCorrect for positive, 3 stars: %d, Incorrect: %d. Accuracy: %f\n Undetermined: %d. Total: %d", correct, incorrect,(float)correct/(float)(incorrect+correct+undetermined), undetermined, undetermined+correct+incorrect);
	//fprintf(fn, "\n\nCorrect for positive, 3 stars: %d, Incorrect: %d. Accuracy: %f\n Undetermined: %d. Total: %d", correct, incorrect, (float)correct / (float)(incorrect + correct + undetermined), undetermined, undetermined + correct + incorrect);

	char ch = getchar();
	fclose(fn);
	fclose(reviewfile);
	return 0;
}
