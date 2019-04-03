#include <Windows.h>
#include <atlstr.h>
#include <stdio.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <thread>

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
void WriteToPipe(std::string string, HANDLE handle)
{
	DWORD dwRead = 0, dwWritten = 0;

	BOOL bSuccess = FALSE;
	bSuccess = WriteFile(handle, string.c_str(), dwRead, &dwWritten, NULL);

	if (!bSuccess)
		std::cerr << "StdInWr Cannot write into child process." << std::endl;
	else
		std::cout << string << std::endl;
}

std::string HandleBuffer(std::string buffer)
{
//	std::cout << "{" << buffer << "}" << std::endl;

	if (buffer == "OK.\n")
	{
		std::cout << "Yes!" << std::endl;
	}
}

HANDLE stdoutRead, stdoutWrite, stderrRead, stderrWrite, stdinWrite;

#define CLOSE_HANDLES CloseHandle(stdoutRead); CloseHandle(stdoutWrite); CloseHandle(stderrRead); CloseHandle(stderrWrite); CloseHandle(stdinWrite);

bool execThreadFinished = false;

std::string ExecCmd(const std::string programName, const std::string commandLine, std::string &outputStr)
{
	std::cout << "entering execcmd" << std::endl;

	std::string strResult; // This might break

	SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	saAttr.bInheritHandle = TRUE;   // Pipe handles are inherited by child process.
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe to get results from child's stdout.
	if (!CreatePipe(&stdoutRead, &stdoutWrite, &saAttr, 0))
	{
		execThreadFinished = true;
		return "FUCKING FAILURE";
	}

	// create a pipe to get results from child's stderr
	if (!CreatePipe(&stderrRead, &stderrWrite, &saAttr, 0))
	{
		// previous creation of pipes might have succeeded
		CLOSE_HANDLES;

		execThreadFinished = true;
		return "FUCKING FAILURE";
	}

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.dwFlags = /*STARTF_USESHOWWINDOW | */STARTF_USESTDHANDLES;
	si.hStdOutput = stdoutWrite;
	si.hStdError = stderrWrite;
	si.hStdInput = stdinWrite;

//	si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.

	PROCESS_INFORMATION pi = { 0 };

	BOOL fSuccess = CreateProcess(programName.c_str(), (LPSTR)commandLine.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (!fSuccess)
	{
		std::cerr << "failed to create process (" << GetLastError() << ")" << std::endl;
		std::cout << "exiting execcmd" << std::endl;

		CLOSE_HANDLES;

		execThreadFinished = true;
		return strResult;
	}

	bool bStatus;
	CHAR aBuf[4096 + 1];
	DWORD dwRead;
	DWORD dwWrite;

	bool bProcessEnded = false;
	while (!bProcessEnded)
	{
		if (GetLastError() == ERROR_BROKEN_PIPE)
		{
			std::cerr << "Broken Pipe!" << std::endl;
		}

		// Give some timeslice (50ms), so we won't waste 100% cpu.
		// TODO: this might be bad
		bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

		// Even if process exited - we continue reading, if there is some data available over pipe.
		for (;;)
		{
			char buf[1024];
			DWORD dwRead = 0;
			DWORD dwAvail = 0;

			if (!::PeekNamedPipe(stdoutRead, NULL, 0, NULL, &dwAvail, NULL))
				break;

			if (!dwAvail) // no data available, return
				break;
			
			// either the amount of characters in the buffer or the size of the buffer
			int toRead = min(sizeof(buf) - 1, dwAvail);
			if (!ReadFile(stdoutRead, buf, toRead, &dwRead, NULL))
			{
				// error, the child process might ended
				std::cerr << "!::ReadFile (stdoutRead)" << std::endl;
				break;
			}

			buf[dwRead] = 0;
			strResult += buf;

//			HandleBuffer("OK\r\n");

			if (buf == "OK\r\n")
			{
				std::cout << "NO!" << std::endl;
			}

			outputStr = buf;

			std::cout << "[" << buf << "]";

			/*
			// min(sizeof(buf) - 1, dwAvail)
			if (!ReadFile(stderrRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL))
			{
				// error, the child process might ended
				std::cerr << "!::ReadFile (stderrRead)" << std::endl;
				break;
			}

			buf[dwRead] = 0;
			strResult += buf;

			outputStr = buf;

			std::cout << "[" << buf << "]";
			*/
		}
	} // for

	CLOSE_HANDLES;

	std::cout << "exiting execcmd" << std::endl;

	execThreadFinished = true;
	return strResult;
} // ExecCmd

int main()
{
	std::cout << "running srcds-mgr" << std::endl;

	std::string programName = "./resources/steamcmd/steamcmd.exe";
	std::string commandLine = "";
	std::string outputStr;
	
	std::thread* execThread = new std::thread(ExecCmd, programName, commandLine, std::ref(outputStr));

	while (!execThreadFinished)
	{
		if (execThreadFinished)
		{
			std::cout << "exec thread finished, joining" << std::endl;

			execThread->join();

			break;
		}
	}

	std::cout << "we're back in main" << std::endl;

	std::cin.get();
}
