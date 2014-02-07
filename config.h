void moveUp();
void moveDown();
void quit();
void enter();
void leave();

struct key {
	uint16_t key;
	uint32_t ch;
	void(*action)(void);
};

struct key keys[] = {
	{0, 'q', &quit},
	{0, 'n', &moveUp},
	{0, 't', &moveDown},
	{0, 'h', &leave},
	{0, 's', &enter},
	{TB_KEY_ARROW_UP, 0, &moveUp},
	{TB_KEY_ARROW_DOWN, 0, &moveDown},
	{TB_KEY_ARROW_LEFT, 0, &leave},
	{TB_KEY_ARROW_RIGHT, 0, &enter},
};
#define NUMKEYS 5
