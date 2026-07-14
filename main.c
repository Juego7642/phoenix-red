#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

int minnn(int a, int b) {
	if(a<b) return a;
	else return b;
}

#define MAX_MESSAGES 30
#define MESSAGE_SIZE 256
#define MESSAGE_DURATION 1.0

char messageBuffer[MAX_MESSAGES][MESSAGE_SIZE];

int messageCount = 0;
int currentMessage = 0;

double messageStartTime = 0.0;
int messageTimerStarted = 0;

void AddBattleMessage(const char *format, ...)
{
    if (messageCount >= MAX_MESSAGES)
        return;

    va_list arguments;

    va_start(arguments, format);

    vsnprintf(
        messageBuffer[messageCount],
        MESSAGE_SIZE,
        format,
        arguments
    );

    va_end(arguments);

    messageCount++;
}

int UpdateBattleMessages(void)
{
    /* No active messages */
    if (currentMessage >= messageCount) {
        messageCount = 0;
        currentMessage = 0;
        messageTimerStarted = 0;

        return 0;
    }

    /* Start timer for the current message */
    if (!messageTimerStarted) {
        messageStartTime = GetTime();
        messageTimerStarted = 1;
    }

    /* Move to the next message after one second */
    if (GetTime() - messageStartTime >= MESSAGE_DURATION) {
        currentMessage++;
        messageTimerStarted = 0;

        /* All messages are finished */
        if (currentMessage >= messageCount) {
            messageCount = 0;
            currentMessage = 0;

            return 0;
        }
    }

    return 1;
}void DrawWrappedText(
    const char *text,
    Rectangle area,
    int fontSize,
    int lineSpacing,
    Color color
) {
    char line[512] = "";
    char word[128] = "";

    int wordLength = 0;
    float y = area.y;

    for (int i = 0;; i++) {
        char c = text[i];

        if (c != ' ' && c != '\n' && c != '\0') {
            if (wordLength < 127)
                word[wordLength++] = c;
        }
        else {
            word[wordLength] = '\0';

            if (wordLength > 0) {
                char testLine[512];

                if (line[0] == '\0')
                    snprintf(testLine, sizeof(testLine), "%s", word);
                else
                    snprintf(testLine, sizeof(testLine), "%s %s", line, word);

                if (MeasureText(testLine, fontSize) <= area.width) {
                    snprintf(line, sizeof(line), "%s", testLine);
                }
                else {
                    DrawText(
                        line,
                        (int)area.x,
                        (int)y,
                        fontSize,
                        color
                    );

                    y += fontSize + lineSpacing;

                    snprintf(line, sizeof(line), "%s", word);
                }
            }

            wordLength = 0;

            if (c == '\n') {
                DrawText(
                    line,
                    (int)area.x,
                    (int)y,
                    fontSize,
                    color
                );

                line[0] = '\0';
                y += fontSize + lineSpacing;
            }

            if (c == '\0')
                break;
        }
    }

    if (line[0] != '\0') {
        DrawText(
            line,
            (int)area.x,
            (int)y,
            fontSize,
            color
        );
    }
}
void DrawBattleTextBox(void)
{
    if (currentMessage >= messageCount)
        return;

    float boxWidth =
        GetScreenWidth() / 2.0f - 40;

    float boxHeight =
        GetScreenHeight() / 2.0f - 40;

    Rectangle bar = {
        20,
        333,
        boxWidth,
        boxHeight
    };

    DrawRectangleRec(
        bar,
        (Color){245, 245, 235, 255}
    );

    DrawRectangleLinesEx(
        bar,
        4,
        BLACK
    );

    Rectangle textArea = {
        bar.x + 15,
        bar.y + 15,
        bar.width - 30,
        bar.height - 30
    };

    DrawWrappedText(
        messageBuffer[currentMessage],
        textArea,
        22,
        2,
        BLACK
    );
}
//                          NOR FIR WAT GRA ELE ICE FIG POI GRO FLY PSY BUG ROC GHO DRA DAR STE FAI
const int type_chart[18][18] = {
/*NORMAL  */ { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  5,  0, 10, 10,  5, 10 },
/*FIRE    */ { 10,  5,  5, 20, 10, 20, 10, 10, 10, 10, 10, 20,  5, 10,  5, 10, 20, 10 },
/*WATER   */ { 10, 20,  5,  5, 10, 10, 10, 10, 20, 10, 10, 10, 20, 10,  5, 10, 10, 10 },
/*GRASS   */ { 10,  5, 20,  5, 10, 10, 10,  5, 20,  5, 10,  5, 20, 10,  5, 10,  5, 10 },
/*ELECTRIC*/ { 10, 10, 20,  5,  5, 10, 10, 10,  0, 20, 10, 10, 10, 10,  5, 10, 10, 10 },
/*ICE     */ { 10,  5,  5, 20, 10,  5, 10, 10, 20, 20, 10, 10, 10, 10, 20, 10,  5, 10 },
/*FIGHTING*/ { 20, 10, 10, 10, 10, 20, 10,  5, 10,  5,  5,  5, 20,  0, 10, 20, 20,  5 },
/*POISON  */ { 10, 10, 10, 20, 10, 10, 10,  5,  5, 10, 10, 10,  5,  5, 10, 10,  0, 20 },
/*GROUND  */ { 10, 20, 10,  5, 20, 10, 10, 20, 10,  0, 10,  5, 20, 10, 10, 10, 20, 10 },
/*FLYING  */ { 10, 10, 10, 20,  5, 10, 20, 10, 10, 10, 10, 20,  5, 10, 10, 10,  5, 10 },
/*PSYCHIC */ { 10, 10, 10, 10, 10, 10, 20, 20, 10, 10,  5, 10, 10, 10, 10,  0,  5, 10 },
/*BUG     */ { 10,  5, 10, 20, 10, 10,  5,  5, 10,  5, 20, 10, 10,  5, 10, 20,  5,  5 },
/*ROCK    */ { 10, 20, 10, 10, 10, 20,  5, 10,  5, 20, 10, 20, 10, 10, 10, 10,  5, 10 },
/*GHOST   */ { 0,  10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 10, 10, 20, 10,  5, 10, 10 },
/*DRAGON  */ { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 10,  5,  0 },
/*DARK    */ { 10, 10, 10, 10, 10, 10,  5, 10, 10, 10, 20, 10, 10, 20, 10,  5,  5,  5 },
/*STEEL   */ { 10,  5,  5, 10,  5, 20, 10, 10, 10, 10, 10, 10, 20, 10, 10, 10,  5, 20 },
/*FAIRY   */ { 10,  5, 10, 10, 10, 10, 20,  5, 10, 10, 10, 10, 10, 10, 20, 20,  5, 10 },
};
//defining minor status effects

