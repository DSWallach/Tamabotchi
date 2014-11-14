// tamabotchi

int HumanFound();
int TimeCheck();
int WanderOrSleep();
int ObstacleDetected();

int Execute(int CurrentBehavior);

void HumanInteract();
void Wander();
void SleepB();
void Avoid();

const int HUMAN_INDEX = 0;
const int HUMAN_CHANNEL = 0;

//constant integer values which represent the three default behaviors
const int AVOID = 0;
const int WANDER = 1;
const int SLEEP = 2;

const int LEFT_IR = 6;
const int RIGHT_IR = 7;

//used in TimeChecker() to determine when to pick a new WanderOrSleep()
float timeB;

int true = 1;
int false = 0;

int main()
{
    set_each_analog_state(0,0,0,0,0,0,1,1);
    int CurrentBehavior = WanderOrSleep();
    
    //WanderOrSleep continual until While Loop chooses new function
    while(1) {
        if (HumanFound()) {
            HumanInteract();
        }
        else if (ObstacleDetected()){
            CurrentBehavior = AVOID;
        }
     else if (TimeCheck()){
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

//what to do when a human was found
void HumanInteract() {
    beep();
}

void Wander() {
    motor(0, 50);
    motor(3, 70);
}

void SleepB() {
    motor(0, 0);
    motor(3, 0);
}

void Avoid() {
    motor(0, -50);
    motor(3, -50);
}

//returns either WANDER or SLEEP
int WanderOrSleep() {
    int r = 1 + (rand() % 5); //% = modulo
    timeB = seconds();
    if (r <= 4){
        return WANDER;
    }
    else{
        return SLEEP;
    }

}

int TimeCheck() {
    if (seconds() >= (timeB + 10000))
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
    
    if(left_ir > avoidThreshold){
        return true;
    }
    
    else if(right_ir > avoidThreshold){
        return true;
    }
    else{
        return false;
    }
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