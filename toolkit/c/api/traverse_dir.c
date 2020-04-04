#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int traverse_dir(char *p_dir)
{
	char fullpath[512] = {};
	DIR *dir = opendir(p_dir);
	struct dirent *ent = NULL;

	while(ent = readdir(dir))
	{
		if ( ! strcmp(".", ent->d_name)
				|| ! strcmp("..", ent->d_name))
		{
			continue;
		}

		sprintf(fullpath, "%s/%s", p_dir, ent->d_name);
		if (4 == ent->d_type) {
			traverse_dir(fullpath);
		} else {
			printf("%s\n", fullpath);
		}
	}

	return 0;
}

int main()
{
	char dirpath[] = ".";
	traverse_dir(dirpath);
	return 0;
}

