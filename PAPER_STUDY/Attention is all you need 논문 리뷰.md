# Attention is all you need 논문 리뷰

## Abstract

recurrent encoder-decoder model에 기반한 attention mechanism 이 가장 좋은 성능을 냈었음 

-> 그래서 새로운 simple network architecture인 Transformer을 제안 (recurrence와 convolution을 완전히 생략한 attention을 이용)



## Introduction

Recurrent neural network (RNN)

- input과 output의 symbol position으로 계산
- 이전 hidden state로 현재 hidden state 생성
- sequential한 계산에 문제점이 존재
- 속도 측면에서 비효율적

Attention mechanism

- input과 output의 거리에 의존하지 않고 모델링
- 가중치가 적용되어 곱해짐

Transformer

- attention mechaninsm에 전적으로 의존해서 모델의 결과를 내보냄
- 순차적으로 입력을 넣지않고 병렬적으로 가능



## Background

sequential한 계산 줄이기 

- Extended Neural GPU, ByteNet, ConvS2S
- 먼 position 사이의 의존을 어렵게 만듬
- Transformer는, 이를 상수 시간의 계산으로 진행

Self-attention

- different position의 sequence들을 계산하기 위한 single sequence과 관련
- target문장을 만들기 위해 source 문장의 hidden 정보를  참고하는 것이 아니라 자기 자신에게 가중치를 부여함
- reading comprehension, abstractive summarization, textual entialment, learning task-independent sentence representation에 쓰임

End-to-end memory 

- recurrent attention에 기반
- question answering, launguage model task에 쓰임



## Model Architecture

### 1) Encoder and Decoder Stacks

<img width="307" alt="image" src="https://user-images.githubusercontent.com/60170358/148228588-fa501a0e-0922-4601-acce-2137fe30bb34.png">

Encoder : 6번 encoder layer 중첩해서 사용, 512 차원

Decoder : 6번 decoder layer 중첩해서 사용, residual connection 사용



### 2) Attention

 query-key-value

<img width="373" alt="image" src="https://user-images.githubusercontent.com/60170358/148373449-21da1e96-c022-4143-98bb-e63d656fc6d0.png">

입력 값을 multi-head-attention에서 V,K,Q에 복제하고 Linear 차원에 넣어주어서 Attention(왼쪽그림) 모델에 넘겨준 뒤, concat함 -> 입력과 출력의 dimension이 같아짐

<img width="193" alt="image" src="https://user-images.githubusercontent.com/60170358/148373640-5497976f-dddf-4b7c-9ba1-f2e3b2752075.png">

additive attention으로 구현

dot product attention이 훨씬 효율적임

softmax는 중간부분이 상대적으로 gradient가 크고 side로 갈수록 작아짐 -> 학습이 잘 안됨 -> scale factor로 나눠줘서 학습 잘되게!



- encoder-decoder attention layer: decoder에서 사용, key와 value 값은 encoder의 출력값에서 가져옴
- self-attention : query, key ,value 모두 같은 곳에서 사용
- decoder : 입력 embedding이 들어왔을때 self-attention 수행, mask를 씌워서 0%로 만들수 있게 하여서 각각의 단어가 앞부분의 단어만 참고할 수 있도록 만듬



### 3) Position-wise Feed-Forward Networks

 <img width="176" alt="image" src="https://user-images.githubusercontent.com/60170358/148374426-08e5573a-6750-483a-8c64-633c426c55e9.png">

입력값과 출력값이 같은 dimension 갖음



### 4) Embeddings and Softmax

### 5) Positional Encoding

 위치에 대한 정보를 넣어줌

<img width="185" alt="image" src="https://user-images.githubusercontent.com/60170358/148374697-07dff5de-ff13-4f3e-be50-1dfcb411e456.png">

학습 가능한 형태로만 만들면 가능



## Why Self-Attention

1) total computational complexity 줄어듬
2) 병렬 처리 가능
3) long-range dependency에서도 잘 처리 가능

<img width="416" alt="image" src="https://user-images.githubusercontent.com/60170358/148374911-03ee6891-ec6a-4da6-9e12-1c4c64fbc3a1.png">

attention 자체가 설명 가능한 모델을 만듬

어떤 단어를 가장 많이 참고해서 만들었는지를 알 수 있음



## 5) Training

English to German, English to French dataset 사용

residual, label smoothing 사용

<img width="371" alt="image" src="https://user-images.githubusercontent.com/60170358/148375180-28427878-636f-4247-9d2a-fcb4b58afc9b.png">

학습 효율이 높고 시간도 짧게 걸림

<img width="420" alt="image" src="https://user-images.githubusercontent.com/60170358/148375299-35ed6e67-6131-4ea5-8af3-44c269e99fa6.png">

큰 모델을 사용했을때 성능이 좋음



## Conclusion

Transformer architecture을 제안

전적으로 attention만 사용-> 높은 정확성, 성능 또한 좋음 

기계번역 task 에서 good-> 뿐만 아니라 다른 다양한 task에도 적용 가능
