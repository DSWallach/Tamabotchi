// tamabotchi

int HumanFound();
int TimeCheck();
int WanderOrSleep();
int ObstacleDetected();

int Execute(int CurrentBehavior);

int HumanInteract();
void Wander();
void SleepB();
void Avoid();

const int HUMAN_INDEX = 0;
const int HUMAN_CHANNEL = 0;

//constant integer values which represent the three default behaviors
const int AVOID = 0;
const int WANDER = 1;
const int SLEEP = 2;

const int FRONT_BUMP = 0;
const int BACK_BUMP = 15;
const int LEFT_PHOTO = 3;
const int RIGHT_PHOTO = 4;
const int LEFT_IR = 6;
const int RIGHT_IR = 7;

//used in TimeChecker() to determine when to pick a new WanderOrSleep()
float timeB;

int true = 1;
int false = 0;

int rightObstacle = 0;
int leftObstacle = 0;

//human variables
int AffectionCount = 50;
int HungerCount = 10;
int InteractionBehavior;

const int LOVE = 2;
const int HATE = 3;
const int LIKE = 4;
const int DISLIKE = 5;
const int NEUTRAL = 6;

//human functions
int AffectionCounter();
int AffectionChange(int x);
int HumanExecute(int InteractionBehavior);
void HumanHate();
void HumanDislike();
void HumanNeutral();
void HumanLike();
void HumanLove();

///////////////DEFAULT BEHAVIOR/////////////////////
int main()
{
    set_each_analog_state(0,0,0,0,0,0,1,1);
    int CurrentBehavior = WanderOrSleep();
    
    //WanderOrSleep continual until While Loop chooses new function
    while(1) {
        if (HumanFound()) {
            printf("HUMAN FOUND\n");
            HumanInteract();
        }
        else if (ObstacleDetected()) {
            CurrentBehavior = AVOID;
        }
        else if (TimeCheck() || CurrentBehavior == AVOID){
            CurrentBehavior = WanderOrSleep();
        }
        
        Execute(CurrentBehavior);
        sleep(0.1);
    }
    
    return 0;
}

//returns a true or false value based on whether or not a human was found
int HumanFound() {
    track_update();
    return ((track_size(HUMAN_CHANNEL, HUMAN_INDEX) > 10) &&
    (track_confidence(HUMAN_CHANNEL, HUMAN_INDEX) > 40));
}

void Wander() {
    //pick random direction
    int SPEED_RIGHT = 40 + rand() % 40;
    int SPEED_LEFT = 40 + rand() % 40;
    motor(0, SPEED_LEFT);//rand() % SPEED_THRESHOLD);
    motor(3, SPEED_RIGHT);//rand() % SPEED_THRESHOLD);
    printf("Wander");
}

void SleepB() {
    int SPEED_THRESHOLD = 10; /*
    if ((int)timeB % 500 > 250) {
    motor(0, -1 * SPEED_THRESHOLD);
    motor(3, -1 * SPEED_THRESHOLD);
    }
    else {
    motor(0, SPEED_THRESHOLD);
    motor(0, SPEED_THRESHOLD);
    } */
    motor(0, 0);
    motor(3, 0);
    printf("Sleep");
}

void Avoid() {
    int SPEED_THRESHOLD = 70;
    if (leftObstacle) {
        //if there's something to the left, back up the right wheel
        motor(3, -1 * SPEED_THRESHOLD);
    } else {
		motor (3, SPEED_THRESHOLD);
	}
    if (rightObstacle) {
        //vice versa
        motor(0, -1 * SPEED_THRESHOLD);
    } else {
		motor(0, SPEED_THRESHOLD);
	}
    printf("avoid");
}

//returns either WANDER or SLEEP
int WanderOrSleep() {
    printf("\nPICKING NEW\n");
    int r = 1 + (rand() % 5); //% = modulo
    HungerCount = HungerCount + 1;
    timeB = seconds();
    if (r <= 4){
        return WANDER;
    }
    else{
        return SLEEP;
    }
    
}

int TimeCheck() {
    if (seconds() >= (timeB + 5))
    return 1;
    else
    return 0;
}

int ObstacleDetected() {
    int left_ir;
    int right_ir;
    int avoidThreshold = 512;
    
    left_ir = analog10(LEFT_IR);
    right_ir = analog10(RIGHT_IR);
    
    leftObstacle = (left_ir > avoidThreshold);
    
    rightObstacle = (right_ir > avoidThreshold);
	
    return rightObstacle || leftObstacle;
    //sleep(0.1);
}

int Execute(int CurrentBehavior) {
    if (CurrentBehavior == WANDER){
        Wander();
    }
    if (CurrentBehavior == SLEEP){
        SleepB();
    }
    if (CurrentBehavior == AVOID){
        Avoid();
    }
}
//number of horizontal pixels 0-159
//////////////////////END DEFAULT BEHAVIOR///////////////////

