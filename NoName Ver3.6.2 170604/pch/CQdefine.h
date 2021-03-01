#pragma once


#define WIN32_LEAN_AND_MEAN

#define KEY_MAX 256
#define CQ_KEY_FRONT 'W'
#define CQ_KEY_BACK 'S'
#define CQ_KEY_LEFT 'A'
#define CQ_KEY_RIGHT 'D'
#define CQ_KEY_INTERACTION 'E'
#define CQ_KEY_ITEM 'G'
#define CQ_KEY_INVENTORY 'I'
#define CQ_KEY_EQUIPMENT 'O'
#define CQ_KEY_SIT VK_CONTROL
#define CQ_KEY_RUN VK_SHIFT
#define CQ_KEY_TOGGLE1 1
#define CQ_KEY_TOGGLE2 2
#define CQ_KEY_TOGGLE3 3
#define CQ_KEY_TOGGLE4 4
#define CQ_KEY_TOGGLE5 5
#define CQ_KEY_TOGGLE6 6
#define CQ_KEY_TOGGLE7 7
#define CQ_KEY_TOGGLE8 8

#define CQ_MOUSE_LEFT 'MLEFT'
#define CQ_MOUSE_RIGHT 'MRIGHT'
#define CQ_MOUSE_WHEEL 'MWHEEL'


#define ReleaseCOM( x ) if( x ){ x->Release(); x = nullptr; }