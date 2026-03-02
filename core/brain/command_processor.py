def proccess_command(command):
    command = command.lower()
    if command == "firefox":
        return "OPEN_FIREFOX"
    elif command == "terminal":
        return "OPEN_TERMINAL"
    elif command == "shutdown":
        return "SHUTDOWN"
    elif command == "exit":
        return "EXIT"
    else:
        return "UNKNOWN"