//what to do when a human was found - HUMAN LAND/////////////////////
int HumanInteract() {
    int InitialHunger = HungerCount;
    while(1) {
        if (!HumanFound()) {
            printf("\nHuman not found, breaking...");
            break;
        }
        else if ((analog10(FRONT_BUMP) < 400 && !(InteractionBehavior == LOVE))|| (digital(BACK_BUMP) == 1 && !(InteractionBehavior == LOVE))) {
            printf("\nKick detected. Affection count before: %d\n", AffectionCount);
            AffectionChange(-15);
            printf("Affection count after: %d", AffectionCount);
            InteractionBehavior = HATE;
        }
        /*
        else if (food()) {
        AffectionCount++
        }
        */
        else {
            //updates the behavior based on AffectionCount
            AffectionCounter();
        }
        
        if (HungerCount < InitialHunger) {
            // or
            //else if (FoodSeen()){
            AffectionChange(10);
            printf("\nHUNGER CHANGED AFFECTION");
            //InteractionBehavior = LOVE;
        }
        if (InteractionBehavior == LOVE) {
            AffectionChange (2);
            printf("\n+2 AFFECTION FROM LOVE BEHAVIOR");
        }
        
        HumanExecute(InteractionBehavior);
        sleep(0.1);
    }
    
    return 0;
}

int AffectionCounter() {
    if (AffectionCount <= 20){
        InteractionBehavior = HATE;
    }
    else if (AffectionCount <= 40){
        InteractionBehavior = DISLIKE;
    }
    else if (AffectionCount <= 60){
        InteractionBehavior = NEUTRAL;
    }
    else if (AffectionCount <= 80){
        InteractionBehavior = LIKE;
    }
    else {
        InteractionBehavior = LOVE;
    }
}

int HumanExecute(int InteractionBehavior) {
    if (InteractionBehavior == HATE){
        HumanHate();
    }
    if (InteractionBehavior == DISLIKE){
        HumanDislike();
    }
    if (InteractionBehavior == NEUTRAL){
        HumanNeutral();
    }
    if (InteractionBehavior == LIKE){
        HumanLike();
    }
    if (InteractionBehavior == LOVE){
        HumanLove();
    }
}

void HumanHate() {
    //EQUIVALENT TO AVOID BEHAVIOR FROM DEFAULT
    int SPEED_THRESHOLD = 70;
    if (leftObstacle) {
        //if there's something to the left, back up the right wheel
        motor(3, -1 * SPEED_THRESHOLD);
    }
    if (rightObstacle) {
        //vice versa
        motor(0, -1 * SPEED_THRESHOLD);
    }
    printf("HumanHate");
}


void HumanDislike() {
    //EQUIVALENT TO WANDER BEHAVIOR
    //pick random direction
    int SPEED_RIGHT = 40 + rand() % 40;
    int SPEED_LEFT = 40 + rand() % 40;
    motor(0, SPEED_LEFT);//rand() % SPEED_THRESHOLD);
    motor(3, SPEED_RIGHT);//rand() % SPEED_THRESHOLD);
    printf("HumanDislike");
}

void HumanNeutral() {
    motor(0, 0);
    motor(3, 0);
    printf("HumanNeutral");
}

//track_x gives x coordinate of pixel that detects color (range from 0 to 159)
void HumanLike() {
    if (ObstacleDetected()) {
        motor(0, 0);
        motor(3, 0);
    }
    
    else if (track_x(0,0) > 120) {
        motor(0, 70);
        motor(3, 50);
    }
    
    else if ((track_x(0,0) > 80) && (track_x < 120)) {
        motor(0, 70);
        motor(3, 70);
    }
    
    else {
        motor(0, 50);
        motor(3, 70);
    }
    printf("HumanLike");
}

void HumanLove() {
    if (ObstacleDetected()){
        int left_ir;
        int right_ir;
        int avoidThreshold = 600; //Check this
        if (left_ir > avoidThreshold) {
            motor(0, 100);
            motor(3, 50);
        }
        else if (right_ir > avoidThreshold) {
            motor(0, 50);
            motor(3, 100);
        }
        else if (track_x(HUMAN_INDEX,HUMAN_CHANNEL) > 120) {
            motor(0, 70);
            motor(3, 50);
        }
        
        else if (track_x(HUMAN_INDEX,HUMAN_CHANNEL) > 80 && track_x(HUMAN_INDEX,HUMAN_CHANNEL) < 120) {
            motor(0, 70);
            motor(3, 70);
        }
        else {
            motor(0, 50);
            motor(3, 70);
        }
    }
    printf("HumanLove");
}
int AffectionChange(int x) {
    if (AffectionCount + x > 100){
        AffectionCount = 100;
    } 
    else if (AffectionCount + x < 0) {
        AffectionCount = 0;
    }
	else {
		AffectionCount = AffectionCount + x;
	}
}



////////////////END HUMAN LAND//////////////////////////

///FOOD CODE////////
/*
int Hunger(){
    if (hungerCount >= 8){
        hungerLevel = NOT_HUNGRY;
    }
    else if (hungerCount >= 3){
        hungerLevel = HUNGRY;
    }
    else {
        hungerLevel = VERY_HUNGRY;
    }
}



void Food(){
    track_update();
    if (hungerLevel == VERY_HUNGRY){
        seek_food();
    }
    
}
*/


