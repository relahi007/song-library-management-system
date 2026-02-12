/* Songs Database Management System */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 100
#define DB_FILE "songs.txt"

/* Song structure holds all information about a single song */
typedef struct {
    char title[MAX_STR], performer[MAX_STR], album[MAX_STR], genre[MAX_STR];
    int year, dur_min, dur_sec;
} Song;

/* ========== UTILITY FUNCTIONS ========== */

/* Removes trailing newline from string (used after fgets) */
void cleaning_input(char *str) {
    int len;
    if (!str) return;
    len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

/* Clears input buffer to prevent leftover input */
void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Converts string to lowercase for case-insensitive comparisons */
void string_lower(char *dest, const char *src) {
    int i;
    for (i = 0; src[i]; i++) dest[i] = tolower(src[i]);
    dest[i] = '\0';
}

/* Validates genre against list of accepted genres (case-insensitive) */
int check_genre(const char *g) {
    static const char *VALID_GENRES[] = { "Rock", "Pop", "Hip-Hop", "Jazz", "Classical", "Electronic", "Country", "R&B", "Metal", "Folk", "Blues", "Other", NULL };
    int i = 0;
    char in[MAX_STR], val[MAX_STR];
    string_lower(in, g);
    while (VALID_GENRES[i]) {
        string_lower(val, VALID_GENRES[i]);
        if (strcmp(in, val) == 0) return 1;
        i++;
    }
    return 0;
}

/* ========== DATABASE MANAGEMENT ========== */

/* Adds a new song to the dynamic array using realloc */
void add_to_db(Song **db, int *count, Song new_song) {
    Song *temp;
    temp = (Song*)realloc(*db, (*count + 1) * sizeof(Song));
    if (temp == NULL) {
        printf("Memory Error! Cannot add song.\n");
        return;
    }
    *db=temp;
    (*db)[*count]=new_song;
    (*count)++;

}

/* ========== FILE I/O ========== */

/* Loads songs from file. Returns number of songs loaded */
int load_db(Song **db) {
    FILE *fp = fopen(DB_FILE, "r");
    Song s;
    char buffer[MAX_STR];
    int count = 0;

    if (*db) { free(*db); *db = NULL; }

    printf("Loading database from '%s'...\n", DB_FILE);
    if (!fp) {
        printf("File '%s' not found. Starting with an empty database.\n", DB_FILE);
        return 0;
    }

    /* Read each song: title, performer, album, year, genre, duration */
    while (fgets(s.title, MAX_STR, fp)) {
        cleaning_input(s.title);
        if (strlen(s.title) == 0) continue;

        fgets(s.performer, MAX_STR, fp); cleaning_input(s.performer);
        fgets(s.album, MAX_STR, fp);     cleaning_input(s.album);
        fgets(buffer, MAX_STR, fp);      s.year = atoi(buffer);
        fgets(s.genre, MAX_STR, fp);     cleaning_input(s.genre);
        fgets(buffer, MAX_STR, fp);      sscanf(buffer, "%d:%d", &s.dur_min, &s.dur_sec);

        add_to_db(db, &count, s);
    }
    fclose(fp);
    return count;
}

/* Saves all songs to file */
void save_db(Song *db, int count) {
    FILE *fp = fopen(DB_FILE, "w");
    int i;
    if (!fp) { printf("Error saving file.\n"); return; }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%s\n%s\n%s\n%d\n%s\n%d:%02d\n\n",
            db[i].title, db[i].performer, db[i].album,
            db[i].year, db[i].genre, db[i].dur_min, db[i].dur_sec);
    }
    fclose(fp);
    printf("Database saved (%d songs)!\n", count);
}

/* ========== USER FEATURES ========== */

/* Prompts user to input a new song with validation */
void input_song(Song **db, int *count) {
    Song s;
    char buf[MAX_STR];
    printf("\n--- ADD SONG ---\n");

    while (1) { printf("Title: "); fgets(s.title, MAX_STR, stdin); cleaning_input(s.title); if (strlen(s.title) > 0) break; }
    while (1) { printf("Performer: "); fgets(s.performer, MAX_STR, stdin); cleaning_input(s.performer); if (strlen(s.performer) > 0) break; }
    while (1) { printf("Album: "); fgets(s.album, MAX_STR, stdin); cleaning_input(s.album); if (strlen(s.album) > 0) break; }
    while (1) { printf("Year (1900-2100): "); fgets(buf, MAX_STR, stdin); s.year = atoi(buf); if (s.year >= 1900 && s.year <= 2100) break; }
    while (1) {
        printf("Genre: "); fgets(s.genre, MAX_STR, stdin); cleaning_input(s.genre);
        if (check_genre(s.genre)) break;
        printf("Invalid genre.\n");
    }
    while (1) {
        printf("Duration (MM:SS): "); fgets(buf, MAX_STR, stdin);
        if (sscanf(buf, "%d:%d", &s.dur_min, &s.dur_sec) == 2 && s.dur_sec < 60) break;
    }

    add_to_db(db, count, s);
    printf("Song added!\n");
}

