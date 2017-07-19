#include <errno.h>
static const char * const error_string[MAXERROR + 1] = {
	[0]			NULL,
	[E_UNSPECIFIED]		"unspecified error",
	[E_BAD_PROC]		"bad process",
	[E_INVAL]		"invalid parameter",
	[E_NO_MEM]		"out of memory",
	[E_NO_FREE_PROC]	"out of process",
	[E_FAULT]		"segmentation fault",
};
const char *strerr(int errno)
{
    if (errno < MAXERROR) return error_string[errno];
    return (const char *)("unknown error");
}
