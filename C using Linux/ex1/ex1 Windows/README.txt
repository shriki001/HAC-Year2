OS1, year B

* Written by  	: Kfir Matityahu & Michael Shriki
* login(s) 		: kfirma / michaelshr

threads for Windows

File    : ex1a.c

Purpose : The program defines a (non-dynamic) array by size of 100,000
		  cells and magrila into it values in range [0,index].
		  The program creates 2 process that will sort the array (in 2
		  separate ways). each child(-process) prints the time it took
		  to him to finish, and more information about him.

-------------------------------------------------------------------------------

Compile : gcc -Wall ex1a1.c -o ex1a1.exe
Run     : ex1a1.exe

Input : Nothing.

Output: from The 2 first childern:
        time it took to do it sort, which sort it performed, the minimum
        and the maximum values in the array.
        annoncment the the 3rd child(son of the father process) performed
        a sequential search and time it took, as well the same data for the
        4th child (son of the 1st child, peformed a binary search).

RunTime: Bubble sort = O(n^2)
		 Quick sort = at worst O(n^2)
		 Binary search = O(m*log(n))
		 Sequential search = O(m*n)
		 In total =  O(n^2)
		 * when n present the size of entered array

===============================================================================

File    : ex1a2.c

Purpose : This program do a bubble sort

-------------------------------------------------------------------------------

Compile : gcc -Wall ex1a2.c -o ex1a2.exe

Input : Nothing.

Output: Nothing.

RunTime: O(n^2) as n represent the size of the array

===============================================================================
File    : ex1a3.c

Purpose : This program do a Quicksort
-------------------------------------------------------------------------------

Compile : gcc -Wall ex1a3.c -o ex1a3.exe

Input : Nothing.

Output: Nothing.

RunTime: O(n^2) at the worst case, as n represent the size of the array.

===============================================================================
File    : ex1a4.c

Purpose : This program do a sequential search on array

-------------------------------------------------------------------------------

Compile : gcc -Wall ex1a1.c -o ex1a1.exe

Input : Nothing.

Output: Nothing.

RunTime: O(m*log(n)) 
         as n represent the size of the array 
		 and m the times we raffled a number
===============================================================================
File    : ex1a4.c

Purpose : This program raffles a number 100 times and do a sequential search
          on array
-------------------------------------------------------------------------------

Compile : gcc -Wall ex1a4.c -o ex1a4.exe

Input : Nothing.

Output: Nothing.

RunTime: O(m*n) 
         as n represent the size of the array 
		 and m the times we raffled a number as n represent the size of the array
