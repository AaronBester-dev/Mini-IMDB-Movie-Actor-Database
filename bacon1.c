#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashfn.h"
#include "util.h"

void val2KeyMiniVersion(FILE * valFile, FILE * keyAndValFile, char * whatToLookFor,int hashNum,char * whereToPutIt);
void key2ValMiniVersion(FILE * keyFile, FILE * keyAndValFile, char * whatToLookFor,int  hashNum, char * whereToPutIt);


int main(int argc, char * argv[]){
    if(argc != 2){
        fprintf( stderr, "Usage: %s 'search term'\n", argv[0]);
        exit(-1);
    }

    FILE * titlePrincipalsValue = fopen("title.principals.vhs","rb");
    FILE * titlePrincipalsKeyAndValues = fopen("title.principals.kv","rb");
    FILE * titlePrincipalsKey = fopen("title.principals.khs","rb");
    FILE * nameBasicsValues = fopen("name.basics.vhs","rb");
    FILE * nameBasicsKeyAndValues = fopen("name.basics.kv","rb");
    FILE * titleBasicsValue = fopen("title.basics.vhs","rb");
    FILE * titleBasicsKeyAndValues = fopen("title.basics.kv","rb");
    FILE * titleBasicsKey = fopen("title.basics.khs","rb");

    int fileSizeOFPrincipalsValue = get_capacity(titlePrincipalsValue);
    int fileSizeOfNameBasicsValue = get_capacity(nameBasicsValues);
    int fileSizeOFPrincipalsKey = get_capacity(titlePrincipalsKey);
    int fileSizeOfTitleBasicsKey = get_capacity(titleBasicsKey);
    int hashNum = 0; 
    int kevinBaconMovieHashNum = 0;
    int peopleInMovieHashNum = 0;
    int kevinBaconMovieCounter = 0;
    int peopleInMovieCounter = 0;
    int movieFound = 0;
    int actorFound = 0;
    int exitProgram = 0;
    int getNextMovie = 0;
    char kevinBaconMovieKey[256];
    char keyOfKevinBacon[256];
    char keyOfActorWhoStarsWithKevinBacon[256];
    char keyOfMovieKevinBaconIsIn[256];
    char actorKey[256];
    char movieTheyAreBothIn[256];
 
    /*Getting users person key*/
    hashNum = hashfn(argv[1],fileSizeOfNameBasicsValue);
    val2KeyMiniVersion(nameBasicsValues,nameBasicsKeyAndValues,argv[1],hashNum,keyOfActorWhoStarsWithKevinBacon);

    /*Getting kevin bacons key*/
    hashNum = hashfn("Kevin Bacon",fileSizeOfNameBasicsValue);
    val2KeyMiniVersion(nameBasicsValues,nameBasicsKeyAndValues,"Kevin Bacon",hashNum,keyOfKevinBacon);


    /*getting key of movie kevin bacon is in*/

    kevinBaconMovieHashNum = hashfn(keyOfKevinBacon,fileSizeOFPrincipalsValue);  

    while( (kevinBaconMovieCounter < fileSizeOFPrincipalsValue) && exitProgram == 0){
        movieFound = 0;
        val2KeyMiniVersion(titlePrincipalsValue,titlePrincipalsKeyAndValues,keyOfKevinBacon,kevinBaconMovieHashNum,kevinBaconMovieKey);
        kevinBaconMovieHashNum++;
        kevinBaconMovieCounter++;
        if(kevinBaconMovieHashNum >= fileSizeOFPrincipalsValue){
            kevinBaconMovieHashNum = 0;
        }
        if(movieFound == 1 && strcmp(kevinBaconMovieKey,"NOT FOUND") == 0){
            exitProgram = 1;
        }
        
        if(strcmp(kevinBaconMovieKey,"NOT FOUND") != 0){
            movieFound = 1;
            /*getting key of people in said movie*/
            peopleInMovieHashNum = hashfn(kevinBaconMovieKey,fileSizeOFPrincipalsKey);
            peopleInMovieCounter = 0;
            actorFound = 0;
            getNextMovie = 0;
            while( (peopleInMovieCounter < fileSizeOFPrincipalsKey) && getNextMovie == 0){
                
                key2ValMiniVersion(titlePrincipalsKey,titlePrincipalsKeyAndValues,kevinBaconMovieKey,peopleInMovieHashNum,actorKey);
                peopleInMovieHashNum++;
                peopleInMovieCounter++;
                if(peopleInMovieHashNum >= fileSizeOFPrincipalsKey){
                    peopleInMovieHashNum = 0;
                }
                if(actorFound == 1 && strcmp(actorKey,"NOT FOUND") == 0){
                    getNextMovie = 1;
                }

                if(strcmp(actorKey,"NOT FOUND") != 0){
                    actorFound = 1;
                    
                    if(strcmp(actorKey,keyOfActorWhoStarsWithKevinBacon) == 0){
                        peopleInMovieHashNum = hashfn(kevinBaconMovieKey,fileSizeOfTitleBasicsKey);
                        key2ValMiniVersion(titleBasicsKey,titleBasicsKeyAndValues,kevinBaconMovieKey,peopleInMovieHashNum,movieTheyAreBothIn);
                        printf("%s\n",movieTheyAreBothIn);
                        return(0);
                    }
                }
    /*Getting key of movie if person exists in it*/
            
           
            }
        }
    }
    

}

void val2KeyMiniVersion(FILE * valFile, FILE * keyAndValFile, char * whatToLookFor,int hashNum,char * whereToPutIt){

    int index = 0;
    char valString[256];

    read_index(valFile,hashNum,&index);
    read_val(keyAndValFile,index,valString);
    if(strcmp(valString,whatToLookFor) == 0){
        read_key(keyAndValFile,index,whereToPutIt);
    }
    else{
        strcpy(whereToPutIt,"NOT FOUND");
    }    
}

void key2ValMiniVersion(FILE * keyFile, FILE * keyAndValFile, char * whatToLookFor,int  hashNum, char * whereToPutIt){

    int index = 0;
    char keyString[256];

    read_index(keyFile,hashNum,&index);
    read_key(keyAndValFile,index,keyString);
    if(strcmp(keyString,whatToLookFor) == 0){
        read_val(keyAndValFile,index,whereToPutIt);
    }
    else{
        strcpy(whereToPutIt,"NOT FOUND");
    }
}

