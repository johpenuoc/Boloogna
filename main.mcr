fn power(x, p)
{
    i = 1;
    until i > p
    {
        x = x * x;
          
        i = i + 1;
    }
}

power(10, 2);