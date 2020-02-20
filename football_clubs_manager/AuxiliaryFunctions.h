#include <stdlib.h>
#include <string.h>


typedef struct Player {
	char *name;
	char *position;
	int score;
	int injured;
	struct Player *next;
	struct Player *prev;
} Player;

typedef struct FootballClub {
	char *name;
	Player *players;
	Player *injured_players;
	struct FootballClub *next;
} FootballClub;

void remove_player(FootballClub *clubs, char *club_name, char *player_name);

char* MyStrdup(char *initialString, char *finalString) {
	finalString = malloc((strlen(initialString) + 1) * sizeof(char));
	strcpy(finalString, initialString);
	return finalString;
}

void create_club(FootballClub **newClub, char *name) {
	*newClub = malloc(sizeof(FootballClub));
	(*newClub)->name = MyStrdup(name, (*newClub)->name);
	(*newClub)->players = (*newClub)->injured_players = NULL;
	(*newClub)->next = NULL;
}

Player* create_player(char *name, char *position, int score, int injured) {
	Player *newPlayer = malloc(sizeof(Player));
	newPlayer->name = MyStrdup(name, newPlayer->name);
	newPlayer->position = MyStrdup(position, newPlayer->position);
	newPlayer->score = score;
	newPlayer->injured = injured;
	newPlayer->next = newPlayer->prev = NULL;

	return newPlayer;
}

FootballClub* search_club(FootballClub *clubs, char *club_name) {
    if (clubs == NULL) return NULL;

    FootballClub *head = clubs;
    while (head) {
        if (!strcmp(head->name, club_name)) return head;
        head = head->next;
    }

    return NULL;
}

Player* search_player(FootballClub *club, char *player_name) {
    Player *head = club->players;
    while (head) {
        if (!strcmp(head->name, player_name)) return head;
        head = head->next;
    }

    return NULL;
}

Player* search_injured_player(FootballClub *club, char *player_name) {
	Player *head = club->injured_players;
	while (head) {
		if (!strcmp(head->name, player_name)) return head;
		head = head->next;
	}

	return NULL;
}

void add_injured_player(FootballClub *club, Player *player) {
	if (club->injured_players == NULL) {
		club->injured_players = player;
		return;
	}

	Player *head = club->injured_players;
	while (head->next) {
		if (strcmp(head->name, player->name) > 0) break;
		head = head->next;
	}
	if (head->prev == NULL && strcmp(head->name, player->name) > 0) {
		head->prev = player;
		player->next = head;
		club->injured_players = player;
		return;
	}
	if (head->next == NULL && strcmp(head->name, player->name) < 0) {
		head->next = player;
		player->prev = head;
		return;
	}
	head->prev->next = player;
	player->prev = head->prev;
	player->next = head;
	head->prev = player;
}

void free_player_aux(Player **player) {
	if (*player == NULL) return;

	free((*player)->name);
	(*player)->name = NULL;
	free((*player)->position);
	(*player)->position = NULL;
	free(*player);
	*player = NULL;
}

void remove_injured_player(FootballClub *club, Player *injured_player) {
	// the only player
	if (!injured_player->prev && !injured_player->next) {
		club->injured_players = NULL;
		free_player_aux(&injured_player);
		return;
	}

	// the first player
	if (!injured_player->prev) {
		injured_player->next->prev = NULL;
		club->injured_players = injured_player->next;
		free_player_aux(&injured_player);
		return;
	}

	// last player 
	if (!injured_player->next) {
		injured_player->prev->next = NULL;
		free_player_aux(&injured_player);
		return;
	}

	// in the middle of the list
	injured_player->prev->next = injured_player->next;
	injured_player->next->prev = injured_player->prev;
	free_player_aux(&injured_player);
}

void second_sort(Player **player, char *name, char *position,
	int score, int injured) { 
	Player *new_player = create_player(name, position, score, injured);

	if (*player == NULL) {
		*player = new_player;
		return;
	}

	if (*player == NULL) {
		*player = new_player;
		return;
	}

	// FIRST SORT: descending by score

	// add at the beginning of the list
	if (score > (*player)->score) {
		new_player->next = *player;
		(*player)->prev = new_player;
		*player = new_player;
		return;
	}

	Player *head = *player;
	while (head && score < head->score) {
		if (!head->next) {
			head->next = new_player;
			new_player->prev = head;
			return;
		}
		head = head->next;
	}

	if (score > head->score) {
        // middle insert when it goes beyond the score
		head->prev->next = new_player;
		new_player->prev = head->prev;
		new_player->next = head;
		head->prev = new_player;
		return;
	}

	// SECOND SORT: descending by name

	while (head->score == score && strcmp(head->name, name) < 0) {
		if (!head->next && head->score == score) {
			head->next = new_player;
			new_player->prev = head;
			return;
		}
		head = head->next;
	}
	// insert at the beginning
	if (!head->prev && head->score == score &&
		strcmp(head->name, new_player->name) > 0) {
		new_player->next = head;
		head->prev = new_player;
		*player = new_player;
		return;
	}
	// middle insert for all other possible cases
	head->prev->next = new_player;
	new_player->prev = head->prev;
	new_player->next = head;
	head->prev = new_player;
}

void get_best_team_aux(Player **best_team, Player *list, int no_players) {
	if (!list) return;

	Player *iterator = list;
	while (iterator && no_players-- > 0) {
		second_sort(best_team, iterator->name,
			iterator->position, iterator->score, iterator->injured);
		iterator = iterator->next;
	}
}