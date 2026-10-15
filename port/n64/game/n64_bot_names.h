/* Halo CE's original ui\random_player_names pool, read from the owned
   Xbox UI map and checked against the owned PC tag. Keep this header
   private to n64_bots.c; the menu asks its public lobby-name API. */
#ifndef N64_BOT_NAMES_H
#define N64_BOT_NAMES_H

#define N64_BOT_PROFILE_NAME_COUNT 40

static char const *const n64_bot_profile_names[N64_BOT_PROFILE_NAME_COUNT] =
{
	"Sleepy", "Dopey", "Grumpy", "Mopey", "Hollywood", "Crazy", "Saucy", "Wheezy",
	"Cupid", "Killer", "The Big L", "Butcher", "Darling", "Disco", "Donut", "Hambone",
	"Dasher", "Prancer", "Noodle", "Weasel", "Snake", "Shadow", "Caboose", "King",
	"Sneak", "The Bear", "Ghost", "Penguin", "Howard", "Jack", "Whisp", "Whicker",
	"Wilshire", "Pirate", "Walla Walla", "Tooth", "Goat", "Stumpy", "Stompy", "Winky"
};

/* Each step is relatively prime to 40, so offset + step * slot visits
   every name exactly once. Names depend only on the prepared lobby seed
   and the bot's stable slot, never on team changes or combat randomness.
   The first 640 seeds cover all 40 offsets and all 16 permitted steps. */
static inline int n64_bot_profile_name_index(unsigned int seed, int bot_index)
{
	static unsigned char const steps[16] =
		{1, 3, 7, 9, 11, 13, 17, 19, 21, 23, 27, 29, 31, 33, 37, 39};
	unsigned int offset = seed % N64_BOT_PROFILE_NAME_COUNT;
	unsigned int step = steps[((seed / N64_BOT_PROFILE_NAME_COUNT) ^ (seed >> 16)) & 15u];
	unsigned int slot = bot_index >= 0 && bot_index < N64_BOT_PROFILE_NAME_COUNT ?
		(unsigned int)bot_index : 0u;

	return (int)((offset + step * slot) % N64_BOT_PROFILE_NAME_COUNT);
}

static inline char const *n64_bot_profile_name(unsigned int seed, int bot_index)
{
	return n64_bot_profile_names[n64_bot_profile_name_index(seed, bot_index)];
}

#endif
