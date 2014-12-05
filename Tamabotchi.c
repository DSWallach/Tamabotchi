// tamabotchi

int HumanFound();
int TimeCheck(double InitialTime, double Duration);
int WanderOrSleep();
int ObstacleDetected();
int FoodSeen();

int Execute(int CurrentBehavior);

int HumanInteract();
void Wander();
void SleepB();
void Avoid();
void Eat();
void SeekFood();
int LovedHumanDetected();
int Spin();


const int HUMAN_INDEX = 0;
const int HUMAN_CHANNEL = 0;
const int FOOD_CHANNEL = 2;
const int FOOD_INDEX = 0;

//constant integer values which represent the three default behaviors
const int AVOID = 0;
const int WANDER = 1;
const int SLEEP = 2;
const int EAT = 3;
const int SEEKFOOD = 4;

const int FRONT_BUMP = 0;
const int BACK_BUMP = 15;
const int LEFT_PHOTO = 3;
const int RIGHT_PHOTO = 4;
const int LEFT_IR = 6;
const int RIGHT_IR = 7;

//used in TimeChecker() to determine when to pick a new WanderOrSleep()
float BehaviorStart;

int true = 1;
int false = 0;

int rightObstacle = 0;
int leftObstacle = 0;

//used in SeekFood() to determine if it should turn and which direction it should turn respectively
int turned = 0;
int TurnOrContinue();
// The default hunger levels when the robot is turned on
int HungerCount = 10;
int HungerLevel = 0;

int HungerUpdate();
// constant integer values that represent the three stages of hunger
const int NOT_HUNGRY = 0;
const int HUNGRY = 1;
const int VERY_HUNGRY = 2;

//used to make robot rock back and forth in sleep behavior
int SleepRock;

//stores the current behavior for the robot to execute (in the default case)
int CurrentBehavior;

//human variables
float HumanBehaviorStart;
int HumanBehaviorDuration;

int HumanGoneFor = 50;

int AffectionCount = 10;
int InteractionBehavior;
// constant integer values that represent the human interaction behaviors
const int LOVE = 2;
const int HATE = 3;
const int LIKE = 4;
const int DISLIKE = 5;
const int NEUTRAL = 6;
const int FEED = 7;
const int WHEREISHUMAN = 8;
const int NEW_HUMAN = 9;
//const int SEEKFOOD = 8;

//human functions
int AffectionCounter();
int AffectionChange(int x);
int HumanExecute(int InteractionBehavior);
void HumanHate();
int HasTurned = 0;
void HumanDislike();
void HumanNeutral();
void HumanLike();
void HumanLove();
void Feed();
void LookAround();
void NewHuman();

void ApproachByVision(int index, int channel);

