# fake-sandbox written in C++ for native win32
Creates dummy processes and simulates a sandboxed/virtualized environment for malware. May be useful to prevent malware from being installed.

Features (fake_sandbox)
+ Create dummy processes
+ Abilitiy to terminate all running simulated processes
+ Almost no CPU usage
+ Collects all ProcessIDs and Process handles in a structure for the termination
+ Supports x86 / x64 architectures from Windows XP SP 3 to Windows 10

Features (dummy)
+ Runs an infinite loop with a long sleep, no other operations
+ Very low executable size (1.536 Bytes)
+ Supports x86 / x64 architectures from Windows XP SP 1 to Windows 10

Bugs:
+ Only able to close the running fake processes when the main application (fake_sandbox) is not terminated. 

TODOs:
+ Avoiding the usage of stored processIDs and handles
+ Termination of fake processes by using simple signature scanning method

Changelog: 
# v1.0 
  [Build] Inital release
