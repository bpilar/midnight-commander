### What works:
* resizing window
* edition mode
* key sequence binds 
* text commands
* help screen

### Command specification

Every bind starting with `:` is interpreted as a command and should follow syntax:

`:command_name [argument...]`

where each argument should have format `${argument_name}`. 
Command name must not contain spaces, arguments' names must not contain `}`.

User should separate command name and argumens with at least one space.
For example bind
`:c ${arg1} ${arg2} ${arg3}`
will create command that user can execute by typing:
`:c val1 val2 val3`.

To pass string with spaces as argument value, 
user should either replace them with escape sequence `\ ` 
or write entire string inside single quote marks (`'`). 
Quote marks inside such strings and at the beginning of other strings 
must be replaced with escape sequence `\'`. 
Backslash should always be escaped. 
Bind command from example above could be executed by typing 
`:c a\ b 'c d' '\' \\'`
to pass strings 
`a b`, `c d` and `' \`.





### TODO
* improve text commands' parameters parsing specification
* refactor code (move text command buffering logic from main loop, improve statusline)

