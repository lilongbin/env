#! /usr/bin/env python
# coding: utf-8
# ִ�н�������ն���ʾ��������ִ��reset����
 
import curses
from curses import KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN
from random import randint
 
# ���˶��ĳ��س���
HEIGHT = 10
WIDTH = 20
FIELD_SIZE = HEIGHT * WIDTH
 
# ��ͷ����λ��snake����ĵ�һ��Ԫ��
HEAD = 0
 
# ��������ͬ���������֣����ھ�����ÿ�����ӻᴦ��ɵ���ʳ���·�����ȣ�
# �����������������Ҫ���㹻��ļ��(>HEIGHT*WIDTH)
FOOD = 0
UNDEFINED = (HEIGHT + 1) * (WIDTH + 1)
SNAKE = 2 * UNDEFINED
 
# ����snake��һά���飬���Զ�ӦԪ��ֱ�Ӽ�������ֵ�ͱ�ʾ���ĸ������ƶ�
LEFT = -1
RIGHT = 1
UP = -WIDTH
DOWN = WIDTH
 
# ������
ERR = -1111
 
# ��һά��������ʾ��ά�Ķ���
# board��ʾ���˶��ľ��γ���
# ��ʼ����ͷ��(1,1)�ĵط�����0�У�HEIGHT�У���0�У�WIDTH��ΪΧǽ��������
# ��ʼ�߳���Ϊ1
board = [0] * FIELD_SIZE
snake = [0] * (FIELD_SIZE+1)
snake[HEAD] = 1*WIDTH+1
snake_size = 1
# �����������Ӧ����ʱ����������̽�Ե��ƶ�ʱʹ��
tmpboard = [0] * FIELD_SIZE
tmpsnake = [0] * (FIELD_SIZE+1)
tmpsnake[HEAD] = 1*WIDTH+1
tmpsnake_size = 1
 
# food:ʳ��λ��(0~FIELD_SIZE-1),��ʼ��(3, 3)
# best_move: �˶�����
food = 3 * WIDTH + 3
best_move = ERR
 
# �˶���������
mov = [LEFT, RIGHT, UP, DOWN]
# ���յ��ļ� �� ����
key = KEY_RIGHT                                                    
score = 1 #����Ҳ��ʾ�߳�
 
# ���һ��cell��û�б������ǣ�û�и�����Ϊfree������true
def is_cell_free(idx, psize, psnake):
    return not (idx in psnake[:psize]) 
 
# ���ĳ��λ��idx�Ƿ����move�����˶�
def is_move_possible(idx, move):
    flag = False
    if move == LEFT:
        flag = True if idx%WIDTH > 1 else False
    elif move == RIGHT:
        flag = True if idx%WIDTH < (WIDTH-2) else False
    elif move == UP:
        flag = True if idx > (2*WIDTH-1) else False # ��idx/WIDTH > 1
    elif move == DOWN:
        flag = True if idx < (FIELD_SIZE-2*WIDTH) else False # ��idx/WIDTH < HEIGHT-2
    return flag
# ����board
# board_refresh��UNDEFINEDֵ����Ϊ�˵���ʳ���·������
# ����Ҫ��ԭ����Ҫ������
def board_reset(psnake, psize, pboard):
    for i in xrange(FIELD_SIZE):
        if i == food:
            pboard[i] = FOOD
        elif is_cell_free(i, psize, psnake): # ��λ��Ϊ��
            pboard[i] = UNDEFINED
        else: # ��λ��Ϊ����
            pboard[i] = SNAKE
     
# �������������������board��
# �����board��ÿ����SNAKEԪ�ص���ʳ���·������
def board_refresh(pfood, psnake, pboard):
    queue = []
    queue.append(pfood)
    inqueue = [0] * FIELD_SIZE
    found = False
    # whileѭ�������󣬳����ߵ����壬
    # ����ÿ�������е����ִ��������ʳ���·������
    while len(queue)!=0: 
        idx = queue.pop(0)
        if inqueue[idx] == 1: continue
        inqueue[idx] = 1
        for i in xrange(4):
            if is_move_possible(idx, mov[i]):
                if idx + mov[i] == psnake[HEAD]:
                    found = True
                if pboard[idx+mov[i]] < SNAKE: # ����õ㲻���ߵ�����
                     
                    if pboard[idx+mov[i]] > pboard[idx]+1:
                        pboard[idx+mov[i]] = pboard[idx] + 1
                    if inqueue[idx+mov[i]] == 0:
                        queue.append(idx+mov[i])
 
    return found
 
