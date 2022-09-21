

### Memory Allocation  - Continuous 연속적인 메모리 할당 기법


#### Variable(Dynamic) Partitioning 

- 프로세스를 메모리의 가변 크기 파티션에 할당하는 것 
- 한 파티션은 하나의 프로세스만 할당
- 사용가능한 메모리 부분과, 사용중인 부분을 나타내는 테이블 유지 
- 초기 : 하나의 거대한 Hole(사용가능)
- 사용하다보면 hole이 산재하게 됨
    - 요청이 들어오면 hole 을 iterate 하면서 적절하게 할당
    - hole이 클경우 2개로 나누어 한조각은 Process에 할당함
    - hole이 인접할 경우 merge
    -
<img src="https://media.geeksforgeeks.org/wp-content/uploads/111-10.png">

Pros

- No Internal Fragmentation
- No restriction on Degree of multi programming
- No Limitation on the size of the process

Cons

- Difficult implementation 
- External Fragmentation 

#### Static Partitioning 

- 정해진 size의 parition을 지정
- 필요한 partition 마다 배수로 할당

Pros 

- Easy to implement
- Little OS Overhead

Cons

- Internal Fragmentation 
- External Fragmentation
- Limit procces ssize 
 

<img src="https://media.geeksforgeeks.org/wp-content/uploads/444-4.png">



### Fragmentation


#### External Fragmentation 

<img src="https://media.geeksforgeeks.org/wp-content/uploads/20200729172413/2581.png">

- sum(memory fragement)  > process requirement, but can not be fullfilled because of non-contiguous manner
- 메모리가 적재되고 제거되는 일이 반복되면서 hole이 파편화 

#### Internal Fragmentation 

- Assgined > Used 

<img src="https://media.geeksforgeeks.org/wp-content/uploads/20190924115421/Untitled-Diagram-146.png">


## Paging

- 물리주소 공간을 연속적으로 할당하면 외부 파편화/ 압축 문제가 발생
- 물리 메모리와 가상 메모리 공간을 정해진 크기로 사상
- 외부 파편화가 발생하지 않음 
    - 모든 가용 프레임은 할당될 수 있기 때문
    - 다만 내부 파편화는 발생
    - 페이지 크기 2048 B * 35 + 1086 B  =  procces memory 72766 B 
    - 2048 - 1086 만큼의 파편화 발생
    - 평균적으로 반페이지 정도의 파편화가 발생
    - 페이지 크기를 그렇다고 줄이면, 페이지 테이블 사이즈가 늘어남 
    - 현재는 대부분 4kb , 8kb (mac 4kb)


### Paging 방법

물리 메모리 : Frame이라는 정해진 단위로 분할
논리 메모리 : Page 라는 정해진 단위로 분할 

* Frame의 크기와 Page의 크기는 같다
* 프로세스가 실행되기 전, 몇 페이지가 필요한지 조사한다.
* 페이지마다 프레임을 할당한다.(페이지 별 프레임 정함 -> 페이지 테이블에 기록 -> 반복)


논리메모리 주소 = 페이지 번호 (p) + 페이지 오프셋(d)

페이지 번호 : 페이지 테이블의 인덱스 
페이지 오프셋 : 물리 메모리로 변환하기 위한 오프셋 


1. 페이지 번호 p 를 추출하여 페이지 테이블의 인덱스로 접근
2. 프레임 번호 f를 get
3. f+ 페이지 오프셋(d)가 물리메모리의 주소가 된다.

<img src="https://i.stack.imgur.com/fbKiU.png">


- page의 크기는 하드웨어가 결정
- 2의 n 거듭제곱 꼴
- 논리 주소 공간의 크기가 2^m , 페이지 크기 2^n 이면
- 전체 페이지의 개수(2^(m-n)) 

<img src="https://d3e8mc9t3dqxs7.cloudfront.net/wp-content/uploads/sites/11/2020/04/Paging3-min.png">

<img src="https://d3e8mc9t3dqxs7.cloudfront.net/wp-content/uploads/sites/11/2020/04/Paging-model-of-logical-and-physical-memory.png">


