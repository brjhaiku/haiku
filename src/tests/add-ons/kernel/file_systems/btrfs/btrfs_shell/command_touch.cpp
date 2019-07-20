#include <stdio.h>
#include <stdlib.h>

#include "syscalls.h"
#include "system_dependencies.h"


namespace FSShell {

	fssh_status_t
	command_touch(int argc, const char* const* argv)
	{
		if(argc < 2 || strcmp(argv[1], "--help") == 0) {
			printf(
					"Usage: %s [FILE]... \n",
					argv[0]
					);
			return FSSH_B_OK;
		}

		const char* const* files = argv + 1;
		for(; *files; files++) {
			const char* file = *files;

			int fd = _kern_open(-1, file, O_RDONLY, O_RDONLY);
			if(fd < 0) {
				// file doesn't exist, create new file
				int fd = _kern_open(-1, file, O_RDWR | FSSH_O_CREAT, O_RDWR);
				if(fd < 0) {
					fssh_dprintf("Error: %s\n", fssh_strerror(fd));
					return FSSH_B_BAD_VALUE;
				}
			}
			else {
				// update existing file's stat
				fssh_struct_stat* fs_stat = new fssh_struct_stat;
				fs_stat->fssh_st_atime = real_time_clock();
				fs_stat->fssh_st_mtime = real_time_clock();
				fs_stat->fssh_st_ctime = real_time_clock();
				uint32 statMask = B_STAT_ACCESS_TIME | B_STAT_MODIFICATION_TIME
					| B_STAT_CHANGE_TIME;
				_kern_write_stat(fd, file, true, fs_stat,
						sizeof(fssh_struct_stat), statMask);
				delete fs_stat;
			}

			status_t status = _kern_close(fd);
			if(status != FSSH_B_OK)
				return status;
		}

		return B_OK;
	}
}
