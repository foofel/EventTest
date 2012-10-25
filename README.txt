C++ deferred event system where you can queue your events, like a message pump.

Its quite fast (of course not as fast as direct function pointers, but has way less boilder plate code) 
and easy to setup: create a class and derive from Event<Yourclass>, thats your event. To Create a Listener 
create a class and add a function taking a const reference of your event, Done. Now you can connect them (see example) 
and send events as you like.

Have fun :)