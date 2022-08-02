#include <G10/GXAI.h>

int create_ai(GXAI_t** ai)
{
	GXAI_t *ret = calloc(1, sizeof(GXAI_t));

	*ai = ret;

	return 0;
}