### 16/11/26
So far, as it is, the program tokenizes the file and then parses it. 

The function to parse is named as 'stack'. I named it as such because, in code, it functions less like a tree and more as a 2d array whereby the second dimension has 'stacks'. Like this:

[=][x][[2][[/][3][7]]]

Hence 'stack'.

The code works, at least in the limited testing its been given. So, assuming parse_expr and parse_assignment work correctly, the next steps should be to expand all of the terminal tokens/functions so that you can write more than simple assignments. 

This shouldn't be massively difficult. Largely, most code is comprised of expressions, statements, assignments, and the rest is a mix or only a little different (I'm only talking in the context of parsing). For example, [IF] [STMT] [CODE] [(optional)ELSE]. The STMT is a comparison (x == y?), which will use the same logic as parse_assignment, just opposite in order. 

That will be the next step. But as of right now, the basic tokenize system and basic parsing system works. Functionally it cannot actually execute the code right now, of course. However, that very difficult part will come soon.