#define VOLATILE_NONE	0
#define VOLATILE_CONFUSED (1 << 0)	//00000001
#define VOLATILE_FLINCHED (1 << 1)	//00000010
#define VOLATILE_SEEDED (1 << 2)	//00000100
#define VOLATILE_INFATUATED (1 << 3)	//00001000

typedef struct Pokemon Pokemon;

typedef enum{
	TYPE_NONE,
	TYPE_NORMAL,
	TYPE_FIRE,
	TYPE_WATER,
	TYPE_GRASS,
	TYPE_ELECTRIC,
	TYPE_ICE,
	TYPE_FIGHTING,
	TYPE_POISON,
	TYPE_GROUND,
	TYPE_FLYING,
	TYPE_PSYCHIC,
	TYPE_BUG,
	TYPE_ROCK,
	TYPE_GHOST,
	TYPE_DRAGON,
	TYPE_DARK,
	TYPE_STEEL,
	TYPE_FAIRY
} ElementalType;

typedef enum{
	CATEGORY_PHYSICAL,
	CATEGORY_SPECIAL,
	CATEGORY_STATUS
} MoveCategory;

typedef enum{
	STATUS_NONE,
	STATUS_POISON,
	STATUS_BURN,
	STATUS_PARALYSIS,
	STATUS_SLEEP,
	STATUS_FREEZE
} StatusEffect;
typedef struct move move;
struct move{
	int BP; //Power, base damage used in the damage formula
	int Accuracy; //Base percent chance (0 to 100) for the move to successfully hit the target before modifiers.
	int Max_PP; //Power Points: Max Amount of times a Pokemon can cast a move before needing to replenish energy.
	int Current_PP;
	char Name[100];
	ElementalType type;
	MoveCategory category;
	StatusEffect effect; 
	int probability; //probability of status effect occuring (0-100)
	int status_effect_condition_rounds_left; //useful for status effects that wear off after limited rounds like sleep

};
struct Pokemon {
	char Name[100];
	//real stats
	int max_health;
	//
	int current_health;
	int max_speed;
	int speed;
	int max_defense;
	int defense;
	int max_attack;
	int attack;
	//
	int special_attack;
	int special_defense;
	ElementalType type;
	//evasiveness and accuracy are in-battle stats (always at 100% at start of match);
	int evasiveness;
	int accuracy;
	int max_evasiveness;
	int max_accuracy;
	move Moves[4];
	int level;

