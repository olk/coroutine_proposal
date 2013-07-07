std::coroutine<std::tuple<int,int>()> c(
    [&](std::coroutine<void(std::tuple<int,int>)> & c){
        c(std::make_tuple(7,11)); // return tuple {7,11}
    });

int x,y;
std::tie(x,y)=c.get();
