std::coroutine<int()> c(
    [&](std::coroutine<void(int)> & c){
        c(5); // return {5} via coroutine c
        c(7); // return {7} via coroutine c
        return 11; // {11} not returned via coroutine c
    });

for (;c;c())
    int ret=c.get();