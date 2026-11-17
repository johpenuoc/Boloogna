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

### 8:59
Nevermind. Nothing can stop the coding. Joking. I got everything done pretty quick. There's always far less than i anticipate.

### 9:07
Didn't expect to be back here again. However, pretty quickly, I managed to find the logic bug hiding in my code. Now, when a function is called, it can contain however many functions as arguemtns required, as the recursive system functions properly. 

Next is to see whether it works with linking...

### 9:20
I believe it is now working! So now it recognises:

player.box.movex(dt, ...); 

I do believe linking will also work with

func().something.something

however, I'm not sure I'm going to be implimenting that within my language. [FUNCTION] will not be a class or object which can have attributes. So [DECIMAL] won't do anything. It'll cause an error.

### 9:30
Next shall be... SCOPES.

The main idea being:

{} <-- this is a scope

It will be called in certain statements, but also all around the code -- not jsut in statements. 

[IF][STMT][SCOPE][(optional)ELSE]

SCOPE will just be collecting a bunch of expressions and whatnot. Effectively, you can write anything in a scope. However, whatever was initialized in the scope will be deleted once out of the scope. Unless its writing to something initialized outside of the scope.

### 9:40
I'll carry it on later. REASONS.

### 10:35
I am back (was back about 15 minutes ago...). I've eaten and I've continued development of the 'parse_scope' function. So, it's essentially a rework of the 'stack' function. Essentially, a scope can kind-of be thought of as a mini-program. Seperate (not really but just how I'm thinking of it) from the rest of the program.

### 11:30
I GOT IT WORKING. I haven't really tested it and I'm rushing (I have a class in less than 30 minutes), but anyways, it appears to work. I'm not sure whether scopes in scopes will work, but I'll see to it later. For now, this is fine. Good work!

### 15:04
Just got back from a 2 hour class.

I've added STACK_TYPEs. They mark any NODES with a type like EXPRESSION or FUNCTION or ARGUMENTS. It'll make the interpreter easier to code and it won't have to figure out what anything is doing. It is explicitly stated.

### 15:18
The scan.c preprocessor is flawed. It allows for multiple spaces outside of a string. Got to fix that.

### 15:32
I got to get to class quickly. However, I did indeed fix the problem. 

Next will be IF statements.

### 18:46
Class was incredibly boring.

Anyways, not so much about programming per-say, however, I have come up with a better idea than 'Boloogna'... MICRO. Meaning Massively Important Coding Repository Origin. It's supposed to be a joke if you couldn't tell. It's neither massive, massively important, a massively important repository, or a massively important repository origin... FOR ANYTHING. It is, however, a 'coding' language. 

I should probably get to some actualy coding now, lul.

### 20:24
Apparently I've found another problem with scan.c. Basically, IT DOESN'T RECOGNISE EQUIVALENCE '=='. Genuinly no idea why. I hate running back over old code. I write it poorly, and thus I can never read it again. I won't be able to sleep knowing this is there.