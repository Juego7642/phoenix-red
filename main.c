#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
int minnn(int a, int b) {
	if(a<b) return a;
	else return b;
}
void delay_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);              // milliseconds
#else
    usleep(milliseconds * 1000);      // microseconds
#endif
}
/*
    Multipliers stored as integers to avoid floats:
    20 = 2.0x (super effective)
    10 = 1.0x (normal)
     5 = 0.5x (not very effective)
     0 = 0.0x (immune)
*/

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
};
struct Pokemon {
	char Name[100];
	//real stats
	int max_health;
	//
	int current_health;
	int speed;
	int defense;
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
int damage_calculator(Pokemon *pokemon, Pokemon *pokemonB,
                      move *Move, double random_factor)
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
int special_damage_calculator(Pokemon *pokemon,Pokemon *pokemonB,
                              move *Move, double random_factor)
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
    if (Move.Current_PP <= 0) {
        return 0;
    }

    // Ignore status moves for now
    if (Move.category == CATEGORY_STATUS) {
        return 0;
    }

    if (Move.BP <= 0) {
        return 0;
    }

    // Prevent invalid array index for TYPE_NONE
    if (Move.type == TYPE_NONE || target.type == TYPE_NONE) {
        return 0;
    }

    int effectiveness =
        type_chart[Move.type - 1][target.type - 1];

