**Main Memory (12주차)**

- Continuous Memory Allocation
  - 연속된 메모리 공간에 할당하는 정책
  - Uni programming : process가 한번에 하나만 -> 공간낭비 , 시스템 활용도가 낮다
  - Base register : smallest physical address갖고있음
  - Limit register : logical address의 범위 갖고있음 
- Fragmentation
  - External
  - Internal
  - =>고정된 크기로 메모리 미리 분할해서 관리가 간편하지만 시스템자원이 낭비됨
  - compaction =>한쪽으로 다 옮겨서 넓은 free영역을 만들자, 그러나 dynamic allocation 
- Memory allocation
  - multiple partition allocation
    - 프로세스가 새로 도착하면, 적재 가능한 충분한 크기의 공간을 검색하고 만약에 충분한 크기의 공간이 없다면 대기
    - 대기하는 과정에서 어디에 배치할것잉ㄴ가..? 
      - first fit :처음에
      - best fit : 있는 것 중에 제일 작은것 
      - worst fit : 제일 넉넉한 것



- Virtual Storage (Memory)

  - non-continous allocation
  - 사용자 프로그램을 여러개의 block으로 분할, 실행시 필요한 block들만 메모리에 적재하고 나머지 block들은 swap device에 존재
    - virtual address - > real address
    - 사용자/프로세스는 실행 프로그램 전체가 메모리에 연속적으로 적재되었다고 가정하고 실행\
   
 
- Paging System

  - physical 메모리를 blocks로 나누는 것을 frame

  - logical 메모리를 blocks로 나누는 것을 page

  - 논리적 분할이아님, 크기에 따른 분할! page 공유및 보호 과정이 복잡함

  - 외부단편화 발생안하지만 내부 단편화는 발생할 수 있음

  - virtual address : v=(p,d), p=page number, d=얼마나 떨어져 있는지

  - Page Map Table 사용

    - Direct Mapping : 기본주소로 부터 파생된 것을 이용해 PMT로 찾아가고 page frame을 찾아감 

 

      ​	3-1 context switching (page falut는 overhead가 크다) => 가능하면 피해야함!

      메모리접근이 2번, PMT를 위한 메모리 공간 필요 

      =>해결방안  Associative mapping

    - Associative mapping  (TLB)

      - PMT를 탐색하기 위한 전용 하드웨어
      - PMT를 병렬탐색

    - Protection issue

      - valid &invalid 를 추가 
      - PTLR :페이지 테이블 사이즈를 줘서 로지컬 어드레스가 valid한 범위에 있는지 파악하기 위한 용도

  - Memory management

    - frame table

  - Page Sharing 

    - shared code : read만 가능, 모든 process가 똑같은 논리주소공간에 코드를 위치시켜야 가능
    - private code and data : 코드만 공유하고 그 코드를 위한 데이터나 레지스터 값은 따로 갖음

    - Procedure pages (코드) : 프로세스들이 shared page 에 대한 정보를 PMT의 같은 entry에 저장하게 한다.
    - Data page (데이터) : 다른 프로세스에 같은 PMT page frame number저장



  

- - structure of the page table
    - page table만해도 4mb인데 이것을 연속적으로 할당하기에는 큰 공간이 필요, 그래서 여러개의 작은 조각으로 나누는 방법이존재
    - 1) hierarchical paging : 페이지 테이블이 계층 구조를 갖는것
      - two level page table scheme : 페이지 테이블 자체를 페이지화 시켜서 하드디스크에 저장해두고 있다가 필요시 메모리로 로드
    - 2) Hashed Page Table :  hash table의 각 엔트리는  linked list구조로 다른 페이지 테이블들을 가르키고 있음, linked list 구조로 저장되는 elements 는 1) virtual page number, 2) mapping 되는 page frame number, 3)구조체 포인터 정보를 갖고 있다. 
      - 가상 주소공간의 가상 p값이 hash table로 hashed되고, 가상 p값이 각 elements의 virtual page number와 비교되며 search함
      - 64bit컴퓨터 가장 적합함
    - 3) Inverted Page Table: 프로세스별 페이지 테이블이 따로있던것을 하나로 합침 
      - memory는 줄었지만, pid도 탐색해야한다.
      - page table을 hashed하거나 associated memory register을 사용하기도 한다.





20-30문항 시험에서... 대다수가 객관식 단답형주관식, 코드의결과...

