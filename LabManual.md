# Welcome @lab.User.FirstName!

This lab environment is used for both DEV-24L (Linux Debugging 1) and DEV-25L (Linux Debugging 2).

**Click [here](#session2 "Session 2 instructions") for the session 2 instructions, which are below session 1.**

Instructor: **Matt Hamrick**, Support Escalation Engineer on the Developer Web Apps team

Alias: **mahamr**

===

# DEV-24L: Linux Debugging 1
## *"Your (buggy) App is Running on Linux??" How Do I Troubleshoot This? Let's Dive in!*

---

### *Information gathering*
Basic OS information: 
`uname -n -p -v`

Hardware resources:
- CPU: `lscpu`
- Memory: `cat /proc/meminfo`
- Network: `ip addr`

Linux task managers:
- `top`
- `htop`

Event logs: many are stored in /var/log - `cd /var/log`

You can output the last 10 lines of a file by using "tail":
`tail /var/log/syslog`

Note: most built-in Linux commands or tools available in common Linux packages have documentation available in the manual. To see this, type "man [command]" like this for the "tail" command used above:

`man tail`

Some commands also have documentation by adding a switch to the command. The switch depends on the program but some typical ones are "--help", "-?", "-h" etc.

`tail --help`

---
### *Lab 1 - Crash*

1. Since we are installing packages, it's recommended practice to update package repositories to ensure you're installing the latest versions. Different Linux distributions use different package managers, but for Ubuntu and this lab, this is how to do it:

 `apt update`

 When not running as "root" but need to run an elevated command, prepend "sudo" to it:

 `sudo apt update` (sudo not needed for this lab)

2. Install the gcc compiler and auto-accept (-y) any prompts:

 `apt -y install gcc`

3. To see what the kernel runs when an application crash happens:

 `sysctl kernel.core_pattern`

4. Ensure the appport service is running:

 `service appport start`

> [!ALERT] The next step is an important step to get dumps for most, if not all, Linux distributions.

5. **By default Linux will not create core dumps for crashing applications.** The command you use to check the max limit of core dump size is:

 `ulimit -c`

 An output of '0' means no core dump will be generated. Generally when core dumps are needed we set the limit to "unlimited" - you will need to do this to enable it:

 `ulimit -c unlimited` 


>[!NOTE] For Step 6 if you do not have *segfault.c* in the lab, run the below command to download it:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/segfault.c
>```

6. Compile segfault.c:

 `gcc -g -o SegFault segfault.c`

7. Run SegFault:

 `./SegFault`
 
8. Verify you have the dump file using the *ls* command with a long list format:

 `ls -l`

9. See the crash event in the syslog by piping the full log to the *grep* command which will then filter the output for "SegFault":
 
 ```cat /var/log/syslog | grep SegFault ```

 Under-the-hood this is sending the output of the *cat* command via the **stdout** stream to the **stdin** stream of *grep* which then applies the requested filter and sends the output via **stdout** to the console.

<br>

**Lab 1 complete!**
We will look at this core dump in Session 2 lab 1

---
### *Lab 2 - Memory Leak*

>[!knowledge] To clear the console window run the *clear* command.

>[!note] For this lab if you do not have *memleak.c* already present, run the below command to download it:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/memleak.c
>```

1. Install *valgrind*

 `apt -y install valgrind`

2. Compile memleak.c

 `gcc -g -o MemLeak memleak.c`

3. Run MemLeak:

 `./MemLeak`

4. Use Valgrind to run MemLeak and capture full memory leak check data:

 `valgrind --leak-check=full --log-file=valgrind.log ./MemLeak`

5. Verify **valgrind.log** has been created using *ls -l*: 

 `ls -l`

6. You can view the file contents using *cat*:

 `cat valgrind.log`

**Lab 2 complete!** We will be looking at this *valgrind* output more in-depth in Session 2 lab 2.

---
### *Lab 3 - High CPU*

>[!NOTE] The tool used in this lab is called *perf* and it's already installed on the lab machines. Some Linux distributions already have this installed while others may need it installed.

>[!NOTE] For this lab if you do not have *highcpu.c* already present, run the below command to download it:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/highcpu.c
>```

1. Compile highcpu.c

 `gcc -g -o HighCpu highcpu.c`

2. Run it to see its bahavior:

 `./HighCpu`

3. Run it again but this time:
  - redirect its stdout output stream to a special Linux file so it's discarded and not displayed on the terminal: **> /dev/null**
  - redirect the stderr output stream to stdout: "**2>&1**" 
  - run it as a background job via the tailing '**&**'

  ```./HighCpu > /dev/null 2>&1 &```

 This frees-up the terminal so we can run other commands.
 
4. Run htop to see the process consuming CPU

 `htop`

5. Run HighCpu again followed by the *jobs -l* command:

 ```./HighCpu > /dev/null 2>&1 &```

 `jobs -l`

6. Now we'll run HighCpu through the **perf** application so it can be traced.

 `perf record -g ./HighCpu`

> [!knowledge] You can also use *perf* to attach to an existing process like this:
> ```
> perf record -g -p ###
> ```
> where ### is the process ID of the process to attach to.

7. Verify **perf.data** has been created using *ls -l*:

 `ls -l`

**Lab 3 complete!** We will be looking at this *perf* output more in-depth in Session 2 lab 3.

===

# DEV-25L: Linux Debugging 2 {#session2}
## *Now That I Have Data from a Buggy Linux App, What Do I Do with It? How Do I Find Out What Happened?*

---

>[!NOTE] The data for this session should already be present in the ~/session2 directory. It was generated using the same methods demonstrated in Session 1. Each lab in this session will specify the files needed along with how to retrieve them if missing.

### *Lab 1 - Crash*

>[!NOTE] For this lab you should see the below files present in the "session2" directory:
> 1. core_SegFault
> 2. core_SegFaultNoDbg
> 3. SegFault
> 4. SegFaultNoDbg
>
> If any of those files are missing, here are the commands to retrieve them:
> 
> core_SegFault
>  ```
>  wget https://github.com/mahamr/GEARS2022SourceFiles/raw/main/core_SegFault
>  ```
>
> core_SegFaultNoDbg
>  ```
>  wget https://github.com/mahamr/GEARS2022SourceFiles/raw/main/core_SegFaultNoDbg
>  ```
>
> SegFault
> ```
> wget https://github.com/mahamr/GEARS2022SourceFiles/raw/main/SegFault
> ```
>
> SegFaultNoDbg
> ```
> wget https://github.com/mahamr/GEARS2022SourceFiles/raw/main/SegFaultNoDbg
> ```
> 
> Since SegFault and SegFaultNoDbg are executables, the "execute" permission needs to be added to them using the below commands if you had to download them:
> 
> `chmod +x SegFault`
>
> `chmod +x SegFaultNoDbg`

#### *Lab 1 Part 1 - No debug symbols*

0. As demonstrated in the previous session it's recommended to update the local package cache before installing anything:

 `apt update`

1. Install GNU Debugger (gdb):

 `apt-get install gdb`

2. Open the core_SegFaultNoDbg core dump with gdb:

 `gdb SegFaultNoDbg core_SegFaultNoDbg`

>[!knowledge] One helpful way to learn or find things in gdb is "*apropos [search term]*" -- this scans through the built-in help documentation of gdb and outputs anything that matches.

3. View the current thread's backtrace (aka stack trace)

 `bt`

4. View the current registers

 `info registers`

 or

 `i r`

5. Disassemble the current instruction pointer and associated function

 `disassemble`

 or

 `disas`

6. The default assembly language flavor in this case is "att". Many folks in the Windows world like myself are more familiar the Intel x86 language, so let's change to that

 `set disassembly-flavor intel`

7. To find the value of a specific register, in this case *rax*:
 
 `i r rax`

8. After the disassembly analysis is finished, type `quit` to exit gdb.

9. Let's open the C source file of SegFault to compare the analysis with the original source.

 `cat ../segfault.c`
>[!NOTE] You can download this file via the below command if it's not available on the lab machine:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/segfault.c
>```

#### *Lab 1 Part 2 - With compiled-in debug symbols*

0. Get back to the session2 folder if you're not already there:
 
 `cd ~/session2`

1. Open the core_SegFault core dump with gdb:

 `gdb SegFault core_SegFault`

2. With debug symbols there is more information present in the core dump. It's not shown in the video but if segfault.c was present next to the core dump then gdb will have also output the original line of source code where the issue occurred.

**Lab 1 complete!**

---
### *Lab 2 - Memory leak*

>[!NOTE] In the ~/session2 directory should be "valgrind.log". If it's not present it can be retrieved using this command:
> ```
> wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/valgrind.log
> ```

1. Output the file contents to the screen:
 
 `cat valgrind.log`

2. After the log analysis let's take a look at the original source for the MemLeak application:
 
 `cat ../memleak.c`

>[!NOTE] You can download this file via the below command if it's not available on the lab machine:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/memleak.c
>```

**Lab 2 complete!**

---
### *Lab 3 - High CPU*

>[!NOTE] In the ~/session2 directory should be "perf.data". If it's not present it can be retrieved using this command:
> ```
> wget https://github.com/mahamr/GEARS2022SourceFiles/raw/main/perf.data
> ```

1. Use the *perf* application to open the perf.data file (it will open perf.data by default if no input file is specified):

 `perf report -g`

2. You can quit *perf* by tapping  `q`

3. After the *perf* analysis let's look at the original source code for the HighCpu application:
 
 `cat ../highcpu.c`

>[!NOTE] You can download this file via the below command if it's not available on the lab machine:
>```
>wget https://raw.githubusercontent.com/mahamr/GEARS2022SourceFiles/main/highcpu.c
>```

**Lab 3 complete!**

===
# Thank you for watching!
