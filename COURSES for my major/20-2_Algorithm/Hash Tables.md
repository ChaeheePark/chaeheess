**Hash Tables**

- Dictionary : Dynamic set data structure for storing items indexed using keys

- supports operations Insert, Search, Delete

- 무한대의 index를 만들어 Table에 저장하는 것은 비현실적이라서, Hash Function을 통해 유한한 index table을 만든 후 그곳에 저장 -> 그러나 **collision** 이 발생할 수 있다.

- Hash tables have no ordering information! (Expensive : Min,Max,sorted)

- ==> What is good hash function?

  1) Easy to Compute

  2) Approximates a random function-> 골고루 table 에 분포되어야함 (비슷한 문자열이나 비슷한 숫자도 전혀 상관없는 index에 들어가게) : independent!

- The Division Method
  - Idea: h(k)=k mod m
  - Advantage: fast
  - Disadvantage: Certain values of m are bad (ex : 2의제곱, 10의제곱) 
  - good choice for m: Primes!
- The Multiplication Method
  - Idea: k*A(특정소수)한 뒤 소수부분만 추출하고, 그 값에 m을 곱하여 정수부분을 추출
  - Advantage: m is not critical!
  - Disadvantage: slower than division method
- Universal Hashing
  - hash function을 여러개 만들어서 collision 발생확률을 줄임 - > table에 골고루 들어가게
  - provides good results on average
- Collision
  - Chaining : Store all elements that hash to same slot in a linked list 
    - insert O(1), Delete O(1), Search O(1) / worst O(n)
    - chain에 연결된 data들의 적재율 : load factor =n/m
  - Open addressing: All elements stored in hash table itself. When collision occur, use a systematic procedure to store elements in free slots of the table.
    - Linear, Quadratic, Double hashing



Universal Hashing, open addressing analysis 다시공부