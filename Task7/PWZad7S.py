from socket import socket, AF_INET, SOCK_DGRAM
from collections import deque


IP_Addr = "127.0.0.1"
port = 5001
buffSize = 1024

UDPServerSocket = socket(AF_INET, SOCK_DGRAM)

UDPServerSocket.bind((IP_Addr, port))

players = {}

R = 'R'.casefold()
P = 'P'.casefold()
S = 'S'.casefold()
possibleMoves = [R, P, S]
playerNames = list()
start = False

def whoWon(playerA, playerB):
    if playerA == R:
        if playerB == R:
            return "Draw", "Draw"
        if playerB == P:
            return "Lost", "Won"
        if playerB == S:
            return "Won", "Lost"
    if playerA == P:
        if playerB == P:
            return "Draw", "Draw"
        if playerB == S:
            return "Lost", "Won"
        if playerB == R:
            return "Won", "Lost"
    if playerA == S:
        if playerB == S:
            return "Draw", "Draw"
        if playerB == R:
            return "Lost", "Won"
        if playerB == P:
            return "Won", "Lost"

while(True):
    # Wait for 2 players to join
    while len(playerNames) < 2 and len(players.values()) < 2:
        comm, addr = UDPServerSocket.recvfrom(buffSize)
        comm = comm.decode().casefold()
        if addr not in players.keys() and len(players.keys()) <= 2:
            players[addr] = deque()
            playerNames.append((addr, comm))

    # Send names to each player
    if not start:
        UDPServerSocket.sendto(playerNames[0][1].encode(), playerNames[1][0])
        UDPServerSocket.sendto(playerNames[1][1].encode(), playerNames[0][0])
        start = True
    
    # Actual game loop
    else:
        comm, addr = UDPServerSocket.recvfrom(buffSize)
        comm = comm.decode().casefold()
        if addr not in players.keys() and len(players.keys()) ==  2:
                UDPServerSocket.sendto("player limit reached".encode(), addr)
        elif comm in possibleMoves:
            if addr not in players.keys() and len(players.keys()) <= 2:
                players[addr] = deque(comm)
            elif addr in players.keys():
                players[addr].append(comm)
            
        else:
            if len(players) == 2 and comm == "KONIEC".casefold():
                playKeys = list(players.keys())
                UDPServerSocket.sendto("KONIEC".encode(), playKeys[0])
                UDPServerSocket.sendto("KONIEC".encode(), playKeys[1])
                players = {}
                playerNames = list()
                start = False
                print(f"Cleared data: {players}, {playerNames}, {start}")
            else:
                UDPServerSocket.sendto("Wrong move".encode(), addr)
        if len(players) == 2:
            playDeques = list(players.values())
            playKeys = list(players.keys())
            if playDeques[0] and playDeques[1]:
                result = whoWon(playDeques[0].popleft(), playDeques[1].popleft())
                print(result)
                UDPServerSocket.sendto(result[0].encode(), playKeys[0])
                UDPServerSocket.sendto(result[1].encode(), playKeys[1])