#pragma once

/* server */
#define IPLength            20
#define UserNameLength      20
#define PasswordLength      20
#define BuffLength          1000
#define tBufferLength       100
#define NOPACK              -1
#define TimeLimit           10

/* game */
#define ChessSize           10
#define PlaneSize           10
#define MaxUserNum          100
#define MaxRoomNum          10000
#define PlaneNum            3

/* common */
#define ERROR               0
#define OK                  1

/* log in */
#define REPEAT              2

/* user buffer */
#define NonePack            0
#define WaitingPack         1
#define InvitePack          2
#define InviteRequestPack   3
#define ReadyPack           4
#define WaitPack            5
#define StartPack           6
#define ClickPack           7
#define CheckPack           8
#define KickPack            9

/* user state */
#define NoConnect           -1
#define NoRoom              -1
#define Unready             -1
#define Ready               -2

/* room state */
#define Available           0
#define Empty               -1
#define NoExist             2
#define Full                3

#define Joined              1
#define Waiting             2
#define GetMap              3
#define Start               4

/* click */
#define NoPlane             1
#define HitPlane            2
#define CritialHit          3

/* check */
#define Wrong               1
#define Right               2
#define GameEnd             3

/* exchange */
// status
#define LOGIN_STATUS        0
#define SEARCH_STATUS       1
#define ROOM_STATUS         2
#define BATTLE_STATUS       3
#define SPECIAL_STATUS      9
// from client
#define RCV_LOG_IN          0

#define RCV_USER_LIST       1
#define RCV_BATTLE_REQ      2
#define RCV_HANDLE_IVT      3

#define RCV_READY           4
#define RCV_LEAVE           5
#define RCV_CHESS           6

#define RCV_CLICK           7
#define RCV_CHECK           8
#define RCV_WAIT            9

#define RCV_LOG_OUT         10
// to client
#define SND_LOG_IN          0

#define SND_USER_LIST       1
#define SND_IVT             2
#define SND_ROOM_INFO       3

#define SND_READY           4
#define SND_STATE           5
#define SND_START           6

#define SND_CLICK_RES       7
#define SND_CHECK_RES       8
#define SND_WAIT            9
#define SND_CLICK_OP        10
#define SND_CHECK_OP        11
#define SND_DROP            12

#define SND_KICK            255
