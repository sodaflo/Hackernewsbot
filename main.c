#include "parson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int compare(char *post);
void parsing();
void get_json();

bool booOne, booTwo, booThree;

int main() {
    get_json();
    parsing();
    printf("done");
    return 0;
}

void get_json(){
    FILE *IP;
    FILE *OP;

    int c;
    int z;
    z = 0;

    system("rm new.json");
    system("rm news.json");

    system("curl http://api.ihackernews.com/page/ > new.json");
    IP = fopen("new.json", "r");
    OP = fopen("news.json", "w+");
    while((c = fgetc(IP)) != EOF){
        if(c == 91){
            z++;
        }
        if(z == 1){
            fputc(c, OP);
        }
        if(c == 93){
            z--;
        }
    }
    fclose(IP);
    fclose(OP);
    printf("hopefully got json");
}

void parsing(){
 	JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;
    size_t i;
    int j;
    FILE *NEWZ;
    FILE *One;
    FILE *Two;
    FILE *Three;
    char post[2000];
    int alreadyposted;
    char buf[2000];
    char systembuf[2000];
    char output_filename[] = "news.json";

    system("rm newz");

    root_value = json_parse_file(output_filename);
    if (json_value_get_type(root_value) != JSONArray) {
        printf("Kein json\n");
        return;
    }

    commits = json_value_get_array(root_value);
    printf("%s %s\n", "title", "url");
    NEWZ = fopen("newz", "w+");
    printf("jippi");
    i = 0;
    booOne = false;
    booTwo = false;
    booThree = false;
    printf("news");
    for (j = 1; j < 4; j++){
        commit = json_array_get_object(commits, i);
        printf("%s: %s\n",
               json_object_get_string(commit,"title"),
               json_object_get_string(commit,"url"));
        sprintf(buf, "%s: %s\n",
               json_object_get_string(commit,"title"),
               json_object_get_string(commit,"url"));
        alreadyposted = compare(buf);
        i++;
    }
    printf("erste Schleife durch");
    i = 0;
    for (j = 1; j < 4; j++) {
        printf("zweite Schleife");
        commit = json_array_get_object(commits, i);
        sprintf(post, "%s: %s\n",
               json_object_get_string(commit,"title"),
               json_object_get_string(commit,"url"));
        fprintf(NEWZ, post);
        alreadyposted = compare(post);
        if(alreadyposted == 0){
            sprintf(systembuf, "./texapp.txt -status=\"%s\"", post);
            system(systembuf);
            printf("\n sending \n");
            if(booOne == false){
                system("rm One");
                One = fopen("One", "w+");
                fprintf(One, "%s", post);
                fclose(One);
                booOne = true;
                printf("saving in one \n");
            }
            else if(booTwo == false){
                system("rm Two");
                Two = fopen("Two", "w+");
                fprintf(Two, "%s", post);
                fclose(Two);
                booTwo = true;
                printf("saving in two \n");
            }
            else if(booThree == false){
                system("rm Three");
                Three = fopen("Three", "w+");
                fprintf(Three, "%s", post);
                fclose(Three);
                booThree = true;
                printf("saving in three \n");
            }

        }
        i++;
    }
    
    /* cleanup code */
    json_value_free(root_value);
}

int compare(char post[2000]){
    FILE *one;
    FILE *two;
    FILE *three;
    int isposted;
    char bufy[2000];

    isposted = 0;

    one = fopen("One", "r");
    two = fopen("Two", "r");
    three = fopen("Three", "r");
    fgets(bufy, 2000, one);
    printf("%s", bufy);
    if(strcmp(bufy, post) == 0){
        isposted = 1;
        booOne = true;
        printf("gotcha");
        }
    fgets(bufy, 2000, two);
    printf("%d", strcmp(bufy, post));
    if(strcmp(bufy, post) == 0){
        isposted = 1;
        booTwo = true;
        printf("gotcha");
    }
    fgets(bufy, 2000, three);
    if(strcmp(bufy, post) == 0){
        isposted = 1;
        booThree = true;
        printf("gotcha");
    }
    fclose(one);
    fclose(two);
    fclose(three);
    return isposted;
}
