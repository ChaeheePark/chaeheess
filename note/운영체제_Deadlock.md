**Deadlock(11주차)**

- Deadlock의 개념 
  - 프로세스가 특정 이벤트를 기다리고 필요한 자원을 기다리는 상태
  - Deadlock vs Starvation
    - Deadlock은 자원을 기다리는데 일어날 가능성이없는경우
    - starvation은 ready 상태에서 process를 기다리고 있는 상태에서 cpu가 할당되지 않는것 (발생할 가능성 있음)
    - agomg 은 starvation을 막기 위한 방법, 시간이 지날수록 낮은 우서순위를 가진 프로세스의 우선 순위를 높여주는 방법
  - 자원 
    - 선점가능자원 : 프로세스, 메인메모리
    - 비선점자원 : 디스크드라이브
    - Deadlock을 발생시킬 수 있는 자원은 ? 
      - Non-preemptible resources
      - Exclusive allocation resources
      - Serially reusable resources
  - Deadlock-4가지 조건 모두 동시에 성립하면 발생
    - mutual exclusion: 최소 하나의 자원이 비공유 모드로 점유됨, 자원은 한번에 한 프로세스에서만 배타적으로 점유
    - hold and wait : 하나의 자원 점유시, 다른 스레드에 점유된 것을 추가로 얻으려면 반드시 대기
    - no preemptation : 자원 선점은 불가능
    - circular wait : 대기중인 프로세스중, 프로세스 i-1은 i번째 프로세스가 점유한 자원을 대기, 순환고리 형태



- 교착상태 해결책
  - 교착상태 방지
    - 모든자원공유허용 (현실적으로불가)
    - 비선점조건거부 (현실적으로불가)
    - 필요자원 한번에 모두 할당 - 무한대기현상, 자원낭비
    - circular wait 조건 거부 (자원낭비)
  - 교착상태 회피
    - 시스템의 상태를 계속 감지	
    - 시스템이 deadlock 상태가 될 가능성 있는 자원 할당 요청 보류
    - 시스템을 항상 safe state로 유지
  - 교착상태 탐지 및 복구
    - deadlock이 발생하면 확인 - > deadlock 주기적으로 발생 확인
    - 자원할당 그래프를 소거해가면서 (edge를 하나씩 지워가면서) deadlock 확인 모든edge가 지워지면 deadlock이 아님
    - 복구
      - 프로세스 termination
      - recource termination
      - 체크포인트/롤백 : 시스템 다운이나 교착상태에 대처하기 위해 종료되는 각 프로세스로부터 가능하면 많은 양의 데이터를 보존하는 기능



- 메모리(주기억장치)
  - Backgrounds
    - address binding : 프로그램의 논리 주소를 실제 메모리의 물리 주소로 매핑하는 과정
    - compile time binding : 프로세스가 메모리에 적제될 위치를 알고있는 경우 
    - load time binding : 메모리 적재위치를 적재 시점에서 시작주소에 반영(상대주소)
    - run time biniding : 실제로 수행될때 (running 상태) address주소정함  - > 이때 거의 binding physical, logical address가 다름
      - MMU : logical -> physical mapping  (cpu코어 안에 탑재되어 가상주소를 실제 메모리 주소로 변환해주는 하드웨어), logical address가 연속적이면 physical addess도 연속적이게 만들어줌
      - ==>연속적인 memory allocation(contiguous allocation)장점은 ? MMU에서 더하기 하나만 실행하면 되니깐 간ㄱ단하다. 하드우어 만들기가 매우 쉽다! 하지만 현재 우리시스템에서는 안쓰임
    - linking 
      - static linking : 실행파일을 만들때 라이브러리를 같이 포함시켜서 .exe파일등을 만듬
      - dynamic linking : 정적라이브러리를 사용하여 컴파일을 하면 링커가 프로그램이 필요로 하는 부분을 라이브러리에서 찾아 실행파일에다 복사하는데 , 실행 파일 내에 라이브러리 코드가 저장되기 때문에 많은 메모리가 소모된다. 그래서 많이 쓰이는 라이브러리는 메모리에 하나만 올리고 저장, 프로그램이 라이브러리를 호출할때 메모리에 있는 라이브러리로 점프해서 실행한 다음 다시 돌아오게 하자!!
    - Swapping : 프로세스는 실행되려면 반드시 메모리에 올라가야 하는데, 프로세스는 메모리에서 잠깐 뒤의 저장공간으로 빠졌다가 다시 메모리에 돌아왔다가 이런식으로 실행됨에 따라 교체된다.
    - swap out - 메인메모리에서  프로세서 할당이 끝나고 수행완료된 프로세스는 swap-device로 보냄, swap in - 새롭게 시작하는 프로세스는 메모리에 적재
    - 모바일에서는 지원하지 않지만 가용메모리가 부족하다면 프로세스를 종료시키는것이 가능한데 종료하기전 응용의 상태를 플래시 메모리에 저장함->나중에빠르게재시작할수있게한다





메인메모리.. 메모리에 올라갈때 relocation register , ..dynamic address를 binding logical->physical ..여러 방법이 가능 ..대부분 excusion타임...store...linking 다른 프로세스현재process 모든 내용 그대로를 background(큰공간)에 보내짐 context switch에서 가장 중요함
