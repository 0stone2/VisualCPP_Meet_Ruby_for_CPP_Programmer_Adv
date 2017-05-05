#include "stdafx.h"
#include "Ruby.h"


BOOL InitRuby()
{
	static BOOL	bSuccess{ FALSE };

	int		argc{ 1 };
	char *	argv[] = { "-CC:\\Script\\Ruby",  };
	VALUE	variable_in_this_stack_frame;

	__try {
		if (TRUE == bSuccess) __leave;

		ruby_sysinit(&argc, (char ***)&argv);
		ruby_init_stack(&variable_in_this_stack_frame);
		ruby_init();
		ruby_init_loadpath();

		bSuccess = TRUE;
	}
	__finally {
		if (FALSE == bSuccess)
		{
			ruby_cleanup(0);
		}
	}

	return bSuccess;
}