	//status effects
	int minor_status_effects;
	StatusEffect effect;
	int status_conditions_rounds_left;\
	//
};
typedef struct BattleContext context;

struct BattleContext{
	Pokemon a;
	Pokemon b;
	move *player_a_active_move;
	move *player_b_active_move;
};

void turn_processing(Pokemon *A, Pokemon *B, move *MoveA);
double type_effectiveness(ElementalType typeA, ElementalType typeB){
	double effectiveness = type_chart[typeA-1][typeB-1]/10.0;
	return effectiveness;
}
int damage_calculator(Pokemon *pokemon, Pokemon *pokemonB, move *Move, double random_factor)
{
	if (pokemonB->defense <= 0) pokemonB->defense = 1;
	int damage = (int)(
		(((((2 * pokemon->level) / 5.0 + 2)
		* Move->BP
		* pokemon->attack / pokemonB->defense) / 50.0 + 2)
		* ((pokemon->type == Move->type) ? 1.5 : 1.0)
		* type_effectiveness(Move->type, pokemonB->type)
		* random_factor)
	);
    return damage;
}
int special_damage_calculator(Pokemon *pokemon,Pokemon *pokemonB, move *Move, double random_factor)
{
	if (pokemonB->special_defense <= 0) pokemonB->special_defense = 1;
    int damage = (int)(
        (((((2 * pokemon->level) / 5.0 + 2)
        * Move->BP
        * pokemon->special_attack
        / pokemonB->special_defense) / 50.0 + 2)
        * ((pokemon->type == Move->type) ? 1.5 : 1.0)
        * type_effectiveness(Move->type, pokemonB->type)
        * random_factor)
    );

    return damage;
}

int get_move_weight(move Move, Pokemon target)
{
	int weight = 0;

    if (Move.Current_PP <= 0) {
        return 0;
    }

    // Prevent invalid array index for TYPE_NONE
    if (Move.type == TYPE_NONE || target.type == TYPE_NONE) {
        return 0;
    }

    int effectiveness = type_chart[Move.type - 1][target.type - 1];
	
    switch (effectiveness) {
        case 20:
            weight += 40;
			break;
        case 10:
            weight += 10;
			break;
        case 5:
            weight += 2;
			break;
        case 0:
            weight += 0;
			break;
    }
	if(Move.effect != STATUS_NONE && target.effect == STATUS_NONE){
		weight += 70;
		//zero truncation!!!
		weight = (weight*Move.probability)/100;
	}
	weight = (weight * (100-(rand() % 25)))/100;
	return weight;
}

