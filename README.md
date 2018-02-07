# Raddi.io
<p>The main purpose of this project to learn how can slither.io and agar.io handle many objects which are food, and players withoutlagging.</p>
<img width="958" alt="screen shot 2018-02-06 at 4 10 29 pm" src="https://user-images.githubusercontent.com/16979642/35892073-76837562-0b5c-11e8-88b7-dbbe86d41e25.png">
<img width="1434" alt="screen shot 2018-02-06 at 4 10 58 pm" src="https://user-images.githubusercontent.com/16979642/35892074-769d0248-0b5c-11e8-930d-e002869f59aa.png">
<img width="316" alt="screen shot 2018-02-06 at 4 11 08 pm" src="https://user-images.githubusercontent.com/16979642/35892076-76b5e894-0b5c-11e8-94b7-ac7f5d720e36.png">
<img width="565" alt="screen shot 2018-02-06 at 4 11 37 pm" src="https://user-images.githubusercontent.com/16979642/35892077-76d05fd0-0b5c-11e8-9d77-3bc44ca718d7.png">

## Server 
<p>The server is written in C++ using raw tcp socket and used libevent to handle connections. The server runs on localhost port 
5454. The sendding rate between server and client is 50fps and it is located in side server.h. You can change it to below 
20 if there are many connections. The sennding rate will not affect game play experience, because the client side uses 
client-prediction method.</p> 
## Client
<p>
The client side is also written in C++ using cocos2dx. The client uses client prediction to handle lag.
</p>
## Build 
<p>To build the server and client you need to have libevent installed.</p>
<p>run make</p>
<p>./main</p>
