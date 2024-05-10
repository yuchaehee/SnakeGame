# Snake Game
### C++ 프로그래밍 언어로 Snake Game을 구현.

------------------------------------------------------------------------------------------------------------------------------------------------------------------


[스네이크 게임 임의 조건]
1. 맵은 46x23 크기로 설정
   (맨 처음 맵, 아이템 맵, 아이템&게이트 맵으로 구분, * 맵은 3차원 배열; map[][][]의 요소는 정수를 가질 것)
   *(map[][][] 이 가질 수 있는 정수 요소: 0, 1, 2, 3, 4, 5)
   *(0: 뱀이 지나다닐 수 있음을 의미)
   *(1: 뱀이 지나다닐 수 없은 벽을 의미)
   *(2: 게이트로 변할 수 없는 벽을 의미)
   *(3: 싱싱한사과를 의미)
   *(4: 썩은사과를 의미)
   *(5: 게이트를 의미)
   
3. 맨 처음 뱀의 이동 방향은 RIGHT(--> 왼쪽방향키를 눌러서 게임을 시작하면 죽음.. 수정 여부 확인 후 조건 바꿀지 말지 결정할 것..)
4. 맨 처음 뱀의 크기는 3 (Snanke 클래스의 SnakeHead 변수 초기화 해주었고, body 라는 이름의 벡터 변수에 몸통 2개를 넣어놓음(==SnakeBody 인스턴스 2개를 넣어놓았다는 의미.))
5. 맨 처음 시작했을 때, 방향키를 누르기 전 까지는 뱀이 움직이지 않도록 Snake 클래스의 moveX, moveY 변수 값을 0으로 설정해놓았음.
6. ■ 는 벽, ★ 은 싱싱한사과, ◎는 썩은사과를 의미함.
7. Food 클래스의 foodId 는 3과 4를 값으로 가질 수 있음(* 3은 싱싱한사과, 4는 썩은 사과).

------------------------------------------------------------------------------------------------------------------------------------------------------------------

[스네이크 게임 프로젝트 내용 설명]
1. 맨 처음 뱀의 이동 방향은 RIGHT로 설정하였음.
   
2. 즉, 맨 처음 게임을 시작할 때 왼쪽 방향키 누르면 죽음..(이건 수정의 여부 판단 후 바꿀지 말지 결정)
   
3. 맵 레벨이 존재한다. 0, 1, 2 레벨로 이루어져 있음(0Lv:맨 처음으로 주어지는 맵, 1Lv:아이템이 등장하는 맵, 2Lv:아이템과 게이트가 등장하는 맵).
   
4. 게임이 시작한 후 일정 시간이 지나면 맵 레벨이 오름(맵 레벨 1 조건: 게임 플레이 시간 5s, 맵 레벨 2 조건: 게임 플레이 시간 40s).
  
5.  SnakeHead와 SnakeBody 클래스를 따로 만들어서 관리함(SnakeBody 는 Snake 클래스에서 vector를 통해 관리함. SnakeHead 는 Snake 클래스에서 단일 변수로 관리하고 있음.)
   
6.  방향키를 누르면 main.cpp 의 main() 함수에서 snake 클래스의 setPos(snankeDirection dir) 을 호출함. 이때, snakeDirection 은 직접 정의한 enum 타입으로, LEFT=0 RIGHT=1 UP=2 DOWN=3 으로 설정했음. setPos(snakeDirection) 함수는 입력된 방향키에 따라 snake 클래스의 moveX, moveY의 값을 바꾸어 뱀이 이동하도록 하는 역할을 함.

