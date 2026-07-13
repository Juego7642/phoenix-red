#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
int main(){
	srand(time(NULL));
	Pokemon Bulbasaur;
	Pokemon Charizard;

	context contxt;
	contxt.a = Bulbasaur;
	contxt.b = Charizard;
	//announce enemy pokemon
	printf("Enemy pokemon appeared\n");
	while(contxt.a.current_health > 0 && contxt.b.current_health > 0){
	
		//announce player turn.
		printf("Go! %s\n", contxt.a.Name);
		for(int i=0; i<4; i++){
			printf("%d. %s PP: %d/%d\n",
	       	i + 1,
		   	contxt.a.Moves[i].Name,
		   	contxt.a.Moves[i].Current_PP,
		   	contxt.a.Moves[i].Max_PP);
		}printf("\n");
		//list moves
		//player makes a choice.
		char s[100];   // move choice
		int valid = 0;
		
		while (!valid) {
		    printf("Enter move name: ");
		    scanf("%99s", s);
		    for (int i = 0; i < 4; i++) {
		        if (strcmp(s, contxt.a.Moves[i].Name) == 0 &&
		            contxt.a.Moves[i].Current_PP > 0) {
		            contxt.player_a_active_move =
		                &contxt.a.Moves[i];
		            valid = 1;
		            break;
		        }
		    }
		    if (!valid) {
		        printf("Invalid move or no PP left.\n");
		    }
		}
		//move selection done.
		//Now we must head on to processing the move.
		turn_processing(&contxt.a, &contxt.b, &contxt.pokemon_a_active_move);
		//check if someone died
		//
		if(contxt.a.current_health <= 0){
			printf("%s fainted.\n", contxt.a.Name);
		}
		if(contxt.b.current_health <= 0){
			printf("%s fainted.\n", contxt.b.Name);
		}
		//enemy uses move.
		//
		//
		//Okay time to code enemy logic
		//
		//
		//
		//
		//
		//first define move weights
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
		//assign that move to active
		contxt.player_b_active_move = contxt.b.Moves[max_weight_index];
		//each move will have a weight e.g. how much it makes sense to use this move 
		//Basically we have to code enemy AI here
		//go back to step 2 (player turn)
	}
		//this goes on until one of them dies
	return 0;
}

	void turn_processing(Pokemon *A, Pokemon *B, move *MoveA){
		if(MoveA->category == MoveCategory.CATEGORY_PHYSICAL){
			int damage = damage_calculator(A, B, MoveA, ((rand() % (255-217+1)) + 217)/255);
			int accuracy = MoveA->Accuracy * (A->accuracy / B->evasiveness);
			if(rand() < accuracy){
				printf("%s used %s!\n", A->Name, MoveA->Name);
				B->current_health -= damage;
				MoveA->Current_PP -=1;
				if(type_effectiveness(MoveA->type, B->type) > 10)
					printf("It was super effective!\n");
				else if(type_effectiveness(MoveA->type, B->type < 10)
					printf("It was not very effective!\n");
				// move lands
			}else{
				// miss
				printf("%s's attack missed!\n", A->Name);
			}
		}
		if(MoveA->category == MoveCategory.CATEGORY_SPECIAL){
			int damage = special_damage_calculator(contxt.pokemon.a, contxt.pokemon.b, contxt.pokemon_a_active_move, ((rand() % (255-217+1)) + 217)/255);
			int accuracy = MoveA->Accuracy * (A->accuracy / B->evasiveness);
			if(rand() < accuracy){
				printf("%s used %s!\n", A->Name, MoveA->Name);
				B->current_health -= damage;
				MoveA->Current_PP -=1;
				if(type_effectiveness(MoveA->type, B->type) > 10)
					printf("It was super effective!\n");
				else if(type_effectiveness(MoveA->type, B->type < 10)
					printf("It was not very effective!\n");
				// move lands
			}else{
				// miss
				printf("%s's attack missed!\n", A->Name);
			}
		}
	}
double type_effectiveness(ElementalType typeA, ElementalType typeB){
	double effectiveness = type_chart[typeA-1][typeB-1]/10;
	return effectiveness;
}
int damage_calculator(Pokemon pokemon, Pokemon pokemonB, move Move, int critical){
	int damage =((((2*pokemon.level*critical)/5 + 2) * BP *  attack/defense)/50 + 2) * ((pokemon.type == Move.type) ? (1.5) : (1)) * type_effectiveness(Move.type, pokemonB.type);
	return damage;
}
int special_damage_calculator(Pokemon pokemon, Pokemon pokemonB, move Move, int critical){
	int damage =((((2*pokemon.level*critical)/5 + 2) * BP *  special_attack/special_defense)/50 + 2) * ((pokemon.type == Move.type) ? (1.5) : (1)) * type_effectiveness(Move.type, pokemonB.type);
	return damage;
}

int get_move_weight(move Move, Pokemon target){
	if (Move.Current_PP == 0){
		return 0;
	}
	//ignore status moves from now
	if (Move.category == MoveCategory.CATEGORY_STATUS){
		return 0;
	}
	if (Move.BP <= 0){
		return 0;
	}
	int effectiveness = type_effectiveness(Move.type, target.type);
	
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