///////////////DEFAULT BEHAVIOR/////////////////////
int main()
{
    set_each_analog_state(0,0,0,0,0,0,1,1);
    CurrentBehavior = WanderOrSleep();
    int SleepRock = false;
    
    //WanderOrSleep continual until While Loop chooses new function
    while(1) {
        if (HungerLevel == VERY_HUNGRY){
            printf("VERY HUNGRY SEEKING FOOD\n");
            SeekFood();
			CurrentBehavior = SEEKFOOD;
        }
        else if (HumanFound()) {
            printf("HUMAN FOUND\n");
            HumanInteract();
        }
        else if (ObstacleDetected()) {
            CurrentBehavior = AVOID;
        }
        else if (FoodSeen() && HungerLevel != NOT_HUNGRY) {
            CurrentBehavior = EAT;
        }
        else if (TimeCheck(BehaviorStart, 5) || CurrentBehavior == AVOID){
            CurrentBehavior = WanderOrSleep();
        }
        HungerUpdate();
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

//Food Behaviors
//Augments behavior as hunger counter changes
int HungerUpdate(){
    if (HungerCount >= 16) {
        HungerLevel = NOT_HUNGRY;
    }
    else if (HungerCount >= 6) {
        HungerLevel = HUNGRY;
    }
    else {
        HungerLevel = VERY_HUNGRY;
    }
}

// Determines if there is food within the field of view of the robot?s camera
int FoodSeen(){
 track_update();
 return ((track_size(FOOD_CHANNEL, FOOD_INDEX) > 10) && (track_confidence(FOOD_CHANNEL, FOOD_INDEX) > 40));
}


// Function used in the SeekFood() behavior
int TurnOrContinue() {
    int temp = rand() % 100;
    return (temp > 75);   
}
// 
void SeekFood(){
	//note that this function is long, sequential function rather than one of the EXECUTE functions
	//which only run for a single timestep
    while (!FoodSeen()){
            motor(0, 80);
            motor(3, 80);
            sleep(0.1);
        if (TurnOrContinue() && (turned == 0)) {
            motor(0, 80);
            motor(3, 40);
            sleep(0.1);
            turned = 1;    
        }    
        if (TurnOrContinue() && (turned == 1)){
            motor(0, 40);
            motor(3, 80);
            sleep(0.1);
            turned = 0;
        }    
    }
    if (FoodSeen() && HumanFound() || FoodSeen() && HumanGoneFor < 30){
        Feed();
    }
    if (FoodSeen() && !HumanFound()){
		CurrentBehavior = EAT;
        Eat();
    }
}

// THe behavior for eating food. That is demonstrated here by the robot running over (eating) a // // piece of colored paper (food).
void Eat(){
	printf("Eat");
    track_update();
    if (track_x(FOOD_CHANNEL, FOOD_INDEX) < 50){
        motor(0, 0);
        motor(3, 50);
        sleep(0.1);
    }
    else if (track_x(FOOD_CHANNEL, FOOD_INDEX) > 110){
        motor(0, 50);
        motor(3, 0);
        sleep(0.1);
    }
    else {
        track_update;
        int initial_y = track_y(FOOD_CHANNEL, FOOD_INDEX);
        int new_y;
        do {
            track_update();
            motor(0, 60);
            motor(3, 60);
        new_y = track_y(FOOD_CHANNEL, FOOD_INDEX);    
        } while (FoodSeen());
        
        if ((new_y > initial_y) && (new_y > 100)) {
            if (HungerCount < 0) {
				printf("Yum");
                HungerCount = 8;
            } else {
				printf("Yum");
                HungerCount = HungerCount + 8;
            }
            if (HungerCount >= 16){
                printf("Now I'm full");
            }
            else {
                printf("I wonder if there's more food around here");
            }
        }
    }
    
}
void Feed() {
	printf("Feeding");
    track_update();
    if (track_x(FOOD_CHANNEL, FOOD_INDEX) < 60){
        motor(0, 0);
        motor(3, 50);
        sleep(0.1);
    }
    else if (track_x(FOOD_CHANNEL, FOOD_INDEX) > 100){
        motor(0, 50);
        motor(3, 0);
        sleep(0.1);
    }
    else {
        track_update;
        int initial_y = track_y(FOOD_CHANNEL, FOOD_INDEX);
        int new_y;
        do {
            track_update();
            motor(0, 60);
            motor(3, 60);
        new_y = track_y(FOOD_CHANNEL, FOOD_INDEX);    
        } while(FoodSeen());
        if ((new_y > initial_y) && (new_y > 100)){
        HungerLevel = HungerLevel + 8;
        AffectionCount = AffectionCount + 15;
            if (HungerLevel >= 16){
        printf("Thanks for the food, I couldn't eat another bite");
        }
        else {
            printf("Please Sir, could I have some more");
        }
    }
    
    }
}
//End of food behaviors

void Wander() {
    //pick random direction
    int SPEED_RIGHT = 40 + rand() % 40;
    int SPEED_LEFT = 40 + rand() % 40;
    motor(0, SPEED_LEFT);//rand() % SPEED_THRESHOLD);
    motor(3, SPEED_RIGHT);//rand() % SPEED_THRESHOLD);
    printf("Wander");
}

void SleepB() {
    int SPEED_THRESHOLD = 40;
    if (SleepRock) {
    motor(0, -1 * SPEED_THRESHOLD);
    motor(3, -1 * SPEED_THRESHOLD);
    }
    else {
    motor(0, SPEED_THRESHOLD);
    motor(0, SPEED_THRESHOLD);
    }
    SleepRock = !SleepRock;
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
    HungerCount = HungerCount - 1;
    BehaviorStart = seconds();
    if (r <= 4){
        return WANDER;
    }
    else{
        return SLEEP;
    }
    
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

//To select from the four non-human interaction behaviors
int Execute(int CurrentBehavior) {
    if (CurrentBehavior == WANDER) {
        Wander();
    }
    if (CurrentBehavior == SLEEP) {
        SleepB();
    }
    if (CurrentBehavior == AVOID) {
        Avoid();
    }
    if (CurrentBehavior == EAT) {
        Eat();
    }
	if (CurrentBehavior == SEEKFOOD) {
		SeekFood();
	}
}
//number of horizontal pixels 0-159
//////////////////////END DEFAULT BEHAVIOR///////////////////

//what to do when a human was found - HUMAN LAND/////////////////////
int HumanInteract() {
    int InitialHunger = HungerCount;
    HumanBehaviorStart = seconds();
    HumanBehaviorDuration = 0;
    HumanGoneFor = 0;
	InteractionBehavior = NEW_HUMAN;
	
	beep();

	
    while(1) {
        
        //This if-else block determines if we stay in HumanInteract mode
        //The "!" means NOT, so if the human is NOT found
        if (!HumanFound()) {
            HumanGoneFor++;
			//look for the human if not in view
			if (AffectionCount > 40) {
				InteractionBehavior = WHEREISHUMAN;
			}
            if (HumanGoneFor > 30) {
                printf("\nHuman not found, breaking...");
                break;
            }
        } else {
            HumanGoneFor = 0;
        }
        
        //These if-branches set new behaviors (and change Affection)
        if ((analog10(FRONT_BUMP) < 400 && !(InteractionBehavior == LOVE))|| (digital(BACK_BUMP) == 1 && !(InteractionBehavior == LOVE))) {
            printf("\nKick detected. Affection count before: %d\n", AffectionCount);
            
            //GOAL: lowers affection slightly, but makes the robot act like it hates you for 3 seconds
            AffectionChange(-15);
            printf("Affection count after: %d", AffectionCount);
            InteractionBehavior = HATE;
            HumanBehaviorDuration = 3;
            HumanBehaviorStart = seconds();
        }
        /*
        else if (food()) {
        AffectionCount++
        }
        */
        else if (TimeCheck(HumanBehaviorStart, HumanBehaviorDuration)){
            //updates the behavior based on AffectionCount
            AffectionCounter();
        }
        
        if (FoodSeen() && (HungerLevel != NOT_HUNGRY)) {
            InteractionBehavior = FEED;
        }
        /*
        if (HungerCount < InitialHunger) {
            // or
            //else if (FoodSeen()){
            AffectionChange(10);
            printf("\nHUNGER CHANGED AFFECTION");
            //InteractionBehavior = LOVE;
        }*/
        
        HumanExecute(InteractionBehavior);
        sleep(0.1);
    }
    
    return 0;
}

int AffectionCounter() {
	HasTurned = 0;
    if (AffectionCount <= 20){
        InteractionBehavior = HATE;
    }
    else if (AffectionCount <= 40) {
        InteractionBehavior = DISLIKE;
    }
    else if (AffectionCount <= 60) {
        InteractionBehavior = NEUTRAL;
    }
    else if (AffectionCount <= 80) {
        InteractionBehavior = LIKE;
    }
    else {
        if (HumanFound()) {
            AffectionChange (2);
            printf("\n+2 AFFECTION FROM LOVE BEHAVIOR");
        }
        InteractionBehavior = LOVE;
    }
    HumanBehaviorStart = seconds();
    HumanBehaviorDuration = 3;
}

int HumanExecute(int InteractionBehavior) {
    if (InteractionBehavior == HATE) {
        HumanHate();
    }
    if (InteractionBehavior == DISLIKE) {
        HumanDislike();
    }
    if (InteractionBehavior == NEUTRAL) {
        HumanNeutral();
    }
    if (InteractionBehavior == LIKE) {
        HumanLike();
    }
    if (InteractionBehavior == LOVE) {
        HumanLove();
    }
    if (InteractionBehavior == FEED) {
        Feed();
    }
	if (InteractionBehavior == WHEREISHUMAN) {
		LookAround();
	}
	if (InteractionBehavior == NEW_HUMAN) {
		NewHuman();
	}
}

void HumanHate() {
    //EQUIVALENT TO AVOID BEHAVIOR FROM DEFAULT
	if (HasTurned == 0) {
		int initialTime = seconds();
		int Duration = .5;
		while (!(TimeCheck(initialTime, Duration))) { 
			motor(0, 100);
			motor(3, -100);
		}
		HasTurned = 1;
	}
    int SPEED_THRESHOLD = 70;
	if (ObstacleDetected()) {
		if (leftObstacle) {
			//if there's something to the left, back up the right wheel
			motor(3, -1 * SPEED_THRESHOLD);
		}
		if (rightObstacle) {
			//vice versa
			motor(0, -1 * SPEED_THRESHOLD);
		}
	} else {
		motor(0, 70);
		motor(3, 70);
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

//The robot stops moving, as if watching cautiously to see what the human does
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
    
    else if (track_x(0,0) > 100) {
        motor(0, 70);
        motor(3, 45);
    }
    
    else if (((int)track_x(0,0) > 60) && ((int)track_x < 100)) {
        motor(0, 70);
        motor(3, 70);
    }
    
    else {
        motor(0, 45);
        motor(3, 70);
    }
    printf("HumanLike");
}

void HumanLove() {
    /*
    motor schema? 40 + distance outside of center for the different motors
    */
    if (LovedHumanDetected()) {
        Spin();}
    else {
        ApproachByVision(HUMAN_INDEX, HUMAN_CHANNEL);
    }


    /*
    else if (track_x(HUMAN_INDEX,HUMAN_CHANNEL) > 110) {
        motor(0, 70);
        motor(3, 50);
    }
    
    else if (track_x(HUMAN_INDEX,HUMAN_CHANNEL) > 50 && track_x(HUMAN_INDEX,HUMAN_CHANNEL) < 110) {
        motor(0, 70);
        motor(3, 70);
    }
    else {
        motor(0, 50);
        motor(3, 70);
    } */
    printf("HumanLove");
}

void LookAround() {	
		printf("LookAround");
		int rand = 1 + random() % 4;
		switch (rand) {
			//if the random number is 0 or 1, halt
			case 0: {
				motor(0,0);
				motor(3,0);
				break;
			}
			case 1: {
				motor(0,0);
				motor(3,0);
				break;
			}
			case 2: {
				motor(0,-50);
				motor(3,50);
			}
			case 3: {
				motor(0, 50);
				motor(3, -50);
			}
		}
}

void NewHuman() {
	if (AffectionCount > 60) {
		printf("Oh! A friendly human\n");
		sleep(.5);
		beep();
	}
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

///////////////// UTILITY FUNCTIONS /////////////////
int TimeCheck(double InitialTime, double Duration) {
    if (seconds() >= (InitialTime + Duration))
    return 1;
    else
    return 0;
}

//For use in HumanLike and HumanLove behaviors, approaches and slows to a stop near target
void ApproachByVision(int index, int channel) {
    track_update(index, channel);
    int baseline = 40;
    int MIDPOINT = 80;
    int x = track_x(index, channel);
    /*int addToLeft;
    int addToRight;
    if ((int)(.5 * (80 - x))) < 0) {
        addToRight = 0}
    else{
        addToRight = (int)(.5 * (80 - x)))
    }
    if ((int)(.5 * (80 - x))) < 0) {
        addToLeft = 0}
    else{
        addToLeft = (int)(.5 * (x - 80)))
    }    
    
    motor(0, baseline + (int)(.5 * (x - 80)));    
    motor(3, baseline + ;    
    */
    if (x > MIDPOINT) {
        motor(0, baseline + (int)(.5 * (x - 80)));
        motor(3, baseline);
    } else {
        motor(0, baseline);
        motor(3, baseline + (int)(.5 * (80 - x)));
    }
}

int Spin(){
		printf("Spin");
        int initialTime = seconds();
        int Duration = 5;
        while (!(TimeCheck(initialTime, Duration))){ 
        motor(0, 100);
        motor(3, -100);}
}

int LovedHumanDetected() {
        int left_ir;
        int right_ir;
	
		left_ir = analog10(LEFT_IR);
		right_ir = analog10(RIGHT_IR);
	
        int avoidThreshold = 700; //Check this
        if (left_ir > avoidThreshold) {
            return true;
        }
        else if (right_ir > avoidThreshold) {
            return true;
        }

    else {
        return false;
    }
}



//////////////////////////////
