/* The bots' names: Nintendo 64 characters -- Mario's, Zelda's, GoldenEye's,
   Perfect Dark's and Rare's other games' (the user's ask), up to 11 letters
   (the most a player's name holds). Keep this header private to n64_bots.c;
   the menu asks its public lobby-name API. */
#ifndef N64_BOT_NAMES_H
#define N64_BOT_NAMES_H

static char const *const n64_bot_profile_names[] =
{
	/* Super Mario (Super Mario 64, Paper Mario, Mario Kart, Mario Party ...) */
	"Mario", "Luigi", "Peach", "Daisy", "Toad", "Toadette", "Toadsworth", "Yoshi", "Birdo",
	"Bowser", "Bowser Jr.", "Wario", "Waluigi", "Donkey Kong", "Rosalina", "Pauline", "Koopa",
	"Goomba", "Boo", "King Boo", "Shy Guy", "Lakitu", "Spiny", "Hammer Bro", "Kamek",
	"Magikoopa", "Bob-omb", "Chain Chomp", "Thwomp", "Whomp", "Wiggler", "Blooper",
	"Dry Bones", "Petey", "Ludwig", "Lemmy", "Larry", "Wendy", "Iggy", "Morton", "Roy Koopa",
	"Kooper", "Goombario", "Bombette", "Parakarry", "Watt", "Sushie", "Lakilester", "Mips",
	"Ukiki", "Klepto", "Eyerok", "Cappy", "Bullet Bill", "Fawful", "Geno", "Mallow", "Smithy",
	"Tatanga",
	/* The Legend of Zelda (Ocarina of Time, Majora's Mask ...) */
	"Link", "Zelda", "Ganondorf", "Ganon", "Navi", "Epona", "Impa", "Saria", "Darunia", "Ruto",
	"Nabooru", "Rauru", "Sheik", "Malon", "Talon", "Ingo", "Mido", "Kaepora", "Skull Kid",
	"Tingle", "Tatl", "Tael", "Majora", "Kafei", "Anju", "Dampe", "Biggoron", "Beedle",
	"Koume", "Kotake", "Twinrova", "Volvagia", "Gohma", "Dodongo", "Morpha", "Bongo Bongo",
	"Agahnim", "Vaati", "Midna", "Zant", "Tetra", "Medli", "Makar", "Komali", "Ghirahim",
	"Groose", "Revali", "Urbosa", "Daruk", "Mipha", "Kass", "Darmani", "Mikau", "Odolwa",
	"Goht", "Gyorg", "Twinmold", "Romani", "Cremia", "Happy Mask", "Gorman", "Grog",
	"King Zora",
	/* GoldenEye 007 (and the Bond villains its multiplayer had) */
	"Bond", "James Bond", "Natalya", "Trevelyan", "Alec", "Xenia", "Onatopp", "Ourumov",
	"Boris", "Grishenko", "Mishkin", "Valentin", "Zukovsky", "Jack Wade", "Dr. Doak", "Jaws",
	"Oddjob", "Mayday", "Samedi", "Scaramanga", "Nick Nack", "Dr. No", "Goldfinger", "Blofeld",
	"Tee Hee", "Moneypenny", "Q", "Stromberg", "Drax", "Zorin", "Kananga",
	/* Perfect Dark */
	"Joanna Dark", "Joanna", "Velvet Dark", "Elvis", "Carrington", "Cassandra", "Trent",
	"Mr. Blonde", "Jonathan", "Dr. Caroll", "Skedar", "Maian", "Grimshaw", "Foster",
	/* Banjo-Kazooie, Banjo-Tooie */
	"Banjo", "Kazooie", "Mumbo Jumbo", "Mumbo", "Gruntilda", "Bottles", "Tooty", "Klungo",
	"Jamjars", "Jinjo", "Brentilda", "Mingella", "Blobbelda", "Humba Wumba", "Mr. Vile",
	"Boggy", "Wozza", "Gobi", "Clanker", "Blubber", "Chimpy", "Conga", "Nipper", "Tiptup",
	"Mr. Patch", "Loggo", "Terry", "Jingaling", "Gnawty", "Tanktup", "Mr. Fit", "Dingpot",
	"Grublin", "Jolly Roger", "Canary Mary", "Lord Woo", "Salty Joe", "Weldar",
	/* Donkey Kong 64, Donkey Kong Country */
	"Diddy Kong", "Diddy", "Lanky Kong", "Tiny Kong", "Chunky Kong", "Cranky Kong",
	"Funky Kong", "Candy Kong", "Wrinkly", "Dixie Kong", "Kiddy Kong", "K. Rool",
	"King K Rool", "Squawks", "Snide", "Troff", "Scoff", "Klaptrap", "Kasplat", "Kritter",
	"Krusha", "Kosha", "Army Dillo", "Dogadon", "Mad Jack", "Puftoss", "Kremling", "Klump",
	"Necky", "Rambi", "Enguarde", "Squitter", "Expresso", "Krow", "Kudgel", "K. Lumsy",
	"Rattly",
	/* Conker's Bad Fur Day */
	"Conker", "Berri", "Panther", "Rodent", "Franky", "Birdy", "Gregg", "Batula", "Don Weaso",
	"Kriplespac", "Haybot", "Tediz", "Buga", "Marvin", "Fire Imp",
	/* Jet Force Gemini */
	"Juno", "Vela", "Lupus", "Mizar", "Floyd", "King Jeff",
	/* Diddy Kong Racing */
	"Timber", "Pipsy", "Bumper", "Krunch", "Drumstick", "T.T.", "Wizpig", "Taj", "Bubbler",
	"Smokey", "Tricky", "Bluey",
	/* Killer Instinct */
	"Jago", "Fulgore", "Orchid", "Spinal", "Sabrewulf", "Glacius", "Cinder", "Riptor",
	"T.J. Combo", "Thunder", "Kim Wu", "Tusk", "Maya", "Gargos", "Eyedol",
	/* more of Rare's */
	"Sabreman", "Battletoad", "Rash", "Zitz", "Pimple", "Dark Queen", "Kameo", "Cooper",
	"Rattle", "Roll", "Jetman", "Mr. Pants",
};
#define N64_BOT_PROFILE_NAME_COUNT ((unsigned int)(sizeof(n64_bot_profile_names) / sizeof(n64_bot_profile_names[0])))

static inline unsigned int n64_bot_name_gcd(unsigned int a, unsigned int b)
{
	while (b)
	{
		unsigned int t = a % b;
		a = b;
		b = t;
	}
	return a;
}

/* A step relatively prime to the count: offset + step * slot visits every
   name exactly once, so no two bots of a game share one. Names depend only
   on the prepared lobby seed and the bot's stable slot, never on team
   changes or combat randomness. */
static inline int n64_bot_profile_name_index(unsigned int seed, int bot_index)
{
	unsigned int count = N64_BOT_PROFILE_NAME_COUNT;
	unsigned int offset = seed % count;
	unsigned int step = 1u + ((seed / count) ^ (seed >> 16)) % (count - 1u);
	unsigned int slot = bot_index >= 0 && (unsigned int)bot_index < count ? (unsigned int)bot_index : 0u;

	while (n64_bot_name_gcd(step, count) != 1u)
		step = step % (count - 1u) + 1u;
	return (int)((offset + step * slot) % count);
}

static inline char const *n64_bot_profile_name(unsigned int seed, int bot_index)
{
	return n64_bot_profile_names[n64_bot_profile_name_index(seed, bot_index)];
}

#endif