/* Lists all songs in the database */
void list_all_songs(Song *db, int count) {
    int i;
    
    if (count == 0) {
        printf("\nDatabase is empty. No songs to display.\n");
        return;
    }
    
    printf("\n--- ALL SONGS IN DATABASE (%d total) ---\n", count);
    for (i = 0; i < count; i++) {
        printf("\n%d. %s\n", i + 1, db[i].title);
        printf("   Artist: %s\n", db[i].performer);
        printf("   Album: %s (%d)\n", db[i].album, db[i].year);
        printf("   Genre: %s | Duration: %d:%02d\n", db[i].genre, db[i].dur_min, db[i].dur_sec);
    }
    printf("\n");
}

/* Searches and displays all songs by a specific artist */
void find_artist(Song *db, int count) {
    char q[MAX_STR], q_l[MAX_STR], cur_l[MAX_STR];
    int i, found = 0;

    printf("Artist name: "); fgets(q, MAX_STR, stdin); cleaning_input(q); string_lower(q_l, q);

    for (i = 0; i < count; i++) {
        string_lower(cur_l, db[i].performer);
        if (strcmp(q_l, cur_l) == 0) {
            printf("- %s \n(Album: %s) [%d]\n", db[i].title, db[i].album, db[i].year);
            found = 1;
        }
    }
    if (!found) printf("No songs found.\n");
}

/* Displays all songs from a specific album and calculates total duration */
void find_album(Song *db, int count) {
    char artist[MAX_STR], album[MAX_STR], artist_lower[MAX_STR], album_lower[MAX_STR], current_artist[MAX_STR], current_album[MAX_STR];
    int i, found = 0, t_sec = 0;

    printf("Artist: "); fgets(artist, MAX_STR, stdin); cleaning_input(artist); string_lower(artist_lower, artist);
    printf("Album: "); fgets(album, MAX_STR, stdin); cleaning_input(album); string_lower(album_lower, album);

    printf("\n--- Songs in '%s' ---\n", album);
    for (i = 0; i < count; i++) {
        string_lower(current_artist, db[i].performer);
        string_lower(current_album, db[i].album);
        if (strcmp(artist_lower, current_artist) == 0 && strcmp(album_lower, current_album) == 0) {
            printf("%d. %s (%d:%02d)\n", ++found, db[i].title, db[i].dur_min, db[i].dur_sec);
            t_sec += (db[i].dur_min * 60) + db[i].dur_sec;
        }
    }
    if (found) printf("Total Duration: %d:%02d\n", t_sec / 60, t_sec % 60);
    else printf("Nothing found.\n");
}

/* Displays all songs released in a specific year */
void find_year(Song *db, int count) {
    int year, i, found = 0;
    char buf[MAX_STR];

    printf("Year: "); fgets(buf, MAX_STR, stdin); year = atoi(buf);

    printf("\n--- Songs from %d ---\n", year);
    for (i = 0; i < count; i++) {
        if (db[i].year == year) {
            printf("- %s (%s)\n", db[i].title, db[i].performer);
            found = 1;
        }
    }
    if (!found) printf("Nothing found.\n");
}

/* Displays all songs of a specific genre */
void find_genre(Song *db, int count) {
    char genre[MAX_STR], genre_l[MAX_STR], cur_l[MAX_STR];
    int i, found = 0;

    printf("Genre: "); fgets(genre, MAX_STR, stdin); cleaning_input(genre); string_lower(genre_l, genre);

    printf("\n--- Songs in '%s' ---\n", genre);
    for (i = 0; i < count; i++) {
        string_lower(cur_l, db[i].genre);
        if (strcmp(genre_l, cur_l) == 0) {
            printf("- %s (%s)\n", db[i].title, db[i].performer);
            found = 1;
        }
    }
    if (!found) printf("Nothing found.\n");
}

