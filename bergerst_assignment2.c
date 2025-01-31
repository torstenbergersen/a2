/*
Program name: bergest_assignment2.c
Author: Torsten Bergersen
This program reads a CSV file with movie data, processes the data into stucts, creates a linked lists of the structs, provides the user with questions to ask about the data, and then prints out the movie data per user choice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie* processMovieFile(char* filePath);
void askQuestions();
void printQuestions();
void moviesReleased();
void highestRated();
void moviesOfLanguages();

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
    
    // linked list testing
    // struct movie* current = head;
    // while (current != NULL) {
    //     printf("Movie: %s\n", current->name);
    //     printf("Year: %d\n", current->year);
    //     printf("Languages: ");

    //     // Print languages
    //     for (int i = 0; i < 5; i++) {
    //         if (current->languages[i] != NULL) {
    //             printf("%s ", current->languages[i]);
    //         }
    //     }
    //     printf("\n");

    //     printf("Rating: %.1f\n", current->rating);
    //     printf("---------------------\n");

    //     // Move to the next movie in the linked list
    //     current = current->next;
    // }

    askQuestions();

    return 0;
};

/* basic structure for following function sourced from provided file for assigment (movies.c) */ 
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
    printf("\nProcessed file %s and parsed data for %d movies\n\n", filePath, movies);
    return head;
};

void askQuestions() {
    int answer;
    while (answer != 4) {
        if (answer < 1 || answer > 4) {
            printf("You entered an incorrect choice. Try again.\n\n");
        }
        printQuestions();
        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &answer);
        if (answer == 1) {
            moviesReleased();
        }
        else if (answer == 2) {
            highestRated();
        }
        else if (answer == 3) {
            moviesOfLanguages();
        }
    }
    return;
}

void printQuestions() {
    printf("1. Show movies released in the specified year\n"
           "2. Show highest rated movie for each year\n"
           "3. Show the title and year of release of all movies in a specific" "language\n"
           "4. Exit from the program\n\n");
};

void moviesReleased() {

};

void highestRated() {

};

void moviesOfLanguages() {

};
