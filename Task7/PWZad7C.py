from socket import AF_INET, SOCK_DGRAM, socket
from sys import argv


def scoreCalc(score, rcv):
    score = score + 1 if rcv == "Won".casefold() else score
    return score


port = int(argv[1])
yourName = argv[2]

yourNameEnc = str.encode(yourName)

serverDetails = ("127.0.0.1", 5001)
clientDetails = ("127.0.0.1", port)
buffSize = 1024
UDPClientSocket = socket(AF_INET, SOCK_DGRAM)

UDPClientSocket.bind(clientDetails)

UDPClientSocket.sendto(yourNameEnc, serverDetails)
rcv = UDPClientSocket.recvfrom(buffSize)[0]
enemyName = rcv.decode()
yourScore, enemyScore = 0, 0
rcv = ""
while(rcv != "KONIEC".casefold()):
    choice = input("R, P, or S: ").casefold()
    # print(choice)
    UDPClientSocket.sendto(choice.encode(), serverDetails)
    rcv = UDPClientSocket.recvfrom(buffSize)[0].decode().casefold()
    # print(rcv)
    yourScore = yourScore + 1 if rcv == "Won".casefold() else yourScore
    enemyScore = enemyScore + 1 if rcv == "Lost".casefold() else enemyScore
    if rcv in ("Koniec".casefold(), "Wrong move".casefold()):
        print(rcv.capitalize())
    else:
        print(f"{yourName}: {yourScore} - {enemyScore} :{enemyName.capitalize()}")

