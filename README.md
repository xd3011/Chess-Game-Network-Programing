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
**Using Makefile**
```
> make
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

## Play Game
**Client 1**
1. Login
```
> 1
> dat
> 1
```
2. Create Room
```
> 1
```
3. Get User Online
```
> 1
```
4. Invite Player
```
> 2
> hieu
```
5. On Game

**Client 2**
1. Login
```
> 1
> hieu
> 1
```
2. Waiting
```
> 2
```
3. Accept Invite/Refuses

   3.1 Accept
   ```
   > 1
   ```
   3.2 Refuses
   ```
   > 2
   ```
5. On Game

## How to move
**Move**
```
> current position-next position (Ex: a2-a3)
```
**Give up** (Xin thua)
```
> giveup
```
Yes/No Opponent Giveup (chấp nhận hoặc từ chối cho đối phương đầu hàng)
```
> yes
> no
```
