#include "../includes/Command.h"

Command::Command(int player_uid) {
	player_id = player_uid;
}

void Command::execute(Game* game) {
}

Command::~Command() {
}
