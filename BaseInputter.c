#include <stdio.h>

int main() {
	FILE *reviewfile = fopen("/review_input.txt", "r");
	FILE *fn = fopen("/review_output.txt", "w+");
	if (fn == NULL) {
		printf("Error opening output file!\n");
		exit(1);
	}
	char Name[] = { "" }, Review[] = { "" };
	char test[] = {""};
	char rev[1000] = { "" };
	int rating =  0;
	fprintf(fn, "Start\n");
	printf("Starting...");
	char input[]= {""};
	do {
		rev[0] = '\0';
		fscanf(reviewfile, "%s", &input);
		printf("input: %s |", input);
		if (strstr(input, "name") !=NULL) {
			fscanf(reviewfile, "%s", &Name);
			printf("name: %s |", Name);
		}
		else if (strstr(input, "review")!=NULL) {
			fscanf(reviewfile, "%s", &input);
			while (strstr(input, "overallrating")==NULL) {
				printf("r: %s |", input);
				strcat(rev, input);
				strcat(rev, " ");
				fscanf(reviewfile, "%s", &input);
			}
			printf("text: %s", rev);
		}if (strstr(input, "overallrating")!=NULL) {
			fscanf(reviewfile, "%d", &rating);
			printf("or: %d |", rating);

		}
		
	} while (input != EOF);
	fprintf(fn, "Name: %s Rev: %s rating: %d", test, rev, rating);
	fscanf(reviewfile, "name %s review %[^\n]%*c overall\": %d", &Name, &Review, &rating);
	printf("%s said %s and rated %d", Name, Review, rating);
	

	fclose(fn);
	fclose(reviewfile);

	return 0;
}