- 32bit CPU 
- 페이지 테이블의 각 항목은 4B(32bit)
- 2^32 개의 frame을 가리킬 수 있음
- frame = 4kb , 2^32 * 4kb = 16TB의 메모리 영역을 가리킬 수 있음



### Frame Table

- 물리 메모리 할당에 관한 정보
- 시스템 당 하나
- 프레임이 비어있는지, 할당되었는지, 할당되었다면 어느 페이지에 할당되었는지를 기록
- 모든 프로세스의 주소를 mapping 할 수 있어야 하기 때문에(system call with memory manipulation), 각 프로세스의 페이지 테이블의 사본을 유지한다. (프로세스는 자신의 페이지 테이블을 접근)


### Hardware Support

- 페이지 테이블을 프로세스 별 자료구조
- 페이지 테이블도 메모리에 있다
- 각 PCB에 페이지 테이블에 대한 포인터를 저장
- page-table base register(PTBR) 을 유지하고, 페이지 테이블을 가리키게 함 
- 다른 페이지 테이블을 이용하려면, 이 레지스터를 변경시킴


### TLB - Translation Look-Aside Buffer

- 메모리에 page table -> 메모리 접근시간이 늘어남
- 메모리 주소 i에 접근시
    - PTBR로 페이지 테이블 주소 획득 후, 페이지 테이블 접근(1회)
    - 프레임 번호와 offset을 결합 한 후 실제 메모리 접근(2회)

<img src="https://media.geeksforgeeks.org/wp-content/uploads/paging-2.jpg">

- TLB : 특수한 소형 하드웨어 캐시 
- key(page num)  value(frame) 구성
- 명령어 파이프라인에 속해있기 때문에, 성능에 손해는 발생하지 않음 
- 32 - 1024 개 유지 
- cache layer처럼 여러 layer를 갖는 경우도 있음
- CPU 가 논리주소 생성 -> MMU 가 TLB 접근 후 hit/ miss
- miss 시 memory 접근 해서 frame 번호 획득 후 cache 추가 
- 중요한(자주 접근하는 프레임)의 경우 고정(커널 코드)

- 만약 프로세스가 context switching 하면 어떻게 되나요? 
- 프로세스 1 -> 페이지 0 , 프로세스 2 -> 페이지 0 은 서로 다른데 (프레임 번호)
- TLB flush를 하는 경우도 있지만, ASIDs(address-space identifier)로 방지 
- TLB 에서 프로세스 ASDI가 TLB ASID 맞는지 확인


ASID | PAGE | FRAME
-|-|-
0|0|16 -> 1 process는 TLB miss


- 현대 CPU는 TLB layer 존재
- L1 TLB (0 cycle) L2 TLB(6 cycle) page table(hundreds cycle)


### Protection 

- 사용중인 페이지의 개수는 프로세스마다 다름
- 페이지 테이블의 크기는 동일함( 전체 논리주소 / 페이지 사이즈) 
- 따라서 유효하지 않은 페이지 num 요청할 경우, invalid page reference trap 
- page 마다 유효/무효 를 표시하는 bit이 존재
- 사용중인 페이지 개수 5개( 0,1, 2,3,4 ) 그럼 6 / 7 --  invalid flag
- 추가적으로 읽기전용 / 읽기, 쓰기 인지 판단하여, 읽기 전용을 쓰려고하면 trap 
- Page table length register 로 비교할 수 도있음
- 위의 경우 PTLR 이 5, 5를 넘는 page num은 invalid로 판단 가능


### Shared Page

- 페이지를 공유 가능 -> 코드 공유 가능 
- standard c lib의 경우 많은 프로세스가 사용 
- libc 를 프로세스 마다 저장하는 경우 2mb * 40 -> 80mb
- shared lib의 경우 shared page로 대부분 구현
- shared memory base IPC의 경우 일부 운영체제의 경우 shared page로 구현

<img src="https://miro.medium.com/max/720/1*mqbOXTHm_tHfI5qg3IYrOA.jpeg">