/* Creates a custom playlist file with user-selected songs */
void create_playlist(Song *db, int count) {
    char filename[MAX_STR], title[MAX_STR], title_l[MAX_STR], cur_l[MAX_STR];
    FILE *fp;
    int i, found, added = 0;

    printf("Enter playlist filename (e.g. my_playlist.txt): ");
    fgets(filename, MAX_STR, stdin); cleaning_input(filename);

    fp = fopen(filename, "w");
    if (!fp) { printf("Error creating file.\n"); return; }

    printf("Enter song titles to add (type 'done' to finish):\n");
    while (1) {
        printf("> "); fgets(title, MAX_STR, stdin); cleaning_input(title);
        if (strcmp(title, "done") == 0) break;

        string_lower(title_l, title);
        found = 0;
        for (i = 0; i < count; i++) {
            string_lower(cur_l, db[i].title);
            if (strcmp(title_l, cur_l) == 0) {
                fprintf(fp, "%s\n%s\n%s\n%d\n%s\n%d:%02d\n\n",
                    db[i].title, db[i].performer, db[i].album,
                    db[i].year, db[i].genre, db[i].dur_min, db[i].dur_sec);
                printf("Added '%s' to playlist.\n", db[i].title);
                found = 1;
                added++;
            }
        }
        if (!found) printf("Song not found.\n");
    }
    fclose(fp);
    printf("Playlist '%s' created with %d songs.\n", filename, added);
}

/* Views and displays all songs from an existing playlist file */
void view_playlist(void) {
    char filename[MAX_STR];
    Song s;
    char buffer[MAX_STR];
    FILE *fp;
    int count = 0, t_sec = 0;

    printf("Enter playlist filename: ");
    fgets(filename, MAX_STR, stdin); cleaning_input(filename);

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Playlist file '%s' not found.\n", filename);
        return;
    }

    printf("\n--- Songs in '%s' ---\n", filename);
    
    /* Read and display each song from playlist */
    while (fgets(s.title, MAX_STR, fp)) {
        cleaning_input(s.title);
        if (strlen(s.title) == 0) continue;

        fgets(s.performer, MAX_STR, fp); cleaning_input(s.performer);
        fgets(s.album, MAX_STR, fp);     cleaning_input(s.album);
        fgets(buffer, MAX_STR, fp);      s.year = atoi(buffer);
        fgets(s.genre, MAX_STR, fp);     cleaning_input(s.genre);
        fgets(buffer, MAX_STR, fp);      sscanf(buffer, "%d:%d", &s.dur_min, &s.dur_sec);

        count++;
        printf("%d. %s - %s\n", count, s.title, s.performer);
        printf("   Album: %s (%d) | Genre: %s | Duration: %d:%02d\n", 
               s.album, s.year, s.genre, s.dur_min, s.dur_sec);
        
        t_sec += (s.dur_min * 60) + s.dur_sec;
    }
    fclose(fp);

    if (count > 0)
        printf("\nTotal: %d songs | Total Duration: %d:%02d\n", count, t_sec / 60, t_sec % 60);
    else
        printf("Playlist is empty.\n");
}

/* ========== MAIN PROGRAM ========== */

int main(void) {
    Song *database = NULL;  /* Dynamic array of songs */
    int count = 0;          /* Number of songs */
    int choice, run = 1, unsaved = 0;
    char check;

    count = load_db(&database);
    printf("Loaded %d songs.\n", count);

    /* Main menu loop */
    while (run) {
        printf("\n--- SONGS DATABASE MANAGEMENT SYSTEM ---\n");
        printf("Current database: %d song(s) loaded\n", count);
        printf("1. List all songs\n2. Add new song\n3. Display all songs by artist\n4. Display songs from an album\n5. List songs by release year\n6. List songs by genre\n7. Save database\n8. Create new playlist\n9. View existing playlist\n10. Exit program\n\nEnter your choice (1-10): ");
        if (scanf("%d", &choice) != 1) { flush_stdin(); continue; }
        flush_stdin();

        switch (choice) {
            case 1: list_all_songs(database, count); break;
            case 2: input_song(&database, &count); unsaved = 1; break;
            case 3: find_artist(database, count); break;
            case 4: find_album(database, count); break;
            case 5: find_year(database, count); break;
            case 6: find_genre(database, count); break;
            case 7: save_db(database, count); unsaved = 0; break;
            case 8: create_playlist(database, count); break;
            case 9: view_playlist(); break;
            case 10:
                if (unsaved) {
                    printf("Unsaved changes. Save? (y/n): ");
                    scanf("%c", &check);
                    if (check == 'y' || check == 'Y') save_db(database, count);
                }
                run = 0; break;
            default: printf("Invalid.\n");
        }
    }

    if (database) free(database);
    return 0;
}