/*
 * Copyright (C) 2017 Tobias Brunner
 * HSR Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * Translate - into _
 */
static char *underscores(char *str)
{
	char *pos;

	for (pos = str; pos && *pos; pos++)
	{
		if (*pos == '-')
		{
			*pos = '_';
		}
	}
	return str;
}

/**
 * Declare a plugin constructor for the given plugin
 */
static void declare_constructor(char *plugin)
{
	char name[128];

	snprintf(name, sizeof(name), "%s", plugin);
	underscores(name);
	printf("plugin_t *%s_plugin_create();\n", name);
}

/**
 * Register (or unregister) a constructor
 */
static void register_constructor(char *plugin, bool reg)
{
	char name[128];

	snprintf(name, sizeof(name), "%s", plugin);
	underscores(name);
	if (reg)
	{
		printf("	plugin_constructor_register(\"%s\", %s_plugin_create);\n",
			   plugin, name);
	}
	else
	{
		printf("	plugin_constructor_register(\"%s\", NULL);\n",
			   plugin);
	}
}

int main(int argc, char* argv[])
{
	int i;

	printf("/**\n");
	printf(" * Register plugin constructors for static libraries\n");
	printf(" * Created by %s\n", argv[0]);
	printf(" */\n");
	printf("\n");
	printf("#include <plugins/plugin.h>\n");
	printf("#include <plugins/plugin_loader.h>\n");
	printf("\n");

	for (i = 1; i < argc; i++)
	{
		declare_constructor(argv[i]);
	}

	printf("\n");
	printf("static void register_plugins() __attribute__ ((constructor));\n");
	printf("static void register_plugins()\n");
	printf("{\n");

	for (i = 1; i < argc; i++)
	{
		register_constructor(argv[i], true);
	}

	printf("};\n");

	printf("\n");
	printf("static void unregister_plugins() __attribute__ ((destructor));\n");
	printf("static void unregister_plugins()\n");
	printf("{\n");

	for (i = 1; i < argc; i++)
	{
		register_constructor(argv[i], false);
	}

	printf("};\n");
	printf("\n");

	return 0;
}
