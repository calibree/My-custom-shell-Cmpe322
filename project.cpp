#include <bits/stdc++.h>
#include <regex>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

int main ()
{

vector<string> history_array; // Keeps previous commands

    while(1){

    cout<<getenv("USER")<<" >>> "; // Prints the username >>>

    string user_command; // Holds the value of command written by user
    getline(cin, user_command); // Takes input

    smatch match; // Holds the matched value of regex

    history_array.push_back(user_command); // Keeps command history

    regex listdir("\\s*listdir\\s*"); // Regex of listdir command
    regex listdir_a("\\s*listdir\\s+-a\\s*"); // Regex of listdir -a command
    regex currentpath("\\s*currentpath\\s*"); // Regex of currentpath command
    regex printfile("\\s*printfile\\s+(\\S+)\\s*"); // Regex of printfile command
    regex printfile_redirect("\\s*printfile\\s+(\\S+)\\s+>\\s+(\\S+)\\s*"); // Regex of printfile > command
    regex footprint("\\s*footprint\\s*"); // Regex of footprint command   
    regex listdir_grep("\\s*listdir\\s*\\|\\s*grep\\s+\"(\\S+)\"\\s*"); // Regex of listdir and grep command
    regex listdir_grep_a("\\s*listdir\\s+-a\\s*\\|\\s*grep\\s+\"(\\S+)\"\\s*"); // Regex of listdir -a and grep command
    regex exit("\\s*exit\\s*"); // Regex of exit command

    pid_t pid;
    pid = fork(); // Creates a child process. Child process does the command and parent process waits for its termination.

    if(pid == 0){ // If it is a child process

    if ( regex_match( user_command, listdir))
        execlp("/bin/ls", "ls ", NULL); // Executes ls command

    else if ( regex_match( user_command, listdir_a))
        execlp("/bin/ls", "ls ", "-a", NULL); // Executes ls -a command

    else if ( regex_match( user_command, currentpath))
        execlp("/bin/pwd", "pwd ", NULL); // Executes pwd command

    else if ( regex_match( user_command, printfile)){
        regex_search(user_command, match, printfile);
        execlp("/bin/cat", "cat ", match.str(1).c_str() , NULL); // Executes cat filename command

}
    else if ( regex_match( user_command, printfile_redirect)){
        regex_search(user_command, match, printfile_redirect);
        int second_file = open(match.str(2).c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644); // Creates or open the file
        dup2(second_file, 1); // Replaces stdout with this file for this process
        execlp("/bin/cat", "cat ", match.str(1).c_str() , NULL); // Executes cat filename1 > filename2 command

}
    else if ( regex_match( user_command, footprint)){

    	for(int i = int(history_array.size())-15; i < int(history_array.size()); i++){

		      if ( i>=0)
		    	   cout<<i+1<<" "<<history_array[i]<<endl; // Prints history

}
      return 0;
}
    else if (regex_match( user_command, listdir_grep)){

        regex_search(user_command, match, listdir_grep);

        int pipe_1[2]; // Creates the pipe array
        pipe(pipe_1); // Makes the array a pipe

        pid_t pid_1 = fork(); // Create new processes to execute both processes

        if( pid_1 == 0){ // If it is the new child process

        dup2(pipe_1[1], 1); // Replaces stdout with pipe_1[1]
        execlp("/bin/ls", "ls ", NULL); // Executes ls command

}
        else{ // If it is a parent process

        wait(NULL); // Wait for its child
        close(pipe_1[1]); // Close write end of the pipe for next command not to wait for more input
        dup2(pipe_1[0], 0); // Replaces stdin with pipe_1[0]
        execlp("/bin/grep", "grep ",match.str(1).c_str() , NULL); // Executes grep command
}

}
    else if (regex_match( user_command, listdir_grep_a)){

        regex_search(user_command, match, listdir_grep_a);

        int pipe_1[2]; // Creates the pipe array
        pipe(pipe_1); // Makes the array a pipe

        pid_t pid_1 = fork(); // Create new processes to execute both processes

        if( pid_1 == 0){ // If it is the new child process

          dup2(pipe_1[1], 1); // Replaces stdout with pipe_1[1]
          execlp("/bin/ls", "ls ", "-a", NULL); // Executes ls -a command

}
        else{ // If it is a parent process

          wait(NULL); // Wait for its child
          close(pipe_1[1]); // Close write end of the pipe for next command not to wait for more input
          dup2(pipe_1[0], 0); // Replaces stdin with pipe_1[0]

          execlp("/bin/grep", "grep ",match.str(1).c_str() , NULL); // Executes grep command
}

}
    else if ( regex_match( user_command, exit)) // If the command is exit
    	return 0;

    else{ // If it is not a valid command
	     cout<<user_command<<": command not found"<<endl;
       return 0;
}


}

    else{

      wait(NULL); // Waits for its child
      if ( regex_match( user_command, exit)) // If the command is exit, it terminates
    	   return 0;
}

    }
    return 0;
}
