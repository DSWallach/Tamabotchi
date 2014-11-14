// tamabotchi

int HumanFound();


void HumanInteract();
void Wander();

const int HUMAN_INDEX = 0;
const int HUMAN_CHANNEL = 0;

int main() 
{
	set_each_analog_state(0,0,0,0,0,0,1,1);
	
	while(1) {
		if (HumanFound()) {
			HumanInteract();
		}
		else {
			Wander();
		}
	}
	
	return 0;
}

int HumanFound() {
	track_update();
	return ((track_size(HUMAN_CHANNEL, HUMAN_INDEX) > 10) && (track_confidence(HUMAN_CHANNEL, HUMAN_INDEX) > 40));
}

void HumanInteract() {
	beep();
}

void Wander() {
	
}
