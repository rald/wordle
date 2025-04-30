#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define WORDS_FILE "words.txt"
#define STRING_MAX 256

char *removenl(char *line) {
    char *p = strchr(line, '\n');
    if (p) *p = '\0';
    return line;
}

char *strupr(char *s) {
    char *p = s;
    while (*p) {
        *p = toupper(*p);
        p++;
    }
    return s;
}

char *getrandword(char *filename) {
    static char selected[STRING_MAX];
    char line[STRING_MAX];
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    size_t n = 0;
    while (fgets(line, sizeof(line), file)) {
        if (rand() % (++n) == 0) {
            removenl(line);
            strncpy(selected, line, STRING_MAX);
        }
    }
    fclose(file);
    return n > 0 ? selected : NULL;
}

bool find(const char *word) {
    char line[STRING_MAX];
    FILE *fp = fopen(WORDS_FILE, "r");
    if (!fp) return false;
    while (fgets(line, STRING_MAX, fp)) {
        removenl(line);
        strupr(line);
        if (!strcmp(line, word)) {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}

void wordle(const char *randomWord, const char *guess) {
    char result[6] = {0};
    int used[5] = {0};
    if (strlen(guess) != 5) {
        puts("five lettered words only");
        return;
    }
    if (!find(guess)) {
        printf("word %s not in list\n", guess);
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (guess[i] == randomWord[i]) {
            result[i] = toupper(guess[i]);
            used[i] = 1;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (result[i] != 0) continue;
        for (int j = 0; j < 5; j++) {
            if (!used[j] && guess[i] == randomWord[j]) {
                result[i] = tolower(guess[i]);
                used[j] = 1;
                break;
            }
        }
        if (result[i] == 0) {
            result[i] = '?';
        }
    }
    printf("  %s\n", result);
}

int main() {
    srand(time(NULL));
    bool quit = false;
    char guess[STRING_MAX];
    char *randomWord = getrandword(WORDS_FILE);
    char n = 0;
    if (!randomWord) return -1;
    strupr(randomWord);
    while (!quit) {
        printf("> ");
        if (!fgets(guess, STRING_MAX, stdin)) break;
        removenl(guess);
        strupr(guess);
        wordle(randomWord, guess);
        if (!strcmp(randomWord, guess)) {
            puts("you got it");
            quit = true;
            continue;
        }
        if (++n == 6) {
            printf("it is %s\n", randomWord);
            quit = true;
        }
    }
    puts("game over");
    return 0;
}
