cls Something :: OtherSomething
{
    @ _init
    {
        this.obj1 = "hello";
        this.obj2 = 10;
    }
    
    fn change_obj1(change_to)
        {
            this.obj1 = change_to;
        }
}

a = Something;

fn power(x, p)
{
    i = 1;
    until i > p
    {
        x *= x;
          
        i += 1;
    }
    
    player.move();
    
    ret (x);
}

num = 0.39827 * power(10, 7);

list = [1, 2,  2 * (0.5 / 1.4), 5, power(0.1, 2)];

list2[5 * 2, 9];
