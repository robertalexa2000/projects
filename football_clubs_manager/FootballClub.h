#ifndef FOOTBALL_CLUB_H_D
#define FOOTBALL_CLUB_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AuxiliaryFunctions.h"


FootballClub* initialize_clubs(int clubs_no, char **names) {
	FootballClub *clubs = NULL;
	int i;
	for (i = clubs_no - 1; i >= 0; i--) {
		FootballClub *newClub;
		create_club(&newClub, names[i]);

		if (clubs == NULL) {
			clubs = newClub;
		} else {
			newClub->next = clubs;
			clubs = newClub;
		}
	}

    /* Iterate through the array of string backwards to insert
    at the beginning */

	return clubs;
}

FootballClub *add_club(FootballClub *clubs, char *name) {
	FootballClub *newClub;
	create_club(&newClub, name);

	if (!clubs) {
		clubs = newClub;
		return clubs;
	}

	FootballClub *head = clubs;
	while (head->next) head = head->next;
	head->next = newClub;

	return clubs;
}

void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score) {
	if (clubs == NULL) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *newPlayer = create_player(player_name, position, score, 0);

	// null list
	if (!club->players) {
		club->players = newPlayer;
		return;
	}

	// more elements in list
	Player *head = club->players;

	// FIRST SORT: by football positions
	if (strcmp(head->position, newPlayer->position) > 0) {
		newPlayer->next = head;
		head->prev = newPlayer;
		club->players = newPlayer;
		return;
	}

	while (strcmp(head->position, newPlayer->position) < 0) {
		if (!head->next) {
			// add a new category of players at the end of the list
			head->next = newPlayer;
			newPlayer->prev = head;
			return;
		}
		head = head->next;
	}
	if (strcmp(head->position, newPlayer->position) > 0) {
        /* insert in the middle by position when there position
        does not exist */

		head->prev->next = newPlayer;
		newPlayer->prev = head->prev;
		newPlayer->next = head;
		head->prev = newPlayer;
		return;
	}

	// SECOND SORT: by score
	while (strcmp(head->position, newPlayer->position) == 0 &&
		head->score > newPlayer->score) {
		if (!head->next && strcmp(head->position, newPlayer->position) == 0) {
			head->next = newPlayer;
			newPlayer->prev = head;
			return;
		}
		head = head->next;
	}
	if (!head->prev && strcmp(head->position, newPlayer->position) == 0 &&
		head->score < newPlayer->score) {
		newPlayer->next = head;
		head->prev = newPlayer;
		club->players = newPlayer;
		return;
	}
	if ( (strcmp(head->position, newPlayer->position) == 0 &&
		head->score < newPlayer->score) ||
		(strcmp(head->position, newPlayer->position) > 0) ) {
		head->prev->next = newPlayer;
		newPlayer->prev = head->prev;
		newPlayer->next = head;
		head->prev = newPlayer;
		return;
	}

	// THIRD SORT: by name
	while (strcmp(head->position, newPlayer->position) == 0 &&
		head->score == newPlayer->score &&
		strcmp(head->name, newPlayer->name) < 0) {
		if (!head->next) {
			head->next = newPlayer;
			newPlayer->prev = head;
			return;
		}
		head = head->next;
	}
	if (!head->prev && strcmp(head->position, newPlayer->position) == 0 &&
		head->score == newPlayer->score &&
		strcmp(head->name, newPlayer->name) > 0) {
		newPlayer->next = head;
		head->prev = newPlayer;
		club->players = newPlayer;
		return;
	}
	head->prev->next = newPlayer;
	newPlayer->prev = head->prev;		
	newPlayer->next = head;
	head->prev = newPlayer;
}				

void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club) {
	if (!clubs) return;

	FootballClub *first_club = search_club(clubs, old_club);
	FootballClub *second_club = search_club(clubs, new_club);
	if (!first_club || !second_club) return;
	Player *player = search_player(first_club, player_name);
	Player *injured_player = NULL;
	if (!player) {
		injured_player = search_injured_player(first_club, player_name);
		if (!injured_player) return;
	}

	if (player) {
		char *position = NULL;
		position = MyStrdup(player->position, position);
		int score = player->score;
		remove_player(clubs, old_club, player_name);
		add_player(clubs, new_club, player_name, position, score);
		free(position);
		position = NULL;
	} else {
		Player *new_injured_player = create_player(player_name,
			injured_player->position, injured_player->score, 1);
		remove_injured_player(first_club, injured_player);
		add_injured_player(second_club, new_injured_player);
	}
}

