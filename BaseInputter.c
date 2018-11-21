#include <stdio.h>
#define positivethreshhold 3

void clearInput(char *input) {
	input[0] = '\0';
}

int main() {
	FILE *reviewfile = fopen("review_input.txt", "r");
	FILE *kwAmpFile = fopen("Keywords/Amplifier_Keywords.txt", "r");
	FILE *kwNegatingFile = fopen("Keywords/Negating_Keywords.txt", "r");
	FILE *kwNegativeFile = fopen("Keywords/Negative_Keywords.txt", "r");
	FILE *kwPosFile = fopen("Keywords/Positive_Keywords.txt", "r");
	FILE *fn= fopen("review_output.txt", "w");
	if (fn == NULL) {
		printf("Error opening output file!\n");
		exit(1);
	}

	int rating =  0;
	float weighted_rating = 0.0f;
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
		positivecount = negativecount = keywordcount = 0;
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
				strcat(input, " ");
				//printf("r:%s|", input);
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
		}if (strstr(input, "overallrating")!=NULL) {
			fscanf(reviewfile, "%d", &rating);
			//printf("or: %d |", rating);
			printf("Name: %s\nReview: %s\nRating: %d\nPositive(Rating 3 or above)? %s\nKeywords: %d, Positive-Negative: %d-%d\n\n", Name, rev, rating, (rating >= positivethreshhold) ? "Yes" : "No", keywordcount, positivecount, negativecount);
			fprintf(fn, "Name: %s\nReview: %s\nRating: %d\nPositive(Rating 3 or above)? %s\nKeywords: %d, Positive-Negative: %d-%d\n\n", Name, rev, rating, (rating >= positivethreshhold) ? "Yes" : "No", keywordcount, positivecount, negativecount);
		}
		
	} while (strstr(input, "fileend")==NULL);
	char ch = getchar();
	fclose(fn);
	fclose(reviewfile);
	return 0;
}
