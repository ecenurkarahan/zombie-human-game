// Write your full name: Ece Nur Karahan, write your KU ID: 80029
//NOTE: my code works only with the maps that has rows in the form of "-H--" not in the form of "- H - -" 
//please consider this while evaluating
#include <stdio.h>
#include <stdlib.h>

#define EMPTY '-'
#define HUMAN 'H'
#define ZOMBIE 'Z'

// Function Prototypes
char** read_map_from_txt(const char* filename, int *width, int *height);
void print_grid(char** grid, int width, int height);
void simulate(char** grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead);
void cleanup_grid(char** grid, int height);


int main(int argc, char **argv) {
    int width, height;
    char *filename ;
    filename=argv[1];
    char **grid = read_map_from_txt(filename, &width, &height);

    printf("Initial Grid:\n");
    print_grid(grid, width, height);
    
    int humans_infected=0;
    int humans_survived=0;
    int zombies_dead =0;
    
    simulate(grid, width, height, &humans_survived, &humans_infected, &zombies_dead);
    
    printf("Humans survived: %d\nHumans infected: %d\nZombies died: %d\n", humans_survived, humans_infected, zombies_dead);
    printf("Final Grid:\n");
    print_grid(grid, width, height);
    
    cleanup_grid(grid, height);

    return 0;
}

char** read_map_from_txt(const char* filename, int *width, int *height) {
    // This function should read width and height from the given filename and assign these values to the pointers given.
    // It also should allocate the memory for the grid and return a pointer for grid.
    // Write your code below
    char line[20];
    int i;
    char row[40];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error reading file");
        exit(1);
    }

    //reading width
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading file");
        fclose(file);
        exit(1);
    }
    // atoi is used to turn a char[] to int
    *width = atoi(line);

    //reading height
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading file");
        exit(1);
        return NULL;
    }
    *height = atoi(line);
    //printf("witdh %d height %d ",*width,*height);
    char **gameGrid = (char **)malloc(*height * sizeof(char *));

    for(i=0;i<*height;i++){
        gameGrid[i]= (char *)malloc((*width+1)*sizeof(char));
        if(gameGrid[i]==NULL){
            //checking if there is memory allocation error
            printf("Unable to allocate memory for one row of game grid\n");
            fclose(file);
            return NULL;
        }
        //here assigning the lines in the file to our grid
        if(fgets(row, sizeof(row), file) == NULL){
            printf("can not read a line from game grid\n");
            return NULL;
        }
        row[strcspn(row, "\n")] = '\0';
        strcpy(gameGrid[i], row);
       // printf("Read line: %s\n", gameGrid[i]);  
    }

        fclose(file);

        return gameGrid;
}


void print_grid(char **grid, int width, int height) {
    // This function should print the grid as in the pdf file for the assignment.
    int i,j;
    for(i=0;i<height;i++){
        printf("%s\n",grid[i]);
    }
}

