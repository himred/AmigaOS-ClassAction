Dear programer,

I'll try as short as possible to explain you how ClassAction works
and the main point to know if you want to continue my work.




ABOUT COMPILATION
-----------------


First, The sources are written for Dice C from Matt Dillon.
Don't expect to compile them with another compiler.

I have used UNIX like ANSI functions such open,seek,fopen
instead of AmigaOS functions Open,Seek,......
Since Dice was accepting almost full ANSI C and I was used to code under UNIX
I have decided to do so.

Just note that Dice is now free, and can be found on any Aminet site.

They are currently 2 versions of CA (ClassAction)

2.83: which is under GadTools GUI and is no more supported since 2 years now.
3.6 : which uses MUI GUI and is the latest release.

To compile it you'll need the includes of:

        - AmigaOS 3.0+  ( provided with your compiler )
        - MUI 3.0+      ( on the MUI SDK on Aminet )
        - ReqTools      ( on the Reqtools SDK on Aminet )

Also you'll need extra .lib for dice, I have provided them in the Directory Extra_Dice_Lib/
you can regenerate them using the fdtolib executables given in the dice Package with the .fd
given in the SDK of MUI or Reqtools. (for example when MUI 3.9 will be released)

for the moment the extra libs are :

console.lib     :       lib for console.device of OS 3.0

dt.lib          :       Datatypes.library lib of OS 3.0

gt.lib          :       Gadtools.library lib of OS 3.0

muil.lib        :       MUI 3.8 lib
muilr.lib       :         "
muis.lib        :         "
muisr.lib       :         "

reqtoolsl.lib   :       Reqtools 2.2c lib
reqtoolsrl.lib  :         "
reqtoolss.lib   :         "
reqtoolssr.lib  :         "



SOURCES
--------

You have now the sources for 4 executables

ClassAction 3.6  and ClassActionPrefs 3.6
ClassAction 2.83 and ClassActionPrefs 2.83
Each of them is in its own directory.

let's look on the source dir of ClassAction 3.6:
You'll find the followng files:

Arexx.c                 :       Functions for Arexx Handling
ca.cd                   :       original english .cd file
CAStart.c               :       source of CAStart an executable as itself
ca_fr.ct                :       French .ct file
ClassAction.info        :       default Icon
Classaction_Rev.h       :       revision header file
Disk.c                  :       functions of load/save/toolstypes operations
Dmakefile               :       makefile for 68000 executables
Dmakefile.20            :       makefile for 68020 executables
exec.c                  :       main recognition routines of CA
Extern.h                :       Definition of globals vars as extern
Global.c                :       Global vars
Includes.h              :       Includes needed by all .c files
key.c                   :       source of the executable key to generate a keyfile
List.c                  :       lists routines
locale.c                :       locale strings
locale.h                :       locale hearder
mui.c                   :       biggest source, all MUI handling
mui.h                   :       a custom made mui.h
Newtypes.h              :       my own new structs
Protos.h                :       prototypes
Req.c                   :       Requesters functions
Wbrun.c                 :       wbrun exe
Xfd.c                   :       Xfdmaster functions
Xfd.h                   :       Xfdmaster header   (no .h given in the orig SDK)
Xfdmaster.a             :       Xfdmaster ASM .lib (no .fd given in orig sdk)

Some of those files are really well commented while some others have no comments at all
Some days I was too lasy to write comments .......


THE BIG POINT
-------------


Well, to save you some hours of braincrushing I'll try to explain how I store Classes and Actions

Each class is a node of a single linked list.
Each action is a node of a single linked list

here is a small schema:


 MemList.head                                                       MemList.tail
    |                                                                   |
    |                                                                   |
    v                                                                   v
 ---------      ---------      -------     --------                  --------
 |       |      |       |      |     |     |      |                  |      |
 |Unknown|----> |Generic|----> | Dir |---->| User |----> ...... ---->| User |
 | Class |      | Class |      |Class|     |Class |                  |Class |
 |       |      |       |      |     |     |  #1  |                  |  #n  |
 ---------      ---------      -------     --------                  --------
     |              |             |           |                         |
     |              |             |           |                         |
     v              v             v           v                         v

   Action         Action        Action      Action                    Action
     #1             #1            #1          #1                        #1

     |              |             |           |                         |
     |              |             |           |                         |
     v              v             v           v                         v

   Action         Action        Action      Action                    Action
     #2             #2            #2          #2                        #2

     |              |             |           |                         |
     |              |             |           |                         |
     v              v             v           v                         v

     .              .             .           .                         .
     .              .             .           .                         .
     .              .             .           .                         .

     |              |             |           |                         |
     |              |             |           |                         |
     |              |             |           |                         |
     v              v             v           v                         v

   Action         Action        Action      Action                    Action
     #n1            #n2          #n3         #n4                        #n4


If you look on the file newtypes.h you'll see definitions of the structures
Class , Action , and ClassList

MemList is a global variable of type ClassList which have 3 fields
head: point to the Unknow Class
tail: point to the last User Class
size: the size of Class List

Then, Accessing to the name of the 3rd action of the 2nd User Class
gives : MemList->next->next->next->next->action->next->next->name

or the stack of last action of the Dir Class gives:

MemList.head->next->next->action->tail->stack


To "simplify" this I have 2 others global vars named
Generic and Directory wich points directly to
the Generic Class and the Dir Class
ie Generic   = MemList.head->next
   Directory = MemList.head->next->next (or Generic->next)

Nota: In ClassAction 2.83 the notion of "Dir Class" was not yet in my mind
thus this Class does not exist. We go directly from Generic Class to user Class #1

Well, Once you have understood this (and it is finally quite easy)
You should be able to understand most of logic of the sources.


I hope this little help file will be useful and good coding !

Salim Gasmi ( salim@gasmi.net )
