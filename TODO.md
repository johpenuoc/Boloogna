## 16/11/26
### About 10
So far, as it is, the program tokenizes the file and then parses it. 

The function to parse is named as 'stack'. I named it as such because, in code, it functions less like a tree and more as a 2d array whereby the second dimension has 'stacks'. Like this:

[=][x][[2][[/][3][7]]]

Hence 'stack'.

The code works, at least in the limited testing its been given. So, assuming parse_expr and parse_assignment work correctly, the next steps should be to expand all of the terminal tokens/functions so that you can write more than simple assignments. 

This shouldn't be massively difficult. Largely, most code is comprised of expressions, statements, assignments, and the rest is a mix or only a little different (I'm only talking in the context of parsing). For example, [IF] [STMT] [CODE] [(optional)ELSE]. The STMT is a comparison (x == y?), which will use the same logic as parse_assignment, just opposite in order. 

That will be the next step. But as of right now, the basic tokenize system and basic parsing system works. Functionally it cannot actually execute the code right now, of course. However, that very difficult part will come soon.

### 11:30
parse_expr WORKS! I mean, I've tested it fairly thoroughly, though I'm sure it isn't completely robust and perfect. But maybe it is! Kinda scared to fully checked because I've gotten a 'programmers high' when code actually works. And strangely I understand it xD. Regardless, now that parse_expr works, I need to move onto the next stage of development: everything else! Expressions and assignments work, so I suppose the next thing would be to get statements working? Not entirely sure yet. Anyways, it's lunch time.

### 15:30
Pretty cool! Now, it understands what I call 'linkers'. That is player.x, conn.port, something.something_else.another_something_else. Pretty cool I think! The next step is to introduce functions. It should be able to recognise a function, its arguments, when it's in an expression and when its simply being called. It should also be able to handle expressions inside of the function. That is, arguments. function(2 + 4, player.x, conn.port, ...); I think it'll be more or less the same as the parse_expr and parse_IDlinker. Honestly, I believe most of it will be, considering that the algorithm is very generalised. 

Time to get coding! I'm plugged in.

### 16:34
Currently, the parse_func is proving to be slightly more difficult. I don't believe that's because of complexity, but rather my being not prudent; I hadn't thought it through very deeply. 

I will continue coding.

### 17:14
I believe it to be working right now. It's dinner time, and so I'm not going to be thoroughly testing it today. I'm sure there are logic bugs, but at the moment it works pretty fine.

## 17/11/26
### 8:41
Not going to develop much today. I have some classes and some work to finish. Perhaps later, but not too sure. I also have a test tomorrow, so we will see; I have to revise. 

If I do code today, I will be developing the current code: making it more robust. I know that it 'works' with simple lines of code, for example:

--> function(arg1, arg2, arg3.obj1, 1 + 4 / (8 * 0.5));

but thats it. Im not sure whether

--> function(arg1, function2(arg1, 1 + 4 * (0.5 - 1)), player.x);

would work. Hence making it more robust. We will see. I do believe that, if it doesn't work, a more thorough reworking of the program will have to commence. I did kind-of rush it, and I hadn't thought it through. I kinda just thought "I will simply copy-and-paste the code for parse_expr, change it slightly, and hope for the best" xD To be honest, most programming is 'hoping for the best'!