char** create_temp_grid(int width, int height) {
    // Initialize a temporary grid with all cells empty. This function is not required but strongly advised for correct implementation.
    // It should return pointer to temporary grid.
    char **tempGrid=(char **)malloc(height*sizeof(char *));
    if(tempGrid!=NULL){
    int i,j;
    for(i=0;i<height;i++){
        tempGrid[i]= (char *)malloc((width+1)*sizeof(char));
        if(tempGrid[i]== NULL){
            //checking if memory for the line is allocated
            printf("memory allocation error");
            return NULL;
        }
        //assigning all the elements to empty char
        for (j = 0; j < width; j++) {
                tempGrid[i][j] = EMPTY;
                }
        tempGrid[i][width]='\0';
    }
    return tempGrid;
    }
    //checking if memory for the grid allocated
    else{
        printf("memory allocation error");
        return NULL;
    }
}
//helper functions for stimulating:
int isHumanLeft(char** grid,int width,int height){
    //this function checks every cell of our grid and returns 1 if a human identifier found 
    int i,j;
    int humanFound=0;
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            if(grid[i][j]==HUMAN){
                humanFound=1;
                break;
            }
        }
    }
    return humanFound;
}
//this function will check for collisions in that grid after possible movements
//here sent positions are the ones that move will happen
//if there is no collision, it will return 1
// also it should check if the given position is valid or not
int collisionandValidityCheck(char **grid, int width, int height, int xPosition, int yPosition){
    int isCollision=0;
    //here it will check if the given position is in the bounds of the grid, if not it will return 0
    if(!(xPosition<width && xPosition>=0 && yPosition<height && yPosition>=0)){
        return isCollision;
    }
    if(grid[yPosition][xPosition]!= EMPTY){
        isCollision=1;
    }
    return !isCollision;
}
//i changed its return type to int to track escape situtations
//if the next move of the human is escape, it will not do anything and return 0
//i did cell manipulation in the simulation loop
//otherwise it will work as usual
int moveHuman(char ** realGrid,char **tempGrid,int width, int height,int moveNum,int xPosition, int yPosition){
    //movenum can only be 0 or 1
    moveNum = moveNum%2;
     //here there will be odd moves which is only to right
    //so xposition will be changed (increment by 1)
    if(moveNum){
        // move should only happen if it is in the bounds and there is no collision
        if(collisionandValidityCheck(realGrid,width,height,xPosition+1,yPosition)){
            //checking if there is escape
            if(xPosition+1==height-1&& yPosition== width-1){
                return 0;
            }
            else{
            tempGrid[yPosition][xPosition+1]=HUMAN;
            return 1;
            }
        }
        //there was an error that which only moving elements stays in the temp grid so i added an else statement
        //to keep non moving objects on their places
        else{
            tempGrid[yPosition][xPosition]=HUMAN;
            return 1;
        }
    }
    //here there will be even moves 
    //yposition will be incremented by 1
    else{
        if(collisionandValidityCheck(realGrid,width,height,xPosition,yPosition+1)){
            if(xPosition==height-1&& yPosition+1== width-1){
                return 0;
            }
            else{
            tempGrid[yPosition+1][xPosition]=HUMAN;
            return 1;
            }
        }
        else{
           tempGrid[yPosition][xPosition]=HUMAN;
           return 1; 
        }
    }
}
void moveZombie(char **realGrid,char **tempGrid,int width,int height,int moveNum,int xPosition, int yPosition){
    if(moveNum==1){
        //1st move, xPosition will be incremented
        if(collisionandValidityCheck(realGrid,width,height,xPosition+1,yPosition)){
            tempGrid[yPosition][xPosition+1]=ZOMBIE;
    }
        else{
            tempGrid[yPosition][xPosition]=ZOMBIE;
        }
    }
    else if (moveNum==2){
        //2.d move yposition will be incremented
        if(collisionandValidityCheck(realGrid,width,height,xPosition,yPosition+1)){
            tempGrid[yPosition+1][xPosition]=ZOMBIE;
    }
        else{
            tempGrid[yPosition][xPosition]=ZOMBIE;
            }
    }
    else if( moveNum==3){
        //3rd move, xposition will be decremented
        if(collisionandValidityCheck(realGrid,width,height,xPosition-1,yPosition)){
            //in this type of moves (going left) leftmost zombie has priority in moving
            //but since my code doesn't change real grid at every move of any element
            // i had to change real grid manually here to allow other zombies in a row to move
            // e.g. -ZZZ -> ZZZ-
            tempGrid[yPosition][xPosition-1]=ZOMBIE;
            realGrid[yPosition][xPosition-1]=ZOMBIE;
            realGrid[yPosition][xPosition]=EMPTY;
    }
        else{
            tempGrid[yPosition][xPosition]=ZOMBIE;
            }
    }
    else if(moveNum ==0){
        //4th move, y position will be decremented
         if(collisionandValidityCheck(realGrid,width,height,xPosition,yPosition-1)){
            //same as the 3rd move, same problem existed while moving up
            //i solved it like this
            tempGrid[yPosition-1][xPosition]=ZOMBIE;
            realGrid[yPosition-1][xPosition]=ZOMBIE;
             realGrid[yPosition][xPosition]=EMPTY;
    }
        else{
                tempGrid[yPosition][xPosition]=ZOMBIE;
            }
    }
}

int isZombieSurrounded(char **grid, int width, int height, int xPosition, int yPosition){
    //this function is to check if zombie is surrounded by humans
    //if result is 1, cell will be marked as empty 
    //there is 6 possible position, it will check them
    char up,down,left,right;
    int isSurrounded=0;
    if(yPosition-1>=0){
        up= grid[yPosition-1][xPosition];
    }
    else{
        up='0';
    }
    if(yPosition+1<height){
        down = grid[yPosition+1][xPosition];
    }
    else{
        down='0';
    }
    if(xPosition+1<width){
        right= grid[yPosition][xPosition+1];
    }
    else{
        right ='0';
    }
    if(xPosition-1>=0){
        left= grid[yPosition][xPosition-1];
    }
    else{
        left ='0';
    }
    if(up==HUMAN&&down==HUMAN){
        isSurrounded=1;
    }
    if(up==HUMAN&&right==HUMAN){
        isSurrounded=1;
    }
    if(up==HUMAN&&left==HUMAN){
        isSurrounded=1;
    }
    if(down==HUMAN&&right==HUMAN){
        isSurrounded=1;
    }
    if(down==HUMAN&&left==HUMAN){
        isSurrounded=1;
    }
    if(right==HUMAN&& left==HUMAN){
        isSurrounded=1;
    }
    return isSurrounded;
    }
