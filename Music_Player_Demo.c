#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#ifdef _WIN32
#include<windows.h>
#else
#includes<unistd.h>
#endif

#define MAX_SONG 10
#define MAX_SIZE 50

// Structure for song details

typedef struct Song{
    char title[MAX_SIZE];
    char album[MAX_SIZE];
    char id[MAX_SIZE];
    short int year;
    double duration;
} Song;
typedef struct Song *song_ptr;

song_ptr song_Box[MAX_SONG];
int pool_insert_index = 0;

typedef struct PlaylistNode *node_ptr;
typedef struct PlaylistNode{
    song_ptr song;
    node_ptr prev_song;
    node_ptr next_song;
} PlaylistNode;

node_ptr header_node = NULL, now_playing = NULL;

//Function to check if the song pool is empty
bool is_pool_empty(){
    return song_Box[0] == NULL;
}

//Function to check if playlist exists
bool does_playlist_exist(){
    return !(header_node == NULL);
}

void play_next_song(){
    if (does_playlist_exist() && now_playing != NULL){
        if (now_playing->next_song != NULL)
            now_playing = now_playing->next_song;
        else
            printf("REACHED END OF PLAYLIST");
    }
    else{
        printf("NO SONG ADDED TO PLAYLIST");
    }
}

void play_prev_song(){
    if (does_playlist_exist() && now_playing != NULL){
        if (now_playing->prev_song != NULL)
            now_playing = now_playing->prev_song;
        else
            printf("REACHED START OF PLAYLIST");
    }
    else{
        printf("NO SONG ADDED TO PLAYLIST");
    }
}
// Function to display song details if available or else propting to create playlist
void show_song_details(){
    if (now_playing == NULL || now_playing->song == NULL){
        printf("~~~~~~~~~~~~~~~~~~~~~~CREATE A PLAYLIST FIRST~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    else{
        printf("\n----------------------------------------------------------------------\n");
        printf("                          SONG DETAILES \n");
        printf("----------------------------------------------------------------------\n");
        printf("\t   TITLE    |   %s\n", now_playing->song->title);
        printf("\t   ALBUM    |   %s\n", now_playing->song->album);
        printf("\t   YEAR     |   %d\n", now_playing->song->year);
        printf("\t  DURATION  |   %0.2lf minutes\n", now_playing->song->duration);
        printf("----------------------------------------------------------------------\n");
    }
    char leave[MAX_SIZE];
    printf("<<<<< Enter any input to return to MAIN MENU\n");
    if (scanf("%s", leave)){
        return;
    }
}
// Displaying main_menu for Music Player Frame with all Menu 
void main_menu(){
    printf("\n----------------------------------------------------------------------\n");
    printf("                              MAIN MENU\n");
    printf("----------------------------------------------------------------------\n");

    if (now_playing == NULL || now_playing->song == NULL){
        printf("                          NO TRACK SELECTED\n");
    }
    else{
        printf("    NOW    |   %s\n", now_playing->song->title);
        printf("  PLAYING  |   %0.2lf minutes\n", now_playing->song->duration);
    }
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Action   \n");
    printf("----------------------------------------------------------------------\n");
    printf("   1   |  Create a new song\n");
    printf("   2   |  Display all available songs\n");
    if (does_playlist_exist())
    	printf("   3   |  Delete playlist\n");
    else
        printf("   3   |  Create a new playlist\n");
    printf("   4   |  Add a song to the playlist\n");
    printf("   5   |  Show playlist\n");
    printf("   6   |  Play previous track\n");
    printf("   7   |  Play next track\n");
    if (now_playing != NULL && now_playing->song != NULL)
        printf("   8   |  Show more information about the song playing\n");
    printf("  -1   |  Exit music player\n");
    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

void show_all_songs_raw(){	
	int i;
    for ( i = 0; i < MAX_SONG && song_Box[i] != NULL; i++)
        printf("%-1d %-3s %-3s %-3d %0.2lfmin\n", (i + 1), song_Box[i]->title, song_Box[i]->album, song_Box[i]->year, song_Box[i]->duration);
}

void show_playlist(){
    if (does_playlist_exist()){
        printf("\n----------------------------------------------------------------------\n");
        printf("                          CURRENT PLAYLIST\n");
        printf("----------------------------------------------------------------------\n");
        printf(" Title                                            | Duration\n");
        printf("----------------------------------------------------------------------\n");
        node_ptr current = header_node;
        while (current != NULL && current->song != NULL){
            node_ptr next = current->next_song;
            printf(" %-48.48s | %2.2lf min\n", current->song->title, current->song->duration);
            current = next;
        }

        printf("----------------------------------------------------------------------");
    }
    else{
        printf("\nYou haven't created a playlist yet.\n");
    }
}

void pagewise_song_display(int step){
	int i;
    printf("\n----------------------------------------------------------------------\n");
    printf("                             SONGS LIST\n");
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Title                                            | Duration\n");
    printf("----------------------------------------------------------------------\n");
    for (i = step - 10; i < step && song_Box[i] != NULL; i++){
        printf("  %2d   |  %-48.48s | %2.2lf min\n", (i + 1), song_Box[i]->title, song_Box[i]->duration);
    }
    printf("----------------------------------------------------------------------\n");
    printf("[Enter -2 to go to prev page] | Page %d | [Enter -1 to go to next page]\n", ((int)step / 10));
    printf("----------------------------------------------------------------------\n");
    printf(" <<<< Enter 0 to go back to main menu.\n");

    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

int song_selector(){
    int song_number = -1;
    int step = 10;
    while (song_number < 0 || song_number > pool_insert_index){
        pagewise_song_display(step);
        char input[MAX_SIZE];
        scanf("%s", input);

        if (!sscanf(input, "%d", &song_number)){
            printf("You seem to have entered an invalid input.\nReturning to main menu.");
            return 0;
        }

        if (song_number == -2){
            if (step > 10){
                step -= 10;
            }
            else{
                printf("\nThis itself is the first page");
            }
        }
        else if (song_number == -1){
            if (step < ceil(pool_insert_index / 10.0) * 10){
                step += 10;
        	}
            else{
                printf("\nThis is the last page");
            }
        }
        else if (song_number < -2 || song_number > pool_insert_index){
            printf("\nENTER A VALID CHOICE\n");
        }
    }

    return song_number;
}

Song *createSong(const char *title, const char *album, const short int year, const double duration, const char *id){
    Song *temp = malloc(sizeof(Song));
    strcpy(temp->title, title);
    strcpy(temp->album, album);
    strcpy(temp->id, id);
    temp->year = year;
    temp->duration = duration;
    pool_insert_index++;
    return temp;
}

//Allocates memory for a new playlist and prompts user to add a song to it
void create_playlist(){
    int song_number = 0, i = 0;

    node_ptr temp = (node_ptr)malloc(sizeof(PlaylistNode));
    printf("\nPick the song you want to add to your new playlist\n");
    song_number = song_selector();
    if (song_number > 0 && song_number <= 100){
        temp->song = song_Box[song_number - 1];
        temp->prev_song = NULL;
        temp->next_song = NULL;
        header_node = temp;
        now_playing = temp;
        printf("---\n%s has been added to your new playlist.\n---", song_Box[song_number - 1]->title);
    }
    else if (song_number == 0){
    	return;
	}
    else{
        printf("\nThere was a problem while handling your request, try again.\n");
    }
}

//Prompts user to pick a song from the pool and add it to the existing playlist
void add_to_playlist(){
    int song_number = 0;
    int step = 10;
    printf("\nPick the song you want to add\n");
    song_number = song_selector();
    if (song_number > 0 && song_number <= pool_insert_index){
        node_ptr new_node = (node_ptr)malloc(sizeof(PlaylistNode));
        node_ptr last = header_node;
        new_node->song = song_Box[song_number - 1];
        new_node->next_song = NULL;

        /* 4. If the Linked List is empty, then make the new
          node as head */
        if (header_node == NULL){
            new_node->prev_song = NULL;
            header_node = new_node;
            now_playing = new_node;
        }
        else{
            //Traverse till the last node
            while (last->next_song != NULL)
                last = last->next_song;

            last->next_song = new_node;
            new_node->prev_song = last;
        }
        printf("%s has been added to your playlist.\n", song_Box[song_number - 1]->title);
    }
    else if (song_number == 0){
    }
    else{
        printf("\nThere was a problem while handling your request.\n");
    }
}

//Function that allows the user to create a song of their own
void user_song_input(){
    int add_another = 1;
    char songname[MAX_SIZE];
    int year;
    int durationms;
    char id[MAX_SIZE];
    char album[MAX_SIZE];
    char uri[MAX_SIZE] = "spotify:track:";
    double duration = 0.0;

    while (add_another && pool_insert_index < MAX_SONG){
        printf("\nEnter the details of the song to create.\n[ Don't give spaces between each word, use _ instead\n");

        printf("Title: ");
        scanf("%s", songname);
        printf("Album Name: ");
        scanf("%s", album);
        printf("Year of release: ");
        if (!(scanf("%d", &year))){
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        printf("Duration(in s): ");
        if (!(scanf("%d", &durationms))){
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        duration = durationms / 60;
        printf("Song ID: ");
        scanf("%s", id);
        song_Box[pool_insert_index] = createSong(songname, album, year, duration, id);
        printf("\nThe song %s has been created.\n\n", songname);
        printf("\nDo you want to create another song?\n(Enter 1 for yes and 0 for no) : ");
        scanf("%d", &add_another);
    }
}

//Helper function to delete the playlist created and free all resources taken up by PlaylistNodes
bool delete_playlist(){
    if (header_node != NULL){
        node_ptr current = header_node;
        while (current != NULL){
            node_ptr next = current->next_song;
            free(current);
            current = next;
        }
        header_node = NULL;
        now_playing = NULL;
        return true;
    }
    return false;
}

//Frees resources that had been dynamically allocated
void free_all_memory(){
	int i; 
    delete_playlist();
    for ( i = 0; i < MAX_SONG && song_Box[i] != NULL; i++)
        free(song_Box[i]);
    printf("\n-------Exited-------");

    //Generate a 2 second delay
    sleep(2);
}

int main(){
    int choice = 0;
    char *terminate = "X";
    int wrong_choice_count = 0;
    while (choice != -1){
        switch (choice){
	        case 0:
	        { //Show menu options
	            main_menu();
	            break;
	        }
	        case 1:
	        {
	            if (pool_insert_index >= MAX_SONG)
	            {
	                printf("Cannot add more songs.\n");
	            }
	
	            user_song_input();
	            break;
	        }
	        case 2:
	        {
	            int user_song_selection = song_selector();
	            break;
	        }
	        case 3:
	        {
	            if (does_playlist_exist())
	            {
	                printf("---");
	                if (delete_playlist())
	                    printf("\nThe playlist was successfully deleted.\n");
	                else
	                    printf("\nYou haven't created a playlist yet. Nothing to delete\n");
	
	                printf("---");
	            }
	            else
	                create_playlist();
	            break;
	        }
	        case 4:
	        {
	            add_to_playlist();
	            break;
	        }
	        case 5:
	        {

	            show_playlist();
	            break;
	        }
	        case 6:
	        {
	            play_prev_song();
	            break;
	        }
	        case 7:
	        {
	            play_next_song();
	            break;
	        }
	        case 8:
	        {
	            show_song_details();
	            break;
	        }
	
	        default:
	        {
	            if (wrong_choice_count == 3){
	                printf("Please enter a valid option or the program will terminate.\n");
	            }
	            if (wrong_choice_count > 3){
	                printf("Sorry you have exceeded the maximum number of retries, terminating..");
	                free_all_memory();
	                exit(1);
	            }
	            else{
	                printf("Please enter a valid option from the main menu below\n");
	            }
	            wrong_choice_count++;
	            break;
	        }
        }
        if (choice != 0){
            main_menu();
        }

        char input[MAX_SIZE];
        scanf("%s", input);

        if (!sscanf(input, "%d", &choice)){
            printf("----\nINVALID INPUT\n");
            choice = 0;
            wrong_choice_count++;
            if (wrong_choice_count > 3){
                printf("Sorry you have exceeded the maximum number of retries, terminating..");
                free_all_memory();
                exit(1);
            }
            continue;
        }
    }
    return 0;
}
