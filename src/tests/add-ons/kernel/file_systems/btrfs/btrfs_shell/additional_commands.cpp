#include "fssh.h"

#include "command_touch.h"


namespace  FSShell {

	void register_additional_commands()
	{
		CommandManager::Default()->AddCommands(command_touch, "touch",
				"Create new empty file(s)", NULL);
	}
}	// namespace FSShell
