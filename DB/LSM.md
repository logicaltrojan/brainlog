

스킵리스트 

- List는 linear 한 접근만 가능함  (finding O(N))
- 확률적 복잡도를 tree 와 비슷하게 
- Apache Cassandra가 secondary index memetable 구현에 사용


- 높이가 다른 여러 노드로 구성
- lv 0 -> 모든 노드와 연결
- lv 1 - N -> lv 1- N까지 같은 레벨의 노드와만 연결
- 노드의 높이는 삽입시 임의의 함수를 통해 결정

- 검색에서도 사용한다. (lucene)
- Document의 inverted index, DocID 

    //pseudo
pointer = head[level]


1. 포인터의 value 확인
2. if 검색키보다 크면
3. 노드가 가리키는 다음레벨 포인터로 포인터 전환
4. 다시 1
5. else 검색키보다 작으면
6. 포인터 이동


Insert 

- 삽입 키가 저장된 노드 / 바로 앞까지 탐색
- 노드의 높이 생성(function)
- 새로운 노드의 키보다 작은 키가 있는 앞 노드의 포인터가 새로운 노드를 가리키도록 수정


fully_linked flag 사용 

- CompareAndSwap으로 flag 조정 


압축

- B-tree와의 차이는 불변구조 
- Page 를 압축하면 당연히 page 보다 사이즈가 작아짐
- 따라서 page : 압축 페이지가 정확히 mapping되지 않음
- 따라서 압축 페이지에 접근하려면 offset을 기록해두어야 함 
- 0으로 채워서 page와 맞출 수는 있는데 그러면 압축을 왜 함 
- offset table 유지
- compaction / flush 시 압축 -> 압축된 페이지를 디스크에 쓰고, 별도 파일에 offset table 저장 
- 만약 read시 offset table로 압축된 페이지를 해제하고, 메모리 로드


비정렬 LSM 스토리지

- 현재까지 설명한 LSM 들은 레벨에 따라 어쨋든 key를 정렬 상태로 유지함 (using tree ( btree ))
- 순서를 보장하지 않으면 -> 레코드를 삽입 순으로 저장함 
- 보통 이럴 경우, 바로 로그에 저장할 수 있으므로(write-a-head 로그가 순서대로 로그를 저장하는 건데, 그냥 이것처럼 레코드를 저장함) write 연산 성능이 좋아짐

Bitcask

- 비트캐스크는 정렬되지 않는 LogStructuredStorage Engine
- memtable 안쓰고, 데이터레코드 바로저장
- 데이터 레코드 read를 위해 저장할때, key / pointer 를 저장함 (keydir)
- 시스템 시작시, 데이터파일을 사용해 keydir 재구성 

Insert / Update

- 데이터 레코드 추가 (append)
- keydir 추가 (변경)
- Update시 append이므로 변경전 데이터가 앞에 있음 (compaction 필요)

Read 

- keydir 조회, 포인터 따라감
- 범위 스캔이 불가능 , key가 정렬되어 있는게 아니기 때문 
- 포인트 쿼리만 수행 가능 


Compaction

- full scan (아마 tail -> head) 
- 삭제 처리
- keydir 업데이트 

WicsKey

- 정렬되지 않은 LSS의 이점을 살리면서, 범위스캔까지 지원
- LSM 트리에 키를 정렬상태로 유지하되, vLog(Value Log, data file이다) 에 append
- vLog는 임의로 저장되어 있음 
- 정렬된 트리에는 마찬가지로 최신의 레코드를 가리키는 키만 저장
- 키만 저장할 경우 compaction이 빨라지는 이점은 있음

- 범위 스캔 수행시 random I/O 발생 (포인터가 레코드 전체 어디있을지 모름)
- 예를 들어 키 1, 2를 fetch하는데, 페이지 27 , 30 을 불러와야할 수도 있음
- 블록을 pre-fetch하더라도 운이 없을 경우 , 페이지가 많아짐 
- 페치가 빠르더라도 전송데이터가 많아짐

Compaction 

- full scan 이지만, 활성된 데이터가 있는 부분을 가리키는 포인터를 유지한다
- vLog 를 scan시 키의 활성화 여부를 알기 위해 tree를 스캔해야한다. 
- N * logN 의 복잡도 ? 
- 일반 LSM 트리는 키 인덱스에 접근하지 않는다. 

LSM 동시성 제어

- LSM은 불변이라 많은 부분에서 동시성에 관한 이점을 가지지만 
- 플러시 / compaction 과정에서 데이터 동시성을 고려해야한다. 
- 카산드라는 작업 순서를 강요 
- 모든 write -> 후 memtable flush ( 관련해서 찾아보려고 하는데 atomicity 관련해서만 나오고..  )
- compa

1. 새로운 멤테이블 생성 ( 읽기 /쓰기 가능 )
2. 기존 테이블 ( 읽기 / 쓰기 불가능 ) 
3. 플러시 대상 디스크 저장 
4. 플러시 메모리  삭제 + 디스트 테이블 생성 
5. write ahead log 삭제 

memtable transition -> flush -> log delete

데이터 유실 상황 

- memtable transition 이후, 기존 memtable에 쓰는 경우 
- flush 도중, memtable 삭제 
- flush 도중, log 가 삭제, 노드 장애 -> replay, 데이터 복원 불가

Log Stacking

- fs 대다수가 log structure 
- 메모리 저장 -> 버퍼 차면 디스크 플러시 
- inplace update는 random access를 유발 
- LSM 구조는 SSD와 잘 맞음 


FTL (Flash Translation Layer) 
https://www.makeuseof.com/tag/solidstate-drives-work-makeuseof-explains/

- SSD 에서도 로그 구조를 쓴다 
- SSD에서 왜 로그 구조의 mapping이 필요할까?  
    - 랜덤 접근으로 페이지 단위로 read/update를 반복할 경우 I/O비용이 커지게 된다.
    - page update가 불가능, 오로지 delete 된 Page에 write 할 수 있다. 
    - page 개별로 delete도 불가능, 오로지 블록 단위로만 삭제 가능 
	- SSD는 블록의 일정 threshold가 지나가면, data 를 memory 에 커밋한뒤, 블록을 지우고, 다시 데이터를 쓴다
- 로그 구조라는 것이 ... 생각해보면, write ( + write * )  + flush + compaction
- FTL은 논리적 페이지 주소 -> 물리적 위치 변환( 사용중 , 삭제 표시 )

LLAMA - LatchFree, LogStructured, Access-method aware

- Bw - Tree 
- 다음 B-Tree의 변형할때 같이 ~ 


OpenChannel SSD 

- 위에서 언급한 FTL 이 없는 SSD 
- 그럼 위에서 말한 기능은 누가하나요? -> OS 
- API 가 열려있다고 생각하면 된다 
- FTL이 없으니까 논리 -> 물리위치 변환은 필요없다..
- 위에서 언급한, 페이지 배치 , 가비지 컬렉션, I/O scheduling 등등을 직접할 수있지만 .. 
- 뭐든지 직접한다는 것은, "답답하면 니들이 뛰던가" 랑 같아서, 잘하면 좋지만, 못하면 단점이다. 