# ����ͷ��ʼ������board��Ԫ��ֵ��
# ����ͷ��Χ4���������ѡ�����·��
def choose_shortest_safe_move(psnake, pboard):
    best_move = ERR
    min = SNAKE
    for i in xrange(4):
        if is_move_possible(psnake[HEAD], mov[i]) and pboard[psnake[HEAD]+mov[i]]<min:
            min = pboard[psnake[HEAD]+mov[i]]
            best_move = mov[i]
    return best_move
 
# ����ͷ��ʼ������board��Ԫ��ֵ��
# ����ͷ��Χ4���������ѡ����Զ·��
def choose_longest_safe_move(psnake, pboard):
    best_move = ERR
    max = -1
    for i in xrange(4):
        if is_move_possible(psnake[HEAD], mov[i]) and pboard[psnake[HEAD]+mov[i]]<UNDEFINED and pboard[psnake[HEAD]+mov[i]]>max:
            max = pboard[psnake[HEAD]+mov[i]]
            best_move = mov[i]
    return best_move
 
# ����Ƿ����׷����β�˶�������ͷ����β������·����
# Ϊ���Ǳ�����ͷ������·
# �����������tmpboard,tmpsnake�н���
def is_tail_inside():
    global tmpboard, tmpsnake, food, tmpsnake_size
    tmpboard[tmpsnake[tmpsnake_size-1]] = 0 # ����ؽ���β��Ϊʳ��(��Ϊ������ģ�������tmpsnake,tmpboard�н���)
    tmpboard[food] = SNAKE # ����ʳ��ĵط�����������
    result = board_refresh(tmpsnake[tmpsnake_size-1], tmpsnake, tmpboard) # ���ÿ��λ�õ���β��·������
    for i in xrange(4): # �����ͷ����β�����ţ��򷵻�False��������follow_tail��׷����β�˶���
        if is_move_possible(tmpsnake[HEAD], mov[i]) and tmpsnake[HEAD]+mov[i]==tmpsnake[tmpsnake_size-1] and tmpsnake_size>3:
            result = False
    return result
 
# ����ͷ������β����һ��
# ���������赲������β��������
def follow_tail():
    global tmpboard, tmpsnake, food, tmpsnake_size
    tmpsnake_size = snake_size
    tmpsnake = snake[:]
    board_reset(tmpsnake, tmpsnake_size, tmpboard) # ��������board
    tmpboard[tmpsnake[tmpsnake_size-1]] = FOOD # ����β��Ϊʳ��
    tmpboard[food] = SNAKE # ��ʳ��ĵط��������
    board_refresh(tmpsnake[tmpsnake_size-1], tmpsnake, tmpboard) # ��ø���λ�õ�����β��·������
    tmpboard[tmpsnake[tmpsnake_size-1]] = SNAKE # ��ԭ��β
 
    return choose_longest_safe_move(tmpsnake, tmpboard) # �������з���(����ͷ�˶�1��)
 
# �ڸ��ַ���������ʱ�������һ�����еķ�������(1��),
def any_possible_move():
    global food , snake, snake_size, board
    best_move = ERR
    board_reset(snake, snake_size, board)
    board_refresh(food, snake, board)
    min = SNAKE
 
    for i in xrange(4):
        if is_move_possible(snake[HEAD], mov[i]) and board[snake[HEAD]+mov[i]]<min:
            min = board[snake[HEAD]+mov[i]]
            best_move = mov[i]
    return best_move
 
def shift_array(arr, size):
    for i in xrange(size, 0, -1):
        arr[i] = arr[i-1]
 
def new_food():
    global food, snake_size
    cell_free = False
    while not cell_free:
        w = randint(1, WIDTH-2)
        h = randint(1, HEIGHT-2)
        food = h * WIDTH + w
        cell_free = is_cell_free(food, snake_size, snake)
    win.addch(food/WIDTH, food%WIDTH, '@')
 
