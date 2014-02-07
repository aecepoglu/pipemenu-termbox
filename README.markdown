README
------------------

**pipe-menu** is a concept not unfamiliar to openbox users. You can read menus from files or from the output of other programs.  

It basically works like this: 

    menu0.txt
    ---------------
    item 1
    item 2
    item 3
    item 4
    item 5

And then you can view that menu like so:

    pipemenu-termbox < menu0.txt

This doesn't do anything though. You'll probably want to do something with your menu. You want to launch certain commands when you select items:

    menu1.txt
    ----------------
    item 1
    item 2,0,firefox bitbucket.org/aecepoglu/pipemenu-termbox
    item 3,0,rm -rf ~/my-very-important-folder
    item 4,0,aplay ~/moo.wav
    item 5,0,sudo shutdown -hP now

And you'll launch the program to show this menu like so:

    pipemenu-termbox < menu1.txt

You can use the the arrow keys to navitage the menu:
* **up**,**down**: selects the item above or below
* **left**: goes to previous menu
* **right**: launches item command, and shows sub-menu if selected item is a sub-menu.

When you select *item 2*, then the program will run *"firefox bitbucket.org/aecepoglu/pipemenu-termbox"*.  
You can see that *menu1.txt* has **CSV** format. The menu-format is:

	<item title>,<isMenu>,<command>,<wantsRefresh>

You can specify any number of fields. So either one of the following combinations is fine:

    <item title>
    <item title>,<isMenu>
    <item title>,<isMenu>,<command>
    <item title>,<isMenu>,<command>,<wantsRefresh>

* item title: This is what the user will see
* isMenu: A boolean(0 or 1) that specifies whether this item is a sub-menu
* command: Shell commands to execute. Can be empty.
* wantsRefresh: A boolean (0 or 1) that specifies whether the current menu should be read and drawn again.

So, you can have submenus like so:

    menu2.txt
    ------------------
    item 1
    the button for the internet,0,iexplorer.exe http://msn.com
    tasks,1,cat ~/todo.txt
    
    todo.txt
    ------------------
    Make a to-do list
    Buy a parrot. Teach the parrot to say "Help! I've been turned into a parrot.!"
    Hire two detectives. Have them follow each other.
    Change name to Simon. Speak in 3rd person.

And when you launch the pipe-menu and have it display _menu2.txt_,then select and enter the 3rd item _tasks_, the menu will start showing contents of _todo.txt_.  
You could build an actual to-do application like this:

    menu3.txt
    --------------
    item 1
    the button for the internet,0,iexplorer.exe http://msn.com
    tasks,1,"cat todo.txt | nl -s ' ' | sed -r 's/[ ]*([0-9]*) (.*)/\2,0,sed -i "\1d" todo.txt,1/'"

    todo.txt
    ------------------
    Make a to-do list
    Buy a parrot. Teach the parrot to say "Help! I've been turned into a parrot.!"
    Hire two detectives. Have them follow each other.
    Change name to Simon. Speak in 3rd person.