    switch (effectiveness) {
        case 20:
            return 40;

        case 10:
            return 10;

        case 5:
            return 2;

        case 0:
            return 0;

        default:
            return 10;
    }
}
void turn_processing(Pokemon *A, Pokemon *B, move *MoveA)
{
    if (MoveA->Current_PP <= 0) {
        printf("%s has no PP left!\n", MoveA->Name);
        return;
    }

    printf("\n%s used %s!\n", A->Name, MoveA->Name);

    MoveA->Current_PP--;
	if (B->evasiveness <= 0) B->evasiveness = 1;
    int accuracy = MoveA->Accuracy * A->accuracy / B->evasiveness;

    if (accuracy > 100)
        accuracy = 100;

    if (rand() % 100 >= accuracy) {
        printf("%s's attack missed!\n", A->Name);
        return;
    }

    double random_factor =
        ((rand() % 39) + 217) / 255.0;

    int damage = 0;

    if (MoveA->category == CATEGORY_PHYSICAL) {
        damage = damage_calculator(A, B, MoveA, random_factor);
    }
    else if (MoveA->category == CATEGORY_SPECIAL) {
        damage = special_damage_calculator(A, B, MoveA, random_factor);
    }
    else {
        printf("Status moves are not implemented yet.\n");
        return;
    }

	printf("%s took %d damage.\n\n", B->Name, minnn(B->current_health, damage));

    B->current_health -= damage;

    if (B->current_health < 0)
        B->current_health = 0;
	

    double effectiveness =
        type_effectiveness(MoveA->type, B->type);

    if (effectiveness > 1.0)
        printf("It was super effective!\n");
    else if (effectiveness == 0.0)
        printf("It had no effect!\n");
    else if (effectiveness < 1.0)
        printf("It was not very effective!\n");

}
int main(){
	srand(time(NULL));
	move Tackle = {40, 100, 35, 35, "Tackle", TYPE_NORMAL, CATEGORY_PHYSICAL};
	move Vine_Whip = {45, 100, 25, 25, "Vine Whip", TYPE_GRASS, CATEGORY_PHYSICAL};
	move Scratch = {40, 100, 35, 35, "Scratch", TYPE_NORMAL, CATEGORY_PHYSICAL};
	move Take_Down = {90, 85, 15, 15, "Take Down", TYPE_NORMAL, CATEGORY_PHYSICAL};
	move Ember = {40, 100, 35, 35, "Ember", TYPE_FIRE, CATEGORY_PHYSICAL};
	Pokemon Bulbasaur = {
        .Name = "Bulbasaur",
        .current_health = 45,
        .max_health = 45,
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
        .minor_status_effects = 0
    };
    Pokemon Charmander = {
        .Name = "Charmander",
        .current_health = 39,
        .max_health = 39,
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
        .minor_status_effects = 0
    };
	context contxt;
	contxt.a = Bulbasaur;
	contxt.b = Charmander;
	//announce enemy pokemon
	printf("Enemy pokemon appeared\n");
	//announce player turn.
	printf("Go! %s\n", contxt.a.Name);
	while(contxt.a.current_health > 0 && contxt.b.current_health > 0){
		int zz=1;
		printf("\n--------ROUND %d--------\n", zz++);
		printf("\n%s : (%d/%d).\n", contxt.b.Name, contxt.b.current_health, contxt.b.max_health);
		printf("%s : (%d/%d).\n\n", contxt.a.Name, contxt.a.current_health, contxt.a.max_health);
		for(int i=0; i<4; i++){
			printf("%d. %s PP: %d/%d\n",
	       	i + 1,
		   	contxt.a.Moves[i].Name,
		   	contxt.a.Moves[i].Current_PP,
		   	contxt.a.Moves[i].Max_PP);
		}printf("\n");
		//list moves
		//player makes a choice.
		int zzz;   // move choice
		int valid = 0;
		
		while (!valid) {
		    printf("Enter move number: ");
		    scanf("%d", &zzz);
			if(((zzz>=1) && (zzz<=4)) && contxt.a.Moves[zzz-1].Current_PP>0) {
					contxt.player_a_active_move = &contxt.a.Moves[zzz-1];
		            valid = 1;
		        	break;
			}
		    //     if (contxt.a.Moves[i].Current_PP > 0) {
		    //         contxt.player_a_active_move =
		    //             &contxt.a.Moves[i];
		    //         valid = 1;
		    //         break;
		    //     }
		    // }
		    if (!valid) {
		        printf("Invalid move or no PP left.\n");
		    }
		}
		//move selection done.
		//Now we must head on to processing the move.
		// turn_processing(&contxt.a, &contxt.b, contxt.player_a_active_move);
		// //check if someone died
		// //
		// if(contxt.a.current_health <= 0){
		// 	printf("%s fainted.\n", contxt.a.Name);
		// }
		// if(contxt.b.current_health <= 0){
		// 	printf("%s fainted.\n", contxt.b.Name);
		// }

		int move_weights[4];
		//get move weights (priority)
		move_weights[0] = get_move_weight(contxt.b.Moves[0], contxt.a);
		move_weights[1] = get_move_weight(contxt.b.Moves[1], contxt.a);
		move_weights[2] = get_move_weight(contxt.b.Moves[2], contxt.a);
		move_weights[3] = get_move_weight(contxt.b.Moves[3], contxt.a);
		int max_weight_index=0;	
		//sort between them to find the one with the most weight
		for(int i=0; i<4; i++){
			if(move_weights[i] > move_weights[max_weight_index])
				max_weight_index = i;
		}
		contxt.player_b_active_move = &contxt.b.Moves[max_weight_index];
		if (contxt.a.speed >= contxt.b.speed) {
			// Player attacks first
			delay_ms(2000);
			turn_processing(&contxt.a, &contxt.b,
							contxt.player_a_active_move);

			if (contxt.b.current_health <= 0) {
				printf("%s fainted!\n", contxt.b.Name);
				break;
			}
			delay_ms(1000);
			// Enemy attacks second
			turn_processing(&contxt.b, &contxt.a,
							contxt.player_b_active_move);

			if (contxt.a.current_health <= 0) {
				printf("%s fainted!\n", contxt.a.Name);
				break;
			}
			delay_ms(1000);
		}
		else {
			// Enemy attacks first
			delay_ms(1000);
			turn_processing(&contxt.b, &contxt.a,
							contxt.player_b_active_move);

			if (contxt.a.current_health <= 0) {
				printf("%s fainted!\n", contxt.a.Name);
				break;
			}
			delay_ms(1000);
			// Player attacks second
			turn_processing(&contxt.a, &contxt.b,
							contxt.player_a_active_move);

			if (contxt.b.current_health <= 0) {
				printf("%s fainted!\n", contxt.b.Name);
				break;
			}
			delay_ms(1000);
		}
		//assign that move to active
		// if (contxt.b.current_health <= 0) {
		// 	printf("%s fainted.\n", contxt.b.Name);
		// 	break;
		// }
		// turn_processing(
		// 	&contxt.b,                       // attacker
		// 	&contxt.a,                       // defender
		// 	contxt.player_b_active_move      // selected move
		// );
		// if (contxt.a.current_health <= 0) {
    	// 	printf("%s fainted.\n", contxt.a.Name);
    	// 	break;
		// }
		// //each move will have a weight e.g. how much it makes sense to use this move 
		// //Basically we have to code enemy AI here
		// //go back to step 2 (player turn)
	}
		//this goes on until one of them dies
	return 0;
}