7.  setPos(snakeDirection) 함수로 snake 의 이동방향과 moveX, moveY 값을 설정한 후, snake 클래스의 move(int x, int y) 함수를 호출함. snake 클래스의 move 함수는 전체적인 뱀의 이동을 관리하는 함수임.
   즉 이 함수에서 snakeHead 클래스의 move(int dx, int dy) 함수와 snakeBody 클래스의 move(int dx, int dy) 함수를 호출함. snakeHead 클래스의 move 함수는 snakeHead 의 위치를 전달된 매개변수의 값과 더해 다시 위치 변수에 대입하고, snakeBody 클래스의 move 함수는 body 의 위치 변수에 전달된 매개변수의 값을 대입함. snakeHead 의 위치를 덧셈연산을 수행하여 바꿔주면, snakeBody 는 이동 전 뱀의 머리 위치로 이동하기만 하면 되기 때문에 둘 함수의 행동이 다르다.
  즉, snakeHead 와 snakeBody 클래스를 총괄하는 snake 클래스의 move 함수에서는 prevX, prevY, tempX, tempY 변수를 선언하여 뱀의 전체적인 위치를 뱀의 사이즈만큼 반복문을 돌면서 업데이트 해줌.
  snake 클래스의 move 함수 마지막에는 removeTail() 과 draw() 함수가 호출되는데 removeTail 은, 미리 저장해둔 뱀의 꼬리 위치로 이동하여 공백을 출력해서, 뱀이 이동하는 것처럼 보이게 하기 위함이다. draw 함수는 전체적인 뱀의 위치 이동이 끝나면 비로소 호출되어 뱀의 사이즈만큼 반복문을 돌아 커서를 이동시켜 출력하는 것이다. 즉, removeTail() 과 draw() 를 통해서 커서를 계속 움직이면서 뱀을 콘솔 창에 프린트 해줌.

8.  일정 시간이 지나면 맵 레벨이 증가하면서 맵의 형태도 바뀜. 즉 맵의 형태가 바뀔 때마다 콘솔 창에 다시 맵을 프린트 해주어야함. 그 역할을 Map 클래스의 setMap() 함수가 수행함.
   
9.  setMap() 함수에서 Map 클래스의 draw() 함수를 호출함. draw() 함수는 호출 되자마자 바로 콘솔 창에 프린트 되었던 모든 것들을 지움. 지우고 난 후 Sleep(100) 을 통해 텀을 줌. 텀이 지나면 다시 맵이 화면에 출력 됨. switch 로 분기를 나눠서 맵 레벨에 따라 map 배열의 요소를 프린트 하는 로직을 각각 작성함.
    
10.  main.cpp 파일에서는 시간과 관련된 변수들(startTime, tempTime, itemEmergeTime, gateEmergeTime) 을 선언하여 관리함.
 
11. while(true) 에서 tempTime = clock() 을 통해 계속 현재 시간을 측정함. 즉, 이 현재 시간에 미리 측정해 놓았던 startTime 을 빼서 게임이 시작된 후 흐른 시간을 계산함.

12. itemEmergeTime 변수는 맵 레벨이 1이 된 후부터 값이 대입되기 시작함. 맨 처음에 itemEmergeTime 에 startTime 변수 값을 대입해 놓음. 그 후로 tempTime-itemEmergeTime 이 7s 차이가 날 때마다 itemEmergeTime 변수에 tempTime 변수의 값을 대입함. 그 후, Food 클래스의 setFoodInfo(int x, int y, int id) 함수를 호출하여 food 인스턴스의 foodId, foodPosX, foodPosY 변수 값을 업데이트 함.
    
13. setFoodInfo(int x, int y, int id) 의 매개변수로 랜덤 값을 넣어줘야 함. 즉, 미리 선언해 놓은 random_device 타입의 randomId, randomPosX, randomPosY 변수를 각각 미리 선언한 uniform_int_distribution<int> 타입의 idDis(3, 4), posXDis(2, 44), posYDis(2, 21) 변수에 전달해줌.

즉, food.setFoodInfo(posXDis(randomPosX), posYDis(randomPosY), idDis(randomId)); 구문은 랜덤한 x, y의 위치값과, 랜덤한 아이템 아이디를 매개변수로 전달하여 food 인스턴스의 foodId, foodPosX, foodPosY 변수 값을 업데이트 하는 것임. (* x 값이 가지는 범위는 (2, 44), y 값이 가지는 범위는 (2, 21), id 값이 가지는 범위는 (3, 4)임.)
    
