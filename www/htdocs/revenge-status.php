<?php
include 'header.php'
?>

<table width="90%" class="centered">
<H2>Revenge Status
</H2>
<P> The first release has already been made. Find on the revenge project page.
</P>

<P> The revenge software application developement has already started. We have built the BFD (binary file decoder, program loader) part and also the x86 to RTL instruction decoder. This will take x86 instruction hex code and convert it directly into RTL instructions. This will be our equivalent of disassembly. We can currently disassemble about 50% of the x86 instructions. The latest svn currently passes the first 13 test sample programs. It produces C source code from the binary .o file. 
</P>
<P> The current source code can be found in the GIT repository under the "git://git.berlios.de/revenge". <A HREF="https://developer.berlios.de/git/?group_id=1165">See GIT repository for latest test version.</A>
</P>
<P> The memory module has been written. The BFD will load the program into memory in much the same way that the linux kernel loads a program into memory so that the CPU can execute it. Any extra details that might happen to be in the binary headers (e.g. debug info), will be loaded into a separate table, so that other modules can use them as they see fit. The BFD and memory module will not execute any program code.
</P>
<P> The CPU Emulator has been partially implemented. This takes the program bytes from the memory module, covert them to RTL, determine which bytes are data and which are instructions, build program flow graphs, determine function parameters and gather meta information.
</P>
<P> Extra processing then takes place, resulting in the final step of creating a C source code representation of the binary program.
</P>
<P> Currently, the test programs test1.c, test2.c, ..., and test14.c work. We can compile them, run revenge on the test4.o file and output a test.c file.
</P>
<P> We can now handle jump or conditional jump instructions.
</P>
<P> 64bit amd64 .o files are now supported in addition to 32bit ia32.
</P>
<P> Currently, SSA form is being introduced. The difficult part of handling the renaming of labels has been started.
</P>
<P> This page was last updated on 20th MarchOctober 2012
</P>
</table>
</table>
<?php
include 'copyright.php' 
?>
</BODY>
</html>