# ������������������У���pbest_move��1��
def make_move(pbest_move):
    global key, snake, board, snake_size, score
    shift_array(snake, snake_size)
    snake[HEAD] += pbest_move
     
 
    # ��esc�˳���getchͬʱ��֤��ͼ�������ԣ�û����ֻ�ῴ�����ս��
    win.timeout(10)
    event = win.getch()
    key = key if event == -1 else event
    if key == 27: return
 
    p = snake[HEAD]
    win.addch(p/WIDTH, p%WIDTH, '*')
 
     
    # ����¼������ͷ����ʳ���λ��
    # �߳���1�������µ�ʳ�����board(��Ϊԭ����Щ·�������Ѿ��ò�����)
    if snake[HEAD] == food:
        board[snake[HEAD]] = SNAKE # �µ���ͷ
        snake_size += 1
        score += 1
        if snake_size < FIELD_SIZE: new_food()
    else: # ����¼������ͷ����ʳ���λ��
        board[snake[HEAD]] = SNAKE # �µ���ͷ
        board[snake[snake_size]] = UNDEFINED # ��β��Ϊ�ո�
        win.addch(snake[snake_size]/WIDTH, snake[snake_size]%WIDTH, ' ')
 
# ���������һ�Σ�Ȼ���ڵ��ô����������пɷ����
# ���в���ʵ���С�
# �������гԵ�ʳ��󣬵õ�����������board��λ��
def virtual_shortest_move():
    global snake, board, snake_size, tmpsnake, tmpboard, tmpsnake_size, food
    tmpsnake_size = snake_size
    tmpsnake = snake[:] # ���ֱ��tmpsnake=snake��������ָ��ͬһ���ڴ�
    tmpboard = board[:] # board���Ѿ��Ǹ�λ�õ���ʳ���·�������ˣ������ټ���
    board_reset(tmpsnake, tmpsnake_size, tmpboard)
     
    food_eated = False
    while not food_eated:
        board_refresh(food, tmpsnake, tmpboard)    
        move = choose_shortest_safe_move(tmpsnake, tmpboard)
        shift_array(tmpsnake, tmpsnake_size)
        tmpsnake[HEAD] += move # ����ͷǰ����һ���µ�λ��
        # ����¼������ͷ��λ��������ʳ���λ��
        # �򳤶ȼ�1������board��ʳ���Ǹ�λ�ñ�Ϊ�ߵ�һ����(SNAKE)
        if tmpsnake[HEAD] == food:
            tmpsnake_size += 1
            board_reset(tmpsnake, tmpsnake_size, tmpboard) # �������к�����board��λ��(label101010)
            tmpboard[food] = SNAKE
            food_eated = True
        else: # �����ͷ����ʳ���λ�ã����¼����λ��Ϊ��ͷ�����һ����Ϊ�ո�
            tmpboard[tmpsnake[HEAD]] = SNAKE
            tmpboard[tmpsnake[tmpsnake_size]] = UNDEFINED
 
# �������ʳ�����·��������ñ�����
def find_safe_way():
    global snake, board
    safe_move = ERR
    # ���������һ�Σ���Ϊ�Ѿ�ȷ������ʳ�����·��������ִ����Ч
    # ���к�õ�����������board�е�λ�ã���tmpboard����label101010
    virtual_shortest_move() # �ú���Ψһ���ô�
    if is_tail_inside(): # ����������к���ͷ��β����ͨ·����ѡ���·����(1��)
        return choose_shortest_safe_move(snake, board)
    safe_move = follow_tail() # ���������follow_tail 1���������������������true
    return safe_move
 
 
curses.initscr()
win = curses.newwin(HEIGHT, WIDTH, 0, 0)
win.keypad(1)
curses.noecho()
curses.curs_set(0)
win.border(0)
win.nodelay(1)
win.addch(food/WIDTH, food%WIDTH, '@')
 
     
while key != 27:
    win.border(0)
    win.addstr(0, 2, 'S:' + str(score) + ' ')               
    win.timeout(10)
    # ���ռ������룬ͬʱҲʹ��ʾ����
    event = win.getch()
    key = key if event == -1 else event
    # ���þ���
    board_reset(snake, snake_size, board)
     
    # ����߿��ԳԵ�ʳ�board_refresh����true
    # ����board�г�������(=SNAKE)��������Ԫ��ֵ��ʾ�Ӹõ��˶���ʳ������·����
    if board_refresh(food, snake, board):
        best_move  = find_safe_way() # find_safe_way��Ψһ���ô�
    else:
        best_move = follow_tail()
             
    if best_move == ERR:
        best_move = any_possible_move()
    # ����һ��˼����ֻ�ó�һ����������һ��
    if best_move != ERR: make_move(best_move)   
    else: break       
         
curses.endwin()
print("\nScore - " + str(score))