void move_status_effect_processing(Pokemon *A, Pokemon *B, move *MoveA){
	int roll = rand() % 100;
	if(MoveA->effect != STATUS_NONE){
		if(B->effect == STATUS_NONE && roll <= MoveA->probability){
			B->effect = MoveA->effect;
			AddBattleMessage("Status effect applied\n");
		}
	}
	if(MoveA->effect == STATUS_POISON){
		B->status_conditions_rounds_left = 15;
	}
	if(MoveA->effect == STATUS_SLEEP){
		B->status_conditions_rounds_left = 1+ rand() % 2;
	}
}
void status_effect_processing(Pokemon *A, Pokemon *B){
	if(A->effect == STATUS_BURN){
		A->current_health -= A->max_health/16;
		AddBattleMessage("%s: %d Damage taken from BURN effect\n", A->Name, A->max_health/16);
	}
	if(A->effect == STATUS_POISON){
		int dam = (16-A->status_conditions_rounds_left)*A->max_health/16;
		if(A->status_conditions_rounds_left > 0)
			A->current_health -= dam;
		else
			A->current_health -= 15 * A->max_health/16;
		AddBattleMessage("%s: %d Damage taken from POISON effect\n", A->Name, dam);
	}
	if(A->effect == STATUS_SLEEP){
		AddBattleMessage("%s is still sleeping.\n", A->Name);
		if(A->status_conditions_rounds_left <= 0){
			A->effect = STATUS_NONE;
			AddBattleMessage("\n%s woke up!\n", A->Name);
		}
	}
	if(A->effect == STATUS_PARALYSIS){
		AddBattleMessage("%s is still paralyzed!\n", A->Name);
		A->speed = (75 * A->max_speed)/100;
	}
	if(A->effect == STATUS_FREEZE){
		int roll = rand() % 256;
		if(roll <= 25){
			AddBattleMessage("%s thawed and can move again!\n", A->Name);
			A->effect = STATUS_NONE;
		}else{
			AddBattleMessage("%s is still frozen and can't do anything\n", A->Name);
		}
	}
	A->status_conditions_rounds_left -= 1;
}
void turn_processing(Pokemon *A, Pokemon *B, move *MoveA)
{
	if(!(A->effect == STATUS_SLEEP || (A->effect == STATUS_PARALYSIS && rand() % 100 <= 25) || A->effect == STATUS_FREEZE)){

		if (MoveA->Current_PP <= 0) {
			AddBattleMessage("%s has no PP left!\n", MoveA->Name);
			return;
		}
		
		AddBattleMessage("\n%s used %s!\n", A->Name, MoveA->Name);
		
		MoveA->Current_PP--;
		if (B->evasiveness <= 0) B->evasiveness = 1;
		int accuracy = MoveA->Accuracy 	* A->accuracy / B->evasiveness;
		
		if (accuracy > 100)
        accuracy = 100;
		
		if (rand() % 100 >= accuracy) {
			AddBattleMessage("%s's attack missed!\n", A->Name);
			return;
		}
		
		double random_factor =
        ((rand() % 39) + 217) / 255.0;
		
		int damage = 0;
		
		if (MoveA->category == CATEGORY_PHYSICAL) {
			damage = damage_calculator(A, B, MoveA, random_factor);
			move_status_effect_processing(A,B,MoveA);
		}
		else if (MoveA->category == CATEGORY_SPECIAL) {
			damage = special_damage_calculator(A, B, MoveA, random_factor);
			move_status_effect_processing(A,B,MoveA);
		}
		else {
			//AddBattleMessage("Status moves are not implemented yet.\n");
			//implement status calculaations
			move_status_effect_processing(A,B,MoveA);
		}
		
		AddBattleMessage("%s took %d damage.\n\n", B->Name, minnn(B->current_health, damage));
		
		B->current_health -= damage;
		
		if (B->current_health < 0)
        B->current_health = 0;
		
		
		double effectiveness =
        type_effectiveness(MoveA->type, B->type);
		
		if (effectiveness > 1.0)
        AddBattleMessage("It was super effective!\n");
		else if (effectiveness == 0.0)
        AddBattleMessage("It had no effect!\n");
		else if (effectiveness < 1.0)
        AddBattleMessage("It was not very effective!\n");
	}

}
int main(){
	srand(time(NULL));
	move Tackle = {40, 100, 35, 35, "Tackle", TYPE_NORMAL, CATEGORY_PHYSICAL, STATUS_PARALYSIS, 100};
	move Vine_Whip = {45, 100, 25, 25, "Vine Whip", TYPE_GRASS, CATEGORY_PHYSICAL};
	move Scratch = {40, 100, 35, 35, "Scratch", TYPE_NORMAL, CATEGORY_PHYSICAL};
	move Take_Down = {90, 85, 15, 15, "Take Down", TYPE_NORMAL, CATEGORY_PHYSICAL};
	move Ember = {40, 100, 35, 35, "Ember", TYPE_FIRE, CATEGORY_PHYSICAL};
	Pokemon Bulbasaur = {
        .Name = "Bulbasaur",
        .current_health = 45,
        .max_health = 45,
		.max_speed = 45,
        .speed = 45,
        .defense = 49,
        .attack = 49,
        .special_attack = 65,
        .special_defense = 65,
        .accuracy = 100,
        .max_accuracy = 100,
        .evasiveness = 100,
        .type = TYPE_GRASS,
        .max_evasiveness = 100,
        .Moves = {Tackle, Scratch, Vine_Whip, Take_Down},
        .level = 5,
        .minor_status_effects = 0,
		.effect = STATUS_NONE
    };
    Pokemon Charmander = {
        .Name = "Charmander",
        .current_health = 39,
        .max_health = 39,
		.max_speed = 25,
        .speed = 25,
        .defense = 25,
        .attack = 25,
        .special_attack = 25,
        .special_defense = 25,
        .accuracy = 100,
        .max_accuracy = 100,
        .evasiveness = 100,
        .type = TYPE_FIRE,
        .max_evasiveness = 100,
        .Moves = {Tackle, Scratch, Ember, Take_Down},
        .level = 5,
        .minor_status_effects = 0,
		.effect = STATUS_NONE
    };
	context contxt;
	contxt.a = Bulbasaur;
	contxt.b = Charmander;
	//announce enemy pokemon
	AddBattleMessage("Enemy pokemon appeared\n");
	//announce player turn.
	//AddBattleMessage("Go! %s\n", contxt.a.Name);
	InitWindow(800, 450, "Pokemon Battle");
	SetTargetFPS(60);

	Texture2D bulbasaurSprite = LoadTexture("bulbasaur_back.png");

	Texture2D charmanderSprite = LoadTexture("charmander_front.png");

	int selectedMove = 0;

	int displayedPlayerHP = contxt.a.current_health;
	int displayedEnemyHP = contxt.b.current_health;

	double lastHpUpdate = 0;

	while (!WindowShouldClose()) {

		int messageActive = UpdateBattleMessages();
		if (!messageActive){
			
			int hpAnimating = displayedPlayerHP != contxt.a.current_health || displayedEnemyHP != contxt.b.current_health;

			int chosenMove = -1;

			if (!hpAnimating && contxt.a.current_health > 0 && contxt.b.current_health > 0) {

				if (IsKeyPressed(KEY_ONE)) chosenMove = 0;
				if (IsKeyPressed(KEY_TWO)) chosenMove = 1;
				if (IsKeyPressed(KEY_THREE)) chosenMove = 2;
				if (IsKeyPressed(KEY_FOUR)) chosenMove = 3;
			}

			if (chosenMove != -1 && contxt.a.Moves[chosenMove].Current_PP > 0) {
				selectedMove = chosenMove;
				contxt.player_a_active_move = &contxt.a.Moves[selectedMove];
				int bestEnemyMove = 0;
				int bestWeight = get_move_weight(contxt.b.Moves[0], contxt.a);
				for (int i = 1; i < 4; i++) {
					int weight =
						get_move_weight(contxt.b.Moves[i], contxt.a);
					if (weight > bestWeight) {
						bestWeight = weight;
						bestEnemyMove = i;
					}
				}
				contxt.player_b_active_move = &contxt.b.Moves[bestEnemyMove];

				if (contxt.a.speed >= contxt.b.speed) {
						turn_processing(&contxt.a, &contxt.b, contxt.player_a_active_move);

					if (contxt.b.current_health > 0) {
						turn_processing(&contxt.b, &contxt.a, contxt.player_b_active_move);
					}
				}
				else {
					turn_processing(&contxt.b, &contxt.a, contxt.player_b_active_move);

					if (contxt.a.current_health > 0) {
						turn_processing( &contxt.a, &contxt.b, contxt.player_a_active_move);
					}
				}

				if (contxt.a.current_health > 0 && contxt.b.current_health > 0) {

					status_effect_processing(&contxt.a, &contxt.b);
					status_effect_processing(&contxt.b, &contxt.a);
				}

				if (contxt.a.current_health < 0) contxt.a.current_health = 0;

				if (contxt.b.current_health < 0) contxt.b.current_health = 0;
			}

			if (GetTime() - lastHpUpdate >= 0.04) {

				if (displayedPlayerHP > contxt.a.current_health)
					displayedPlayerHP--;

				if (displayedEnemyHP > contxt.b.current_health)
					displayedEnemyHP--;

				lastHpUpdate = GetTime();
			}

		}
		BeginDrawing();
		ClearBackground(RAYWHITE);

		//textbox
		DrawBattleTextBox();

		DrawTextureEx(bulbasaurSprite, (Vector2){90, 190}, 0.0f, 3.0f, WHITE);
		DrawTextureEx(charmanderSprite, (Vector2){560, 40},0.0f, 3.0f, WHITE);

		DrawText(TextFormat("%s  Lv.%d", contxt.b.Name, contxt.b.level),50, 40, 25, BLACK);
		DrawText(TextFormat("HP: %d/%d", displayedEnemyHP, contxt.b.max_health),50, 70, 22, BLACK);

		/* Player real stats */
		DrawText(TextFormat("%s  Lv.%d",contxt.a.Name,contxt.a.level),500, 240, 25, BLACK);
		DrawText(TextFormat("HP: %d/%d",displayedPlayerHP, contxt.a.max_health),500, 270, 22, BLACK);

		Vector2 positions[4] = {
			{400, 330},
			{590, 330},
			{400, 385},
			{590, 385}
		};

		for (int i = 0; i < 4; i++) {

			int x = positions[i].x;
			int y = positions[i].y;

			if (selectedMove == i)
				DrawRectangle(x, y, 180, 45, LIGHTGRAY);

			DrawRectangleLines(x, y, 180, 45, BLACK);

			DrawText(
				TextFormat("%d. %s",
						i + 1,
						contxt.a.Moves[i].Name),
				x + 10, y + 5, 18, BLACK
			);

			DrawText(
				TextFormat("PP: %d/%d",
						contxt.a.Moves[i].Current_PP,
						contxt.a.Moves[i].Max_PP),
				x + 10, y + 27, 12, DARKGRAY
			);
		}

		if (contxt.a.current_health <= 0) {
			DrawText(
				TextFormat("%s fainted!", contxt.a.Name),
				280, 180, 30, RED
			);
		}
		else if (contxt.b.current_health <= 0) {
			DrawText(
				TextFormat("%s fainted!", contxt.b.Name),
				280, 180, 30, RED
			);
		}
		
		EndDrawing();
	}

	UnloadTexture(bulbasaurSprite);
	UnloadTexture(charmanderSprite);
	CloseWindow();

	return 0;
}
