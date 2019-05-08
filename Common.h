#pragma once

#define ERROR               0
#define OK                  1
#define REPEAT              2

#define IPLength            20
#define UserNameLength      20
#define PasswordLength      20
#define BuffLength          1000

#define ChessSize           10
#define PlaneSize           10
#define MaxUserNum          100
#define MaxRoomNum          1024
#define OnePageRoomNum      10
#define PlaneNum            3

#define ThrowPack           19434

#define LoginState          0
#define SearchingState      1
#define RoomState           2
#define StartState          3
#define OperateState        4
#define WaitingState        5
#define KickState           6

#define NoConnect           -1
#define NoRoom              -1
#define Unready             -1
#define Ready               -2
#define Empty               -1

#define CreateRoomError     -1

#define NoExist             2
#define Full                3

#define Available           0
#define Joined              1
#define Waiting             2
#define GetMap              3
#define Start               4

#define NoPlane             1
#define HitPlane            2
#define CritialHit          3

#define Wrong               1
#define Right               2
#define GameEnd             3
