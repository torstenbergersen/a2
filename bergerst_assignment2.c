/*
Program name: bergest_assignment2.c
Author: Torsten Bergersen
This program reads a CSV file with movie data, processes the data into stucts, creates a linked lists of the structs, provides the user with questions to ask about the data, and then prints out the movie data per user choice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// for min and max ints
#include <limits.h>

struct movie* processMovieFile(char* filePath);
void askQuestions(struct movie* head);
void printQuestions();
void moviesReleased(struct movie* head);
void highestRated(struct movie* head);
void moviesOfLanguages(struct movie* head);

struct movie{
    char* name;
    int year;
    char* languages[5];
    double rating;
    struct movie* next;
};

int main (int argc, char **argv){
    
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return 1;
    }
    struct movie* head = processMovieFile(argv[1]);

    askQuestions(head);

    return 0;
};

/* 
basic structure for reading csv from provided file for assigment (movies.c) 
*/ 
struct movie* processMovieFile(char* filePath){
    char *currLine = NULL;
    size_t len = 0;

    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    // amount of movies (-1 to account for title line)
    int movies = -1;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // Read the file line by line
    while(getline(&currLine, &len, movieFile) != -1) {
        if (movies != -1) {
            struct movie* newMovie = malloc(sizeof(struct movie));
            if (newMovie == NULL) {
                printf("Memory allocation failed\n");
                exit(1);
            }

            // tokenizer for line parsing
            char* saveptr1;
            char* token = strtok_r(currLine, ",", &saveptr1);

            // store movie name
            newMovie->name = strdup(token);

            // store release year
            token = strtok_r(NULL, ",", &saveptr1);
            newMovie->year = atoi(token);

            // process languages
            token = strtok_r(NULL, ",", &saveptr1);
            char* saveptr2;
            char* langToken = strtok_r(token, ";", &saveptr2);
            int i = 0;
            while (langToken != NULL) {
                // remove leading or trailing '[ ]'
                if (langToken[0] == '[') {
                    langToken++; 
                }
                size_t len = strlen(langToken);
                if (len > 0 && langToken[len - 1] == ']') {
                    langToken[len - 1] = '\0';  // Replace ']' with null terminator
                }

                newMovie->languages[i] = strdup(langToken);
                i++;
                langToken = strtok_r(NULL, ";", &saveptr2);
            }
            // fill remaining slots with NULL
            for (; i < 5; i++) {
                newMovie->languages[i] = NULL;
            }

            // store rating
            token = strtok_r(NULL, ",", &saveptr1);
            newMovie->rating = atof(token);

            // append the movie to the linked list
            if(head == NULL){
                head = newMovie;
                tail = newMovie;
            } else{
                tail->next = newMovie;
                tail = newMovie;
            }
        }   
        movies++;
    }

    // Free the memory allocated by getline for currLine
    free(currLine);
    // Close the file
    fclose(movieFile);
    printf("\nProcessed file %s and parsed data for %d movies\n", filePath, movies);
    return head;
};

void askQuestions(struct movie* head) {
    int answer;
    while (answer != 4) {
        if (answer < 1 || answer > 4) {
            printf("You entered an incorrect choice. Try again.\n");
        }
        printQuestions();
        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &answer);
        if (answer == 1) {
            moviesReleased(head);
        }
        else if (answer == 2) {
            highestRated(head);
        }
        else if (answer == 3) {
            moviesOfLanguages(head);
        }
    }
    return;
}

void printQuestions() {
    printf("\n1. Show movies released in the specified year\n"
           "2. Show highest rated movie for each year\n"
           "3. Show the title and year of release of all movies in a specific" "language\n"
           "4. Exit from the program\n\n");
};

void moviesReleased(struct movie* head) {
    int year;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    // bool int for presence of data
    int present = 0;
    while (head != NULL) {
        if (head->year == year) {
            printf("%s\n", head->name);
            present = 1;
        }
        head = head->next;
    };
    if (present == 0) {
        printf("No data about movies released in the year %d\n", year);
    };
};

void highestRated(struct movie* head) {

    int min_year = INT_MAX;
    int max_year = INT_MIN;

    struct movie* temp = head;
    while (temp != NULL) {
        if (temp->year < min_year) min_year = temp->year;
        if (temp->year > max_year) max_year = temp->year;
        temp = temp->next;
    }

    // movie year range
    int range = max_year - min_year + 1;

    // array to store best movies per year
    struct movie** best_movies = calloc(range, sizeof(struct moive*));

    while (head != NULL) {
        // map year to array index
        int index = head->year - min_year;
        if (best_movies[index] == NULL || head->rating > best_movies[index]->rating) {
            best_movies[index] = head;
        }
        head = head->next;
    }

    for (int i = 0; i < range; i++) {
        if (best_movies[i] != NULL) {
            printf("%d %.1f %s\n", best_movies[i]->year, best_movies[i]->rating, best_movies[i]->name);
        }
    }

    // cleanup
    free(best_movies); 

};

void moviesOfLanguages(struct movie* head) {
    char language[20];
    printf("Enter the language for which you want to see movies: ");
    scanf("%s", language);

    // bool int for presence of data
    int present = 0;
    while (head != NULL) {
        // search for language in each movie struct
        for (int i = 0; i < 5 && head->languages[i] != NULL; i++) {
            if (strcmp(head->languages[i], language) == 0) {
                printf("%d %s\n", head->year, head->name);
                present = 1;
            }
        };
        head = head->next;
    };
    if (present == 0) {
        printf("No data about movies released in %s\n", language);
    };
};
