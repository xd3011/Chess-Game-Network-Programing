# C Chess
> Online chess game written in C, using sockets, threads, dynamic memory and more

## Group Members
Dao Xuan Dat - 20205065 <br>
Pham Minh Duc - 20205068 <br>
Tran Trung Hieu - 20204971 <br>

## Build
**Server**
```
> gcc cchess-server.c board.c -o server -pthread
```
**Client**
```
> gcc cchess-client.c -o client -pthread
```

## Run
**Server**
```
> ./server
```
**Client**
```
> ./client localhost 8080
```

## How to move
```
> current position-next position (Ex: a2-a3)
