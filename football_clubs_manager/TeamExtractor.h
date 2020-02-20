
#ifndef TEAM_EXTRACTOR_H_D
#define TEAM_EXTRACTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FootballClub.h"


Player* union_teams(FootballClub *clubs, char *club_A, char *club_B) {
	if (!clubs) return NULL;

	FootballClub *first_club = search_club(clubs, club_A);
	FootballClub *second_club = search_club(clubs, club_B);
	if (!first_club || !second_club) return NULL;

	FootballClub *club;
	create_club(&club, "club");

	Player *head = first_club->players;
	while (head) {
		add_player(club, "club", head->name, head->position, head->score);
		head = head->next;
	}
	head = second_club->players;
	while (head) {
		add_player(club, "club", head->name, head->position, head->score);
		head = head->next;
	}

	Player *player = club->players;
	free(club->name);
	club->name = NULL;
	free(club);
	club = NULL;	
	return player;
}

Player* get_best_player(FootballClub *clubs, char *position) {
	if (!clubs) return NULL;

	Player *best_player = NULL; // a Player *pointer used for reference
	FootballClub *club = clubs; // club list iterator
	Player *player = NULL; // uninjured players iterator of each club
	while (club) {
		player = club->players;
		while (player) {
			if (strcmp(player->position, position) == 0) {
				best_player = player;
				club = NULL;
				break;
			}
			player = player->next;
		}
		
		if (!club) break;
		club = club->next;
	}
	if (!best_player) return NULL;

	club = clubs;
	while (club) {
		player = club->players;
		while (player) {
			if (strcmp(player->position, best_player->position) == 0 &&
				player->score > best_player->score) best_player = player; 
			player = player->next;
		}
		club = club->next;
	}

	Player *return_player = create_player(best_player->name,
		best_player->position, best_player->score, 0);
	return return_player;
}

Player* get_top_players(FootballClub *clubs, int N) {
	if (!clubs) return NULL;

	Player *return_list = NULL, *sorted_player = NULL;
	FootballClub *club = clubs;
	Player *player = NULL;
	while (club) {
		player = club->players;
		while(player) {
			second_sort(&sorted_player,
				player->name, player->position,
				player->score, 0);
			player = player->next;
		}

		Player *head = sorted_player;
		int cN = N;
		while (cN-- > 0 && head) {
			second_sort(&return_list, head->name,
				head->position, head->score, 0);
			head = head->next;
		}

		destroy_player_list(sorted_player);
		sorted_player = NULL;

		club = club->next;
	}

	return return_list;
}

Player *get_players_by_score(FootballClub *clubs, int score) {
	if (!clubs) return NULL;
	if (score < -100 || score > 100) return NULL;

	Player *players_by_score = NULL;
	FootballClub *club = clubs;
	Player *player = NULL, *injured_player = NULL;
	while (club) {
		player = club->players;
		while (player) {
			if (player->score >= score) second_sort(&players_by_score,
				player->name, player->position,
				player->score, 0);
			player = player->next;
		}

		injured_player = club->injured_players;
		while (injured_player) {
			if (injured_player->score >= score) second_sort(&players_by_score,
				injured_player->name, injured_player->position,
				injured_player->score, 1);
			injured_player = injured_player->next;
		}
		club = club->next;
	}

	return players_by_score;
}

Player *get_players_by_position(FootballClub *clubs, char *position) {
	if (!clubs) return NULL;

	Player *players_by_position = NULL;
	FootballClub *club = clubs;
	Player *player = NULL, *injured_player = NULL;
	while (club) {
		player = club->players;
		while (player) {
			if (!strcmp(player->position, position))
				second_sort(&players_by_position, player->name,
				player->position, player->score, 0);
			player = player->next;
		}

		injured_player = club->injured_players;
		while (injured_player) {
			if (!strcmp(injured_player->position, position))
				second_sort(&players_by_position, injured_player->name,
				injured_player->position, injured_player->score, 1);
			injured_player = injured_player->next;
		}
		club = club->next;
	}

	return players_by_position;
}

Player *get_best_team(FootballClub *clubs) {
	if (!clubs) return NULL;

	Player *atacanti = get_players_by_position(clubs, "atacant");
	Player *fundasi = get_players_by_position(clubs, "fundas");
	Player *mijlocasi = get_players_by_position(clubs, "mijlocas");
	Player *portari = get_players_by_position(clubs, "portar");
	
	Player *return_list = NULL;
	get_best_team_aux(&return_list, atacanti, 3);
	get_best_team_aux(&return_list, fundasi, 4);
	get_best_team_aux(&return_list, mijlocasi, 3);
	get_best_team_aux(&return_list, portari, 1);

	destroy_player_list(atacanti);
	destroy_player_list(fundasi);
	destroy_player_list(mijlocasi);
	destroy_player_list(portari);

	return return_list;
}

#endif // TEAM_EXTRACTOR_H_INCLUDED