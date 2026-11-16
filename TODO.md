## 16/11/26
### About 10 AM
So far, as it is, the program tokenizes the file and then parses it. 

The function to parse is named as 'stack'. I named it as such because, in code, it functions less like a tree and more as a 2d array whereby the second dimension has 'stacks'. Like this:

[=][x][[2][[/][3][7]]]

Hence 'stack'.

The code works, at least in the limited testing its been given. So, assuming parse_expr and parse_assignment work correctly, the next steps should be to expand all of the terminal tokens/functions so that you can write more than simple assignments. 

This shouldn't be massively difficult. Largely, most code is comprised of expressions, statements, assignments, and the rest is a mix or only a little different (I'm only talking in the context of parsing). For example, [IF] [STMT] [CODE] [(optional)ELSE]. The STMT is a comparison (x == y?), which will use the same logic as parse_assignment, just opposite in order. 

That will be the next step. But as of right now, the basic tokenize system and basic parsing system works. Functionally it cannot actually execute the code right now, of course. However, that very difficult part will come soon.

### 11:30 AM
parse_expr WORKS! I mean, I've tested it fairly thoroughly, though I'm sure it isn't completely robust and perfect. But maybe it is! Kinda scared to fully checked because I've gotten a 'programmers high' when code actually works. And strangely I understand it xD. Regardless, now that parse_expr works, I need to move onto the next stage of development: everything else! Expressions and assignments work, so I suppose the next thing would be to get statements working? Not entirely sure yet. Anyways, it's lunch time.