void remove_player(FootballClub *clubs, char *club_name, char *player_name) {
	if (!clubs) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *player = search_player(club, player_name);
	Player *injured_player = NULL;
	if (!player) {
		injured_player = search_injured_player(club, player_name);
		if (!injured_player) return;
	}

	if (!injured_player) {
		if (!player->prev && !player->next) {
			club->players = NULL;
			free_player_aux(&player);
			return;
		}

		if (!player->prev) {
			player->next->prev = NULL;
			club->players = player->next;
			free_player_aux(&player);
			return;
		}

		if (!player->next) {
			player->prev->next = NULL;
			free_player_aux(&player);
			return;
		}

		player->prev->next = player->next;
		player->next->prev = player->prev;
		free_player_aux(&player);
	} else {
		if (!injured_player->prev && !injured_player->next) {
			club->injured_players = NULL;
			free_player_aux(&injured_player);
			return;
		}

		if (!injured_player->prev) {
			injured_player->next->prev = NULL;
			club->injured_players = injured_player->next;
			free_player_aux(&injured_player);
			return;
		}

		if (!injured_player->next) {
			injured_player->prev->next = NULL;
			free_player_aux(&injured_player);
			return;
		}

		injured_player->prev->next = injured_player->next;
		injured_player->next->prev = injured_player->prev;
		free_player_aux(&injured_player);
	}
}

void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score) {
	if (!clubs) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *player = search_player(club, player_name);
	Player *injured_player = NULL;
	if (!player) {
		injured_player = search_injured_player(club, player_name);
		if (!injured_player) return;
	}

	if (player) {
		char *position = NULL;
		position = MyStrdup(player->position, position);
		remove_player(clubs, club_name, player_name);
		add_player(clubs, club_name, player_name, position, score);
		free(position);
		position = NULL;
	} else {
		Player *new_injured_player = create_player(player_name,
			injured_player->position, score, 1);
		remove_injured_player(club, injured_player);
		add_injured_player(club, new_injured_player);
	}
}

void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score) {
	if (!clubs) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *player = search_player(club, player_name);
	Player *injured_player = NULL;
	if (!player) {
		injured_player = search_injured_player(club, player_name);
		if (!injured_player) return;
	}

	if (player) {
		remove_player(clubs, club_name, player_name);
		add_player(clubs, club_name, player_name, position, score);
	} else {
		Player *new_injured_player = create_player(player_name, position, score, 1);
		remove_injured_player(club, injured_player);
		add_injured_player(club, new_injured_player);
	}
}

void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no) {
	if (clubs == NULL) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *player = search_player(club, player_name);
	if (!player) return;

	Player *injuredPlayer = create_player(player->name,
		player->position, player->score, 1);
	injuredPlayer->score = injuredPlayer->score - 0.1 * days_no;
	if (injuredPlayer->score < -100) injuredPlayer->score = -100;
	remove_player(clubs, club_name, player_name);
	add_injured_player(club, injuredPlayer);
}

void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name) {
	if (!clubs) return;

	FootballClub *club = search_club(clubs, club_name);
	if (!club) return;
	Player *injured_player = search_injured_player(club, player_name);
	if (!injured_player) return;

	char *position = NULL;
	position = MyStrdup(injured_player->position, position);
	int score = injured_player->score;
	remove_injured_player(club, injured_player);
	add_player(clubs, club_name, player_name, position, score);
	free(position);
	position = NULL;
}

// Frees memory for a list of Player.
void destroy_player_list(Player *player) {
	if (!player) return;

	while (player) {
		Player *tmp = player;
		player = player->next;
		free_player_aux(&tmp);
	}
}

// Frees memory for a list of FootballClub.
void destroy_club_list(FootballClub *clubs) {
	if (!clubs) return;

	while (clubs) {
		destroy_player_list(clubs->players);
		destroy_player_list(clubs->injured_players);
		free(clubs->name);
		clubs->name = NULL;
		FootballClub *tmp = clubs;
		clubs = clubs->next;
		free(tmp);
		tmp = NULL;
	}
}

// Displays a list of players.
void show_list(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	while (player) {
		fprintf(f, "(%s, %s, %d, %c) ", 
			player->name,
			player->position,
			player->score,
			player->injured ? 'Y' : '_');
		player = player->next;
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}

// Displays a list of players in reverse.
void show_list_reverse(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	if (player) {
		while (player->next) {
			player = player->next;
		}
		while (player) {
			fprintf(f, "(%s, %s, %d, %c) ", 
				player->name,
				player->position,
				player->score,
				player->injured ? 'Y' : '_');
			player = player->prev;
		}
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}


// Displays information about a football club.
void show_clubs_info(FILE *f, FootballClub *clubs) {
	fprintf(f, "FCs:\n");
	while (clubs) {
		fprintf(f, "%s\n", clubs->name);
		fprintf(f, "\t");
		show_list(f, clubs->players, 0);
		fprintf(f, "\t");
		show_list(f, clubs->injured_players, 0);
		clubs = clubs->next;
	}
}

#endif // FOOTBALL_CLUB_H_INCLUDED