14. setFoodInfo 함수를 통해 food 인스턴스의 변수들이 업데이트 완료되면, 다음으로 map.changeMapState(map.mapLevel, food.foodPosX, food.foodPosY, food.prevPosX, food.prevPosY, food.foodId); 구문을 통해 맵의 상태를 변화시킴. changeMapState(int mapLevel, int x, int y, int prevX, int prevY, int itempId) 함수는 map[mapLevel][prevY][prevX]=0; 구문을 통해 이전 위치에 있던 음식을 없애고, map[mapLevel][y][x]=itemid; 구문을 통해 새로 입력 받은 x, y 위치에 아이템을 놓음.
    
15. 맵의 상태 변화를 마친 후 다음으로 map.setMap(); 구문을 통해 콘솔 창에 맵을 다시 그림. 그 다음으로 food.prevPosX = food.foodPosX; food.prevPosY = food.foodPosY; 구문을 통해 음식의 현재 위치를 이전 위치로 저장함(다음에 changeMapState 함수가 실행될 때, 현재 음식의 위치에 있던 음식을 없애기 위해, 현재 위치를 이전 위치로 저장하는 것!)

16. 게임 진행 도중 뱀의 머리가 싱싱한 사과랑 부딪히면(==뱀의 머리와 싱싱한사과의 위치가 같으면) snake 의 snakeTouchApple() 함수를 호출하여 뱀의 사이즈를 1 늘려주고, body 벡터에 새로운 snakeBody 인스턴스를 생성하여 넣어줌. (* 이때 새로운 인스턴스의 위치는 꼬리쪽이 늘어나는 것이므로 현재 이동 방향과 반대되는 곳으로 늘어나야함. 즉, 현재 이동 방향이 LEFT 라고 예를 들면, moveX=-1, moveY = 0 일 때, -moveX 해서 +1, -moveY 해서 0 이 되고. 이를 새로운 인스턴스를 body 에 넣기 전 body 의 마지막에 있던 인스턴스의 x, y 에 더해주면 맨 마지막에 꼬리가 하나 붙음.)
    
17. 게임 진행 도중 뱀의 머리가 썩은 사과랑 부딪히면(== 뱀의 머리와 썩은 사과의 위치가 같으면) 뱀의 사이즈가 -1 됨. 즉, snake 클래스의 snakeTouchPoisonApple() 함수를 호출하여 뱀의 사이즈를 1만큼 줄이고, 맨 마지막 꼬리 위치로 가서 공백을 출력하여 꼬리가 잘린 것처럼 보이도록 함. 최종적으로 body.pop_back(); 구문을 통해 실질적으로 꼬리가 잘리게 됨
    
18. 만약 뱀의 사이즈가 3보다 작아지면 snake.isDie = true; 구문을 통해 while 문을 벗어나서 게임을 종료함. 
   
19. 게임 진행 도중 뱀의 머리랑 뱀의 몸통이 부딪히면 게임을 종료해야함. 그러나 snake.moveX==0, snake.moveY==0 이 동시에 만족될 때(==게임 시작 직후 방향키를 누르기 전 상태) 에는 머리랑 몸이 닿아도 죽으면 안 되므로 따로 조건문을 걸어주어야 함. (즉, 0,0 을 제외한 경우에서는 뱀의 사이즈-1 만큼 반복문을 돌면서 뱀의 머리와 뱀의 몸의 위치가 같은지를 판단 할 것임. 만약 같다면 snake.isDie=true; 구문을 통해 while 문을 빠져나와 게임을 종료함.)
    
20. 게임 진행 도중 뱀의 머리가 벽과 부딪히면 게임을 종료해야함. snake.snakeTouchWall(); 구문을 통해 뱀의 머리가 벽과 닿았는지의 여부를 확인함. 만약 닿았다면 isDie=true; 구문을 통해 while 문을 빠져나와 게임을 종료함.

21. -------------------------------------------------------------게이트 기능 추가 하면 이어 쓸 것-------------------------------------------------------------
