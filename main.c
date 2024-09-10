/*
Name: Duyen Vu
Date: 06/26/2023
Description: Assignment 3 Dynamic Arrays
Usage: gcc Vu_Duyen_hw3.c -o hw3 -lm
./hw3 -s <seed>
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// dropDirtyBall function
void dropDirtBall(int** landmass, int maxWidth, int maxHeight,
int centerX, int centerY, int radius, int power) {
    for (int x = centerX - radius; x <= centerX + radius; x++) {
        for (int y = centerY - radius; y <= centerY + radius; y++) {
            if (x >= 0 && x < maxWidth && y >= 0 && y < maxHeight) {
                // Calculate the distance between the dirtball center and the current point
                double distance = sqrt(pow((x - centerX), 2) + pow((y - centerY), 2));
                if (distance <= radius) {
                    // Calculate the impact value by using the distance and power
                    int impactVal = power - (int)floor(distance);
                    landmass[y][x] = landmass[y][x] + impactVal;
                }
            }
        }
    }
} 

// findMax function
int findMax(int** map, int width, int height) {   
    // Find the maximum value of the map
    int maxVal = map[0][0];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] > maxVal) {
                maxVal = map[y][x];
            }
        }
    }
    return maxVal;
}
    
// nomarlizeMap function
void normalizeMap(int** map, int width, int height, int maxVal) {   // Normalize the value of the map
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[y][x] = floor(((double)map[y][x] / maxVal) * 255);
        }
    }
}

double frand() {
    return (double)rand() / ((double)RAND_MAX + 1);
}

// printLand function
void printLand(int** land, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%d ", land[y][x]);   // Print 2D array
            if (x < width - 1) {
                printf(" ");   // Print a space between each value (except the last one in a row)
            }
        }
        printf("\n");
    }
}

// finalizeMap function
char** finalizeMap(int** map, int maxX, int maxY, int waterLine) {
    int landZone = 255 - waterLine;
    char** finalMap = (char**)malloc((maxY + 1) * sizeof(char*));
    
    // Calculate thresholds for each land type
    int deepWater = floor(0.5 * waterLine);
    int shallowWater = waterLine;
    int coast = floor(waterLine + (0.15 * landZone));
    int plains = floor(waterLine + (0.4 * landZone));
    int forests = floor(waterLine + (0.8 * landZone));

    // Create the polished map by generating the final map with character symbols based on the land values
    for (int y = 0; y < maxY; y++) {
        finalMap[y] = (char*)malloc((maxX + 1) * sizeof(char));
        for (int x = 0; x < maxX; x++) {
            int value = map[y][x];

            if (value < deepWater) {
                finalMap[y][x] = '#';   // Deep water
                
            } else if (value >= deepWater && value <= shallowWater) {
                finalMap[y][x] = '~';   // Shallow water
                
            } else if (value > waterLine && value < coast) {
                finalMap[y][x] = '.';   // Coast/beach
                
            } else if (value >= coast && value < plains) {
                finalMap[y][x] = '-';   // Plains/grass
                
            } else if (value >= plains && value < forests) {
                finalMap[y][x] = '*';   // Forests
                
            } else {
                finalMap[y][x] = '^';   // Mountains
            }
        } 
        
        finalMap[y][maxX] = '\0'; 
    }
    
    finalMap[maxY] = NULL;
    
    return finalMap;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));   // Initialize the random number generator with the current time
    
    int width, height;
    printf("Welcome to Duyen's CSE240 Terraformer!!\n\n");
    printf("Enter the width of the terrain: ");
    scanf("%d", &width);   // Get a width from the user
    printf("Enter the height of the terrain: ");
    scanf("%d", &height);   // Get a height from the user
    
    int seed = time(NULL);   // Default seed = current time
    
    // Check if argument 1 is -s and argument 2 is the value of the seed
    if (argc >= 3 && strcmp(argv[1], "-s") == 0) {
        seed = atoi(argv[2]); 
    }
    
    srand(seed); // Get the random number generator by using provided seed
    
    // Create an allocated 2D integer array based on the information above
    int** landmass = (int**)malloc(height * sizeof(int*));
    for (int y = 0; y < height; y++) {
        landmass[y] = (int*)malloc(width * sizeof(int));
        for (int x = 0; x < width; x++) {
            landmass[y][x] = 0;  // Initialize the array to 0
        }
    } 
    
    int waterLine, radius, powerRating, numDirtballs;
    printf("Enter value for waterline (40-200): ");   // Ask the user to enter a value from 40 to 200
    scanf("%d", &waterLine);
    printf("Enter dirtball radius (minimum 2): ");   // Ask for the radius, it should be 2 or higher
    scanf("%d", &radius);
    printf("Enter dirtball power rating (minimum = radius): ");   // Ask the user to enter a power rating
    scanf("%d", &powerRating);
    printf("Enter number of dirtballs to drop: ");   // Ask for the number of dirtyballs drop
    scanf("%d", &numDirtballs);
    
    // Generate the landmass by dropping dirtballs
    for (int i = 0; i < numDirtballs; i++) {
        // Generate random values for width and height
        int centerX = frand() * width;
        int centerY = frand() * height;
        dropDirtBall(landmass, width, height, centerX, centerY, radius, powerRating);
    }
    
    // Output raw landmass array to console and file
    FILE* rawFile = fopen("raw_landmass.txt", "w");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(rawFile, "%4d", landmass[y][x]);
        } fprintf(rawFile, "\n");
    } 
    
    fclose(rawFile);   // Close the file
    
    int maxVal = findMax(landmass, width, height);   // Find the maximum value
    normalizeMap(landmass, width, height, maxVal);   // Normalize the landmass array
    
    // Output Normalized landmass array to console and file
    FILE* normalizedFile = fopen("normalized_landmass.txt", "w");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(normalizedFile, "%4d", landmass[y][x]);
        } fprintf(normalizedFile, "\n");
    }
    
    fclose(normalizedFile);  // Close the file
    
    char** finalMap = finalizeMap(landmass, width, height, waterLine);   // Generate the finalMap
    
    FILE* finalFile = fopen("final_landmass.txt", "w");
    // Write the final map to final_landmass file
    for (int y = 0; y < height; y++) {
        fprintf(finalFile, "%s\n", finalMap[y]);
    } 
    
    fclose(finalFile);
    
    // Free memory allocated for the array landmass and finalMap
    for (int y = 0; y < height; y++) {
        free(landmass[y]);
    }
    free(landmass);
    
    for (int y = 0; y < height; y++) {
        free(finalMap[y]);
    }
    free(finalMap);
    
    return 0;
}