int isHumanSurrounded(char **grid, int width, int height, int xPosition, int yPosition){
    //this function also has same logis as iszombiesurrounded function
    //if result is 1, cell will be zombie
    //there is 4 possible position
    char up,down,left,right;
    int isSurrounded=0;
    if(yPosition-1>=0){
        up= grid[yPosition-1][xPosition];
    }
    else{
        up='0';
    }
    if(yPosition+1<height){
        down = grid[yPosition+1][xPosition];
    }
    else{
        down='0';
    }
    if(xPosition+1<width){
        right= grid[yPosition][xPosition+1];
    }
    else{
        right ='0';
    }
    if(xPosition-1>=0){
        left= grid[yPosition][xPosition-1];
    }
    else{
        left ='0';
    }
    if(left==ZOMBIE){
        isSurrounded=1;
    }
    if(right==ZOMBIE){
        isSurrounded=1;
    }
    if(down==ZOMBIE){
        isSurrounded=1;
    }
    if(up==ZOMBIE){
        isSurrounded=1;
    }
    return isSurrounded;
}
void simulate(char** grid, int width, int height, int *humans_survived, int *humans_infected, int *zombies_dead) {
    // This function should simulate the grid according to rules until there is no human left.
    // It should call print_grid after each step of simulation. Don't forget to add blank lines to make your output clear.
    // Here are a few hints for this.
    // 1- Write a helper function to check whether there are any human left in the grid. Then you can use the output of this function in your while loop.
    // 2- Write functions for human and zombie movements, collision checkers, and call them under this function for each iteration.
    // 3- Behavior of the movement changes for each step. So keep track of simulation steps, and call movement functions with simulation_step modulo 4.
    // 4- If you try to do each change in the original grid directly, then you'll see it is quite complicated. Therefore, do your changes in a temporary grid and then move to original grid for each step.
    // 5- Use Valgrind to ensure there is no memory leaks as it will cause points deduction!
    //i keep the count of simulation step to take modulo of it and find move numbers accordingly
    int simulation_step=0;
    int moveNum;
    int i,j;
    while(isHumanLeft(grid,width,height)){
        simulation_step++;
        moveNum = simulation_step%4;
        char **tempGrid= create_temp_grid(width,height);
        //here i first find humans and move them
        // both move functions will manipulate the temporary grid
        //TO ESCAPE: i designed my movehuman function in a way that it will check if humans next move is exit cell
        //if it is exit cell, human is counted as escaped
        for(i=0;i<height;i++){
            for (j=0;j<width;j++){
                char eachCell= grid[i][j];
                if(eachCell== HUMAN){
                    //in this if, it will move humans if return value is 1
                    //if not, it will count human as escaped
                    if(!(moveHuman(grid,tempGrid,width,height,moveNum,j,i))){
                        tempGrid[i][j]= EMPTY;
                        *humans_survived= *humans_survived+1;
                    }
                    
                }
                //if grid is not human, we do not care in this step
                else{
                    continue;
                }
            }
        }
        //here is a middle grid to keep the moves of humans and initial places of zombies
        //there was a problem about zombies and humans being in the same place since i am checking from original grid
        //i fixed it by making a middle level grid
        //this grid keeps zombies in their first places and humans in their moved places
        char **middleGrid = create_temp_grid(width,height);
        for(i=0; i<height; i++){
            for(j=0; j<width; j++){
                char humanCells= tempGrid[i][j];
                char zombieCells= grid[i][j];
                if(humanCells==HUMAN){
                    middleGrid[i][j]= HUMAN;
                }
                if(zombieCells==ZOMBIE){
                    middleGrid[i][j]=ZOMBIE;
                }
            }
        }
        //then moving zombies
        for(i=0;i<height;i++){
            for (j=0;j<width;j++){
                char eachCell= middleGrid[i][j];
                if(eachCell== ZOMBIE){
                    //i passed middle grid as real grid to check for collisions due to update reasons
                    moveZombie(middleGrid,tempGrid,width,height,moveNum,j,i);
                }
                else{
                    continue;
                }
            }
        }
        //here it will kill zombies if they are surrounded by 2 human in any position
        for(i=0;i<height;i++){
            for (j=0;j<width;j++){
                char eachCell= tempGrid[i][j];
                if(eachCell== ZOMBIE){
                    if(isZombieSurrounded(tempGrid,width,height,j,i)){
                        tempGrid[i][j]=EMPTY;
                        *zombies_dead= *zombies_dead+1;
                    }
                }
                else{
                    continue;
                }
            }
        }
        //here after killing zombies, it will turn humans into zombies if they are surrounded by at least one zombie
        for(i=0;i<height;i++){
            for (j=0;j<width;j++){
                char eachCell= tempGrid[i][j];
                if(eachCell== HUMAN){
                    if(isHumanSurrounded(tempGrid,width,height,j,i)){
                        tempGrid[i][j]=ZOMBIE;
                        *humans_infected = *humans_infected + 1;
                    }
                }
                else{
                    continue;
                }
            }
        }
        //here we are copying manipulated grid to original grid for other simulation steps
        for ( i = 0; i < height; i++) {
            strcpy(grid[i], tempGrid[i]);
        } 
        //we should free temp grid and middle grid after copying them bc we will not need it anymore
        cleanup_grid(tempGrid,height);
        cleanup_grid(middleGrid,height);
        printf("%d\n", simulation_step-1);
        print_grid(grid, width, height);

    }

}

// i checked for memory leaks and there was no leak
void cleanup_grid(char** grid, int height) {
    // This function should free the allocated memory for any given grid.
    int i;
     for ( i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);
}




