#include <stdio.h>
#define positivethreshhold 3
int main() {
	FILE *reviewfile = fopen("review_input.txt", "r");
	FILE *fn= fopen("review_output.txt", "w");
	if (fn == NULL) {
		printf("Error opening output file!\n");
		exit(1);
	}
	char Name[50] = {""};
	int rating =  0;
	fprintf(fn, "Start\n");
	char input[8000]= {""};
	char rev[10090] = { "" };
	//Cycle through words in review input file
	do {
		rev[0] = '\0';
		input[0] = '\0';
		fscanf(reviewfile, "%s", &input);
		//printf("input: %s |", input);
		if (strstr(input, "name") !=NULL) {
			fscanf(reviewfile, "%s", &Name);
			//printf("name: %s |", Name);
		}
		else if (strstr(input, "review")!=NULL) {
			fscanf(reviewfile, "%s", &input);
			while (strstr(input, "overallrating")==NULL) {
				//printf("r: %s |", input);
				strcat(rev, input);
				strcat(rev, " ");
				fscanf(reviewfile, "%s", &input);
			}
			//printf("text: %s", rev);
		}if (strstr(input, "overallrating")!=NULL) {
			fscanf(reviewfile, "%d", &rating);
			//printf("or: %d |", rating);
			printf("\nName: %s\nReview: %s\nRating: %d\nPositive(Rating 3 or above)? %s\n\n", Name, rev, rating,(rating>=positivethreshhold) ? "Yes":"No");
			fprintf(fn, "\nName: %s\nReview: %s\nRating: %d\nPositive(Rating 3 or above)? %s\n", Name, rev, rating, (rating >= positivethreshhold) ? "Yes" : "No");
		}
		
	} while (strstr(input, "fileend")==NULL);
	fclose(fn);
	fclose(reviewfile);
	return 0;
}