# Temporal Iterators

Temporal iterators allow the standard library to be used to iterate over *time*.  So, if  you have some thing that you expect to happen within some specific time-frame, temporal iterators allow you to write code that looks like this:
```

using kjc::chrono::iterator;
using namespace std::chrono_literals;_

my_async_class.some_async_operation();
if (std::find_if(iterator{100ms, 5ms}, iterator{}, [&my_async_class](auto&&){return my_async_class.true_when_done(); }_) == kjc::chrono::iterator{}) {
    std::cerr << "The thing didn't happen in the required time-frame :(" << std::endl;    
}

// We're all good, the thing happened in time
```

Alternatively, if you want to trigger some specific event at some interval, then:
```
using kjc::chrono::iterator;
using namespace std::chrono_literals;

std::for_each(iterator{10s, 50ms}, iterator{}, [](auto&& time_point_){
    std::cout << time_point.time_since_epoch() << ": Doing the thing at this time" << std::endl;
});
```

Anyway, you get the point. It's a sort of alternative syntax to the usual timer-type class that also does this kind of thing.  This could be nice, because it very clearly separates the time-based part from the event-based part. So, it could be easier to think of and write test code for your algorithms with a temporal component.