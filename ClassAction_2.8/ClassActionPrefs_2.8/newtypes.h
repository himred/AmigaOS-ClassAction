#define NOACT LocaleMsg[CAP_NOACT]
#define UNKNOWN LocaleMsg[CAP_UNKNOWN]

struct Action
{
	char name[20];
	char exec[250];
	char wbrun;
	char stack[8];
	char delay[3];
	struct Action *next;
};

struct Class
{
	char name[20];
	char suffix[100];
	char value[5][50];
	struct Class *next;
	struct Action *action;
	struct Action *actiontail;
	int nbaction;
};

struct ClassList
{
	struct Class *head;
	struct Class *tail;
	int size;
};
